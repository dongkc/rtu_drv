#ifndef __METADATA_H__
#define __METADATA_H__

#include "blobheap.h"

class ClrMethod;
class ClrField;
class ClrType;
class ClrTypeDef;
class ClrTypeDefExtern;
class ClrTypeField;
class ClrNameSpace;
class ClrMethodImgHeader;
class ClrMethodToken;
class ClrStringLiteral;
class ClrMethodDebug;
class ClrMethodNativeImage;
class ClrMetaAssembly;

class ClrCodeHeap;

class MetaCharRef
{
    public:
    inline operator const char*() 
    {   
        return (const char*)this + ((unsigned)m_dist)*sizeof(uint16);
    }
    inline void operator=(const char* p) 
    {
        uint32 dist32 = (p - (char*)this)/sizeof(uint16);
        if (dist32 < UINT16_MAX)
        {
            // fits into the 64K distance space
            m_dist = (uint16)dist32;
        }
    }
    private:
    uint16  m_dist;

};

class MetaAbsRef32
{
    public:
    inline bool operator == (const MetaAbsRef32& r) const
    {
        return m_addr == r.m_addr;
    }
    void* get(void* pBase)
    {
        return (char*)pBase + m_addr*sizeof(uint16);   
    }
    unsigned get()
    {
        return m_addr*sizeof(uint16);   
    }
    inline void set(void* pBase, void* pDst)
    {
        m_addr = ((char*)pDst - (char*)pBase)/sizeof(uint16);
    }
    inline bool isNull()
    {
        return (m_addr == 0) ? true : false;
    }
    private:
    
    uint32  m_addr;
};
class MetaSRef32
{
    public:
    void* get()
    {
        return (char*)this + m_dist*sizeof(uint16);
    }
    void set(void* pDst)
    {
        m_dist = ((char*)pDst - (char*)this)/(int)sizeof(uint16);
    }
    inline bool isNull()
    {
        return (m_dist == 0) ? true : false;
    }
    private:
    sint32  m_dist;
};

class MetaSRef128K
{
    public:
    void* get(void* pBase);
    void set(ClrCodeHeap* pWsp, void* pBase, void* pDst);
    void set(void* pBase, void* pDst)
    {
        m_dist = 0;   
        if (pDst != NULL)
        {
            sint32 dist32 = ((char*)pDst - (char*)pBase)/(int)sizeof(uint32);
            if (dist32 > SINT16_MIN && dist32 < SINT16_MAX)
            {
                m_dist = (sint16)dist32;  
            }
        }
    }
    inline void clear()
    {
        m_dist = 0;   
    }
    private:
    sint16  m_dist;
};
class MetaURef128K
{
    public:
    void* get()
    {
        void* result = NULL;
        if (m_dist != 0)
        {
            result = (char*)this + ((unsigned)m_dist)*sizeof(uint16);
        }
        return result;
    }
    inline void* get2()
    {
        return (char*)this + ((unsigned)m_dist)*sizeof(uint16);
    }
    void set(void* pDst)
    {
        m_dist = 0;
        if (pDst != NULL)
        {
            uint32 dist32 = ((char*)pDst - (char*)this)/sizeof(uint16);
            if (dist32 < UINT16_MAX)
            {
                // fits into the 64K distance space
                m_dist = (uint16)dist32;
            }
        }
    }
    inline void clear()
    {
        m_dist = 0;   
    }
    private:
    uint16  m_dist;
};



class MetaAny
{
    // placeholder for any meta objects   
    uint8   any;
};

class MetaLimit
{
    public :
    MetaLimit()
    {
        pNext = NULL;  
        pEnd  = NULL;  
    }
    
    void* pNext;  
    void* pEnd;  
};

class ClrMetaData
{
public:
    static bool Swap(CAppImageHdr* pHeader, unsigned imageSize);
};

// set the compatible ID of the eCLR metadata here !
#define ECLRMETA_COMPATID       (ECLRMETA_COMPATID_BASE+0)

// enable bitfield access (not compatible with all C++ compilers)
//#define META_BITFIELD_SUPPORT
#define META_SETBITFIELD16(flags16, val, mask, shift)  \
        flags16 &= (uint16)( ~(mask << shift) ); \
        flags16 |= (uint16)( ((val) & mask) << shift ); 
class ClrMetaDataDef
{
    public :
    enum ClrMetaDefSpec
    {
        CompatLevel     = ECLRMETA_COMPATID,
        // mata token associated to the meta section of the eCLR image file
        MetaUnmanaged       = 0x10,
        MetaMetaDef         = MetaUnmanaged+0x0,
        MetaAssemblyDef     = MetaUnmanaged+0x1,
        MetaNameSpace       = MetaUnmanaged+0x2,
        MetaType            = MetaUnmanaged+0x3,
        MetaTypeField       = MetaUnmanaged+0x5,
        MetaFieldNames      = MetaUnmanaged+0x6,
        MetaStringLiteral   = MetaUnmanaged+0x7,
        MetaTypeStaticData  = MetaUnmanaged+0x8,
        MetaApplStaticDataInfo = MetaUnmanaged+0x9,
        MetaTypeDefExtern   = MetaUnmanaged+0xA,
        MetaProxy           = MetaUnmanaged+0xB,
        MetaAlignment       = MetaUnmanaged+0xC,
        MetaVirtualTable    = MetaUnmanaged+0xD,
        MetaInterfaceTable  = MetaUnmanaged+0xE,
        MetaProxy2          = MetaUnmanaged+0xF,
        // mata token associated to the code section of the eCLR image file
        MetaManaged         = 0x30,
        MetaAssemblyFile    = MetaManaged+0x0,
        MetaCode            = MetaManaged+0x1,
        MetaToken           = MetaManaged+0x2,
        MetaMethodTokenMap  = MetaManaged+0x3,
        MetaAssemblyRef     = MetaManaged+0x4,
        MetaDebugInfo       = MetaManaged+0x5,
        MetaApplicationFile = MetaManaged+0x6,
        MetaUnresolved      = MetaManaged+0x7,
        MetaFieldIdTab      = MetaManaged+0x8,
        MetaArrayShape      = MetaManaged+0x9,
        MetaManagedEnd
    };
    enum ClrMetaDefBitFields
    {
        IntSizeMask         = 0xF,
        IntSizeShift        = 0,
        CompatIdMask        = 0xFF,
        CompatIdShift       = 8,
        BitFieldsEnd
    };

    ClrMetaDataDef();
    static ClrMetaDataDef* createMetaDataDef(ClrCodeHeap* pWorkSpace);
    static ClrMetaDataDef* createMetaDataDef(BlockHeap* h);
    static inline ClrMetaDataDef* getFirst(BlockHeap* h)
    {
        return (ClrMetaDataDef*)h->getFirst(ClrMetaDataDef::MetaMetaDef);
    }
    inline ClrMetaDataDef* getNext(BlockHeap*  h)
    {
        return (ClrMetaDataDef*)h->getNext(this, ClrMetaDataDef::MetaMetaDef);
    }
    inline int getFlags()
    {
        return m_flags;
    }
    inline int getCompatId() const
    {
        return (m_flags2 >> CompatIdShift) & CompatIdMask;
    }
    inline int getIntSize() const
    {
        return m_flags2 & IntSizeMask;
    }
    bool isCompatible(const ClrMetaDataDef& meta);
    void swap();

    static inline unsigned calculate_size()
    {
        return sizeof(ClrMetaDataDef);   
    }

    inline void* operator new(size_t, ClrMetaDataDef* o)
    {
        return o;
    }
    
    typedef struct _bits
    { 
       uint16
            m_intsize   : 4,      // size of the native int data type
            m_res1      : 4,      // reserve 4 bit
            m_compatId  : 8;      // internal id for compatability 
    } Bits;

private:
    uint16 m_flags;
    uint16 m_flags2;
};

class ClrProxy
{
    public:
    enum ClrProxySpec
    {
        // signature of the complete iterator : 0x0000011F
        Sign = 0x100+ClrMetaDataDef::MetaProxy2,
        Any = 0
    };
    static  ClrProxy* get(ClrCodeHeap* pWsp, void* pAny);
    static  ClrProxy* create(ClrCodeHeap* pWsp, void* pAny);
    static inline ClrProxy* getFirst(BlockHeap* h)
    {
        return (ClrProxy*)h->getFirst(ClrMetaDataDef::MetaProxy2);
    }
    inline ClrProxy* getNext(BlockHeap*  h)
    {
        return (ClrProxy*)h->getNext(this, ClrMetaDataDef::MetaProxy2);
    }
    
    inline void* get()
    {
        return (char*)this + m_dist * sizeof(uint16);
    }
    inline void set(void* pAny)
    {
       m_dist = ((char*)pAny - (char*)this) / (int) sizeof(uint16); 
    }   
    void swap();

    private:

    inline ClrProxy(void* pAny)
    {
        set(pAny);
    }

    inline void* operator new(size_t, ClrProxy* o)
    {
        return o;
    }

    sint32  m_dist;
};

class ClrNameSpace
{
    public :
    static ClrNameSpace* createNameSpace(ClrCodeHeap* h, const char* nameSpace);
    static ClrNameSpace* createNameSpaceExpl(ClrCodeHeap* h, const char* nameSpace);
    static ClrNameSpace* clone(BlockHeap* pMeta, ClrNameSpace* pNsp);
    static inline ClrNameSpace* getFirst(BlockHeap* h)
    {
        return (ClrNameSpace*)h->getFirst(ClrMetaDataDef::MetaNameSpace);
    }
    inline ClrNameSpace* getNext(BlockHeap*  h)
    {
        return (ClrNameSpace*)h->getNext(this, ClrMetaDataDef::MetaNameSpace);
    }

    inline  const char* getName()
    {
        return name;
    }

    void swap();

    private:

    inline ClrNameSpace(const char* nameSpace)
    {
        strcpy(name, nameSpace); 
    }

    static inline unsigned calculate_size(const char* nameSpace)
    {
        return strlen(nameSpace) + 1;
    }

    inline void* operator new(size_t, ClrNameSpace* o)
    {
        return o;
    }

    char name[1];

};

class ClrAssemblyDef
{
    public :
    enum ClrAssemblyDefSpec
    {
        Native      = 0x0001,
        Active      = 0x0002,
        Corlib      = 0x0004
    };

    static ClrAssemblyDef* createAssembly(ClrCodeHeap* h, const char* assName, unsigned nTypes);
    static inline ClrAssemblyDef* getFirst(BlockHeap* h)
    {
        return (ClrAssemblyDef*)h->getFirst(ClrMetaDataDef::MetaAssemblyDef);
    }
    inline ClrAssemblyDef* getNext(BlockHeap*  h)
    {
        return (ClrAssemblyDef*)h->getNext(this, ClrMetaDataDef::MetaAssemblyDef);
    }
    static ClrAssemblyDef* getAssembly(BlockHeap* h, const char* name);
    static ClrAssemblyDef* clone(BlockHeap* h, ClrAssemblyDef* pAssemblyDef);
    void clone(ClrAssemblyDef* pAssemblyDef);
    
    inline  void setActive()
    {
        flags |= Active;
    }
    inline  void resetActive()
    {
        flags &= ~Active;
    }
    
    inline  bool isActive()
    {
        return (flags & Active) ? true : false;
    }

    inline  bool isLoaded()
    {
        return !isActive();
    }

    inline  void setNative()
    {
        flags |= Native;
    }
    inline  bool isNative()
    {
        return (flags & Native) ? true : false;
    }

    inline void setCorlib()
    {
        flags |= Corlib;
    }
    inline bool isCorlib()
    {
        return (flags & Corlib) ? true : false;
    }


    inline  const char* getName()
    {
        return (const char*)&(getTypeTab())[nType];
    }

    inline unsigned getTypeAmount()
    {
        return nType;
    }

    ClrTypeDef* getType(unsigned n);
    ClrTypeDef* getType(const char* nameSpace, const char* className);
    bool setType(ClrCodeHeap* pWsp, ClrTypeDef* pType, unsigned n);

    inline void setVersion(unsigned major, unsigned minor, unsigned build, unsigned revsn)
    {
        vMajor      = (uint16)major;
        vMinor      = (uint16)minor;
        vBuild      = (uint16)build;
        vRevsn      = (uint16)revsn;
    }
    inline void getVersion(EclrVersion& version)
    {
        version.major = vMajor;
        version.minor = vMinor;
        version.build = vBuild;
        version.revsn = vRevsn;
    }

    void clearTypes();

    void swap(bool hasNaturalEndianess);

    private:

    typedef MetaSRef128K                     TypeRef;

    inline TypeRef* getTypeTab()
    {
        return(TypeRef*)(this+1);
    }
    

    ClrAssemblyDef(const char* name, unsigned nType);
    ~ClrAssemblyDef();

    static unsigned calculate_size(const char* name, unsigned nType);

    inline void* operator new(size_t, ClrAssemblyDef* o)
    {
        return o;
    }
    void operator delete(void* p);
    uint16  flags;
    uint16  vMajor;
    uint16  vMinor;
    uint16  vBuild;
    uint16  vRevsn;
    uint16  nType;
    // 2 byte aligned

    // TypeRef typeTab[1];
    // char    name[1];

};

class ClrArrayShapeType
{
public:
    ClrArrayShapeType();
    enum ArrayShapeSpec
    {
        MaxRank     = 7
    };
    void init();
    bool getShape(ClrTypeDef* pType);
 
    unsigned m_rank;
    unsigned m_sizes[MaxRank];
    int m_loBounds[MaxRank];
};

class ClrType
{
    public :
    static ClrType* createType(ClrCodeHeap* h, ClrTypeDef* pExtend, const char* nameSpace, const char* name, 
            unsigned nMethod, unsigned nIf, unsigned methodsSize);
    static ClrType* createType(ClrCodeHeap* h, ClrTypeDef* pExtend, const char* nameSpace, const char* name);
    static inline ClrType* getFirst(BlockHeap* h)
    {
        return (ClrType*)h->getFirst(ClrMetaDataDef::MetaType);
    }
    inline ClrType* getNext(BlockHeap*  h)
    {
        return (ClrType*)h->getNext(this, ClrMetaDataDef::MetaType);
    }
    static ClrType* createPtrType(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pExtends, bool skipBaseType = false);
    static ClrType* createFuncPtrType(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pParamTab[], unsigned nParam);
    static ClrType* createArrayType(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrArrayShapeType* pShape, ClrTypeDef* pElemType);
    static ClrType* createArrayType(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pElemType, unsigned rank, const char* name);
    static ClrType* clone(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pType);
    static ClrTypeDef* cloneGeneric(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pType);
    static ClrTypeDef* cloneFuncPtr(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pType);
    
    inline ClrTypeDef* getTypeInfo()
    {
        return (ClrTypeDef*)((char*)this+sizeof(ClrType));
    }

    static inline unsigned fixed_sizeof()
    {
        return sizeof(ClrType);
    }

    inline unsigned getGlobalVSlot()
    {
        return getVTableEntry();   
    }
    inline void setGlobalVSlot(unsigned slot)
    {
        setVTableEntry(slot);   
    }
    inline unsigned getGlobalISlot()
    {
        return getITableEntry();   
    }
    inline void setGlobalISlot(unsigned slot)
    {
        setITableEntry(slot);   
    }
    inline void clear()
    {
        vMethodTab      = 0;   
        vInterfaceTab   = 0;
    }

    unsigned getVTableEntry()
    {
        return (unsigned)vMethodTab;  
    }
    void setVTableEntry(void* pVTable)
    {
        vMethodTab = (TabRef)pVTable;  
    }
    void setVTableEntry(unsigned addr)
    {
        vMethodTab = (TabRef)addr;  
    }
    
    TargetPtrType* getVTableReference()
    {
        return (TargetPtrType*)vMethodTab;  
    }


    unsigned getITableEntry()
    {
        return (unsigned)vInterfaceTab;  
    }
    void setITableEntry(PtrType addr)
    {
        vInterfaceTab = addr;  
    }
    void setITableEntry(void* pITable)
    {
        vInterfaceTab = (TabRef)pITable;  
    }
    
    void swap(bool hasNaturalEndianess);

    private:

    ~ClrType();

    static unsigned calculate_size(const char* name, unsigned nMethod, unsigned nIf, unsigned methodsSize);

    inline void* operator new(size_t, ClrType* o)
    {
        return o;
    }
    void operator delete(void* p);

    typedef TargetPtrType           TabRef;

    TabRef  vMethodTab;   
    TabRef  vInterfaceTab;

};

class ClrVirtualTable
{
    public :
    enum ClrVirtualTableSpec
    {
 
        Relocated       = 0x0001,
        
        ClrVirtualTableAny  = 0   
    };
    inline void setRelocated()
    {
        m_flags |= Relocated;
    }
    inline bool isRelocated()
    {
        return (m_flags & Relocated) ? true : false;
    }

    ClrVirtualTable(unsigned slots);
    static ClrVirtualTable* create(ClrCodeHeap* h, unsigned slots);
    static inline ClrVirtualTable* getFirst(BlockHeap* h)
    {
        return (ClrVirtualTable*)h->getFirst(ClrMetaDataDef::MetaVirtualTable);
    }
    inline ClrVirtualTable* getNext(BlockHeap*  h)
    {
        return (ClrVirtualTable*)h->getNext(this, ClrMetaDataDef::MetaVirtualTable);
    }
    
    inline unsigned getSlots()
    {
        return m_slots;
    }
    inline PtrType getSlotEntry(unsigned slot)
    {
        return (getSlotTab())[slot]; 
    }
    inline bool setSlotEntry(PtrType slotEntry, unsigned slot)
    {
        bool result = false;
        if (slot < m_slots)
        { 
            (getSlotTab())[slot] = slotEntry; 
            result = true;
        }
        return result;       
    }
    
    void* getSlotReference(unsigned slot)
    {
        return &((getSlotTab())[slot]); 
    }
    bool realloc(ClrCodeHeap* h, unsigned slots);

    
    void swap(bool hasNaturalEndianess);
    private:

    typedef PtrType  SlotEntry;
    inline PtrType* getSlotTab()
    {
        return (PtrType*)(this+1);
    }

    static unsigned calculate_size(unsigned slots);

    static inline unsigned fixed_sizeof()
    {
        return sizeof(ClrVirtualTable);   
    }

    inline void* operator new(size_t, ClrVirtualTable* o)
    {
        return o;
    }
   
    uint16  m_flags;
    uint16  m_unused;
    uint32  m_slots;
     
};

class ClrInterfaceTable
{
    public :
 
    enum ClrInterfaceTableSpec
    {
 
        Relocated   = 0x0001,
        
        ClrTableAny = 0   
    };
    inline void setRelocated()
    {
        m_flags |= Relocated;
    }
    inline bool isRelocated()
    {
        return (m_flags & Relocated) ? true : false;
    }

 
    static ClrInterfaceTable* create(ClrCodeHeap* h, unsigned slots);
    static inline ClrInterfaceTable* getFirst(BlockHeap* h)
    {
        return (ClrInterfaceTable*)h->getFirst(ClrMetaDataDef::MetaInterfaceTable);
    }
    inline ClrInterfaceTable* getNext(BlockHeap*  h)
    {
        return (ClrInterfaceTable*)h->getNext(this, ClrMetaDataDef::MetaInterfaceTable);
    }
    
    inline unsigned getSlots()
    {
        return m_slots;
    }
    inline PtrType getSlotEntry(unsigned slot)
    {
        return (getSlotTab())[slot]; 
    }
    inline bool setSlotEntry(PtrType slotEntry, unsigned slot)
    {
        bool result = false;
        if (slot < m_slots)
        { 
            (getSlotTab())[slot] = slotEntry; 
            result = true;
        }
        return result;       
    }
    inline bool setSlotEntry(void* slotEntry, unsigned slot)
    {
        bool result = false;
        if (slot < m_slots)
        { 
            (getSlotTab())[slot] = (PtrType)slotEntry; 
            result = true;
        }
        return result;       
    }
    
    void* getSlotReference(unsigned slot)
    {
        return &((getSlotTab())[slot]); 
    }
    bool realloc(ClrCodeHeap* h, unsigned slots);
    
    void swap(bool hasNaturalEndianess);
    private:

    ClrInterfaceTable(unsigned slots);

    inline PtrType* getSlotTab()
    {
        return (PtrType*)(this+1);
    }

    static unsigned calculate_size(unsigned slots);

    static inline unsigned fixed_sizeof()
    {
        return sizeof(ClrInterfaceTable);   
    }

    inline void* operator new(size_t, ClrInterfaceTable* o)
    {
        return o;
    }
   
    uint16  m_flags;
    uint16  m_unused;
    uint32  m_slots;
     
};

class ClrTypeDef
{
    public :
    enum ClrTypeDefSpec
    {
        // specifications
        MaxImplMethods          = 4095, // maximum amount of implemented methods per type 
        MaxImplInterface        = 15,   // maximum amount of implemented interfaces per type 
        MaxInterfaceAmount      = 64,   // maximum amount of interfaces per application 
        MaxArrayDimension       = 7,    // maximum amount of array dimensions
#if 0
        uint16    
            m_external    : 1,  // 0: 
            m_unused1     : 1,  // 1: 
            m_enum        : 1,  // 2:
            m_elementary  : 1,  // 3:
            m_class       : 1,  // 4:
            m_interface   : 1,  // 5:
            m_generic     : 1,  // 6:
            m_explLayout  : 1,  // 7:
            m_delegate    : 1,  // 8:
            m_native      : 1,  // 9:
            m_changedLayout         : 1,  // 10:
            m_changedStaticLayout   : 1,  // 11:
            m_dcgImpl     : 1,  // 12: type implements the non-generic dcg
            m_unused13    : 1,  // 13: unused
            m_unused14    : 1,  // 14: unused
            // internal usage
            m_tempflag    : 1;  // 15: internal usage, DCG
#endif
        // bitfield definition for m_flag
        ExternalBitMask         = 0x0001,
        Flag0Unused1BitMask     = 0x0002,
        External2BitMask        = 0x0003,
        EnumBitMask             = 0x0004,
        ElementaryBitMask       = 0x0008,
        ClassBitMask            = 0x0010,
        InterfaceBitMask        = 0x0020,
        GenericBitMask          = 0x0040,
        ExplLayoutBitMask       = 0x0080,
        DelegateBitMask         = 0x0100,
        NativeBitMask           = 0x0200,
        ChangedLayoutBitMask    = 0x0400,
        ChangedStaticLayoutBitMask  = 0x0800,
        DcgImplBitMask          = 0x1000,
        TempFlagBitMask         = 0x8000,
#if 0
    typedef struct _typespec
    {
        uint16
        field16   : 4,      // fieldref distance (bit 16..19)
        type      : 5,      // CLI element type spec, see part.II,cap.22.1.5
        align     : 3,      // align of the type -> 1 .. 8
        rank      : 3,      // dimensions for arrays 0...7  
        unused    : 1;      // currently unused, must be 0  
    } TypeSpec;

    typedef struct _refspec
    {
        uint16
        methods : 12,       // amount of methods associated to the type (0..4095)
        ifs     : 4;        // amount of interfaces implemented by this type (0..15)
    } RefSpec;
#endif
        // bitfield definition for m_flag2
        Field16_19Mask          = 0x000F,
        Field16_19Shift         = 0,
        TypeMask                = 0x001F,
        TypeShift               = 4,
        AlignMask               = 0x0007,
        AlignShift              = 9,
        RankMask                = 0x0007,
        RankShift               = 12,

        // bitfield definition for m_flag3
        MethodsMask             = 0x0FFF,
        MethodsShift            = 0,
        IfsMask                 = 0x000F,
        IfsShift                = 12,

        ClrTypeDefSpecAny       = 0
    };

    ClrTypeDef(const char* name, unsigned nMethod, unsigned nIf);
    static ClrTypeDef* findType(BlockHeap* h, const char* pNameSpace, const char* pName);
    static ClrTypeDef* findArrayType(BlockHeap* h, const char* pNameSpace, const char* pName, int dimension);

    static ClrTypeDef* findType2(BlockHeap* h, ClrTypeDef* pTypeInfo);
    inline bool isSizeCalculated()
    {
        return (m_tSize != 0) ? true : false;
    }

    void setSize(uint32 size);
    unsigned getSize();
    unsigned getValueSize();
    inline unsigned getObjectSize()
    {
        return m_tSize;
    }
    inline void setObjectSize(uint32 size)
    {
        m_tSize = size;
    }
    unsigned getStaticSize();

    inline void setEnum()
    {
        m_flags |= EnumBitMask;
    }
    inline bool isEnum()
    {
        return (m_flags & EnumBitMask) ? true : false;
    }
    inline void setGeneric()
    {
        m_flags |= GenericBitMask;
    }
    inline bool isGeneric()
    {
        return (m_flags & GenericBitMask) ? true : false;
    }
    inline void setClass()
    {
        m_flags |= ClassBitMask;
    }
    inline bool isClass()
    {
        return (m_flags & ClassBitMask) ? true : false;
    }
    inline bool isValueType()
    {
        return !isClass();
    }
    inline void setValueType()
    {
        m_flags &= ~ClassBitMask;
    }
    inline bool hasInterface()
    {
        return (m_flags3 & IfsMask) ? true : false;
    }
    inline bool isInterface()
    {
        return (m_flags & InterfaceBitMask) ? true : false;
    }
    inline void setInterface()
    {
        m_flags |= InterfaceBitMask;
    }
    inline void setExplicitLayout()
    {
        m_flags |= ExplLayoutBitMask;
    }
    inline bool isExplicitLayout()
    {
        return (m_flags & ExplLayoutBitMask) ? true : false;
    }
    inline void setElementary()
    {
        m_flags |= ElementaryBitMask;
    }
    inline bool isElementary()
    {
        return (m_flags & ElementaryBitMask) ? true : false;
    }
    inline bool isExternal()
    {
        // check bit 0 and 1 
        return ((m_flags & External2BitMask) == External2BitMask) ? true : false;
    }
    inline void setDelegate()
    {
        m_flags |= DelegateBitMask;
    }
    inline bool isDelegate()
    {
        return (m_flags & DelegateBitMask) ? true : false;
    }
    inline void setNative()
    {
        m_flags |= NativeBitMask;
    }
    inline bool isNative()
    {
        return (m_flags & NativeBitMask) ? true : false;
    }
    inline void setInstanceChanged()
    {
        m_flags |= ChangedLayoutBitMask;
    }
    inline bool isInstanceChanged()
    {
        return (m_flags & ChangedLayoutBitMask) ? true : false;
    }
    inline void setStaticChanged()
    {
        m_flags |= ChangedStaticLayoutBitMask;
    }
    inline bool isStaticChanged()
    {
        return (m_flags & ChangedStaticLayoutBitMask) ? true : false;
    }
    inline bool isTempFlag()
    {
        return (m_flags & TempFlagBitMask) ? true : false;
    }
    inline void setTempFlag()
    {
        m_flags |= TempFlagBitMask;
    }
    inline void resetTempFlag()
    {
        m_flags &= ~TempFlagBitMask;
    }
    void setIdcgImpl()
    {
        m_flags |= DcgImplBitMask;
    }
    bool isIdcgImpl()
    {
        return (m_flags & DcgImplBitMask) ? true : false;
    }

    // GC Helper functions
    // non-static layout
    void setObjectMember();
    bool hasObjectMember();
    bool hasObjectMemberDeep();
    void setPointerMember();
    bool hasPointerMember();
    bool hasRefMember();
    bool hasRefMemberDeep();
    // static layout
    void setObjectMemberStatic();
    bool hasObjectMemberStatic();
    void setPointerMemberStatic();
    bool hasPointerMemberStatic();
    bool hasRefMemberStatic();
    
    inline bool isPointer()
    {
        bool result = (isGeneric() == true) && (getType() == TypePtr);
        return result;
    }
    inline bool isFuncPtr()
    {
        bool result = (getType() == TypeFuncPtr);
        return result;
    }
    inline bool isArray()
    {
        bool result = (getType() == TypeSZarray) || (getType() == TypeArray);
        return result;
    }
    inline bool isSZArray()
    {
        bool result = getType() == TypeSZarray;
        return result;
    }
    inline bool isMultiDimArray()
    {
        bool result = getType() == TypeArray;
        return result;
    }
    inline void setRank(unsigned rank)
    {
        META_SETBITFIELD16(m_flags2, rank, RankMask, RankShift);
    }
    inline unsigned getRank()
    {
        return (m_flags2 >> RankShift) & RankMask;
    }
    inline unsigned getSizeOffset()
    {
        return (char*)&m_tSize - (char*)this;
    }

    bool setMethod(ClrMethod* m, unsigned n);
    bool setField(ClrField* f, unsigned n);
    bool setInterface(ClrCodeHeap* pWsp, ClrTypeDef* pIf, unsigned n);
    bool setExtends(ClrCodeHeap* pWsp, ClrTypeDef* pExt);
    
    ClrMethod* getMethod(ClrMethod* pMethod);
    ClrMethod* getMethod(const char* name);
    ClrMethod* getMethod(const char* name, int paramTabSize);
    ClrMethod* getMethod(const char* name, int paramTabSize, ClrTypeDef* pParamTab[]);
    ClrMethod* getMethod(const char* name, int paramTabSize, ClrTypeDef* pParamTab[], unsigned* pSlot);
    ClrMethod* getMethod(unsigned n);

    ClrField* getField(const char* name);
    ClrField* getFieldByName(const char* name, unsigned nameLength);
    ClrField* getField(const char* name, unsigned& slot);
    ClrField* getField(unsigned n);
    ClrField* getFieldByOffset(unsigned offset, bool isStatic);

    bool isCastclass(ClrTypeDef* pTypeInfo);

    inline unsigned getMethodAmount()
    {
        return m_flags3 & MethodsMask;
    }

    unsigned getFieldAmount();

    ClrTypeDef* getInterface(unsigned n);
    inline unsigned getInterfaceAmount()
    {
        return (m_flags3 >> IfsShift) & IfsMask;
    }
    bool setClrNameSpace(ClrCodeHeap* pWsp, ClrNameSpace* pNsp);
    inline ClrNameSpace* getClrNameSpace()
    {
        return (ClrNameSpace*)m_nameSpace.get(this);
    }
    const char* getNameSpace();
    inline const char* getName()
    {   
        unsigned ifs = (m_flags3 >> IfsShift) & IfsMask;
        return (const char*)&(getInterfaceTab())[ifs];
    }
    inline ClrTypeDef* getExtends()
    {
        return (ClrTypeDef*)m_extends.get(this);
    }
    ClrTypeDef* getGenericType();
    bool setGenericType(ClrCodeHeap* pWsp, ClrTypeDef* pTypeInfo);
    inline void setType(unsigned ttok)
    {
        META_SETBITFIELD16(m_flags2, ttok, TypeMask, TypeShift);
    }
    inline unsigned getType()
    {
        return (m_flags2 >> TypeShift) & TypeMask;
    }

    inline ClrType* getClrType()
    {
        // internal def
        return (ClrType*)((char*)this-sizeof(ClrType));
    }

    inline void setAlign(unsigned maxAlign)
    {
        META_SETBITFIELD16(m_flags2, maxAlign-1, AlignMask, AlignShift);
    }
    inline unsigned getAlign()
    {
        return ((m_flags2 >> AlignShift) & AlignMask) + 1;
    }

    void setReferenced();
    void setNativeReferenced();

    void setTypeField(ClrTypeField* pTypeField);
    ClrTypeField* getTypeField();

    void getMethodLimit(MetaLimit& typeMethodLimit);

    static bool cmpLogical(ClrTypeDef* pType1, ClrTypeDef* pType2);
    static bool cmpLayout(ClrTypeDef* pType1, ClrTypeDef* pType2);
    
    typedef MetaSRef128K                    IfRef;
    typedef MetaSRef128K                    NameSpaceRef;
    typedef MetaSRef128K                    TypeExtRef;
    typedef MetaURef128K                    MethodRef;
    typedef uint32                          TypeSize;

#define TYPEINFO_SET_SIZE32LE(val32, size)  (uint32)(size)
#define TYPEINFO_SET_SIZE32BE   TYPEINFO_SET_SIZE32LE

#define TYPEINFO_SET_SIZE64LE(val64, size)  \
    (uint64)( (val64&~0xFFFFFFFF) | size)
#define TYPEINFO_SET_SIZE64BE(val64, size)  \
    (uint64)( val64 | ((uint64)size<<32))


    typedef struct _typespec
    {
        uint16
        field16   : 4,      // fieldref distance (bit 16..19)
        type      : 5,      // CLI element type spec, see part.II,cap.22.1.5
        align     : 3,      // align of the type -> 1 .. 8
        rank      : 3,      // dimensions for arrays 0...7  
        unused    : 1;      // currently unused, must be 0  
    } TypeSpec;

    typedef struct _refspec
    {
        uint16
        methods : 12,       // amount of methods associated to the type (0..4095)
        ifs     : 4;        // amount of interfaces implemented by this type (0..15)
    } RefSpec;


    void clone(ClrTypeDef* pType);
    void printFullName(eString<128>& fullName);
    ClrMethod* addMethod(unsigned n, const char* name, unsigned nParam, int flags, MetaLimit& limit);
    bool cloneMethod(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pTypeInfo, MetaLimit& typeMethodLimit);
    bool cloneMethodParams(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeDef* pTypeInfo);
    void clearReferences();

    void swap(bool hasNaturalEndianess);

    static unsigned calculate_size(const char* name, unsigned nMethod, unsigned nIf, unsigned methodsSize);
    inline void* operator new(size_t, ClrTypeDef* o)
    {
        return o;
    }
    void operator delete(void* p);

    protected:
    inline ClrTypeField* getTypeFieldInline()
    {
        unsigned dist = (unsigned)(m_flags2 & Field16_19Mask) << 16;
        dist = (dist + m_fieldRef)*sizeof(uint32);
        return (ClrTypeField*)((char*)this + dist);
    } 

    inline MethodRef* getMethodTab()
    {
        return (MethodRef*)(m_typedef_end);
    }
    inline IfRef* getInterfaceTab()
    {
        return (IfRef*)(&(getMethodTab())[m_flags3 & MethodsMask]);
    }

    static inline unsigned fixed_sizeof()
    {
        ClrTypeDef* p = (ClrTypeDef*)1000;
        return (char*)p->m_typedef_end - (char*)p;   
    }



    // type description
    // typeinfo flags
    typedef struct _bits
    {
        uint16    
            m_external    : 1,  // 0: 
            m_unused1     : 1,  // 1: 
            m_enum        : 1,  // 2:
            m_elementary  : 1,  // 3:
            m_class       : 1,  // 4:
            m_interface   : 1,  // 5:
            m_generic     : 1,  // 6:
            m_explLayout  : 1,  // 7:
            m_delegate    : 1,  // 8:
            m_native      : 1,  // 9:
            m_changedLayout         : 1,  // 10:
            m_changedStaticLayout   : 1,  // 11:
            m_dcgImpl     : 1,  // 12: type implements the non-generic dcg
            m_unused13    : 1,  // 13: unused
            m_unused14    : 1,  // 14: unused
            // internal usage
            m_tempflag    : 1;  // 15: internal usage, DCG
    } Bits;
    // 0: flags
    uint16  m_flags;
    // 2: type info , sizeof(2)    
    uint16  m_flags2;
    // 4: field distance bit 0..15
    uint16  m_fieldRef;   
    // 6: amount of methods/interfaces
    uint16  m_flags3;

    // 8: !! must be 4 byte aligned !!
    TypeSize        m_tSize;      // 8: object size (16 bit for tiny targets)
    TypeExtRef      m_extends;    // 12(10): reference to extended type
    NameSpaceRef    m_nameSpace;  // 14(12): reference to name space
    // sizeof typeinfo 16 bytes
  
    // dynamic part
    char            m_typedef_end[1];
    // dynamic fields are not visible by the debugger, skip the declaration
    // IfRef    ifTab[1];               // sizeof 2
    // uint8    name[1]            
};

class ClrTypeDefExtern
{
    public :
    static ClrTypeDefExtern* createTypeDefExtern(ClrCodeHeap* h, const char* name_space, const char* name);
    static ClrTypeDefExtern* createTypeDefExtern(ClrCodeHeap* h, ClrNameSpace* pns, const char* name);
    static inline ClrTypeDefExtern* getFirst(BlockHeap* h)
    {
        return (ClrTypeDefExtern*)h->getFirst(ClrMetaDataDef::MetaTypeDefExtern);
    }
    inline ClrTypeDefExtern* getNext(BlockHeap*  h)
    {
        return (ClrTypeDefExtern*)h->getNext(this, ClrMetaDataDef::MetaTypeDefExtern);
    }
    
    const char* getNameSpace();
    inline const char* getName()
    {
        return (const char*)(this+1);
    }

    void swap();
    private:
    enum SpecIntern
    {
        External        = 0xFFFF,   // 
        Any             = 0
    };

    typedef MetaSRef128K                NameSpaceRef;

    ClrTypeDefExtern(ClrNameSpace* pns, const char* name);
    ~ClrTypeDefExtern();

    static unsigned calculate_size(const char* name);

    inline void* operator new(size_t, ClrTypeDefExtern* o)
    {
        return o;
    }
    void operator delete(void* p);

    uint16 flags;   
    NameSpaceRef    nameSpace;
    // 4 byte align

    // dynamic part
    //char            name[1];

};

class ClrTypeDefExternBaseType
{

    public :

    inline unsigned getType()
    {
        return type;
    }

    private:


    uint16 flags;   
    uint16 type;

};

class ClrMethod
{
    public :
    
    enum ClrMethodDefSpec
    {
        // public semantic flags
        Native      = 0x0001,   // !! used in ClrInvokeGeneric (system*.asm)
        Static      = 0x0002,   // !! used in ClrInvokeGeneric (system*.asm)

        Constructor = 0x0004,
        Virtual     = 0x0008,
        HasReturn   = 0x0010,
        EntryMethod = 0x0020,
		Debug		= 0x0040,
		Abstract    = 0x0080,

        TypeInitializer = 0x0100,
        InternalCall    = 0x0200,
		Referenced		= 0x0400,

        Null            = 0x0000
    };

    enum ClrMethodAuxFlags
    {
        // internal usage (needed only for compiling)
        // aux flags used in m_auxFlags.m_flags
        Checked         = 0x0001,
        RequestCheck    = 0x0002,
        Compiled        = 0x0004,
        RequestCompile  = 0x0008
    };
    enum ClrMethodFlags2
    {
#if 0
    typedef struct _bits
    {
        uint16                  
            m_nParam    : 5,    // 0..4 : amount of parameters (0..31)
            m_ifImpl    : 1,    // true: interface implementation
            m_vSlot     : 10;   // 6..15 : virtual table slot (0 .. 1023)
    } Bits;
#endif
        ParamMask       = 0x001F,
        ParamShift      = 0,
        IfImplBitMask   = 0x0020,
        VslotMask       = 0x02FF,
        VslotShift      = 6,
        Flag2Any        = 0
    };

    inline const char* getName()
    {
        return (const char*)&(getParamTab())[m_flags2 & ParamMask];
    }
    inline bool isConstructor()
    {
        return (m_flags & Constructor) ? true : false;
    }
    inline bool isTypeInitializer()
    {
        return (m_flags & TypeInitializer) ? true : false;
    }
    inline bool isVirtual()
    {
        return (m_flags & Virtual) ? true : false;
    }

    inline void setNative()
    {
        setCompiled();
        m_flags |= Native;
    }

    inline void resetNative()
    {
        m_flags &= ~Native;
    }

    inline bool isNative()
    {
        return (m_flags & Native) ? true : false;
    }

    inline void setStatic()
    {
        m_flags |= Static;
    }
    inline void resetStatic()
    {
        m_flags &= ~Static;
    }
    inline bool isStatic()
    {
        return (m_flags & Static) ? true : false;
    }

    inline bool hasThis()
    {
        return !isStatic();
    }

    inline void setEntryMethod()
    {
        m_flags |= EntryMethod;
    }
    inline bool isEntryMethod()
    {
        return (m_flags & EntryMethod) ? true : false;
    }

    inline bool hasReturn()
    {
        return (m_flags & HasReturn) ? true : false;
    }
    
    inline bool isInternalCall()
    {
        return (m_flags & InternalCall) ? true : false;
    }

    inline void setInterfaceImpl()
    {
        m_flags2 |= IfImplBitMask;
    }
    inline bool isInterfaceImpl()
    {
        return (m_flags2 & IfImplBitMask) ? true : false;
    }
    inline void setReferenced()
    {
        m_flags |= Referenced;
    }
    inline bool isReferenced()
    {
        return (m_flags & Referenced) ? true : false;
    }
    inline void setDebug()
    {
        m_flags |= Debug;
    }
    inline bool isDebug()
    {
        return (m_flags & Debug) ? true : false;
    }

    inline bool isAbstract()
    {
        return (m_flags & Abstract) ? true : false;
    }

    // auxiliary flags
    inline void setCompiled()
    {
        m_auxFlags.m_flags &= ~RequestCompile;
        m_auxFlags.m_flags |= Compiled;
    }
    inline bool isCompiled()
    {
        return (m_auxFlags.m_flags & Compiled) ? true : false;
    }
    inline bool isRequestCompile()
    {
        return (m_auxFlags.m_flags & RequestCompile) ? true : false;
    }
    inline void setRequestCompile()
    {
        if (isCompiled() == false && isNative() == false)
        {
            m_auxFlags.m_flags |= RequestCompile;
        }
        setReferenced();
    }
    inline void setRequestCheck()
    {
        if (isChecked() == false)
        {
            m_auxFlags.m_flags |= RequestCheck;
        }
    }
    inline bool isRequestCheck()
    {
        return (m_auxFlags.m_flags & RequestCheck) ? true : false;
    }
    inline void setChecked()
    {
        m_auxFlags.m_flags &= ~RequestCheck;
        m_auxFlags.m_flags |= Checked;
    }
    inline bool isChecked()
    {
        return (m_auxFlags.m_flags & Checked) ? true : false;
    }
    
    inline unsigned argAmount()
    {
        unsigned result = m_flags2 & ParamMask;
        if (hasReturn() == true)
        {   // do not count the return type
            result--;
        }
        return result;        
    }
    inline unsigned paramAmount()
    {
        return m_flags2 & ParamMask;        
    }
    inline void setVirtualSlot(unsigned slot)
    {
        META_SETBITFIELD16(m_flags2, slot, VslotMask, VslotShift);
    }
    inline unsigned getVirtualSlot()
    {
        return (m_flags2 >> VslotShift) & VslotMask;        
    }

    // static invocation
    void invokeStatic();
    void invokeStatic(void* pParam);

    // non-static invocation
    void invoke(Object* pObject);
    void invoke(Object* pObject, void* pParam);

    ClrToken retType();
    ClrTypeDef* getParam(unsigned n);
    bool setParam(ClrCodeHeap* pWsp, ClrTypeDef* pType, unsigned n);

    ClrTypeDef* getArg(unsigned n);
    void setArg(ClrTypeDefExtern* pType, unsigned n);

    void clearParams();

    inline void setInterfaceImpl(unsigned ifslot)
    {
        m_auxFlags.m_ifImpl = (uint16)ifslot;  
    }
    
    inline unsigned getInterfaceImpl()
    {
        return m_auxFlags.m_ifImpl;  
    }
    
    ClrMethod(const char* pname, unsigned nParam, int flags);
    static unsigned calculate_size(const char* name, unsigned nParam);
    
    inline void* operator new(size_t, ClrMethod* o)
    {
        return o;
    }

    inline VoidFVoid* getVtbl()
    {
        return (VoidFVoid*)this;
    }

    inline void setEntry(VoidFVoid addr)
    {
        *((VoidFVoid*)&m_methodPtr) = addr;
        #ifdef PLATFORM_PROCFAMILY_ARMTHUMB
        m_methodPtr |= 1;
        #endif
    }
    inline void setEntry(PtrType addr)
    {
        m_methodPtr = addr;
        #ifdef PLATFORM_PROCFAMILY_ARMTHUMB
        m_methodPtr |= 1;
        #endif
    }

    inline VoidFVoid getEntry()
    {
        return *((VoidFVoid*)&m_methodPtr);
    }
    inline PtrType getEntry2()
    {
        return m_methodPtr;
    }

    inline void setEntry(ClrMethod& method)
    {
        m_methodPtr = method.m_methodPtr;
    }

    void fullName(eString<128>& fullName);
    static bool isEqual(ClrMethod* pMethod1, ClrMethod* pMethod2);
    void _dump();
    typedef struct auxflags
    {
        uint16  m_ifImpl;
        uint16  m_flags;
    } AuxFlags;
    void swap(bool hasNaturalEndianess);
    private:
    typedef MetaSRef128K                    TypeRef;

    inline TypeRef* getParamTab()
    {
        return (TypeRef*)typedef_end;
    }

    static inline unsigned fixed_sizeof()
    {
        ClrMethod* p = (ClrMethod*)1000;
        return (char*)p->typedef_end - (char*)p;   
    }

    // 0 :  method pointer 32/64 bit
    union
    {                                        
        PtrType     m_methodPtr;        
        AuxFlags    m_auxFlags;     
    };
    // 4 :  method flags
    uint16  m_flags;            
    typedef struct _bits
    {
        uint16                  
            m_nParam    : 5,    // 0..4 : amount of parameters (0..31)
            m_ifImpl    : 1,    // true: interface implementation
            m_vSlot     : 10;   // 6..15 : virtual table slot (0 .. 1023)
    } Bits;
    
    // 6:  method spec
    uint16 m_flags2;
    // 8: 4 byte align
    // dynamic part
    char  typedef_end[1];

    // dynamic fields are not visible by the debugger, skip the declaration
    // TypeRef   paramTab[1];               
    // uint8    name[1];

};

class ClrField
{
    public :
    enum ClrFieldFlags
    {
#if 0
                uint16
            m_val8          : 6,    // offset bit 0..5 (represents bit 16..21 of the fieldoffset) 
            // flags
            m_flag3         : 1,    // unused
            m_flag4         : 1,    // unused
            m_static        : 1,    // field has static semantic
            m_literal       : 1,    // field is a literal (literalvalue is assigned by setValueRef())
            m_flag5         : 1,    // unused
            m_valueByRef    : 1,    // value is referenced by ValRef
            m_readonly      : 1,    // field has readonly semantic
            m_native        : 1,    // field is native represented (c++ interface)
            m_OPC           : 1,    // field has OPC semantic
            m_retain        : 1;    // field has retantive semantic
#endif
        Val8Mask        = 0x003F,
        Val8Shift       = 0,
        Flag3BitMask    = 0x0040,
        Flag4BitMask    = 0x0080,
        StaticBitMask   = 0x0100,
        LiteralBitMask  = 0x0200,
        Flag5BitMask    = 0x0400,
        ValueByRefBitMask  = 0x0800,
        ReadOnlyBitMask = 0x1000,
        NativeBitMask   = 0x2000,
        OPCBitMask      = 0x4000,
        RetainBitMask   = 0x8000,
        ClrFieldFlagsAny = 0
    };

    inline void setStatic()
    {
        m_flags |= StaticBitMask;
    }
    inline bool isStatic()
    {
        return (m_flags & StaticBitMask) ? true : false;
    }
    inline void setLiteral()
    {
        m_flags |= LiteralBitMask;
    }
    inline bool isLiteral()
    {
        return (m_flags & LiteralBitMask) ? true : false;
    }
    inline void setValueByRef()
    {
        m_flags |= ValueByRefBitMask;
    }
    inline bool isValueByRef()
    {
        return (m_flags & ValueByRefBitMask) ? true : false;
    }
    inline void setNative()
    {
        m_flags |= NativeBitMask;
    }
    inline bool isNative()
    {
        return (m_flags & NativeBitMask) ? true : false;
    }
    inline bool isManaged()
    {
        return !isNative();
    }
    inline void setOPC()
    {
        m_flags |= OPCBitMask;
    }
    inline bool isOPC()
    {
        return (m_flags & OPCBitMask) ? true : false;
    }
    inline void setRetain()
    {
        m_flags |= RetainBitMask;
    }
    inline bool isRetain()
    {
        return (m_flags & RetainBitMask) ? true : false;
    }
    inline void setReadOnly()
    {
        m_flags |= ReadOnlyBitMask;
    }
    inline bool isReadOnly()
    {
        return (m_flags & ReadOnlyBitMask) ? true : false;
    } 

    inline ClrTypeDef* getType()
    {
        return (ClrTypeDef*)m_type.get(this);      
    }
    inline const char* getName()
    {
        return m_name;
    }

    int getTypeSig();
    bool setType(ClrCodeHeap* pWsp, ClrTypeDef* pExtends);

    uint32 getLiteral();
    void getLiteral(ClrInt64& val);

    static bool isLiteralByRef(uint32 val);
    static bool isLiteralByRef(ClrInt64& val);
    bool setLiteral(ClrInt64& val);
    bool setLiteral(uint32 val);
    bool setLiteral(MetaAny* valref);

    bool setName(const char* name);

    bool setOffset(unsigned offs);
    unsigned getOffset();

    void setAddress(void* addr);
    void setAddress(ClrField& field);
    bool setAddressRef(MetaAny* addrref);
    void* getAddress();
    PtrType getAddress2();

    bool isValueType();

    inline bool isClass()
    {
        bool result = false;
        if (isLiteral() == false)
        {
            result = !isValueType();
        }
        return result;
    }

    unsigned getSize();
    unsigned getAlign();
    bool isSizeCalculated();
    bool isSigned();

    inline unsigned getValueRefOffset()
    {
        return  (char*)getValueRef()-(char*)this;
    }
    static bool cmpInternal(ClrField* pField1, ClrField* pField2);

    ClrField();
    
    inline void* operator new(size_t, ClrField* o)
    {
        return o;
    }

    typedef MetaSRef128K                    TypeRef;
    typedef MetaURef128K                    ValRef;
    typedef MetaCharRef                     NameRef;

    void dump();
    void swap(bool hasNaturalEndianess);


#define FIELDINFO_FIELD_OFFSET(classname, fieldname) \
    ( (char*)(&((classname*)100L)->fieldname) - (char*)((classname*)100L) )

//              fieldoffset layout
//
//                 15               7 6 5 4 3 2 1 0
//                 +---------------+---------------+
//              0  | flags         |   |  offs     |  16 .. 21    // possible value 0 .. 4.194.303
//                 +---------------+---------------+
//              2  |  offs 0 ..    15              |
//                 +---------------+---------------+
//              4  |    type                       |
//                 +---------------+---------------+
//              6  |    name                       |
//                 +---------------+---------------+

#define FIELDINFO_SET_OFFSET32LE(val32, offset32)  \
    ( (val32&0xFFC0) | ((offset32)<<16) | (((offset32)>>16)&0x3F) )

#define FIELDINFO_SET_OFFSET32BE(val32, offset32)  \
    ( (val32&0xFFC00000) | ((offset32)&0x3FFFFF) )

#define FIELDINFO_SET_OFFSET64LE(val64, offset32)  \
    ( (val64&~0xFFFF000F) | ((offset32)<<16)) | ((offset32)>>16) )

#define FIELDINFO_SET_OFFSET64BE(val64, offset32)  \
    ( (val64&0xFFF00000FFFFFFFFL) | (((offset32)&0xFFFFF)<<32) )

#ifdef PLATFORM_CTORS_NOTSUPPORTED
#ifdef PLATFORM_PREPROCESSOR_NEEDS_CTORS
// hack mainly because of GNU 4.x.x, the compiler generates ctor code for the FIELDINFO_FIELD_OFFSET macro
// without correct macro implementation, the type layout of native classes is not watched by the built-in linker
#undef FIELDINFO_SET_OFFSET32LE
#undef FIELDINFO_SET_OFFSET32BE
#undef FIELDINFO_SET_OFFSET64LE
#undef FIELDINFO_SET_OFFSET64BE
#define FIELDINFO_SET_OFFSET32LE(val32, offset32)  ( val32 )
#define FIELDINFO_SET_OFFSET32BE(val32, offset32)  ( val32 )
#define FIELDINFO_SET_OFFSET64LE(val64, offset32)  ( val64 )
#define FIELDINFO_SET_OFFSET64BE(val64, offset32)  ( val64 )
#endif
#endif


    private:

    inline void setValueRef(MetaAny* valref)
    {
        ((ValRef*)&m_val16)->set(valref);
    }
    inline MetaAny* getValueRef()
    {
        return (MetaAny*)((ValRef*)&m_val16)->get();
    }
    inline MetaAny* getValueRef2()
    {
        return (MetaAny*)((ValRef*)&m_val16)->get2();
    }
    inline uint32 getMaxValue()
    {
        return 0x3fffff;    // 22 bits means 2^22 - 1
    }

    // field info layout
    typedef struct _bits
    {
        uint16
            m_val8          : 6,    // offset bit 0..5 (represents bit 16..21 of the fieldoffset) 
            // flags
            m_flag3         : 1,    // unused
            m_flag4         : 1,    // unused
            m_static        : 1,    // field has static semantic
            m_literal       : 1,    // field is a literal (literalvalue is assigned by setValueRef())
            m_flag5         : 1,    // unused
            m_valueByRef    : 1,    // value is referenced by ValRef
            m_readonly      : 1,    // field has readonly semantic
            m_native        : 1,    // field is native represented (c++ interface)
            m_OPC           : 1,    // field has OPC semantic
            m_retain        : 1;    // field has retantive semantic
    } Bits;
    uint16  m_flags;
    uint16  m_val16;          // union : ValRef isValueByRef() == true
    //   !!! must be 4 byte aligned !!!
    TypeRef m_type;           // fields type, reference to TypeInfo
    NameRef m_name;           // union : BackRef isNameByRef() == false
    
};
class MetaTypeFieldLimit
{
    public :
    MetaTypeFieldLimit()
    {
        pValueNext = NULL;  
        pValueEnd  = NULL;  
        isOk = false;
    }
    void* pValueNext;  
    void* pValueEnd;  
    bool  isOk;
};

class ClrTypeField
{

    public :
    enum ClrTypeFieldSpec
    {
        // internal usage
        MaxChunkFields  = 2048,         // must be 2^N
#if 0
        uint16                  // the first 16 bits
            m_hasObjectMember       : 1,    // class layout has object member
            m_hasPointerMember      : 1,    // class layout has pointer member
            m_hasObjectMemberStatic : 1,    // static class layout has object member
            m_hasPointerMemberStatic: 1,    // static class layout has pointer member
            m_flag5     : 1,    // unused
            m_flag6     : 1,    // unused
            m_flag7     : 1,    // unused
            m_flag8     : 1,    // unused
            m_iChunk    : 4,    // current chunk            |
            m_nChunk    : 4;    // amount of chunks     highest bit
#endif
        HasObjectMemberBitMask          = 0x0001,
        HasPointerMemberBitMask         = 0x0002,
        HasObjectMemberStaticBitMask    = 0x0004,
        HasPointerMemberStaticBitMask   = 0x0008,
        IChunkMask                      = 0x000F,
        IChunkShift                     = 8,
        NChunkMask                      = 0x000F,
        NChunkShift                     = 12,

        MaxChunks       = 30
    };

    static ClrTypeField* createTypeField(ClrCodeHeap* wsp, unsigned nChunk, unsigned iChunk, 
        unsigned nField, unsigned iFields, MetaTypeFieldLimit& limit);
    static inline ClrTypeField* getFirst(BlockHeap* h)
    {
        return (ClrTypeField*)h->getFirst(ClrMetaDataDef::MetaTypeField);
    }

    inline ClrTypeField* getNext(BlockHeap* h)
    {
        return (ClrTypeField*)h->getNext(this, ClrMetaDataDef::MetaTypeField);
    }
    static ClrTypeField* clone(ClrCodeHeap* pWsp, BlockHeap* pMeta, ClrTypeField* pTypeField);

    inline unsigned getFieldAmount()
    {
        return m_nFields;
    }

    // non-static layout
    inline void setObjectMember()
    {
        m_flags |= HasObjectMemberBitMask;
    }
    inline bool hasObjectMember()
    {
        return (m_flags & HasObjectMemberBitMask) ? true : false;
    }
    inline void setPointerMember()
    {
        m_flags |= HasPointerMemberBitMask;
    }
    inline bool hasPointerMember()
    {
        return (m_flags & HasPointerMemberBitMask) ? true : false;
    }
    // static layout
    inline void setObjectMemberStatic()
    {
        m_flags |= HasObjectMemberStaticBitMask;
    }
    inline bool hasObjectMemberStatic()
    {
        return (m_flags & HasObjectMemberStaticBitMask) ? true : false;
    }
    inline void setPointerMemberStatic()
    {
        m_flags |= HasPointerMemberStaticBitMask;
    }
    inline bool hasPointerMemberStatic()
    {
        return (m_flags & HasPointerMemberStaticBitMask) ? true : false;
    }

    ClrField* getField(unsigned n);
    ClrField* getField(const char* name, unsigned& slot);
    unsigned getSlot(ClrField* pField);
    bool setName(unsigned n, const char* name, MetaTypeFieldLimit& limit);

    bool setTypeField(ClrTypeField* pTypeField, unsigned iChunk);
    ClrTypeField* getTypeField(unsigned n);

    bool setLiteral(ClrField* pField, ClrInt64 val, MetaTypeFieldLimit& limit);   
    static unsigned accumulateLiteral(unsigned prevAccum, ClrInt64& val64, unsigned size);
    bool setLiteral(ClrField* pField, uint32 val, MetaTypeFieldLimit& limit);   
    bool setAddress(ClrField* pField, MetaTypeFieldLimit& limit);

    static bool checkCompact(unsigned nField, unsigned fieldNamesSize, unsigned fieldLiteralSize);

    bool realloc(ClrCodeHeap* pWsp, MetaTypeFieldLimit& limit);
    unsigned calcMetaSize();

    void swap(bool hasNaturalEndianess);
    private:

    typedef MetaSRef32                  ChunkRef;

    ClrTypeField(unsigned nChunk, unsigned iChunk, unsigned nChunkFields);
    ~ClrTypeField();


    
    inline ClrField* getFieldTab()
    {
        unsigned idx = getChunkAmount();
        return (ClrField*)&(getChunkTab())[idx];
    }
    inline ChunkRef* getChunkTab()
    {
        return (ChunkRef*)(this+1);
    }

    inline void setChunk(unsigned nChunk, unsigned iChunk)
    {
        m_flags |= (nChunk & NChunkMask) << NChunkShift;
        m_flags |= (iChunk & IChunkMask) << IChunkShift;
    }

    inline unsigned getCurrentChunk()
    {
        return (m_flags >> IChunkShift) & IChunkMask;
    }

    inline unsigned getChunkAmount()
    {
        return (m_flags >> NChunkShift) & NChunkMask;
    }

    inline unsigned getChunkFieldAmount()
    {
        return min(m_nFields, MaxChunkFields);
    }

    static uint32 calculate_size(unsigned nChunk, unsigned nFields);
    
    inline void* operator new(size_t, ClrTypeField* o)
    {
        return o;
    }
    void operator delete(void* p);

    typedef struct _bits
    {
        uint16                  // the first 16 bits
            m_hasObjectMember       : 1,    // class layout has object member
            m_hasPointerMember      : 1,    // class layout has pointer member
            m_hasObjectMemberStatic : 1,    // static class layout has object member
            m_hasPointerMemberStatic: 1,    // static class layout has pointer member
            m_flag5     : 1,    // unused
            m_flag6     : 1,    // unused
            m_flag7     : 1,    // unused
            m_flag8     : 1,    // unused
            m_iChunk    : 4,    // current chunk            |
            m_nChunk    : 4;    // amount of chunks     highest bit
    } Bits;
    // 0:
    uint16  m_flags;
    // 2: sizeof 2
    uint16  m_nFields;
    // 4 byte aligned

    // dynamic fields are not visible by the debugger, skip the declaration

};

class ClrStringLiteral
{
    public :
    static ClrStringLiteral* createStringLiteral(ClrCodeHeap* pWsp, unsigned literalSize, MetaLimit& limit);
    static inline ClrStringLiteral* getFirst(BlockHeap* h)
    {
        return (ClrStringLiteral*)h->getFirst(ClrMetaDataDef::MetaStringLiteral);
    }
    inline ClrStringLiteral* getNext(BlockHeap*  h)
    {
        return (ClrStringLiteral*)h->getNext(this, ClrMetaDataDef::MetaStringLiteral);
    }
    String* addLiteral(const uint16* data, unsigned len, MetaLimit& limit);
    String* addLiteral(String* pString, MetaLimit& limit);

    inline String* getFirst() 
    {
        return (String*)this;
    }
    String* getNext(String* pString);

    void swap(bool hasNaturalEndianess, unsigned length);

    private:

    String  string;
};

class ClrApplStaticDataInfo
{
    public :
    static ClrApplStaticDataInfo* createApplStaticDataInfo(ClrCodeHeap* pWsp, unsigned nTypeDef);
    static inline ClrApplStaticDataInfo* getFirst(BlockHeap* h)
    {
        return (ClrApplStaticDataInfo*)h->getFirst(ClrMetaDataDef::MetaApplStaticDataInfo);
    }
    inline ClrApplStaticDataInfo* getNext(BlockHeap*  h)
    {
        return (ClrApplStaticDataInfo*)h->getNext(this, ClrMetaDataDef::MetaApplStaticDataInfo);
    }

    void* next()
    {
        BlockHeap::iterator it((const void*)this);
        return (char*)this + (*it).size();
    }

    uint32 getTypeOffset(ClrTypeDef* pTypeDef);
    uint32 getSize();

    bool setType(ClrTypeDef* pType, uint32 size, unsigned n);
    ClrTypeDef* getType(unsigned n, uint32& size);
    ClrTypeDef* getType(uint32 offset);
    ClrTypeDef* getType2(uint32 segmentOffs, uint32& typeOffs);
    inline unsigned getTypeAmount()
    {
        return m_nTypes;
    }
    typedef MetaSRef32                  TypeRef;
    void swap(bool hasNaturalEndianess);

    private:

    typedef struct _staticdatadef
    {
        TypeRef     type;
        uint32      size;   
    } StaticDataDef;

    inline StaticDataDef* getStaticDataTab()
    {
        return (StaticDataDef*)(this+1);
    }

    ClrApplStaticDataInfo(unsigned nTypeDef);

    inline void* operator new(size_t, ClrApplStaticDataInfo* o)
    {
        return o;
    }
    void operator delete(void* p);

    uint16  m_flags;
    uint16  m_nTypes;
    // 4 byte align
    // dynamic part
    // dynamic fields are not visible by the debugger, skip the declaration

    
};

class ClrTypeStaticData : public Object
{
    public :
    static inline ClrTypeStaticData* getFirst(BlockHeap* h)
    {
        return (ClrTypeStaticData*)h->getFirst(ClrMetaDataDef::MetaTypeStaticData);
    }
    inline ClrTypeStaticData* getNext(BlockHeap*  h)
    {
        return (ClrTypeStaticData*)h->getNext(this, ClrMetaDataDef::MetaTypeStaticData);
    }

    void* getAddress(unsigned offset)
    {
        void* result = (void*)((char*)this + offset);
        return result;
    }
    
    unsigned getSize();
    
    inline void* getData()
    {
        return this+1;
    }


    void swap();

    private:

    inline ClrTypeStaticData() {}

    inline void* operator new(size_t, ClrTypeStaticData* o)
    {
        return o;
    }

    // dynamic part
    // 4 byte align
    // dynamic fields are not visible by the debugger, skip the declaration

};

// managed meta heap classes
class ClrMetaAssembly
{
    public :
    enum ClrMetaAssemblySpec
    {
        Loaded      = 0x01
    };

    static ClrMetaAssembly* createMetaAssembly(ClrCodeHeap* h, const char* name, uint32 size);
    inline unsigned AddRef()
    {
        return ++refCnt;
    }
    unsigned Release();
    inline unsigned reference()
    {
        return refCnt;
    }

    inline uint32 getSize()
    {
        return size;
    }

    inline const uint8* getData()
    {
        return (const uint8*)&name[nName];
    }

    inline const char* getName()
    {
        return (const char*)name;
    }

    inline void setLoaded()
    {
        flags |= Loaded;
    }

    inline bool isLoaded()
    {
        return (flags & Loaded) ? true : false;
    }

    private:

    ClrMetaAssembly(const char* _name, uint32 fsize);

    static unsigned nameLength(const char* name);
    static unsigned calculate_size(const char* name, uint32 size);

    static inline unsigned fixed_sizeof()
    {
        ClrMetaAssembly* p = (ClrMetaAssembly*)1000;
        unsigned result = (char*)p->name - (char*)p;
        return result;   
    }

    inline void* operator new(size_t, ClrMetaAssembly* o)
    {
        return o;
    }
    void operator delete(void* p);

    uint32  size;
    uint16  refCnt;
    uint16  flags;
    uint8   nName;
    // dynamic part
    char    name[1];
    // dynamic fields are not visible by the debugger, skip the declaration
    //uint8   data[1];   

};

class ClrMethodHeader
{
    public :
    enum ClrMethodHeaderSpec
    {
        DummyImg    = 0x00,     // 00000
        TokenImg    = 0x01,     // 00001
        CodeImg     = 0x02,     // 00010
        DebugImg    = 0x03,     // 00011
        ImgTypeMask = 0x03      // 00011
    };

    typedef MetaAbsRef32                MethodRef;

    ClrMethodHeader(enum ClrMethodHeaderSpec spec);

    inline unsigned AddRef()
    {
        return ++m_refCnt;
    }
    inline unsigned reference()
    {
        return m_refCnt;
    }
    unsigned Release();

    bool setBacklink(BlockHeap* metaHeap, ClrMethod* lnk);
    ClrMethod* getBacklink(BlockHeap* metaHeap);

    uint16      m_refCnt;
    uint16      m_flags; 
    MethodRef   m_backlink;


    inline void* operator new(size_t, ClrMethodHeader* o)
    {
        return o;
    }
    
    void operator delete(void* p);

};

class ClrMethodToken : public ClrMethodHeader
{
    public :
    
    static ClrMethodToken* createMethodToken(ClrCodeHeap* h, ClrMethod* pMethod, const uint8* pTok, unsigned ilLength,  const uint8* pLsig, unsigned sigLength);
    static void assignClone(ClrCodeHeap* pWsp, ClrMethod* pMethod, ClrMethod* pMethodClone);
    static void relocate(ClrCodeHeap* pWsp, long reloc);
    static inline ClrMethodToken* getFirst(BlockHeap* h)
    {
        return (ClrMethodToken*)h->getFirst(ClrMetaDataDef::MetaToken);
    }
    inline ClrMethodToken* getNext(BlockHeap* h)
    {
        return (ClrMethodToken*)h->getNext(this, ClrMetaDataDef::MetaToken);
    }
    static ClrMethodToken* getToken(ClrCodeHeap* h, ClrMethod* pMethod);

    unsigned Release();

    inline const uint8* getTokenCode()
    {
        const uint8* p = getToken();
        return p;
    }

    inline unsigned getTokenCodeLength()
    {
        return codeSize;
    }

    inline const uint8* getLocalVarSignature()
    {
        const uint8* result = NULL;
        if (sigSize > 0)
        {
            result = sigTab;
        }
        return result;
    }

    inline unsigned getLocalVarSignatureLength()
    {
        return sigSize;
    }

    inline void setMetaToken(uint32 token)
    {
        m_methodToken = token;
    }
    inline uint32 getMetaToken()
    {
        return m_methodToken;
    }
    void swap();

    private :
    
    inline const uint8* getToken()
    {
        const uint8* p = (const uint8* )&sigTab[sigSize];
        return p;
    }

    static inline uint32 calculate_size(unsigned ilLength, unsigned sigLength)
    {
        uint32 result = fixed_sizeof();

        // count the signature (if exist)
        result += sigLength;
        result += ilLength;

        return result;   
    }

    static inline unsigned fixed_sizeof()
    {
        ClrMethodToken* p = (ClrMethodToken*)1000;
        return (char*)p->sigTab - (char*)p;   
    }

    ClrMethodToken(const uint8* pIl, unsigned ilLength, const uint8* lsig, unsigned sigLength);

    uint32  codeSize;
    uint32  m_methodToken;  // .Net method metatoken
    uint16  sigSize;

    uint8   sigTab[1];
    // dynamic fields are not visible by the debugger, skip the declaration
    //uint8   ilTab[1];
    
    inline void* operator new(size_t, ClrMethodToken* o)
    {
        return o;
    }
    void operator delete(void* p);

};

class ClrMethodCode
{
    public :
    enum ClrMethodCodeSpec
    {
        RelocTab32  = 0x0001,
        EntryMethod = 0x0002,
        BigEndian   = 0x0004,
        
        // internal
        RelocExceeded  = 0x8000
    };

    static ClrMethodCode* createMethodCode(ClrCodeHeap* h, ClrMethod* pMethod, unsigned ilLength);
    static inline ClrMethodCode* getFirst(BlockHeap* h)
    {
        return (ClrMethodCode*)h->getFirst(ClrMetaDataDef::MetaCode);
    }
    inline ClrMethodCode* getNext(BlockHeap* h)
    {
        return (ClrMethodCode*)h->getNext(this, ClrMetaDataDef::MetaCode);
    }

    static ClrMethodCode* getCode(ClrCodeHeap* h, ClrMethod* pMethod);

    inline void setReloc32()
    {
        m_flags |= RelocTab32;
    }
    inline void resetReloc32()
    {
        m_flags &= ~RelocTab32;
    }
    inline bool isReloc32()
    {
        return (m_flags & RelocTab32) ? true : false;
    }
    inline void setRelocExceeded()
    {
        m_flags |= RelocExceeded;
    }
    inline bool isRelocExceeded()
    {
        return (m_flags & RelocExceeded) ? true : false;
    }
    void setBigEndian()
    {
        m_flags |= BigEndian;
    }
    inline bool isBigEndian()
    {
        return (m_flags & BigEndian) ? true : false;
    }

    inline VoidFVoid entryPoint()
    {
        return (VoidFVoid)getCodeStart();
    }
    inline void* startPoint()
    {
        return (void*)getCodeStart();
    }

    inline unsigned getEntryOffset()
    {
        return (char*)getCodeStart()-(char*)this;
    }
    inline void* getCode(unsigned offset)
    {
        return getCodeStart()+offset;
    }
    inline unsigned getOffset(void* pCode)
    {
        return (uint8*)pCode - getCodeStart();
    }


    inline uint32 getCodeSize()
    {
        return m_codeSize*sizeof(uint16);
    }

    inline void setEntryMethod()
    {
        m_flags |= EntryMethod;
    }
    inline bool isEntryMethod()
    {
        return (m_flags & EntryMethod) ? true : false;
    }

    bool setEntry(BlockHeap* metaHeap);

    bool realloc(uint32 code_size);

    inline unsigned getRelocAmount()
    {
        return m_nReloc;
    }

    bool incReloc();
    bool setRelocEntry(RelocationEntry& renty, unsigned n);
    bool getRelocEntry(RelocationEntry& renty, unsigned n);
    
    void recalcReloc(unsigned start, int displacement);
    
    bool setBacklink(BlockHeap* metaHeap, ClrMethod* lnk);
    ClrMethod* getBacklink(BlockHeap* metaHeap);


//  relocation entry32 layout
//
//     31           16           0
//     +------------+------------+
//  0  | meta address * 4        |  quad address to meta object (0 .. 2^31)
//     +------------+------------+
//  4  | code offset          |T |  code offset (0 .. 2^29) , bit 0,1 Relocation type
//     +------------+------------+

    class RelocEnty32
    {
        public:
        inline void setMeta(unsigned meta)
        {
            metaAddr = meta/sizeof(uint32);   
        }
        inline unsigned getMeta()
        {
            return metaAddr*sizeof(uint32);   
        }
        inline void setCode(unsigned code, int type)
        {
            code = code/sizeof(TargetInstructionType);
            codeAddr = code<<2 | type;   
            codeAddr |= type & 0x3;   
        }
        inline unsigned getCode()
        {
            unsigned result = codeAddr>>2;
            result = result*sizeof(TargetInstructionType);
            return result;
        }
        inline int getType()
        {
            return codeAddr & 0x3;
        }
        inline void clone(RelocEnty32& entry32)
        {
            metaAddr = entry32.metaAddr;    
            codeAddr = entry32.codeAddr;    
        }
        inline uint32 getMetaFlat()
        {
            return metaAddr;   
        }
        inline uint32 getCodeFlat()
        {
            return codeAddr;   
        }
        void swap();
        private:
        uint32 metaAddr;
        uint32 codeAddr;

    };
//              relocation entry16 layout
//
//                 15            7           0
//                 +------------+------------+
//              0  | meta address * 4        |  quad address to meta object (0 .. 262143)
//                 +------------+------------+
//              2  | code offset          |T |  code offset (0 .. 16383) , bit 0,1 Relocation type
//                 +------------+------------+

    class RelocEnty16
    {
        public:
        inline void setMeta(unsigned meta)
        {
            metaAddr = (uint16)(meta/sizeof(uint32));   
        }
        inline unsigned getMeta()
        {
            return metaAddr*sizeof(uint32);   
        }
        inline void setCode(unsigned code, int type)
        {
            code = code/sizeof(TargetInstructionType);
            codeAddr = (uint16)(code<<2 | type);   
            codeAddr |= (uint16)(type & 0x3);   
        }
        inline unsigned getCode()
        {
            unsigned result = codeAddr>>2;
            result = result*sizeof(TargetInstructionType);
            return result;
        }
        inline int getType()
        {
            return codeAddr & 0x3;
        }
        inline void clone(RelocEnty32& entry32)
        {
            metaAddr = (uint16)entry32.getMetaFlat();    
            codeAddr = (uint16)entry32.getCodeFlat();    
        }
        void swap();
        private:
        uint16 metaAddr;
        uint16 codeAddr;
    };
    void swap(bool hasNaturalEndianess);

    private :

    ClrMethodCode(uint32 maxCodeSize);
    
    uint32 calculate_size();


    inline uint8* getCodeStart()
    {
        return (uint8*)(this+1);
    }

    RelocEnty16* getRelocTab();

    inline void setCodeSize(uint32 size)
    {
        m_codeSize = size/sizeof(uint16);
    }
    typedef MetaAbsRef32                MethodRef;

    void copyAndConvertRelocTable(RelocEnty16* pRelocNew16, RelocEnty32* pRelocOld32);

    inline void* operator new(size_t, ClrMethodCode* o)
    {
        return o;
    }
    void operator delete(void* p);

//  MethodCode layout
//
//     31           16           0
//     +------------+------------+
//  0  | method meta address     |  quad address to method info object
//     +------------+------------+
//  4  | code size               |  native code size div sizeof(uint16)
//     +------------+------------+
//  8  | nreloc        flags     |  flags, amount of relocation info
//     +------------+------------+
//  12 | unused       stacksize  |  worst case stack consumption (div 8)
//     +------------+------------+


    MethodRef   m_backlink;
    uint32      m_codeSize;
    uint16      m_flags; 
    uint16      m_nReloc;
    uint16      m_stackUsed;
    uint16      m_unused;
    // 8 byte align
    // dynamic fields are not visible by the debugger, skip the declaration
    //uint8   codeArea[1];
    //uint32  relocTab[1];
};

class ClrDebugInfoItem
{
    public:
    enum ClrDebugInfoFlags
    {
        // 8 bit available
        Branch      = 0x01,
        Label       = 0x02,
        EnterBank   = 0x04,
        LeaveBank   = 0x08,
        Call        = 0x10,
        Return      = 0x20,
        Switch      = 0x40,
        // internal compiler usage
        Assigned    = 0x80,
        UnAssigned  = 0x00
    };
    inline void setFlags(int f)
    {
        m_flags |= f;
    }
    inline void resetFlags(int f)
    {
        m_flags &= ~f;
    }
    inline int getFlags()
    {
        return m_flags;
    }

    inline bool isAssigned()
    {
        return (m_flags & Assigned) ? true : false;
    }
    inline void setAssigned()
    {
        m_flags |= Assigned;
    }

    inline void setLabel()
    {
        m_flags |= Label;
    }
    inline bool isLabel()
    {
        return (m_flags & Label) ? true : false;
    }
    inline void setBranch()
    {
        m_flags |= Branch;
    }
    inline void resetBranch()
    {
        m_flags &= ~Branch;
    }
    inline bool isBranch()
    {
        return (m_flags & Branch) ? true : false;
    }
    inline bool isSwitch()
    {
        return (m_flags & Switch) ? true : false;
    }
    inline void setBankEnter()
    {
        m_flags |= EnterBank;
    }
    inline bool isBankEnter()
    {
        return (m_flags & EnterBank) ? true : false;
    }
    
    inline void setBankLeave()
    {
        m_flags |= LeaveBank;
    }

    inline void resetBankLeave()
    {
        m_flags &= ~LeaveBank;
    }


    inline bool isBankLeave()
    {
        return (m_flags & LeaveBank) ? true : false;
    }

    inline unsigned getEvalstackDepth()
    {
        return m_evalstack;
    }
    inline void setEvalstackDepth(unsigned depth)
    {
        m_evalstack = depth;
    }

    inline void setCall()
    {
        m_flags |= Call;
    }
    inline bool isCall()
    {
        return (m_flags & Call) ? true : false;
    }
    inline void setReturn()
    {
        m_flags |= Return;
    }
    inline bool isReturn()
    {
        return (m_flags & Return) ? true : false;
    }
    inline void setNative(unsigned addr)
    {
        addr = TARGET_SET_ADDRESS(addr);
        m_naAddr    = (uint16)addr;    
        m_naHigh    = (uint16)(addr >> 16);    
    }
    inline unsigned getNative()
    {
        unsigned result = ((unsigned)m_naHigh << 16) + m_naAddr;
        result = TARGET_GET_ADDRESS(result);
        return result;
    }
    
    inline void setIlAddr(unsigned addr)
    {
        m_ilAddr    = (uint16)addr;
        m_ilHigh    = (uint16)(addr >> 16);    
    }
    inline unsigned getIlAddr()
    {
        return ((unsigned)m_ilHigh << 16) + m_ilAddr;    
    }
    
    inline void setIlDstAddr(unsigned addr)
    {
        m_addr2     = (uint16)addr;
        m_addr2High = (uint16)(addr >> 16);    
    }
    inline unsigned getIlDstAddr()
    {
        return ((unsigned)m_addr2High << 16) + m_addr2;    
    }
    inline void setBranchDestination(unsigned addr)
    {
        m_addr2     = (uint16)addr;
        m_addr2High = (uint16)(addr >> 16);    
    }
    inline unsigned getBranchDestination()
    {
        return ((unsigned)m_addr2High << 16) + m_addr2;    
    }
    inline void setMethodHandle(unsigned methodHandle)
    {
        setIlDstAddr(methodHandle);
    }
    inline unsigned getMethodHandle()
    {
        return getIlDstAddr();
    }
    inline void setReturnHelperDist(unsigned dist)
    {
        setIlDstAddr(dist);
    }
    inline unsigned getReturnHelperDist()
    {
        return getIlDstAddr();
    }
    inline void setSwitchLabelAmount(unsigned nLabel)
    {
        setIlDstAddr(nLabel);
    }
    inline unsigned getSwitchLabelAmount()
    {
        return getIlDstAddr();
    }

    bool setBranchOffset(unsigned branchOffset)
    {
        bool result = false;
        if (branchOffset < 256)
        {
            m_dist = branchOffset;
            result = true;
        }
        return result;
    }
    unsigned getBranchOffset()
    {
        return m_dist;
    }
    private:

    uint16  m_ilAddr;       // 16 Mb maximum il code per method
    uint16  m_naAddr;       // 16 Mb maximum native code per method
    uint16
            m_naHigh    : 8,    // native Address 16..23
            m_ilHigh    : 8;    // il Address 16..23
    uint16                  
            m_flags     : 8,    // flags
            m_evalstack : 8;    // current evaluation stack depth
    uint16  m_addr2;        // 16 Mb maximum 
    uint16                  
            m_addr2High : 8,    // addr2 16..23
            m_dist      : 8;    // branchoffset
};

class ClrDebugInfoLocalVars
{
public :
    void Init(void* pBase, ClrTypeDef* pTypeInfo, int displ);
    bool isSet()
    {
        return !m_type.isNull();
    }
    ClrTypeDef* getTypeInfo(void* pBase)
    {
        return (ClrTypeDef*)m_type.get(pBase);
    }
    unsigned getTypeHandle()
    {
        return m_type.get();
    }
    int getOffset()
    {
        int result = m_offs;
        if (m_sign != 0)
        {
            result = -result;
        }
        return result;
    }
    bool isRegister()
    {
        return (m_isreg != 0) ? true : false;
    }
    private:
    MetaAbsRef32    m_type;
    uint16          
                    m_sign  : 1,
                    m_isreg : 1,
                    m_empty : 14;
    uint16          m_offs;
    
};

class ClrDebugInfo : public ClrMethodHeader
{
    public :

    typedef ClrDebugInfoItem*  iterator;

    static ClrDebugInfo* createDebugInfo(ClrCodeHeap* pWsp, ClrMethod* pMethod, unsigned nLabel, unsigned nLoc, unsigned nArg);
    static inline ClrDebugInfo* getFirst(BlockHeap* h)
    {
        return (ClrDebugInfo*)h->getFirst(ClrMetaDataDef::MetaDebugInfo);
    }
    inline ClrDebugInfo* getNext(BlockHeap* h)
    {
        return (ClrDebugInfo*)h->getNext(this, ClrMetaDataDef::MetaDebugInfo);
    }



    inline unsigned getDebugLabelAmount()
    {
        return m_nLabel;
    }
    inline unsigned getLocalAmount()
    {
        return m_nLoc;
    }
    inline unsigned getArgAmount()
    {
        return m_nArg;
    }

    iterator getMap(unsigned ilAddr);
    bool sortLabel(iterator it);
    bool recalcMap(uint32 start, int displacement);

    iterator begin();
    iterator end();

    bool setMapIl(iterator it, uint32 ilAddr);
    bool setMapBranch(iterator it, uint32 destAddr);
    bool setMapLabel(iterator it);
    iterator getNextLabel(iterator itbegin, iterator itend, unsigned ilAddr);
    inline ClrDebugInfoItem* getIlToNativeMap()
    {
        return (ClrDebugInfoItem*)(debuginfo_end + m_nVarItems*sizeof(ClrDebugInfoLocalVars));
    }
    inline ClrDebugInfoLocalVars* getLocal(unsigned idx)
    {
        ClrDebugInfoLocalVars* result = NULL;
        if (idx < m_nLoc)
        {
            result = (ClrDebugInfoLocalVars*)(debuginfo_end + idx*sizeof(ClrDebugInfoLocalVars));
        }
        return result;
    }
    bool setLocal(unsigned idx, void* pBase, ClrTypeDef* pTypeInfo, int displ);

    void dump(void* pBase);
    void swap();
    unsigned Release();
    private :

    typedef MetaAbsRef32                TypeRef;

    ClrDebugInfo(unsigned nDebugLabel, unsigned nLoc, unsigned nArg, unsigned nIntern);
    ~ClrDebugInfo();
    
    ClrDebugInfoItem* findKey(uint32 ilAddr);
    iterator binSearchKey(iterator itbegin, iterator itend, unsigned ilAddr);
    iterator seqSearchKey(iterator itbegin, iterator itend, unsigned ilAddr);
    static int compLabel(const void * p1, const void *p2);
    static inline unsigned fixed_sizeof()
    {
        ClrDebugInfo* p = (ClrDebugInfo*)1000;
        return (char*)p->debuginfo_end - (char*)p;   
    }
    void operator delete(void* p);

    uint32  m_nLabel;
    uint16  
            m_nArg  : 6,
            m_nLoc  : 10;
    uint16  m_nVarItems;
    char    debuginfo_end[1];

    // dynamic fields are not visible by the debugger, skip the declaration
    // ClrDebugInfoLocalVars locTab[nLoc]; // locals tab              
    // ClrDebugInfoLocalVars argTab[nArg]; // arg tab              
    // ClrDebugInfoLocalVars intTab[nIntern]; // intern tab
    // m_nVarItems = nLoc+nArg+nIntern;
    // IlToNativeMap  ilMap[nLab];  // il to native map
};

class ClrMethodTokenMapItem
{
    public :
    MetaAbsRef32    m_method;
    uint32          m_token;
};

class ClrMethodTokenMap
{
    public :

    typedef ClrMethodTokenMapItem*  iterator;

    static ClrMethodTokenMap* createMethodTokenMap(BlockHeap* pHeap, unsigned nMethod);
    static inline ClrMethodTokenMap* getFirst(BlockHeap* pHeap)
    {
        return (ClrMethodTokenMap*)pHeap->getFirst(ClrMetaDataDef::MetaMethodTokenMap);
    }
    inline ClrMethodTokenMap* getNext(BlockHeap* pHeap)
    {
        return (ClrMethodTokenMap*)pHeap->getNext(this, ClrMetaDataDef::MetaMethodTokenMap);
    }
    iterator begin()
    {
        return (iterator)(this+1);
    }
    void swap();
    private :

    ClrMethodTokenMap(unsigned nMethod)
    {
        m_nMethod = nMethod;
    }
    ~ClrMethodTokenMap();
    static unsigned calculate_size(unsigned nMethod)
    {
        return sizeof(ClrMethodTokenMap) + nMethod*sizeof(ClrMethodTokenMapItem);
    }
    
    static inline unsigned fixed_sizeof()
    {
        return sizeof(ClrMethodTokenMap);   
    }
    inline void* operator new(size_t, ClrMethodTokenMap* o)
    {
        return o;
    }
    void operator delete(void* p);

    uint32  m_nMethod;

    // dynamic fields are not visible by the debugger, skip the declaration
};

class ClrAssemblyRef
{
    public :
    enum ClrAssemblyRefSpec
    {
        Null    = 0x0000
    };


    static ClrAssemblyRef* createAssemblyRef(ClrCodeHeap* h, ClrAssemblyDef* pAssemblyDef, unsigned nTypeDef, unsigned nTypeRef, unsigned nMemberRef, unsigned nLiteralRef, unsigned nTypeSpec);
    static void assignClone(ClrCodeHeap* pWsp, ClrTypeDef* pType, ClrTypeDef* pTypeClone);
    void assignClone(ClrTypeDef* pType, ClrTypeDef* pTypeClone);
    static void assignClone(ClrCodeHeap* pWsp, ClrAssemblyDef* pAssembly, ClrAssemblyDef* pAssemblyClone);
    static void assignClone(ClrCodeHeap* pWsp, String* pString, String* pStringClone);
    void assignClone(String* pString, String* pStringClone);
    static void relocate(ClrCodeHeap* pWsp, long reloc);
    void relocate(void* pBase, long reloc);
    static inline ClrAssemblyRef* getFirst(BlockHeap* h)
    {
        return (ClrAssemblyRef*)h->getFirst(ClrMetaDataDef::MetaAssemblyRef);
    }
    inline ClrAssemblyRef* getNext(BlockHeap*  h)
    {
        return (ClrAssemblyRef*)h->getNext(this, ClrMetaDataDef::MetaAssemblyRef);
    }

    inline unsigned AddRef()
    {
        return ++m_refCnt;
    }
    unsigned Release();
    inline unsigned reference()
    {
        return m_refCnt;
    }
    inline unsigned getIndexTabSize()
    {
        return m_nTypeDef;
    }
    bool setTypeRef(ClrTypeDef* pType, unsigned n);
    bool setTypeSpec(ClrTypeDef* pType, unsigned n);
    bool setFieldRef(ClrTypeDef* pType, ClrField* pField, unsigned n, unsigned slot);
    bool setMethodRef(ClrTypeDef* pType, ClrMethod* pMethod, unsigned n, unsigned slot);
    bool setStringLiteral(String* pString, uint32 idx, unsigned n);

    // 6. tab
    inline uint16* getFieldIndexTab()
    {
        return(uint16*)&(getStringIndexTab())[m_nLiteralRef];
    }
    // 7. tab
    inline uint16* getMethodIndexTab()
    {
        return(uint16*)&(getFieldIndexTab())[m_nTypeDef];
    }
    // 8. tab
    inline uint16* getMemberSlotTab()
    {
        return(uint16*)&(getFieldIndexTab())[m_nTypeDef*2];
    }
 
    ClrTypeDef* getTypeRef(unsigned n);
    ClrTypeDef* getTypeSpec(unsigned n);
    ClrField* getFieldRef(unsigned n);
    ClrField* getFieldRef(unsigned n, ClrTypeDef** ppType);
    ClrMethod* getMethodRef(unsigned n);
    ClrTypeDef* getMemberTypeRef(unsigned n);
    String* getStringLiteral(uint32 idx);

    typedef ClrTypeDef*         TypeRef;
    typedef ClrTypeDef*         MemberTypeRef;
    typedef String*             StringRef;

    // allways 32 bit reference
    typedef MetaAbsRef32        AssemblyDefRef;

    private:

    ClrAssemblyRef(ClrAssemblyDef* pAssemblyDef, unsigned nTypeDef, unsigned nTypeRef, unsigned nMemberRef, unsigned nLiteralRef, unsigned nTypeSpec);
    ~ClrAssemblyRef();
    
    // referance table layout :
    
    // 1. tab
    inline TypeRef* getTypeRefTab()
    {
        return (TypeRef*)(this+1);
    }
    // 2. tab
    inline TypeRef* getMemberTypeRefTab()
    {
        return(TypeRef*)&(getTypeRefTab())[m_nTypeRef];
    }
    // 3. tab
    inline TypeRef* getTypeSpecTab()
    {
        return(TypeRef*)&(getMemberTypeRefTab())[m_nMemberRef];
    }
    // 4. tab
    inline StringRef* getStringLiteralTab()
    {
        return(StringRef*)&(getTypeSpecTab())[m_nTypeSpec];
    }
    // 5. tab
    inline uint32* getStringIndexTab()
    {
        return(uint32*)&(getStringLiteralTab())[m_nLiteralRef];
    }
    
    static uint32 calculate_size(unsigned nTypeDef, unsigned nTypeRef, unsigned nMemberRef, unsigned nLiteral, unsigned nTypeSpec);

    inline void* operator new(size_t, ClrAssemblyRef* o)
    {
        return o;
    }
    void operator delete(void* p);

    AssemblyDefRef      m_backlink;
    uint16  m_refCnt;
    uint16  m_nTypeDef;
    // 8 byte align
    uint16  m_nTypeRef;
    uint16  m_nTypeSpec;
    uint16  m_nMemberRef;
    uint16  m_nLiteralRef;
    // 8 byte align

    // dynamic part

};
// managed meta heap classes
class ClrMetaUnresolved
{
    public :

    static ClrMetaUnresolved* create(ClrCodeHeap* h, uint32 token, const char* an, const char* nsp, const char* cn, const char* mn);
    static inline ClrMetaUnresolved* getFirst(BlockHeap* h)
    {
        return (ClrMetaUnresolved*)h->getFirst(ClrMetaDataDef::MetaUnresolved);
    }
    inline ClrMetaUnresolved* getNext(BlockHeap*  h)
    {
        return (ClrMetaUnresolved*)h->getNext(this, ClrMetaDataDef::MetaUnresolved);
    }
    inline uint32 getToken()
    {
        return m_token;
    }
    inline const char* getAssemblyName()
    {
        return (const char*)(this+1);
    }
    inline const char* getMemberName()
    {
        return (const char*)(this+1) + m_anamelen;
    }
    private:

    inline void* operator new(size_t, ClrMetaUnresolved* o)
    {
        return o;
    }
    void operator delete(void* p);

    uint32  m_token;
    uint16  m_anamelen;
    uint16  m_unused;
    // dynamic part
    //char    m_aname[1];

};

#if 0
class ClrArrayShape
{
    public :
    static ClrArrayShape* create(ClrCodeHeap* pHeap, ClrTypeDef* pTypeInfo, ClrField* pField, unsigned lengthTab[], unsigned nDim);
    static ClrArrayShape* find(ClrCodeHeap* pWorkSpace, ClrField* pField);
    static void assignClone(ClrCodeHeap* pWorkSpace, ClrField* pField, BlockHeap* pMetaClone, ClrTypeDef* pTypeInfoClone, ClrField* pFieldClone);
    static void skip(ClrCodeHeap* pWorkSpace, ClrTypeDef* pTypeInfo);
    static inline ClrArrayShape* getFirst(BlockHeap* h)
    {
        return (ClrArrayShape*)h->getFirst(ClrMetaDataDef::MetaArrayShape);
    }
    inline ClrArrayShape* getNext(BlockHeap*  h)
    {
        return (ClrArrayShape*)h->getNext(this, ClrMetaDataDef::MetaArrayShape);
    }
    inline uint32* getLengthTab()
    {
        return (uint32*)(this+1);
    }
    inline uint32* getLowerBoundsTab()
    {
        return getLengthTab()+m_ndim;
    }
    inline unsigned getDim()
    {
        return m_ndim;
    }
    inline uint32* getShapeTab()
    {
        return getLengthTab();
    }
    inline unsigned getShapeSize()
    {
        return m_ndim*2*sizeof(uint32);
    }
    private:
    typedef MetaAbsoluteRef32<ClrField>       FieldRef;
    typedef MetaAbsoluteRef32<ClrTypeDef>     TypeRef;
    
    bool setFieldInfo(BlockHeap* metaHeap, ClrField* lnk);
    ClrField* getFieldInfo(BlockHeap* metaHeap);
    bool setTypeInfo(BlockHeap* metaHeap, ClrTypeDef* lnk);
    ClrTypeDef* getTypeInfo(BlockHeap* metaHeap);
    
    inline void* operator new(size_t, ClrArrayShape* o) { return o; }
    void operator delete(void* p);
    
    TypeRef     m_typeref;
    FieldRef    m_fieldref;
    uint32      m_size;
    uint16      m_ndim;
    uint16      m_flags;
    // dynamic part    
    
};
#endif


class ClrApplicationFile
{
    public :
    enum ClrApplicationFileSpec
    {
        Null    = 0x0000
    };

    ClrApplicationFile()
    {
        m_flags = 0;
        m_start = 0;             
        m_size  = 0;             
    }

    static ClrApplicationFile* create(BlockHeap* h, const char* pName, uint32 size);
    static inline ClrApplicationFile* getFirst(BlockHeap* h)
    {
        return (ClrApplicationFile*)h->getFirst(ClrMetaDataDef::MetaApplicationFile);
    }
    inline ClrApplicationFile* getNext(BlockHeap* h)
    {
        return (ClrApplicationFile*)h->getNext(this, ClrMetaDataDef::MetaApplicationFile);
    }

    inline const char* getName()
    {
        return (const char*)(this+1);
    }
    inline uint32 getSize()
    {
        return m_size;
    }
    inline void setStart(unsigned start)
    {
        m_start = (uint16)(start/sizeof(uint16));
    }

    inline unsigned getStart()
    {
        return m_start*sizeof(uint16);
    }

    inline void* begin()
    {
        return (char*)this + getStart();
    }

    static uint32 calculate_size(const char* pName, uint32 size);
    void swap();

    private:

    ClrApplicationFile(const char* pName, uint32 size);


    inline void* operator new(size_t, ClrApplicationFile* o)
    {
        return o;
    }
    void operator delete(void* p);

    uint16  m_flags;
    uint16  m_start;             
    uint32  m_size;             
    // 4 byte align

};

#endif //#ifndef __METADATA_H__
