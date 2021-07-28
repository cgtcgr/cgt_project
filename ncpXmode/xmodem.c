#include "ota-bootload-ncp.h"

/*-----------------------------------------------------------------------------------*/
/* -----------------------------Target Dependent : code ---------------------------*/
/* 
 * This  blinks GPIO control.
 */
#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

#define PINCONFIG  104 /* P1-18 */
#define PINRESET 108  /* P1-07 */

static int
GPIOExport(int pin)
{
	#define BUFFER_MAX 4 /*warning*/
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int
GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int
GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";

#define DIRECTION_MAX 35
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

static int
GPIORead(int pin)
{
#define VALUE_MAX 30
	char path[VALUE_MAX];
	char value_str[3];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for reading!\n");
		return(-1);
	}

	if (-1 == read(fd, value_str, 3)) {
		fprintf(stderr, "Failed to read value!\n");
		return(-1);
	}

	close(fd);

	return(atoi(value_str));
}

static int
GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
	char path[VALUE_MAX];
	int fd;

	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return(-1);
	}

	if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}

int resetNcp()
{
	int repeat = 1;
	/*
	 * Enable GPIO pins
	 */
	if (-1 == GPIOExport(PINCONFIG) || -1 == GPIOExport(PINRESET))
	{
		return(1);
	}
		
	/*
	 * Set GPIO directions
	 */
	if (-1 == GPIODirection(PINCONFIG, OUT) || -1 == GPIODirection(PINRESET, OUT))
	{
		return(2);
	}
		
	do {
		/*
		 * Write GPIO value
		 */
		if (-1 == GPIOWrite(PINCONFIG, LOW))
		{
			return(3);		
		}		

		usleep(500 * 1000);
		if (-1 == GPIOWrite(PINRESET, LOW))
		{
			return(4);
		}
			
		usleep(500 * 1000);
		if (-1 == GPIOWrite(PINRESET, HIGH))
		{
			return(5);
		}
			
	}
	while (repeat--);

	/*
	 * Disable GPIO pins
	 */
	sleep(2);
	if (-1 == GPIOUnexport(PINCONFIG) || -1 == GPIOUnexport(PINRESET))
	{
		return(6);
	}
		
	return(0);
}

int exitNcp()
{
	int repeat = 1;
	/*
	 * Enable GPIO pins
	 */
	if (-1 == GPIOExport(PINCONFIG) || -1 == GPIOExport(PINRESET))
	{
		return(1);
	}
		
	/*
	 * Set GPIO directions
	 */
	if (-1 == GPIODirection(PINCONFIG, OUT) || -1 == GPIODirection(PINRESET, OUT))
	{
		return(2);
	}
		
	do {
		/*
		 * Write GPIO value
		 */
		if (-1 == GPIOWrite(PINCONFIG, LOW))
		{
			return(3);		
		}		

		usleep(500 * 1000);
		if (-1 == GPIOWrite(PINRESET, LOW))
		{
			return(4);
		}
			
		usleep(500 * 1000);
		if (-1 == GPIOWrite(PINRESET, HIGH))
		{
			return(5);
		}
			
	}
	while (repeat--);

	/*
	 * Disable GPIO pins
	 */
	sleep(1);
	if (-1 == GPIOUnexport(PINCONFIG) || -1 == GPIOUnexport(PINRESET))
	{
		return(6);
	}
		
	return(0);
}


int main()
{
	printf("start app \n");
	

	int ret  = resetNcp();
	printf("reset ncp ret = %d \n",ret);


	if(emberAfOtaBootloadCallback())
	{
		printf("restart ncp \n");
		//exitNcp();
	}
	return 1;
}

