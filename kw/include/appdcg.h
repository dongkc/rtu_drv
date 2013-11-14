#ifndef __APPDCG_H__
#define __APPDCG_H__

#include "gcheap.h"

#define UNMANAGED_POINTER_TREATMENT_NEWSTYLE

typedef struct _clrdlcstatistic
{
    uint32 m_usedMemory;
    uint32 m_timeAll;
    uint32 m_timeDlcProlog;
    uint32 m_timeAppStart;
    uint32 m_timeCreateCopyType;
    uint32 m_timeCreateCopyList;
    uint32 m_timeRelocateStatic;
    uint32 m_timeIdcgProlog;
    
    uint32 m_timeDlcChange;
    uint32 m_timeCopyList;
    uint32 m_timeIdcgChange;
    uint32 m_timeSwapList;
    uint32 m_timeMetaSwapList;
    
    uint32 m_timeDlcEpilog;
    uint32 m_timeIdcgEpilog;
    uint32 m_timeGCEpilog;
    uint32 m_timeAppTerminate;
    
    uint16 m_trials;

} ClrDlcStatistic;

class ClrAppDcg : public ClrAllocator
{
public:
    enum LastError
    {
        NoError                     = 0,
        NoMemory                    = 1,
        UnmanagedPointerIllegal     = 2,
        RetainIllegal               = 3,
        IdcgPrologFailed            = 4,
        IdcgTypeChanged             = 5,
        RealtimeViolation           = 6,
        Internal                    = 7 
    };

    ClrAppDcg(ClrAppDomain* pBackgroundDomain);
    bool Prolog(ClrDlcStatistic& stat);
    void ExecCopyList(ClrDlcStatistic& stat);
    void Change(ClrDlcStatistic& stat);
    bool Epilog(ClrDlcStatistic& stat);
    void SetLastError(enum LastError lastError)
    {
        m_dcgLastError = lastError;
    }
    unsigned GetLastError()
    {
        return m_dcgLastError;
    }
    
    // currently there is a problem with ms embedded studio 
    // to support nested classes in a private context !
//private:
    enum InstanceType
    {
        NorGround  = 0,
        ForeGround = 1,
        BackGround = 2,
        IdcgImplementation = 3
    };
    
    class CopyTypeBase : public ClrAllocator
    {
    public:
        CopyTypeBase(ClrTypeDef* pTypeInfo)
        {
            m_pTypeInfo = pTypeInfo;
        }
        virtual ~CopyTypeBase();
        virtual void Copy(Object* p1, Object* p2);
        // for debug purposes 
        virtual unsigned CopyBytes();
        virtual void Dump();

        ClrTypeDef* m_pTypeInfo;                                // associated type info
    };

 
    class IDcgEntry : public eListNode<IDcgEntry>, public CopyTypeBase //public ClrAllocator
    {
    public:
        IDcgEntry(ClrTypeDef* pTypeInfo, ClrMethod* pChange, ClrMethod* pProlog, ClrMethod* pEpilog);
        bool Prolog(Object* p1, Object* p2);
        void Change(Object* p1, Object* p2);
        void Epilog(Object* p1, Object* p2);
        bool Prolog(ClrAppDomain* pAppDomain, Object* p1, Object* p2);
        void Change(ClrAppDomain* pAppDomain, Object* p1, Object* p2);
        void Epilog(ClrAppDomain* pAppDomain, Object* p1, Object* p2);
        inline bool isDefaultImpl()
        {
            return m_isDefaultImpl;
        }

        // virtual override
        void Copy(Object* p1, Object* p2);

    private:
        void (IDcgEntry::* m_pChange)(Object*);
        bool (IDcgEntry::* m_pProlog)(Object*);
        void (IDcgEntry::* m_pEpilog)(Object*);
        bool m_isDefaultImpl;
        
        void ChangeDefault(Object* p);
        bool PrologDefault(Object* p);
        void EpilogDefault(Object* p);

    };
    class IDcgList
    {
    public:
        ~IDcgList();
        bool CreateEntry(ClrTypeDef* pTypeInfo, ClrMethod* pChange = NULL, ClrMethod* pProlog = NULL, ClrMethod* pEpilog = NULL);
        IDcgEntry* GetEntry(ClrTypeDef* pTypeInfo);
        unsigned Size()
        {
            return m_idcgList.size();
        }
        void Release();
        void Dump();

    private:
        eList<IDcgEntry> m_idcgList;
    };
    class IDcgInstEntry : public eListNode<IDcgInstEntry>, public ClrAllocator
    {
    public:
        IDcgInstEntry(Object* pInst1, Object* pInst2, IDcgEntry* pEntry);
        void Change();
        bool Prolog();
        void Epilog();

    //private:
        Object*     m_pInst1;
        Object*     m_pInst2;
        IDcgEntry*  m_pIDcg;
    };
    class IDcgInstList
    {
    public:
        IDcgInstList()
        {
            m_pAppDomain = NULL;
        }
        ~IDcgInstList();
        bool CreateEntry(Object* pInst1, Object* pInst2, IDcgEntry* pEntry);
        unsigned Size()
        {
            return m_idcgInstList.size();
        }
        void SetAppDomain(ClrAppDomain* pAppDomain)
        {
            m_pAppDomain = pAppDomain;
        }
        void Change();
        bool Prolog();
        void Epilog();
        void Release();
        void Dump();
    private:
        eList<IDcgInstEntry> m_idcgInstList;
        ClrAppDomain*    m_pAppDomain;
    };
    class RelocList
    {
    public:
        RelocList();
        ~RelocList();

        bool AddEntry(ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2, Object* pStaticInst);
        bool Create(unsigned listSize);

        unsigned Size()
        {
            return m_nEntry;
        }
        unsigned Capacity()
        {
            return m_nListSize;
        }
        void Release();
        bool Process(ClrAppDomain* pAppDomain);
        void Dump();
    private:
        unsigned m_nListSize;
        unsigned m_nEntry;
        ClrTypeDef**        m_pTypeInfoTab;
        ClrTypeStaticData** m_pStaticDataTab;
    };
    class MetaSwapList
    {
    public:
        MetaSwapList();
        ~MetaSwapList();

        bool AddEntry(Object* pInst, ClrType* pType);
        bool AddEntryUnused(Object* pInst, ClrType* pType);
        void SetUnused();
        void ResetAux();


        bool Create(unsigned listSize);

        unsigned Size()
        {
            return m_nEntry;
        }
        unsigned Capacity()
        {
            return m_nListSize;
        }
        void Release();
        void Process();
        bool IsEntry(Object* pObject);
        void Dump();
        void DumpUnused();
    private:
        typedef struct _swapentry
        {
            Object*     m_pInst;
            ClrType*    m_pType;
        } SwapEntry;

        SwapEntry*  m_pEntryTab;
        unsigned    m_nListSize;
        unsigned    m_nEntry;
        unsigned    m_nEntryUnused;
    };

    class TypeMapList
    {
    public:
        TypeMapList();
        ~TypeMapList();

        bool AddEntry(ClrType* pType1, ClrType* pType2);
        bool Create(unsigned listSize);
        ClrType* GetType2(ClrType* pType1);
        ClrType* GetType1(ClrType* pType2);

        unsigned Size()
        {
            return m_nEntry;
        }
        unsigned Capacity()
        {
            return m_nListSize;
        }
        void Release();
        void Dump();
    private:
        typedef struct _mapentry
        {
            ClrType*    m_pType1;
            ClrType*    m_pType2;
        } MapEntry;

        MapEntry*  m_pEntryTab;
        unsigned    m_nListSize;
        unsigned    m_nEntry;
    };

#ifdef UNMANAGED_POINTER_TREATMENT_NEWSTYLE
    class ObjectList
    {
    public:
        ObjectList();
        ~ObjectList();

        bool AddEntry(Object* pInst);
        bool Create(unsigned listSize);

        unsigned Size()
        {
            return m_nEntry;
        }
        unsigned Capacity()
        {
            return m_nListSize;
        }
        void Release();
        Object* FindInstance(void* pData, unsigned& offset);
    protected:
        unsigned m_nListSize;
        unsigned m_nEntry;
        Object** m_pObjectTab;
    };

    class InstanceList : public ObjectList
    {
    public:
        Object* FindInstance(void* pData, unsigned& offset);
        Object* FindInstance(const char* pNameSpace, const char* pName);
    };

    class DelegateList : public ObjectList
    {
    public:
        void SwapInstance(Object* pInst, Object* pNewInst);
    };
#else
    class DelegateList
    {
    public:
        DelegateList();
        ~DelegateList();

        bool AddEntry(Object* pDelgate);
        bool Create(unsigned listSize);

        unsigned Size()
        {
            return m_nEntry;
        }
        unsigned Capacity()
        {
            return m_nListSize;
        }
        void Release();
        void SwapInstance(Object* pInst, Object* pNewInst);
        void Dump();
    private:
        unsigned m_nListSize;
        unsigned m_nEntry;
        Object** m_pDelegateTab;
    };
#endif //#ifdef UNMANAGED_POINTER_TREATMENT_NEWSTYLE


    class CopyType : public ClrAllocator
    {
    public:
        CopyType();
        ~CopyType();
        void Init();
        unsigned m_offs1;
        unsigned m_offs2;
        unsigned m_size;
        CopyTypeBase* m_subCopy;
        void (*m_pCopy)(void* pInst1, void* pInst2);
        CopyType* m_pNext;
        void Release();
    private:
    };



    class CopyTypeEclr : public eListNode<CopyTypeEclr>, public CopyTypeBase
    {
    public:
        typedef void (*ConvertFunc)(void*, void*);

        CopyTypeEclr(ClrTypeDef* pTypeInfo);
        virtual ~CopyTypeEclr();
        static CopyTypeEclr* Create(ClrTypeDef* pTypeInfo, CopyType* pSegments);
        // virtual override
        void Copy(Object* p1, Object* p2);
        unsigned CopyBytes();
        void Dump();

        CopyType* GetFlatCopySeg();
        static ConvertFunc GetConvertFunc(ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
        static bool IsCopyFunction(ConvertFunc pCopyFunc);

        static void ConvertMemcpy(void* p1, void* p2);
        static void ConvertImpossible(void* p1, void* p2);

        static const char* GetMethodName(ConvertFunc pFunc);
        static void SetConvertionSupport();

        CopyType*   m_pSegments;
    private:
        // optimized copy functions
        static void copy4_4(void* p1, void* p2);
        static void copy4_8(void* p1, void* p2);
        static void copy4_12(void* p1, void* p2);
        
        static void copy2_2(void* p1, void* p2);
        static void copy2_4(void* p1, void* p2);
        static void copy2_6(void* p1, void* p2);
        
        static void copy1_1(void* p1, void* p2);
        static void copy1_2(void* p1, void* p2);
        static void copy1_3(void* p1, void* p2);
        // conversion function
        static void convi1_i2(void* p1, void* p2);
        static void convi1_i4(void* p1, void* p2);
        static void convi1_i8(void* p1, void* p2);
        static void convu1_i2(void* p1, void* p2);
        static void convu1_i4(void* p1, void* p2);
        static void convu1_i8(void* p1, void* p2);
        static void convi2_i4(void* p1, void* p2);
        static void convi2_i8(void* p1, void* p2);
        static void convu2_i4(void* p1, void* p2);
        static void convu2_i8(void* p1, void* p2);
        static void convu4_i8(void* p1, void* p2);
        static void convi4_i8(void* p1, void* p2);
        static void convr4_r8(void* p1, void* p2);
    };

    class CopyTypeArray : public eListNode<CopyTypeArray>, public CopyTypeBase
    {
    public:
        static CopyTypeArray* Create(ClrTypeDef* pTypeInfo, ClrTypeDef* pElemInfo1, ClrTypeDef* pElemInfo2, unsigned arraySize, CopyType& copyType);
        // virtual override
        void Copy(Object* p1, Object* p2);
        unsigned CopyBytes();
        void Dump();


        ClrTypeDef* m_pElemInfo1;
        ClrTypeDef* m_pElemInfo2;
        unsigned    m_arraySize;
    private:
        unsigned    m_offs1;
        unsigned    m_offs2;
        unsigned    m_size;
        CopyTypeBase* m_subCopy;
        void (*m_pCopy)(void* pInst1, void* pInst2);

        CopyTypeArray(ClrTypeDef* pTypeInfo);

    };

    class TypeCopyList
    {
    public:
        ~TypeCopyList();
        CopyTypeEclr* CreateEntry(ClrTypeDef* pTypeInfo, CopyType* pSegments, bool isStatic);
        bool CreateArrayEntry(ClrTypeDef* pTypeInfo, ClrTypeDef* pElemInfo1, ClrTypeDef* pElemInfo2, unsigned arraySize, CopyType& copyType);
        CopyTypeEclr* GetTypeCopy(ClrTypeDef* pTypeInfo, bool isStatic);
        CopyTypeArray* GetArrayTypeCopy(ClrTypeDef* pTypeInfo, ClrTypeDef* pElemInfo1, ClrTypeDef* pElemInfo2, unsigned arraySize);
        CopyTypeArray* GetArrayTypeCopy(ClrTypeDef* pTypeInfo);
        CopyTypeEclr* GetTypeMemCopy(ClrTypeDef* pTypeInfo, unsigned size);
        unsigned Size();
        void Release();
        void Dump();
    //private:
        eList<CopyTypeEclr> m_typeCopyListInstance;
        eList<CopyTypeEclr> m_typeCopyListStatic;
        eList<CopyTypeArray> m_typeArrayCopyList;
    };
    class CopyListEntry : public eListNode<CopyListEntry>, public ClrAllocator
    {
    public:
        CopyListEntry(Object* p1, Object* p2);
        Object* m_pInstSrc;
        Object* m_pInstDst;
        CopyTypeBase* m_pTypeCopy;

    };
    class CopyList
    {

    public:
        ~CopyList();
        bool CreateEntry(Object* p1, Object* p2, CopyTypeBase* pTypeCopy);
        void Process();
        unsigned Size()
        {
            return m_copyList.size();
        }
        unsigned CopyBytes();
        void Release();
        void Dump(ClrGcHeap* pGcHeap);
    private:
        eList<CopyListEntry> m_copyList;

    };
    class SwapListEntry : public eListNode<SwapListEntry>, public ClrAllocator
    {
    public:
        SwapListEntry(Object* pObj, Object** pObjRef);
        Object* m_pInst;
        Object** m_pRef;
    };
    class SwapList
    {
    public:
        ~SwapList();
        bool CreateEntry(Object* pObj, Object** pObjRef);
        unsigned Size()
        {
            return m_swapList.size();
        }
        bool ExportSwapRef(Object** pRefTab[], unsigned size);     

        void Release();
        void Process();
        void Dump(ClrGcHeap* pGcHeap);
    private:
        eList<SwapListEntry> m_swapList;
    };

    // create type copy lists
    bool appendCopyType(CopyType** ppAnchor, CopyType& copyType);
    bool createCopyTypeFunc(ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2, bool isStatic);
    bool createCopyTypeFixedArr(ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2, CopyType& copyType);
    bool createCopyTypeFixedString(ClrTypeDef* pTypeInfo2, CopyType* pCopyType);
    bool checkType(ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkType2(ClrTypeDef* pTypeInfo2);
    bool collectChangedTypeList();

    // create instance copy list and swap lists
    bool getCopyType(ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2, CopyType& copyType);
    bool insertIDcgImpl(ClrTypeDef* pTypeInfo, ClrTypeDef* pBaseTypeInfo);
    bool insertIDcgImpl(ClrTypeDef* pTypeInfo);
    bool checkPointer2(Object** ppFieldinst2, ClrTypeDef* pTypeInfo2); 
    bool checkInstanceRetainPointer(Object** ppFieldInst1, Object** ppFieldInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkInstancePointer(Object** ppFieldInst1, Object** ppFieldInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2, ClrTypeDef* pParentTypeInfo);
    bool checkInstanceStatic(Object* pInst1, Object* pInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkInstanceStatic(Object* pInst1, Object* pInst2, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkInstanceStatic2(Object* pInst2, ClrTypeDef* pTypeInfo2);
    bool checkInstance(Object* pInst1, Object* pInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkInstanceValueType(void* pInst1, void* pInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkInstanceClass(Object** ppInst1, Object** ppInst2, int activeInst, ClrTypeDef* pParentTypeInfo);
    bool checkArrayElements(void* pInst1, void* pInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2, unsigned arraySize);
    bool checkInstanceFixedArray(void* pInst1, void* pInst2, int activeInst, ClrTypeDef* pTypeInfo1, ClrTypeDef* pTypeInfo2);
    bool checkInstanceArray(Object** ppFieldInst1, Object** ppFieldInst2, int activeInst);
    bool checkInstanceSingleDelegate(Object** ppInst);
    bool checkInstanceSingleString(Object** ppInst);
    bool checkInstanceSingleValueType(void* pInst, ClrTypeDef* pTypeInfo);
    bool checkInstanceSingle(Object* pInst, ClrTypeDef* pTypeInfo);
    bool checkInstanceSingle1(Object* pInst);
    bool checkInstanceSingle2(Object* pInst);
    CopyTypeEclr* createArrayMemCopy(ClrTypeDef* pTypeInfo, ClrTypeDef* pElemType2, unsigned size, CopyType& copyType);
    CopyTypeArray* createArrayTypeCopy(ClrTypeDef* pTypeInfo, ClrTypeDef* pElemType1, ClrTypeDef* pElemType2, unsigned size, CopyType& copyType);
    CopyTypeEclr* createMemCopy(ClrTypeDef* pTypeInfo, unsigned size);
    bool createCopyEntry(Object* p1, Object* p2, ClrTypeDef* pTypeInfo, bool isStatic);
    bool createArrayCopyEntry(void* p1, void* p2, ClrTypeDef* pElemTypeInfo1, ClrTypeDef* pElemTypeInfo2, unsigned size, CopyType& copyType);
    bool createMemCopyEntry(void* p1, void* p2, unsigned size);
    bool createSwapEntry(Object** ppFieldInst1, Object** ppFieldInst2, int activeInst);
    bool collectCopyList();

    bool releaseUnusedObjects();
    void calcStackUsage();

    // lists
    TypeMapList   m_typeMap;
    TypeCopyList  m_typeCopyList;
    IDcgList      m_idcgList;
    RelocList     m_relocList;
    IDcgInstList  m_icgInstList;
    SwapList      m_swapList;
    CopyList      m_copyList;
    MetaSwapList  m_metaSwapList;
#ifdef UNMANAGED_POINTER_TREATMENT_NEWSTYLE
    InstanceList  m_staticInstList1;
    InstanceList  m_staticInstList2;
#endif
    // object references
    ClrGcHeap*    m_pGcHeap;
    ClrAppDomain* m_pAppDomain1;
    ClrAppDomain* m_pAppDomain2;
    // additional type infos
    ClrTypeDef*   m_pTypeString2;
    ClrTypeDef*   m_pTypeArray2;
    ClrTypeDef*   m_pTypeValueType2;
    // internal infos
    unsigned      m_dcgLastError;
    int           m_maxStackUsed;
    char*         m_pStackTop;
    bool          m_retainChanged;
    bool          m_changeExecuted;

    // Test
    unsigned m_nMetaSwapped;

};

#endif //#define __APPDCG_H__
