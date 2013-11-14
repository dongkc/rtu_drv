#ifndef __APPDEBUG_H__
#define __APPDEBUG_H__

class ClrMethodFrame
{
public:
    void GetEntries(Object*& pThis, ClrMethodFrame*& pNext, void*& pCaller);
    // cil-version >= 2.0.0
    void GetEntries2(Object*& pThis, ClrMethodFrame*& pNext, void*& pCaller);
};

class ClrAppBreakpoint : public eListNode<ClrAppBreakpoint>, public ClrAllocator
{
public:
    enum BreakpointType
    {
        BpPermanent     = 1,
        BpTemporary     = 2,
        BpContinue      = 4,
        BpTemporaryAndContinue = BpTemporary|BpContinue,
        BpAll           = BpTemporary|BpPermanent,
        BpNone          = 0
    };

    ClrAppBreakpoint(BpHandlerStub pBpHandler, void* pCode, uint32 methHandle, uint32 methOffs, int bpType);
    ~ClrAppBreakpoint();
    bool Set();
    bool Reset();
    bool IsItem(void* pCode)
    {
        return (m_pCode == pCode) ? true : false;
    }
    inline void* GetCode()
    {
        return m_pCode;
    }

    inline unsigned GetOffset()
    {
        return m_methOffs;
    }
    inline unsigned GetMethodHandle()
    {
        return m_methHandle;
    }
    inline int GetType()
    {
        return m_bpType;
    }
    inline void SetType(int type)
    {
        m_bpType = type;
    }
    inline bool IsTypePermanent()
    {
        return (m_bpType & BpPermanent) ? true : false;
    }
    inline bool IsTypeTemporary()
    {
        return (m_bpType & BpTemporary) ? true : false;
    }
    inline bool IsTypeContinue()
    {
        return (m_bpType & BpContinue) ? true : false;
    }
private:
    BpHandlerStub m_pBpHandler;
    void*  m_pCode;
    uint32 m_methHandle;
    uint32 m_methOffs;
    int    m_bpType;
    int    m_backupSize;
    char   m_backup[16];

};

class ClrDebugFrame : public eListNode<ClrDebugFrame>, public ClrAllocator
{
public:
    ClrDebugFrame();
    ClrDebugFrame(ClrMethodFrame* pMethodFrame, Object* pThis, uint32 methHandle, uint32 methOffset);
    inline void* GetThisPointer()
    {
        return m_pThis;
    }
    inline void* GetFramePointer()
    {
        return m_pMethodFrame;
    }
    inline uint32 GetMethHandle()
    {
        return m_methHandle;
    }
    inline uint32 GetMethOffset()
    {
        return m_methOffs;
    }
    void Clone(const ClrDebugFrame& frame);
private:
    ClrMethodFrame* m_pMethodFrame;
    Object* m_pThis;
    uint32 m_methHandle;
    uint32 m_methOffs;
};

class ClrDebugChain : public eListNode<ClrDebugChain>, public ClrAllocator
{
public:
    ClrDebugChain(EThread* pThread, int excCode);
    ~ClrDebugChain();
    bool InsertFrame(ClrMethodFrame* pMethodFrame, Object* pThis, uint32 methHandle, uint32 methOffset);
    inline EThread* GetThread()
    {
        return m_pThread;
    }
    inline int GetException()
    {
        return m_excCode;
    }
    unsigned GetFrameCount();
    bool GetFrame(ClrDebugFrame& frame, unsigned index);
    static unsigned GetId()
    {
        return s_id;
    }
    void Dump(ClrAppDomain* pAppDomain);
private:
    eList<ClrDebugFrame> m_frameList;
    EThread*    m_pThread;
    int         m_excCode;
    static unsigned s_id;

};

#endif //#define __APPDEBUG_H__
