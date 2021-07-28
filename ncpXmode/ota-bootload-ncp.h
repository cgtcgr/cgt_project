#ifndef NCP_H_ 
#define NCP_H_ 

#include <stdlib.h>

#include <stdio.h>
 
#include <string.h>
 
#include <errno.h>
 
#include <unistd.h>
 
#include <sys/types.h>
 
#include <sys/stat.h>
 
#include <fcntl.h>
 
#include <termios.h>
 
#include <unistd.h>

#include <stdbool.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
bool emAfStartNcpBootloaderCommunications(void);
void emAfPostNcpBootload(bool success);
bool emAfRebootNcpAfterBootload(void);

// These primitives are called by the Xmodem code to pass data via
// the specific mechanism (UART or SPI).
bool emAfBootloadSendData(const uint8_t *data, uint16_t length);
bool emAfBootloadSendByte(uint8_t byte);
bool emAfBootloadWaitChar(uint8_t *data, bool expect, uint8_t expected);
bool emberAfOtaBootloadCallback();

 int uart_init();

#endif