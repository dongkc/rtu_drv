#ifndef __GCHEAP_H__
#define __GCHEAP_H__

#include "blobheap.h"
#include "log.h"

#ifdef __DEBUG__    
#define GCHEAP_VERIFY
#endif


class ClrTypeStaticData;
class ClrField;
class CMutexImpl;
class ClrGcHeap : protected BlockHeap
{
public:
    enum NodeType
    {
        DataObject          = 0x40,
        // heap object modifier
        Unused              = 0x01,
        Marked              = 0x02,
        Auxiliary           = 0x04,
        Used                = 0x08,
        Static              = 0x20,

        StaticDataObject    = DataObject | Static,
        UnusedDataObject    = DataObject | Unused,
        TypeMask            = 0x7F,
        Any                 = 0
    }; 
    ClrGcHeap();
    ~ClrGcHeap();

    bool Release();
    bool Release(BlockHeap* pMetaHeap);
    void Collect();
    void CollectAux(BlockHeap* pMetaHeap, Object** pRefTab[], unsigned refSize);
    unsigned GetReferenceAmount(Object* pObject);
    unsigned GetReferenceList(Object* pObject, Object* pBase[] = NULL, ClrField* pFields[] = NULL, unsigned refSize = 0);
    unsigned GetReferenceListAll(Object* pObject, Object* pBase[], ClrField* pFields[], int fieldOffset[], unsigned refSize);
    const char* GetRefPath(Object* pObj);
    Object*  GetStaticObject(ClrTypeDef* pTypeInfo);
    Object* GetObject(void* pData, ClrField*& pField);
    Object* GetObject(void* pData, unsigned& offset);
    Object* GetObject(void* pData);
    bool IsObject(void* pData);
    Object* Alloc(ClrType* pType, unsigned size);
    Object* AllocStatic(ClrType* pType, unsigned size);
    bool Assign(void* pData, unsigned nDataSize);
    void MutexEnter();
    void MutexLeave();
    
    void Statistic(CHeapStatistic& stat)
    {
        statistic(stat);
    }
    inline void* Base()
    {
        return base();
    }
    inline unsigned Size()
    {
        return size();
    }
    inline void Clear()
    {
        clear();
    }
    inline bool Validate(void* p)
    {
        return validate(p);
    }
    inline void SetAppDomain(ClrAppDomain* pAppDomain)
    {
        m_pAppDomain = pAppDomain;
    }
    inline ClrAppDomain* GetAppDomain()
    {
        return m_pAppDomain;
    }
    bool IsCurrDomain(Object* pObj);

    // heap object semantic
    static bool IsStatic(Object* pObject);
    static void SetAuxFlag(Object* pObject);
    static void ResetAuxFlag(Object* pObject);
    static bool IsAuxFlag(Object* pObject);
    static void SetUnused(Object* pObject);
    static bool IsUnused(Object* pObject);
  
    unsigned GetSize(Object* pObject)
    {
        BlockHeap::iterator it = (void*)pObject;
        return (*it).size();
    }

    void Dump();
    void DumpReferenceList(Object* pRefObj);
    bool CheckUnmanagedPointer();

    Object* SBegin();
    Object* SNext(Object* pObj);
    Object* DBegin();
    Object* DNext(Object* pObj);
    Object* Begin();
    Object* Next(Object* pObj);

private:
    inline static bool typeVerify(int type)
    {
        return ((type & DataObject) == DataObject) ? true : false;
    }
    inline static bool isData(int type)
    {
        return ((type & DataObject) == DataObject) ? true : false;
    }
    inline static bool isUnused(int type)
    {
        return ((type & UnusedDataObject) == UnusedDataObject) ? true : false;
    }
    inline static bool isStatic(int type)
    {
        return ((type & StaticDataObject) == StaticDataObject) ? true : false;
    }
    inline static bool isInstance(int type)
    {
        return ((type & StaticDataObject) == DataObject) ? true : false;
    }
    inline static bool isAuxiliary(int type)
    {
        return ((type & Auxiliary) == Auxiliary) ? true : false;
    }
    inline static bool isMarked(int type)
    {
        return ((type & Marked) == Marked) ? true : false;
    }
    inline static bool isUsed(int type)
    {
        return ((type & Used) == Used) ? true : false;
    }
    static void setUsed(Object* pObject);
    static void setUsed(Object** ppInst, Object** pRefTab[], unsigned refTabSize);

    void markAndSweep();
    void markRefUsed();
    void markAndSweepAux(BlockHeap* pMetaHeap);
    void finalize(Object* pObj);
    void setRefUsed(Object* pObj);
    void setRefUsedStatic(Object* pObj);
    void setRefUsedValueType(void* pInst, ClrTypeDef* pTypeInfo);
    void checkObject(Object* pObj, ClrTypeDef* pTypeInfo);
    static int getType(Object* pObject)
    {
        BlockHeap::iterator it = (void*)pObject;
        int type = (*it).type();
    #ifdef GCHEAP_VERIFY
        if (typeVerify(type) == false)
        {
            DBG((LF_BLOBHEAP, LL_ERROR, "illegal type %02X at %08X", type, pObject));
        }
    #endif
        return type;
    }
    ClrAppDomain* m_pAppDomain;
    Object***     m_pRefTab;
    unsigned      m_refTabSize;
#ifdef PLATFORM_THREADING_SUPPORT
    CMutexImpl*   m_pMutex;
#endif
    // TEST
    public:
    int m_refUsedCnt;
    int m_loop1;
};


#endif //#define __GCHEAP_H__

