#ifndef __GT_LIB10_H__
#define __GT_LIB10_H__
// KW-Software eCLR Native Library Builder version 2.1.2.0
// GT_LIB10.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "mscorlib.h"

// public scope class for library implementation
// use GT_LIB10::init() and GT_LIB10::loadLibrary() during startup 
// to make the library accessible by the managed application.
class GT_LIB10
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

class GT_FUN_ADD : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Int32 __Process(Int32 p0, Int32 p1, Int32 p2, Int32 p3, Int32 p4, Int32 p5, Int32 p6, Int32 p7);
    GT_FUN_ADD();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class GT_FUN_MUL : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Int32 __Process(Int32 p0, Int32 p1, Int32 p2, Int32 p3, Int32 p4, Int32 p5, Int32 p6, Int32 p7);
    GT_FUN_MUL();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class GT_FB_PID1 : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    GT_FB_PID1();
    void ctor();
    void __Init();
    void __Process();
    Boolean EN1;
    Boolean CLK1;
    Boolean COUNT1;
    Int32 COUNT_old1;
    Int32 COUNT0;
    Boolean ENO1;
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class GT_FB_PID2 : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    GT_FB_PID2();
    void ctor();
    void __Init();
    void __Process();
    Int32 In0;
    Int32 In1;
    Int32 Out0;
    Int32 Out1;
    Int32 lTemp1;
    // @End automatically generated code
    // insert additional private methods and member here ! 
};


}; // class GT_LIB10

#endif //#ifndef __GT_LIB10_H__
