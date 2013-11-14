#include "gpio.h"

int gpio_pin_exists(int port)
{
	int pin[] = {60,61,62,63,96,97,98,99,100,101,102,103};
	int i;
	
	for(i=0; i < 12; i++)
	    if(port == pin[i])
	        return 0;
	return -1;
}        

int gpio_open(int port, int DDR)
{
	FILE *f;
	char file[35],ris[5];
	
	if(gpio_pin_exists(port) == -1)
	    return -1;
	
	f = fopen(EXPORT, "w");
	if (fprintf(f, "%d\n", port) < 0)
	    return -2;
	fclose(f);

	sprintf(file, "/sys/class/gpio/gpio%d/direction", port);
	f = fopen(file, "w");

	if (!DDR)
	    sprintf(ris,IN);
	else
	    sprintf(ris,OUT);
	    
	if (fprintf(f, "%s",ris) < 0)
	    return -2;
	
	fclose(f);
	
	return 0;
}

int gpio_close(int port)
{
	FILE *f;
	
	if(gpio_pin_exists(port) == -1)
	    return -1;
	
	f = fopen(UNEXPORT, "w");
	if(fprintf(f, "%d\n", port) < 0)
	    return -2;
	fclose(f);
	return 0;
}

int gpio_read(int port)
{
	FILE *f;
	char file[31];
	int i;
	
	if(gpio_pin_exists(port) == -1)
	    return -1;
	
	sprintf(file, "/sys/class/gpio/gpio%d/value", port);
	f = fopen(file, "r");
	fscanf(f, "%d", &i);
	fclose(f);
	return i;
}

int gpio_write(int port, int value){
	FILE *f;
	char file[35],ris[5];
	
	if (gpio_pin_exists(port) == -1)
	    return -1;
	
	sprintf(file, "/sys/class/gpio/gpio%d/value", port);
	f = fopen(file, "w");

	if (!value)
	    sprintf(ris,LOW);
	else
	    sprintf(ris,HIGH);
	
	if (fprintf(f, "%s",ris) < 0)
	    return -2;
	
	fclose(f);
	return 0;
}
