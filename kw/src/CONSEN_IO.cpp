// CONSEN_IO.cpp native assembly code template.
// KW-Software embedded CLR native assembly builder tool generated source file.
// use this file to implement your own native code

#include "eclr.h"
#include "CONSEN_IO.h"
#include "CONSEN_SHM.h"
#include "modbus.h"

extern "C" void bsp_init(void);
extern "C" void bsp_input_update(void);
extern "C" void bsp_output_update(void);

//8路ai输入
unsigned short adc_buf[32];
// 8路DI输入
unsigned char input_buf[32];
// 8路DO输出
unsigned char output_buf[32];

extern unsigned char SHM_Buff_SYS[];

bool CONSEN_IO::init()
{
// if necessary, insert some initialization code for the CONSEN_IO library here !
// call CONSEN_IO::init() somewhere before CONSEN_IO::loadLibrary() is called.
// loadLibrary() have to be called after the ClrContoller object is instanciated
// for detailed information see the native library builder documentation
    return true;
}

bool CONSEN_IO::isCompatible(const EclrVersion& requestedVersion)
{
// this function is called by the runtime if an application will be bound
// to this library with the requested version.
// return true means that this library fits to the requested interface.
// return false forces the runtime to search for other libraries with the same name by calling isCompatible()
// if no libraries could be found, the runtime breaks the loading process with linker-error
// for multiple library version support read the documentation.
    return true;
}

bool CONSEN_IO::load(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be started.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to initialize some internal objects.
// If the initialization is done ok, return true
// othewise return false.
    return true;
}

bool CONSEN_IO::unload(ClrAppDomain* pAppDomain)
{
// This function is called by the runtime if an application will be terminated.
// pAppDomain is the domain object witch hosts the current application process.
// At this point, the firmware library is able to finalize some internal objects.
// If the finalization is done ok, return true
// othewise return false.
    return true;
}

// class CONSEN_IO.KWIODriver implementation
CONSEN_IO::KWIODriver::KWIODriver()
{
// implement your constructor code here !
}

void __PInvoke__ CONSEN_IO::KWIODriver::ctor()
{
// automatically generated auxiliary contructor : calls the contructor by displacement new operator !
// @Begin automatically generated code, do not modify !
    new (this) CONSEN_IO::KWIODriver();
// @End automatically generated code
}

void __PInvoke__ CONSEN_IO::KWIODriver::Init(SZArray* p0, SZArray* p1)
{
// implement your code here !
}

void __PInvoke__ CONSEN_IO::KWIODriver::Open()
{
// implement your code here !
	printf("KWIODriver::Open()\n");
	bsp_init();
}

void __PInvoke__ CONSEN_IO::KWIODriver::Close()
{
// implement your code here !
	printf("KWIODriver::Close()\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::BeginReadAll()
{
// implement your code here !
	printf("BeginReadAll\n");
	bsp_input_update();

    for (int i = 0; i < 4; ++i) {
        SHM_Buff_SYS[i] = modbus_get_byte_from_bits(input_buf, i * 8, 8);
    }

    uint16_t *adc_ptr = (uint16_t*)(&SHM_Buff_SYS[512]);
    for (int i = 0; i < 32; ++i) {
        *adc_ptr++ = adc_buf[i];
    }

}

void __PInvoke__ CONSEN_IO::KWIODriver::BeginReadGroup(Int32 p0)
{
// implement your code here !
	printf("BeginReadGroup\n");
}

SByte __PInvoke__ CONSEN_IO::KWIODriver::ReadSByte(Int32 p0)
{
// implement your code here !
	if(p0<512)
	{
		return SHM_Buff_SYS[p0];
	}
	else
	{
		return 0;
	}

}

Int16 __PInvoke__ CONSEN_IO::KWIODriver::ReadInt16(Int32 p0)
{
// implement your code here !
    if(p0<1024)
	{
		return (Int16)((Int32)SHM_Buff_SYS[512+p0*2] +(Int32)SHM_Buff_SYS[512+p0*2+1]*256);
	}
	else
	{
		return 0;
	}
}

Int32 __PInvoke__ CONSEN_IO::KWIODriver::ReadInt32(Int32 p0)
{
// implement your code here !
printf("ReadInt32\n");
    return 0;
}

Byte __PInvoke__ CONSEN_IO::KWIODriver::ReadByte(Int32 p0)
{
// implement your code here !
	if(p0<512)
	{
		return SHM_Buff_SYS[p0];
	}
	else
	{
		return 0;
	}

}

UInt16 __PInvoke__ CONSEN_IO::KWIODriver::ReadUInt16(Int32 p0)
{
// implement your code here !
   if(p0<1024)
	{
		return (UInt16)((UInt32)SHM_Buff_SYS[512+p0*2] +(UInt32)SHM_Buff_SYS[512+p0*2+1]*256);
	}
	else
	{
		return 0;
	}

}

UInt32 __PInvoke__ CONSEN_IO::KWIODriver::ReadUInt32(Int32 p0)
{
// implement your code here !
	printf("ReadUInt32");
    return 0;
}

UInt32 __PInvoke__ CONSEN_IO::KWIODriver::ReadSingle(Int32 p0)
{
// implement your code here !
	if(p0<128)
	{
		return (UInt32)SHM_Buff_SYS[2560+p0*4]+(UInt32)(SHM_Buff_SYS[2560+p0*4+1]<<8)+(UInt32)(SHM_Buff_SYS[2560+p0*4+2]<<16)+(UInt32)(SHM_Buff_SYS[2560+p0*4+3]<<24);

		
	}
	else
	{
		return 0;
	}
}

UInt64 __PInvoke__ CONSEN_IO::KWIODriver::ReadDouble(Int32 p0)
{
// implement your code here !
	printf("ReadDouble");
    return 0;
}

void __PInvoke__ CONSEN_IO::KWIODriver::ReadByteBuffer(Int32 p0, Byte* p1, Int32 p2)
{
// implement your code here !

}

void __PInvoke__ CONSEN_IO::KWIODriver::EndReadGroup()
{
// implement your code here !
	printf("EndReadGroup\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::EndReadAll()
{
// implement your code here !
	printf("EndReadAll\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::BeginWriteAll()
{
// implement your code here !
	printf("BeginWriteAll\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::BeginWriteGroup(Int32 p0)
{
// implement your code here !
printf("BeginWriteGroup\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteSByte(Int32 p0, SByte p1)
{
// implement your code here !
	if(p0<512)
	{
	SHM_Buff_SYS[3072+p0]=p1;
	}
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteInt16(Int32 p0, Int16 p1)
{
// implement your code here !
	if(p0<256)
	{
		SHM_Buff_SYS[3584+p0*2]=(Byte)(p1&0xff);
		SHM_Buff_SYS[3584+p0*2+1]=(Byte)(p1>>8);
	}
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteInt32(Int32 p0, Int32 p1)
{
// implement your code here !

}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteByte(Int32 p0, Byte p1)
{
// implement your code here !
	if(p0<512)
	{
	SHM_Buff_SYS[3072+p0]=p1;
	}
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteUInt16(Int32 p0, UInt16 p1)
{
// implement your code here !
		if(p0<256)
	{
		SHM_Buff_SYS[3584+p0*2]=(Byte)(p1&0xff);
		SHM_Buff_SYS[3584+p0*2+1]=(Byte)(p1>>8);
	}
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteUInt32(Int32 p0, UInt32 p1)
{
// implement your code here !
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteSingle(Int32 p0, UInt32 p1)
{
// implement your code here !
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteDouble(Int32 p0, UInt64 p1)
{
// implement your code here !
}

void __PInvoke__ CONSEN_IO::KWIODriver::WriteByteBuffer(Int32 p0, Byte* p1, Int32 p2)
{
// implement your code here !
	printf("WriteByteBuffer\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::EndWriteGroup()
{
// implement your code here !
	printf("EndWriteGroup\n");
}

void __PInvoke__ CONSEN_IO::KWIODriver::EndWriteAll()
{
// implement your code here !
	printf("EndWriteAll\n");

    modbus_set_bits_from_bytes(output_buf, 0, 32, &SHM_Buff_SYS[3 * 1024]);
	bsp_output_update();
}

