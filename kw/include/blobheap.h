#ifndef __HEAP_H__
#define __HEAP_H__

class CHeapStatistic
{
    public:
    void clear()
    {
        memset(this, 0, sizeof(*this));
    }
    uint32  m_usedBlocks;
    uint32  m_unusedBlocks;
    uint32  m_usedSize;
    uint32  m_unusedSize;
    uint32  m_usedMaxBlock;
    uint32  m_unusedMaxSize;
    uint32  m_heapSize;
};


class BlobHeapNode
{
    public :
    enum BlobHeapNodeSpec
    {
        TypeMask    = 0x7F,
        AuxFlag     = 0x80,
        Any

    };

#if PLATFORM_DATAMODEL == PLATFORM_DATAMODEL_ILP32_64
    // support 64 bit alignment
    typedef uint64  node_type;
#else
    typedef uint32  node_type;
#endif


    BlobHeapNode()
    {
        m_node = 0;   
    }

    inline void set(int type, unsigned length = 0)
    {
        m_node = ((node_type)length << 8) | (type & TypeMask);
    }
    inline unsigned length() const
    {
        return (unsigned)(m_node >> 8);
    }
    inline unsigned size() const
    {
        return length()*sizeof(m_node);
    }  

    inline int type() const
    {
        return (int)m_node & TypeMask;
    }  

    inline void* data() const
    {
        return (void*)(this + 1);
    }
    inline void setType(int type)
    {
        set(type, length());
    }
    inline void setAuxFlag()
    {
        m_node |= AuxFlag;
    }
    inline bool isAuxFlag()
    {
        return (m_node & AuxFlag) ? true : false;
    }
    inline void resetAuxFlag()
    {
        node_type aux = AuxFlag;
        m_node &= ~aux;
    }
    void swap();
    private :
    node_type m_node;
};

class BlobHeapIterator
{
    public :

    typedef BlobHeapIterator    iterator;
    typedef BlobHeapNode        iterator_node;

    // constructors
    inline BlobHeapIterator()
    {
        p = NULL;
    }
    inline BlobHeapIterator(const iterator_node* pinode)
    {
        p = (iterator_node*)pinode;
    }
    inline BlobHeapIterator(const void* pdata)
    {
        p = (iterator_node*)pdata - 1;
    }


    // copy constructor
    inline BlobHeapIterator(const BlobHeapIterator& it)
    {
        p = it.p;
    }
    // assignment constructor
    inline BlobHeapIterator& operator=(const BlobHeapIterator& it)
    {
        p = it.p;
        return *this;
    }
    
    // get iterator node
    inline iterator_node& operator*() const
    { 
        return *p; 
    }
    // get iterator node reference
    inline operator iterator_node*() const 
    {   
        return p;
    }

    // prefix form of increment iterator (fetch and increment !!)
    inline const iterator operator++() 
    {
        p += p->length() + 1;
        return *this;
    }
    // post increment iterator (not fetch and increment !!)
    // be aware that ... = it++ don't work
    inline const iterator& operator++(int) 
    {
        p += p->length() + 1;
        return *this;
    }
    // arithmetik operators
    inline iterator& operator-=(uint32 dec)
    {
        p -= dec;
        return *this;
    }
    inline iterator& operator+=(uint32 inc)
    {
        p += inc;
        return *this;
    }
    inline int operator-(const iterator& rit) const
    {
        return (p - rit.p);
    }
    // comparison operators
    inline bool operator<(const iterator& it) const
    {
        return p < it.p;
    }
    inline bool operator<=(const iterator& it) const 
    {
        return p <= it.p;
    }

    inline bool operator>(const iterator& it) const 
    {
        return p > it.p;
    }
    inline bool operator>=(const iterator& it) const 
    {
        return p >= it.p;
    }
    inline bool operator!=(const iterator& it) const
    {
        return p != it.p;
    }

 
    // count the amount of iterator nodes
    inline static sint32 length(const iterator& it1, const iterator& it2)
    {
        // skip the iterator size
        sint32 result = (it2.p - it1.p) - 1;
        return result;
    }
    // count the amount of bytes
    inline static sint32 length_bytes(const iterator& it1, const iterator& it2)
    {
        sint32 result = length(it1, it2)*sizeof(iterator_node);
        return result;
    }
    static inline sint32 distance(const iterator& it1, const iterator& it2)
    {
        sint32 result = (it2.p - it1.p)*sizeof(iterator_node);
        return result;
    }
    static inline uint32 size(const iterator& it1, const iterator& it2)
    {
        uint32 result = (it2.p - it1.p)*sizeof(iterator_node);
        result += sizeof(iterator_node);
        return result;
    }


    protected :
    iterator_node* p;
};

class BlockHeap
{
    public :
    typedef BlobHeapIterator    iterator;
    typedef BlobHeapNode        iterator_node;

    enum GrowingPolice
    {
        Down,
        Up
    };
    enum BlockType
    {
        Empty       = 0x00,
        End         = 0x01,
        Proxy       = 0x02
    };
    enum AllocPolice
    {
        FitExact,
        BiggestFree,
        FirstHit
    };
    enum BlockHeapSpec
    {
        granularity = sizeof(iterator_node)
    };

    BlockHeap();
    BlockHeap(GrowingPolice grow);
    bool assign(void* p, uint32 size)
    {
        iterator start((iterator_node*)p);
        return assign(start, size);
    }
    bool assign(iterator start, uint32 size);
    bool assign(const iterator& itstart, const iterator& itend);

    inline iterator_node* base()
    {
        return begin(); 
    }

    inline const iterator& begin()
    {
        return m_first;
    }
    inline const iterator& end()
    {
        return m_last;
    }
    
    inline const iterator begin(int type)
    {
        return next(begin(), type);
    }
    const iterator next(const iterator& prev, int type);

    bool validate(const iterator& it);
    bool validate(const iterator& it, int type);
    bool validate();
    
    bool validate(void* p);
    iterator get_iterator(void* p, unsigned& offset);
    iterator get_iterator(void* p);

    void* getFirst();
    void* getFirst(int type);
    void* getNext(void* p);
    void* getNext(void* p, int type);

    void* alloc(int nodeType, uint32 size, GrowingPolice gp);
    inline void* alloc(int nodeType, uint32 size)
    {
        return alloc(nodeType, size, m_gp);
    }
    bool free(void* p);
    uint32 size();

    void clear();

    bool isAssigned();
    void init();
    
    void statistic(CHeapStatistic& stat);
    void dump();

    protected:
    
    iterator collectBubble(iterator it);
    inline GrowingPolice getGrowingPolice()
    {
        return m_gp;
    }
    inline void setGrowingPolice(GrowingPolice g)
    {
        m_gp = g;
    }

    inline static uint32 calculate_blockamount(uint32 size)
    {
        // align granularity
        size = align_up(size, (unsigned)granularity);
        // calculate granularity blocks plus one for the block descriptor 
        return size/granularity + 1; 
    }

    iterator        m_first;
    iterator        m_last;
    GrowingPolice   m_gp;

    static iterator_node  s_dummy;

};

#endif //#define __HEAP_H__

