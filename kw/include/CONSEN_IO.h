#ifndef __CONSEN_IO_H__
#define __CONSEN_IO_H__
// KW-Software eCLR Native Library Builder version 2.1.2.0
// CONSEN_IO.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "mscorlib.h"
#include "pcoslib.h"

// public scope class for library implementation
// use CONSEN_IO::init() and CONSEN_IO::loadLibrary() during startup 
// to make the library accessible by the managed application.
class CONSEN_IO
{
// @Begin automatically generated code, do not modify !
public:
enum LibVersion { Major=1, Minor=0, Build=0, Revision=0 };
static bool init();
static bool loadLibrary();
static bool isCompatible(const EclrVersion& requestedVersion);
static bool load(ClrAppDomain* pAppDomain);
static bool unload(ClrAppDomain* pAppDomain);
static unsigned s_modulHndl;
// @End automatically generated code

class KWIODriver : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    KWIODriver();
    void ctor();
    void Init(SZArray* p0, SZArray* p1);
    void Open();
    void Close();
    void BeginReadAll();
    void BeginReadGroup(Int32 p0);
    SByte ReadSByte(Int32 p0);
    Int16 ReadInt16(Int32 p0);
    Int32 ReadInt32(Int32 p0);
    Byte ReadByte(Int32 p0);
    UInt16 ReadUInt16(Int32 p0);
    UInt32 ReadUInt32(Int32 p0);
    UInt32 ReadSingle(Int32 p0);
    UInt64 ReadDouble(Int32 p0);
    void ReadByteBuffer(Int32 p0, Byte* p1, Int32 p2);
    void EndReadGroup();
    void EndReadAll();
    void BeginWriteAll();
    void BeginWriteGroup(Int32 p0);
    void WriteSByte(Int32 p0, SByte p1);
    void WriteInt16(Int32 p0, Int16 p1);
    void WriteInt32(Int32 p0, Int32 p1);
    void WriteByte(Int32 p0, Byte p1);
    void WriteUInt16(Int32 p0, UInt16 p1);
    void WriteUInt32(Int32 p0, UInt32 p1);
    void WriteSingle(Int32 p0, UInt32 p1);
    void WriteDouble(Int32 p0, UInt64 p1);
    void WriteByteBuffer(Int32 p0, Byte* p1, Int32 p2);
    void EndWriteGroup();
    void EndWriteAll();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};


}; // class CONSEN_IO

#endif //#ifndef __CONSEN_IO_H__
