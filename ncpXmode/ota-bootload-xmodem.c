/***************************************************************************//**
 * @file
 * @brief Routines for sending data via xmodem
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "ota-bootload-xmodem.h"
#include "crc16.h"
//------------------------------------------------------------------------------

#define SOH   (0x01)
#define EOT   (0x04)
#define ACK   (0x06)
#define NAK   (0x15)
#define CAN   (0x18)

#define DATA_SIZE        (128)
#define HEADER_SIZE      (3)
#define FOOTER_SIZE      (2)
#define FULL_BLOCK_SIZE  (HEADER_SIZE + DATA_SIZE + FOOTER_SIZE)

#define CONTROL_OFFSET      (0)
#define BLOCK_NUM_OFFSET    (1)
#define BLOCK_COMP_OFFSET   (2)
#define DATA_OFFSET         (3)
#define CRC_H_OFFSET        (131)
#define CRC_L_OFFSET        (132)

#define UNINITIALIZED       (0)
#define START_TRANSMISSION  (1)
#define SENDING             (2)


static uint8_t state = UNINITIALIZED;
static uint8_t buffFinger;
static uint8_t dataBuff[DATA_SIZE];
static int blockNum;

//------------------------------------------------------------------------------

static bool sendBlock(uint8_t blockNum, const uint8_t *data)
{
    //printf("sendBlock 11111111111111\n");
  int i;
  int retry = 5;
  uint8_t status = NAK;
  uint8_t fullBlock[FULL_BLOCK_SIZE];

  fullBlock[CONTROL_OFFSET] = SOH;
  fullBlock[BLOCK_NUM_OFFSET] = blockNum;
  fullBlock[BLOCK_COMP_OFFSET] = ~blockNum;
  //printf("sendBlock a\n");
  for (i = 0; i < DATA_SIZE; i++) {
    //crc = halCommonCrc16(data[i], crc); //crc    
    fullBlock[DATA_OFFSET + i] = data[i];
  }
  //printf("sendBlock b\n");
  uint16_t crc = 0;
  crc = btl_crc16Stream(data, DATA_SIZE, crc);
  fullBlock[CRC_H_OFFSET] = (crc>>8) & 0xFF;
  fullBlock[CRC_L_OFFSET] = crc & 0xFF;
 // printf("sendBlock c\n");
  while ( (status == NAK) && (retry > 0) ) {
    //print "block %d (" % num,
    //for i in range(0,len(data)):
    //  print "%02x" % ord(data[i]),
    if (!emAfBootloadSendData(fullBlock, FULL_BLOCK_SIZE)) 
    {
      printf("sendBlock() fail 1\n");
      return false;
    }
    retry--;
    if (!emAfBootloadWaitChar(&status, false, 0)) {
      printf("sendBlock() fail 2\n");
      return false;
    }
    while ( status == 'C' ) {
      // may have leftover C characters from start of transmission
      if (!emAfBootloadWaitChar(&status, false, 0)) {
        printf("sendBlock() fail 3\n");
        return false;
      }
    }
  }

  return (status == ACK);
}

void emAfInitXmodemState(bool startImmediately)
{
  if (startImmediately) {
    // skip checking for 'C' characters
    state = SENDING;
  } else {
    state = START_TRANSMISSION;
  }

  buffFinger = 0;
  blockNum = 1;
}

bool emAfSendXmodemData(const uint8_t *data, int length, bool finished)
{
  printf("emAfSendXmodemData length:%d \n",length);
  uint8_t rxData;
  int i;

  if (state == START_TRANSMISSION) {
    sleep(1);
    if (emAfBootloadWaitChar(&rxData, true, 'C')) {
      printf("sending\n");
      state = SENDING;
    } else {
      printf("NoC\n");
      return false;
    }
  }

  if (state == SENDING) 
  {
    for (i = 0; i < length; i++) 
    {
      dataBuff[buffFinger++] = data[i];
      if (buffFinger >= DATA_SIZE) 
      {
        usleep(100000);
        printf("block start %d %d %d\n", blockNum,i,length);        //fsync();
  
        if (!sendBlock(blockNum, dataBuff)) 
        {
          printf("sendblock err\n");
         // emberAfCoreFlush();
          return false;
        } 
        buffFinger = 0;
        blockNum++;
      }
    }
    printf("buffFinger :%d\n",buffFinger);
    printf("finished :%d\n",finished);
    if ( finished ) 
    {
      if ( buffFinger != 0) {
        // pad and send final block
        bool result;
        while (buffFinger < DATA_SIZE) {
          dataBuff[buffFinger++] = 0xFF;
        }
        printf("final block %d\n", blockNum);
        usleep(100);
        result = sendBlock(blockNum, dataBuff);
        if (!result) {
          return false;
        }
      }
      printf("EOT\n", blockNum);
      emAfBootloadSendByte(EOT);
      if (!emAfBootloadWaitChar(&rxData, true, ACK)) {
        printf("NoEOTAck\n");
        return false;
      }
    }
  } 
  else {
    printf("badstate\n");
    return false;
  }
  return true;
}
