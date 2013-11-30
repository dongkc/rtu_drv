#ifndef __CONSEN_SHM_H__
#define __CONSEN_SHM_H__
// KW-Software eCLR Native Library Builder version 2.1.2.0
// CONSEN_SHM.h native assembly header template.
// use this file to implement and/or extend your native c++ classes.
// insert additional private methods and member only outside of the @Begin/@End comment pair

#include "mscorlib.h"

//extern unsigned char SHM_Buff_SYS[];
//extern unsigned char SHM_Buff_TCP[];
//extern unsigned char SHM_Buff_COM[];
//extern uint8 g_retain[];
bool fnConfigFileLoad();

// public scope class for library implementation
// use CONSEN_SHM::init() and CONSEN_SHM::loadLibrary() during startup 
// to make the library accessible by the managed application.
class CONSEN_SHM
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

class GT_SYSTEM_SHM : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Void* GetMemPtr();
    GT_SYSTEM_SHM();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 


};

class GT_TCP_SHM : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Void* GetMemPtr();
    GT_TCP_SHM();
    void ctor();
	
    // @End automatically generated code
    // insert additional private methods and member here ! 
};

class GT_COM_SHM : public Object
{
    // @Begin automatically generated code, do not modify inside @Begin/@End comment pair !
    public:
    static Void* GetMemPtr();
    GT_COM_SHM();
    void ctor();
    // @End automatically generated code
    // insert additional private methods and member here ! 
};


}; // class CONSEN_SHM

#endif //#ifndef __CONSEN_SHM_H__
