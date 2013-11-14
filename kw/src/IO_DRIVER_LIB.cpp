// IO_DRIVER_LIB.cpp native assembly code template.
// KW-Software embedded CLR native assembly builder tool generated source file.
// use this file to implement your own native code

#include "eclr.h"
#include "IO_DRIVER_LIB.h"

/*
#include "gpio.h"
//DO 管脚为: 60, 61, 62, 63
#define GPIO_PIN_D01 60
#define GPIO_PIN_DO0 61
#define GPIO_PIN_DO3 62
#define GPIO_PIN_DO2 63


#define GPIO_PIN_DI0    103      //DI0
#define GPIO_PIN_DI1    101      //DI1
#define GPIO_PIN_DI2     99      //DI2
#define GPIO_PIN_DI3     97      //DI3
#define GPIO_PIN_DI4     96      //DI4
#define GPIO_PIN_DI5    102      //DI5
#define GPIO_PIN_DI6     98      //DI6
#define GPIO_PIN_DI7    100      //DI7
 int gpio_open(int port, int DDR);
int gpio_close(int port);
int gpio_read(int port);
int gpio_write(int port, int value);
*/


extern "C" void bsp_init(void);
extern "C" void bsp_input_update(void);
extern "C" void bsp_output_update(void);


//8路ai输入
unsigned short adc_buf[8];
// 8路DI输入
unsigned char input_buf[8];
// 8路DO输出
unsigned char output_buf[8];


bool IO_DRIVER_LIB::init()
{
// if necessary, insert some initialization code for the IO_DRIVER_LIB library here !
// call IO_DRIVER_LIB::init() somewhere before IO_DRIVER_LIB::loadLibrary() is called.
// loadLibrary() have to be called after the ClrContoller object is instanciated
// for detailed information see the native library builder documentation
    return true;
}

bool IO_DRIVER_LIB::isCompatible(const EclrVersion& requestedVersion)
{
// this function is called by the runtime if an application will be bound
// to this library with the requested version.
// return true means that this library fits to the requested interface.
// return false forces the runtime to search for other libraries with the same name by calling isCompatible()
// if no libraries could be found, the runtime breaks the loading process with linker-error
// for multiple library version support read the documentation.
    return true;
}

bool IO_DRIVER_LIB::load(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be started.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to initialize some internal objects.
// If the initialization is done ok, return true
// othewise return false.
    return true;
}

bool IO_DRIVER_LIB::unload(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be terminated.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to finalize some internal objects.
// If the finalization is done ok, return true
// othewise return false.
    return true;
}

// class IO_DRIVER_LIB.KWIODriver implementation
IO_DRIVER_LIB::KWIODriver::KWIODriver()
{
// implement your constructor code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::ctor()
{
// automatically generated auxiliary contructor : calls the contructor by displacement new operator !
// @Begin automatically generated code, do not modify !
    new (this) IO_DRIVER_LIB::KWIODriver();
// @End automatically generated code
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::Init(SZArray* p0, SZArray* p1)
{


	
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::Open()
{
	bsp_init();
/*
printf("IO_DRIVER_LIB::KWIODriver::Open()\n");

int error = 0;	
	if (gpio_open(GPIO_PIN_D01, GPIO_WRITE) < 0) {
	    puts("Unable to open PIN file");
	  //  return -1;
	}	
	if (gpio_open(GPIO_PIN_DO0, GPIO_WRITE) < 0) {
	    puts("Unable to open PIN file");
	 //   return -1;
	}
	if (gpio_open(GPIO_PIN_DO3, GPIO_WRITE) < 0) {
	    puts("Unable to open PIN file");
	  //  return -1;
	}
	if (gpio_open(GPIO_PIN_DO2, GPIO_WRITE) < 0) {
	    puts("Unable to open PIN file");
	    //return -1;
	}


	if (gpio_open(GPIO_PIN_DI0, GPIO_READ) < 0) {
	    puts("8 Unable to open PIN file");
	    //return -1;
	}
	if (gpio_open(GPIO_PIN_DI1, GPIO_READ) < 0) {
	    puts("6 Unable to open PIN file");
	    //return -1;
	}
	if (gpio_open(GPIO_PIN_DI2, GPIO_READ) < 0) {
	    puts("4 Unable to open PIN file");
	    //return -1;
	}
	if (gpio_open(GPIO_PIN_DI3, GPIO_READ) < 0) {
	    puts("2 Unable to open PIN file");
	    //return -1;
	}
	if (gpio_open(GPIO_PIN_DI4, GPIO_READ) < 0) {
	    puts("1 Unable to open PIN file");
	    //return -1;
	}
	if (gpio_open(GPIO_PIN_DI5, GPIO_READ) < 0) {
	    puts("7 Unable to open PIN file");
	    //return -1;
	}	
	if (gpio_open(GPIO_PIN_DI6, GPIO_READ) < 0) {
	    puts("3 Unable to open PIN file");
	    //return -1;
	}
	if (gpio_open(GPIO_PIN_DI7, GPIO_READ) < 0) {
	    puts("5 Unable to open PIN file");
	   // return -1;
	}
*/
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::Close()
{
/*
	gpio_close(GPIO_PIN_DO0);
	gpio_close(GPIO_PIN_D01);
	gpio_close(GPIO_PIN_DO2);
	gpio_close(GPIO_PIN_DO3);


	gpio_close(GPIO_PIN_DI4);
	gpio_close(GPIO_PIN_DI3);
	gpio_close(GPIO_PIN_DI6);
	gpio_close(GPIO_PIN_DI2);
	gpio_close(GPIO_PIN_DI7);
	gpio_close(GPIO_PIN_DI1);
	gpio_close(GPIO_PIN_DI5);
	gpio_close(GPIO_PIN_DI0);
*/
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginReadAll()
{
// implement your code here !
	bsp_input_update();
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginReadGroup(Int32 p0)
{
// implement your code here !
}

SByte __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadSByte(Int32 p0)
{
// implement your code here !
    return 0;
}

Int16 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadInt16(Int32 p0)
{
// implement your code here !
	return adc_buf[p0-8];
    return 0;
}

Int32 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadInt32(Int32 p0)
{
// implement your code here !
    return 0;
}

Byte __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadByte(Int32 p0)
{
/*
return 
gpio_read(GPIO_PIN_DI0) 
+ (gpio_read(GPIO_PIN_DI0) <<1)
+ (gpio_read(GPIO_PIN_DI1) <<2)
+ (gpio_read(GPIO_PIN_DI2) <<3)
+ (gpio_read(GPIO_PIN_DI3) <<4)
+ (gpio_read(GPIO_PIN_DI4) <<5)
+ (gpio_read(GPIO_PIN_DI5) <<6)
+ (gpio_read(GPIO_PIN_DI6) <<7);
;
*/
    return input_buf[p0];
    return 0;
}

UInt16 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadUInt16(Int32 p0)
{
// implement your code here !
    return 0;
}

UInt32 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadUInt32(Int32 p0)
{
// implement your code here !
    return 0;
}

UInt32 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadSingle(Int32 p0)
{
// implement your code here !
    return 0;
}

UInt64 __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadDouble(Int32 p0)
{
// implement your code here !
    return 0;
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::ReadByteBuffer(Int32 p0, Byte* p1, Int32 p2)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndReadGroup()
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndReadAll()
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginWriteAll()
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::BeginWriteGroup(Int32 p0)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteSByte(Int32 p0, SByte p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteInt16(Int32 p0, Int16 p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteInt32(Int32 p0, Int32 p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteByte(Int32 p0, Byte p1)
{
/*
int error = 0;
if (p0 != 0) return ;
if ((p1&0x01))
	    error = gpio_write(GPIO_PIN_DO0, 0);
else	   
	    error = gpio_write(GPIO_PIN_DO0, 1);
if ((p1&0x02))	    
	    error = gpio_write(GPIO_PIN_D01, 0);
else	    
	    error = gpio_write(GPIO_PIN_D01, 1);
if ((p1&0x04))	    
	    error = gpio_write(GPIO_PIN_DO2, 0);
else	    
	    error = gpio_write(GPIO_PIN_DO2, 1);
if ((p1&0x08))	    
	    error = gpio_write(GPIO_PIN_DO3, 0);
else	   
	    error = gpio_write(GPIO_PIN_DO3, 1);
*/	  
output_buf[p0] = p1;  
	
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteUInt16(Int32 p0, UInt16 p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteUInt32(Int32 p0, UInt32 p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteSingle(Int32 p0, UInt32 p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteDouble(Int32 p0, UInt64 p1)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::WriteByteBuffer(Int32 p0, Byte* p1, Int32 p2)
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndWriteGroup()
{
// implement your code here !
}

void __PInvoke__ IO_DRIVER_LIB::KWIODriver::EndWriteAll()
{
// implement your code here !
	bsp_output_update();
}

