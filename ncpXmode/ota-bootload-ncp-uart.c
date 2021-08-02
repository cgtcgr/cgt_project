
#include <sys/select.h>
#include "ota-bootload-ncp.h"

//------------------------------------------------------------------------------
// Globals

#define NULL_FILE_DESCRIPTOR  (-1)
static const char returnChar = 0x0A; // line feed
static const char runChar =  '2';
static const char beginDownload = '1';
static const char xmodemStartChar = 'C'; 

#define BOOTLOADER_DELAY  3     // seconds
#define MAX_ERROR_LENGTH  255

// This is setup to be greater than the time between 'C' characters
// spit out by the bootloader to indicate that it is ready for an Xmodem
// transfer.
#define READ_TIMEOUT_SEC  3

#define MAX_BYTES_WITHOUT_PROMPT 200

// We store the last few bytes so that we can check whether we received
// the expected bootloader prompt "BL >"
#define MAX_RECEIVED_BYTES  4
static uint8_t receivedBytesCache[MAX_RECEIVED_BYTES];
static const char menuPrompt[] = "BL >";

// This is sized to be big enough to read "\r\nbegin upload\r\n"
// with some additional extra fuzz.
#define MAX_BYTES_WITHOUT_XMODEM_START  200

//------------------------------------------------------------------------------
// Forward Declarations

static void delay(void);
static int checkFdForData(void);
static void storeReceivedByte(uint8_t byte);
static bool checkForBootloaderMenu(void);
static bool checkForXmodemStart(void);

#define errorPrint(...) fprintf(stderr, __VA_ARGS__)
#define messagePrint(...) printf(__VA_ARGS__)

static int serialFd = NULL_FILE_DESCRIPTOR;

//------------------------------------------------------------------------------

bool emAfStartNcpBootloaderCommunications(void)
{
  serialFd = NULL_FILE_DESCRIPTOR;
  char errorString[MAX_ERROR_LENGTH];
  int tryTimes = 1;
  bool ret = false;
  sleep(4);
	printf("Setting up serial port\n");
	if (0 != uart_init()) 
	{         //  建立串口连接 ！！
      printf("Error: Could not setup serial port");
      return false;
	}

  delay();
  //sleep(1);
  // do
  // {
  //     if (!checkForBootloaderMenu()) {  //发送0xoa  等待返回"BL >"; 
  //     errorPrint("Failed to connnect bootloader.\n");
  //       continue;
  //     }
  //     else
  //     {
  //         break;
  //     }
  // }while(tryTimes--);

  // tryTimes = 1;
  // sleep(1);
  // uint8_t data[1024];
  // ssize_t bytes = read(serialFd, &data, 1000);
  // if (bytes >=0) {
  //     errorPrint("Read left data %d\n", bytes);
  // }

  do{
      if(!emAfBootloadSendByte(beginDownload)) { //发送‘1’；
        errorPrint("Failed to set bootloader in download mode.\n");
        continue;
      }

      if(checkForXmodemStart()) //等待ncp返回'C';
      {
        ret = true;
        break;
      }
      else
      {
        continue;
      }
  }while(tryTimes--);

  return ret;
}

static bool checkForXmodemStart(void)
{
  printf("checkForXmodemStart\n");
  uint8_t bytesSoFar = 0;
  int startTimes=0;
  while (bytesSoFar < MAX_BYTES_WITHOUT_XMODEM_START) 
  {
    if(bytesSoFar%50 == 0)
    {
      if(!emAfBootloadSendByte(beginDownload))
      { //发送‘1’；
          errorPrint("Failed to set bootloader in download mode.\n");
          return false;
      }
      sleep(1);
    }
    int status = checkFdForData();
    if (status <= 0) {
      // Timeout or error
      return false;
    }

    uint8_t data;
    ssize_t bytes = read(serialFd, &data, 1);
    if (bytes < 0) {
      errorPrint("Read failed: %s\n", strerror(errno));
      return false;
    }

    // debug
    printf("checkForXmodemStart Got<%c,%d>\n", (char)data,(char)data);

    if (data == xmodemStartChar) {
      startTimes++;
    }
    else
    {
      startTimes=0;
    }
    if(startTimes>=1)
    {
      return true;
    }
    bytesSoFar++;
  }

  errorPrint("Failed to get Xmodem start message from bootloader.\n");
  return false;
}

bool emAfBootloadSendData(const uint8_t *data, uint16_t length)
{
  printf("emAfBootloadSendData >>>>>>>>>>>>>>>>>>>>> %d\n",length);
  // for(int i = 0 ; i < length; i++)
  // {
	// 	printf("%02x  ",data[i]);
  // }
  // printf("\n emAfBootloadSendData <<<<<<<<<<<<<<<<<<<<<\n");

  for(int i = 0 ; i < length; i++)
  {
    int ret;
    uint8_t fullBlock[1];
    fullBlock[0] = data[i];
		if (1 != write(serialFd,fullBlock,1)) 
		{         // count
			errorPrint("Error: Failed to write to serial port ret: %d", ret);
			return false;
  	}
  }
  // if (length != write(serialFd,
  //                     data,
  //                     length)) 
  //                     {         // count
  //   errorPrint("Error: Failed to write to serial port: %s",
  //              strerror(errno));
  //   return false;
  // }
  fsync(serialFd);
  return true;
}

bool emAfBootloadSendByte(uint8_t byte)
{
  return emAfBootloadSendData(&byte, 1);
}

static bool checkForBootloaderMenu(void)
{
  // verbose debug
  printf("checkForBootloaderMenu()\n");

  memset(receivedBytesCache, 0, MAX_RECEIVED_BYTES);

  // Send a CR to the bootloader menu to trigger the prompt to echo back.
  if (!emAfBootloadSendByte(returnChar)) {
    return false;
  }
  bool done = false;
  int totalBytes = 0;
  while (!done) {
    int status = checkFdForData();

    if (status <= 0) {
      return false;
    }
    uint8_t data;
    ssize_t bytes = read(serialFd, &data, 1);
    if (bytes < 0) {
      errorPrint("Error: read() failed: %s\n", strerror(errno));
      return false;
    } else if (bytes == 0) {
      continue;
    }
    totalBytes++;
    storeReceivedByte(data);

    // verbose debug
    //printf("%c", data);
    int i = 0;
    for(; i< MAX_RECEIVED_BYTES;i++)
    {
      if(receivedBytesCache[i] != menuPrompt[i])
      {
          break;
      }
    }
    if(i == MAX_RECEIVED_BYTES)
    {
      done = true;
      continue;
    }

    if (totalBytes > MAX_BYTES_WITHOUT_PROMPT) {
      errorPrint("Got %d bytes without seeing the bootloader menu prompt.\n",
                 MAX_BYTES_WITHOUT_PROMPT);
      return false;
    }
  }
  tcflush(serialFd, TCIFLUSH);
  // while(checkFdForData()>0)
  // {
  //     uint8_t data;
  //     ssize_t bytes = read(serialFd, &data, 1);
  //     printf("Got checkFdForData<%c,%x>\n", (char)data, (char)data);
  // }
  return true;
}

bool emAfRebootNcpAfterBootload(void)
{
  delay();
  messagePrint("Rebooting NCP\n");

  if (!checkForBootloaderMenu()) {
    errorPrint("Failed to get bootloader menu prompt.\n");
    return false;
  }

  if (!emAfBootloadSendByte(runChar)) {
    return false;
  }

  delay();        // arbitrary delay to give NCP time to reboot.
  close(serialFd);
  serialFd = NULL_FILE_DESCRIPTOR;
  return true;
}

static void storeReceivedByte(uint8_t newByte)
{
  // We right shift all the bytes in the array.  The first byte on the array
  // will be dumped, and the new byte will become the last byte.
  uint8_t i;
  for (i = 0; i < MAX_RECEIVED_BYTES - 1; i++) {
    receivedBytesCache[i] = receivedBytesCache[i + 1];
  }
  receivedBytesCache[i] = newByte;
}

int open_serial(const char *path, int baud)
{
    struct termios tty;

    serialFd = open(path, O_RDWR | O_SYNC);
    if (serialFd < 0)
    {
        perror("open");
        return -errno;
    }

    memset(&tty, 0, sizeof(tty));
    if (tcgetattr(serialFd, &tty) != 0)
    {
        perror("tcgetattr");
        return -errno;
    }

    cfsetospeed(&tty, baud);
    cfsetispeed(&tty, baud);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_iflag &= ~IGNBRK;                         // disable break processing
    tty.c_lflag = 0;                                // no signaling chars, no echo,
    // no canonical processing
    tty.c_oflag = 0;                                // no remapping, no delays
    tty.c_cc[VMIN]  = 1;                            // read doesn't block
    tty.c_cc[VTIME] = 5;                            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);                // ignore modem controls,
    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);              // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
 //tty.c_oflag &= ~(ONLCR);
    if (tcsetattr(serialFd, TCSANOW, &tty) != 0)
    {
        perror("tcsetattr");
        return -errno;
    }

    return serialFd;
}

int uart_init()
{
	int rv = -1;
	struct termios options;
	 serialFd = open("/dev/ttyS3",O_RDWR | O_NOCTTY ); //打开串口设备
	 if(serialFd < 0){
		 printf("open tty failed:%s\n", strerror(errno));
		 return -1;
	}
	printf("open devices sucessful!\n");

	memset(&options, 0, sizeof(options));
	rv = tcgetattr(serialFd, &options); //获取原有的串口属性的配置
	if(rv != 0)
	{
		printf("tcgetattr() failed:%s\n",strerror(errno));
    close(serialFd);
		return -1;
	}

	//options.c_cflag|=(CLOCAL|CREAD ); // CREAD 开启串行数据接收，CLOCAL并打开本地连接模式
	//options.c_cflag &=~CSIZE;// 先使用CSIZE做位屏蔽  
	//options.c_cflag |= CS8; //设置8位数据位
	//options.c_cflag &= ~PARENB; //无校验位
	 cfsetispeed(&options, B115200);
	 cfsetospeed(&options, B115200);
    options.c_cflag |= CLOCAL;               // ignore modem inputs
    options.c_cflag |= CREAD;                // receive enable (a legacy flag)
    options.c_cflag &= ~CSIZE;               // 8 data bits
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;              // no parity
    options.c_lflag &= ~ (ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(BRKINT | INLCR | IGNCR | ICRNL | INPCK
                      | ISTRIP | IMAXBEL | IXON | IXOFF | IXANY);
	//tcflush(serialFd ,TCIFLUSH);
	/* tcflush清空终端未完成的输入/输出请求及数据；TCIFLUSH表示清空正收到的数据，且不读取出来 */

  options.c_cflag |= (CLOCAL | CREAD);
  options.c_oflag &= ~(ONLCR);
	if((tcsetattr(serialFd, TCSANOW,&options))!=0)
	{
		printf("tcsetattr failed:%s\n", strerror(errno));
    close(serialFd);
		return -1;
	}
	return 0;    
}

static void delay(void)
{
  // Empirically I have found that we have to delay to give the bootloader
  // time to launch before we see the LEDs flash and the bootloader menu
  // is available.  Not sure if this could be improved.
  messagePrint("Delaying %d seconds\n", BOOTLOADER_DELAY);
  sleep(BOOTLOADER_DELAY);
}

// CYGWIN NOTE
//   Cygwin does NOT properly handle select() with regard to serial ports.
//   I am not sure exactly what will happen in that case but I believe
//   it will not timeout.  If all goes well we will never get a timeout,
//   but if things go south we won't handle them gracefully.
static int checkFdForData(void)
{
  fd_set readSet;
  struct timeval timeout = {
    READ_TIMEOUT_SEC,
    0,                  // ms. timeout value
  };

  FD_ZERO(&readSet);
  FD_SET(serialFd, &readSet);

  int fdsWithData = select(serialFd + 1,         // per the select() manpage
                                                 //   highest FD + 1
                           &readSet,             // read FDs
                           NULL,                 // write FDs
                           NULL,                 // exception FDs
                           (READ_TIMEOUT_SEC > 0 // passing NULL means wait
                            ? &timeout           //   forever
                            : NULL));

  // Ideally we should check for EINTR and retry the operation.
  if (fdsWithData < 0) {
    errorPrint("FATAL: select() returned error: %s\n",
               strerror(errno));
    return -1;
  }

  if (fdsWithData == 0) {
    errorPrint("Error: Timeout occurred waiting for read data.\n");
  }
  return (fdsWithData);  // If timeout has occurred, this will be 0,
                         // otherwise it will be the number of FDs
                         // with data.
}

// TODO: would be better to make this work based on any qty of characters
//  being allowed to be received until the timeout is reached.  Current
//  behavior only looks at one character and waits up to the timeout for it to
//  arrive
bool emAfBootloadWaitChar(uint8_t *data, bool expect, uint8_t expected)
{
  int tryTimes = 1;
  do
  {
     int status = checkFdForData();
    if (status <= 0) {
      // Timeout or error
      printf("emAfBootloadWaitChar timeout \n");
      continue;
    }

    ssize_t bytes = read(serialFd, data, 1);
    if (bytes < 0) {
      printf("Read failed: %s\n", strerror(errno));
      continue;
    }

    // debug 
   printf("Got <%c,%x>\n", (char)*data, (char)*data);

    if (expect)
     {
      if(((*data) == expected))
      {
        return true;
      }
      else{
        continue;
      }
    } else {
      return true;
    }
  } while (tryTimes--);
  return false;
}

