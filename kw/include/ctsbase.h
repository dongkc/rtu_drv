#ifndef __CTSBASE_H__
#define __CTSBASE_H__


// mapping of 'native' clr-target types
#if PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP32
// 32 bit data model integer, long, pointer = 32 bit
typedef sint32  TargetNativeInt;
typedef uint32  TargetPtrType;

#elif PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP32_64
// 32 bit data model on a 64 bit platform, alignment 64 bit !!
typedef sint32  TargetNativeInt;
typedef uint32  TargetPtrType;

#endif

typedef TargetPtrType    IntPtr;
typedef TargetPtrType    UIntPtr;
typedef TargetPtrType    PtrType;
typedef TargetNativeInt  NativeIntType;

class ClrInt64
{
    public:
#if (PLATFORM_TARGET_ENDIANESS == TARGET_BIG_ENDIAN)
    uint32  high;
    uint32  low;
#else
    uint32  low;
    uint32  high;
#endif
    inline ClrInt64() 
    { 
        low     = 0;
        high    = 0;
    }
    inline ClrInt64(const ClrInt64& v) 
    { 
        low     = v.low;
        high    = v.high;
    }
    inline ClrInt64(uint32 val32) 
    { 
        low     = val32;
        high    = 0;
    }
    inline ClrInt64(sint32 val32) 
    { 
        low     = val32;
        high    = 0;
        if (val32 < 0)
        {
            high = (uint32)-1;
        }
    }
    inline ClrInt64(void* val) 
    { 
        low  = (uint32)val;
#if PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP64
        high = (uint32)((PtrType)val>>32);
#else
        high = 0;
#endif
    }
    inline ClrInt64& operator=(uint32 val32) 
    { 
        low     = val32;
        high    = 0;
        return *this; 
    }
    inline ClrInt64& operator=(sint32 val32) 
    { 
        low     = val32;
        high    = 0;
        if (val32 < 0)
        {
            high = (uint32)-1;
        }
        return *this; 
    }
    inline ClrInt64& operator=(void* val) 
    { 
        low  = (uint32)val;
#if PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP64
        high = (uint32)((PtrType)val>>32);
#else
        high = 0;
#endif
        return *this; 
    }
    inline bool operator==(const ClrInt64& v) const
    {
        return (v.low == low && v.high == high) ? true : false;
    }
    inline bool operator!=(const ClrInt64& v) const
    {
        return (v.low != low || v.high != high) ? true : false;
    }
};
// CTS base type implementation     //          values for base types
typedef void    Void;               // TypeVoid     1
typedef uint8   Boolean;            // TypeBool     2
typedef uint16  Char;               // TypeChar     3
typedef sint8   SByte;              // TypeInt8     4
typedef uint8   Byte;               // TypeUint8    5
typedef sint16  Int16;              // TypeInt16    6
typedef uint16  UInt16;             // TypeUint16   7
typedef sint32  Int32;              // TypeInt32    8
typedef uint32  UInt32;             // TypeUint32   9
#ifdef PLATFORM_LONGLONG_SUPPORT
typedef sint64  Int64;
typedef uint64  UInt64;
#else
typedef ClrInt64  Int64;
typedef ClrInt64  UInt64;
#endif

typedef float   Single;
typedef double  Double;

//union EclrSingleArg
//{
//    UInt32 u;
//    Single s;
//};
class EclrSingleArg
{
public:
    inline EclrSingleArg() 
    {
        v.u = 0;
    }
    inline EclrSingleArg(UInt32 u) 
    {
        v.u = u;
    }
    inline EclrSingleArg(Single s) 
    {
        v.s = s;
    }
    union Value
    {
        UInt32 u;
        Single s;
    };
    Value v;
};

//#ifdef PLATFORM_LONGLONG_SUPPORT
//union EclrDoubleArg
//{
//    UInt64 u;
//    Double d;
//};
//#else
//union EclrDoubleArg
//{
//    UInt32 u;
//    Double d;
//};
//#endif
class EclrDoubleArg
{
public:
    inline EclrDoubleArg() 
    {
        v.u = 0;
    }
#ifdef PLATFORM_LONGLONG_SUPPORT
    inline EclrDoubleArg(UInt64 u) 
    {
        v.u = u;
    }
#else
    inline EclrDoubleArg(UInt64) 
    {
    // __TODO__ double arg support for systems without 64 bit support
    }
#endif
    inline EclrDoubleArg(Double d) 
    {
        v.d = d;
    }
    union Value
    {
#ifdef PLATFORM_LONGLONG_SUPPORT
        UInt64 u;
#else
        // __TODO__ double arg support for systems without 64 bit support
        UInt32 u;
#endif
        Double d;
    };
    Value v;
};

class ClrTypeDef;
class ClrType;
class String;

class Object
{
    public:

    inline Object() 
    {
        // the default contructor must be empty !!!
    }
    inline Object(ClrType* type) 
    { 
        *((ClrType**)(&m_type)) = type; 
    }
    void ctor();

    // virtual base functions
    enum VirtualSlots
    {
        ToStringSlot    = 0,
        EqualsSlot      = 1,
        GetHashCodeSlot = 2,
        FinalizeSlot    = 3
    };
    String* ToString();
    Boolean Equals(Object*);
    Int32 GetHashCode();
    void Finalize();


    inline ClrType* getType() const
    {
        return *((ClrType**)(&m_type)); 
    }
    inline void setType(ClrType* type)
    {
        *((ClrType**)(&m_type)) = type; 
    }
    inline void setType(PtrType type)
    {
        m_type = type; 
    }
    inline void* operator new(size_t, Object* o)
    {
        return o;
    }
    void swap();

    private:
    PtrType m_type;  // ClrType* type      
};

class ValueType
{
    public:
    inline ValueType() 
    {
        // the default contructor must be empty !!!
    }
    void ctor();
    
    private:

    inline void* operator new(size_t, ValueType* o)
    {
        return o;
    }

};

// class System.String
class String : public Object
{
    public :
    typedef const Char*   const_iterator; 
    typedef Char*         iterator; 

    String() : Object(), m_max(0), m_len(0) { }

    // auxiliary contructor must be inline !!
    String(unsigned max, unsigned len = 0) : Object()
    {
        m_max = (uint16)max;
        m_len = (uint16)len;
    }
    
    void ctor();

    static String* Concat(String* s1, String* s2);
    Char get_Chars(Int32 charPos);
    Int32 get_Length();
    
    inline unsigned length() 
    { 
        return m_len; 
    }
    inline unsigned limit() 
    {   
        return m_max; 
    }
    inline const_iterator begin() 
    { 
        return (const_iterator)(this+1); 
    }
    inline const_iterator end() 
    { 
        return begin() + m_len; 
    }
    inline char ascii(const_iterator it) 
    { 
        return (char)*it; 
    }

    static unsigned calculate_size(unsigned len)
    {
        unsigned result = sizeof(String) + (len)*sizeof(uint16);
        result = align_up(result, sizeof(Object));
        return result;
    }
    void ascii(char* buffer, int max_elem);

    unsigned size();

    void append(const char* s);

    void assign(String& s);

    void swap(bool hasNaturalEndianess);
    private:
    
    void append(String& s);
    
    // private data
    uint16 m_max;
    uint16 m_len;
};

class FixedUtf8String
{
public:
    static void Copy(FixedUtf8String* pSrc, FixedUtf8String* pDst);
    static bool IsString(ClrTypeDef* pTypeInfo);
    void Assign(FixedUtf8String* pSrc);
    void Assign(const char* pCString);
    
    inline unsigned GetLength()
    {
        return m_len;
    }
    inline void SetLength(unsigned len)
    {
        m_len = (uint16)len;
    }
    inline void SetCapacity(unsigned capacity)
    {
        m_max = (uint16)capacity;
    }
    inline unsigned Capacity()
    {
        return m_max;
    }
    inline uint8* Begin()
    {
        return (uint8*)(this+1);
    }
private:
    // private data
    uint16 m_max;
    uint16 m_len;
};

// class System.Array
class Array : public Object
{
    public :
    Array() { }
    Array(ClrType* t) : Object(t) { }
    void ctor();

    static void Copy(Array* srcArr, Int32 srcIdx, Array* dstArr, Int32 dstIdx, Int32 length);
    ClrTypeDef* getElemTypeInfo();
    
    private :

};

class SZArray : public Array
{
    public :
    inline SZArray() {}
    SZArray(ClrType* t, int num);
    void ctor();

    inline unsigned getLength()
    {
        return (unsigned)m_numElem;
    }
    static inline int getSizeof()
    {
        return sizeof(Object)+sizeof(TargetNativeInt);
    }
    static inline int getLengthOffset()
    {
        return sizeof(Object);
    }
    static inline int getLengthSize()
    {
        return sizeof(PtrType);
    }
    inline void* getData()
    {
        return this+1;
    }
    
    private :

    TargetNativeInt m_numElem; 


};

#endif //#define __CTSBASE_H__
