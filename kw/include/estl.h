#ifndef __ESTL_H__
#define __ESTL_H__

//---------------------------------------------------------
//    Segment
//---------------------------------------------------------
class eSegment 
{
    public:
    void  *buf;
    int    len;

    inline eSegment()
    {
        clear();
    }

    inline eSegment(void *p, int n)
    {
        buf = p;
        len = n;
    }

    inline void clear()
    {
        buf = NULL;
        len = 0;
    }
    inline void inc(int i)
    {
        buf  = (char*) buf + i;
        len -= i;
    }


};

// template vector
template <class T, int m>
class eVector  {

      public:
      typedef T* iterator;
      typedef T& reference;
  
      inline iterator start() const { return (T*)(&___vector[0]); }
      inline iterator limit() const { return (T*)(&___vector[sizeof(T)*(m)]); }


        eVector() {
            clear();
            }
        eVector(eVector& v) {
            copy(v);
            }
        eVector& operator=(eVector& v) {
            copy(v);
            return *this;
            }

      inline iterator begin() const { return start(); }
      inline iterator end() const   { return finish; }
      inline int index(iterator x) const { return x - begin(); }

      inline void clear() { finish = begin(); }
      inline void blow() { finish = begin() + m; }

      inline void push_back(const T& x) {
            if (finish < limit()) {
                  *finish++ = x;
                  }
            }
      inline bool empty() const {
            return begin() == end();
            }
      inline bool full() const {
            return end() >= limit();
            }
      inline unsigned size() const {
            return (unsigned)(end() - begin());
            }
      inline unsigned capacity() const {
            return (unsigned)m;
            }
      inline unsigned free() const {
            return capacity() - size();
            }
      //it's better to return a refence type iterator operator[](size_type n) { return begin() + n; }
      inline reference operator[](unsigned n) { return *(begin() + n); }

      protected:
      iterator finish;

      inline void copy(eVector& v) {
            clear();
            iterator i = v.begin();
            for (int n = v.size(); n > 0; n--) {
                  *finish++ = *i++;
                  }
            }

      private:
      char ___vector[sizeof(T)*m];

      };

template <class T, int m>
class eStack : public eVector<T, m> 
{
    public:
    typedef eVector<T, m> baseclass;
    typedef typename baseclass::reference reference;

    eStack() : eVector<T, m>() {}

    inline void push(const T& t) 
    {
        push_back(t);
    }
    inline const T& pop() 
    {
        return *(--baseclass::finish);
    }
    inline const T& pop(int n) 
    {
        if (n <= (int)baseclass::size()) 
        {
            baseclass::finish -= n;
        }
        return *baseclass::finish;
    }
    inline int used() const 
    {
        return baseclass::size();
    }
    
    inline reference operator[](unsigned n) 
    { 
        return *(baseclass::end() - (n + 1)); 
    }
    
};

// template queue
template <class T, int m>
class eQueue  {

      public:
      typedef T* iterator;
      typedef T& reference;
      typedef size_t size_type;

      inline eQueue() { clear(); }
      inline eQueue(eQueue& x) { clear(); copy(x); }
      inline eQueue& operator=(eQueue& x) { copy(x); return *this; }

      inline void clear() { it_finish = it_start = start(); n = 0; }

      inline iterator begin() const { return it_start; }
      inline iterator end() const   { return it_finish; }
      inline reference front() { return *it_start; }
      inline iterator next(iterator x) const {
            if (++x >= limit()) x = start();
            return x;
            }

      inline iterator start() const { return (T*)(&___vector[0]); }
      inline iterator limit() const { return (T*)(&___vector[sizeof(T)*(m+1)]); }

      inline bool push_back(const T& x) {

            if (n < m) {
                  *it_finish = x;
                  it_finish = next(it_finish);
                  n++;
                  return true;
                  }
            return false;
            }

   
      inline reference pop_front() {
            const iterator x = it_start;
            it_start = next(it_start);
            n--;
            return *x;
            }



      inline bool empty() const {
            return n <= 0;
            }
      inline bool full() const {
            return n >= m;
            }
      inline size_type size() const {
            return size_type(n);
            }
      inline size_type capacity() const {
            return size_type(m);
            }

      inline bool check() {
            if (it_start < start() || it_start >= limit())
                 return false;
            if (it_finish < start() || it_finish >= limit())
                 return false;
            return true;
            }

      protected:
      inline void copy(eQueue& x) {
            for (iterator i = x.begin(); i != x.end(); i = x.next(i)) {
                  push_back(*i);
                  }
            }

      private:

      iterator    it_finish;
      iterator    it_start;
      int         n;
      char ___vector[sizeof(T)*(m+1)];


      };

template <class T, int m>
class ePool {

      public:
      ePool() { init(); }
      ~ePool() {};

      inline void clear() {
            __stack.clear();
            ___vector.clear();
            init();
            }
          
      T* alloc() {
            T* result = NULL;
            if (__stack.empty() == false) {
                  result = __stack.pop();
                  result = new (result) derived_class();
                  }
            //D(DBG_ERR,"Pool: alloc %08X, stack.size=%d\n", result, __stack.used());
            return result;
            }

      void destroy(T* item) {
            derived_class* x = (derived_class*)item;
            if (x->__link == item) {
                  // item's origin is part of the vector
                  delete x;
                  __stack.push(item);
                  //D(DBG_ERR,"Pool: destroy %08X, stack.size=%d\n", item, __stack.used());
                  }
            else {
                  //D(DBG_ERR,"Pool: destroy %08X failed!\n", item);
                  }
            }

      private:

      class derived_class : public T {

            public:
            inline void* operator new(size_t, T* c) {
                  return c;
                  };
            inline void operator delete(void* ) { };
            derived_class() {
                  __link = this;
                  }
            ~derived_class() {
                  __link = NULL;
                  }
            T* __link;

            };

      void init() {
            // blow the vector up to the maximum size
            ___vector.blow();
            typename eVector<derived_class, m>::iterator i = ___vector.begin();
            for (; i < ___vector.end(); i++) {
                  T* item = &(*i);
                  __stack.push(item);
                  }
            }

      eStack<T*, m> __stack;
      eVector<derived_class, m> ___vector;
      
      };

template <int m>
class eString : public eVector<char, m> 
{

    public:
    typedef eVector<char, m> baseclass;
    eString() { *baseclass::finish = 0; }
    eString(const char* s) { append(s); }
    eString(const char* s, int n) { append(s, n); }
    ~eString() {};

    inline eString& operator=(eString& s) { clear(); append(s); return *this; }
    inline eString& operator=(const char* s) { clear(); append(s); return *this; }
    inline operator const char *() const { return baseclass::begin(); }
    inline eString& operator+=( const char* s) { append(s); return *this; }
    inline eString& operator+=( const char c) { append(c); return *this; }
    inline bool operator==(eString& s) const 
    { 
        if (baseclass::size() == s.size())
        {
              return (strncmp(baseclass::begin(), s.begin(), baseclass::size()) == 0);
        }
        return false;
    }

    inline int length() const { return baseclass::size(); }
    inline int find(const char c) { return -1; }


    inline void clear() { baseclass::clear(); *baseclass::finish = 0; }

    void append(const char c);
    void append(const char* s);
    void append(const char* s, int n);

};

template <int m>
void eString<m>::append(const char c) {
      if ((baseclass::capacity() - baseclass::size() - 1) > 0) {
            *baseclass::finish++ = c;
            }
      // terminate
      *baseclass::finish = 0;
      }
template <int m>
void eString<m>::append(const char* s) 
{
    for (int i = baseclass::capacity() - baseclass::size() - 1; (i > 0) && (*s != 0); i--) 
    {
        *baseclass::finish++ = *s++;
    }
    // terminate
    *baseclass::finish = 0;
}

template <int m>
void eString<m>::append(const char* s, int n) 
{
    for (int i = min(n, baseclass::capacity() - baseclass::size() - 1); i > 0; i--) 
    {
        *baseclass::finish++ = *s++;
    }
    // terminate
    *baseclass::finish = 0;
}

// newstyle template list
template<class T>
class eListNode 
{
    public :
    typedef eListNode<T>     node_type;
    
    eListNode() 
    {
        init();
    }
    ~eListNode() 
    {
        unchain();
    }

    inline void init() 
    {
        next = (T*)this;  
        prev = (T*)this;  
    }

    void unchain()
    {
        ((node_type*)prev)->next = next;
        ((node_type*)next)->prev = prev;
        init();
    }
    inline bool isLinked() 
    {
        return prev != this && next != this;
    }

    T*  next;
    T*  prev;

};
#if 0
// Workaround for BCC3.1 (Borlandc 3.1)
// bcc don't support inlining of this function
template<class T>
void eListNode<T>::unlink() 
{
    prev->next = next;
    next->prev = prev;
    init();
}
#endif

template<class T>
class eListIterator 
{
    public:
    typedef eListIterator<T> iterator;
    typedef eListNode<T>     node_type;
    eListIterator(T* t)
    {
        node = t;
    }

    inline T& operator*() 
    { 
        return *node; 
    }

    // prefix form of increment iterator (fetch and increment !!)
    inline iterator operator++() 
    {
        node = ((node_type*)node)->next;
        return *this;
    }
    inline iterator operator--() 
    {
        node = ((node_type*)node)->prev;
        return *this;
    }
    #if 0
    // postfix form of increment iterator (fetch and increment !!)
    // be aware that ... = it++ don't work
    iterator operator++(int) 
    {
        node = ((node_type*)node)->next;
        return *this;
    }
    iterator operator--(int) 
    {
        node = ((node_type*)node)->prev;
        return *this;
    }
    #endif
    inline bool operator!=(const iterator& x) const 
    { 
        return node != x.node; 
    }
    inline bool operator==(const iterator& x) const 
    { 
        return node == x.node; 
    }
    static inline bool equ(const iterator& x1, const iterator& x2)
    {
        return x1.node == x2.node; 
    } 
    inline void unchain() 
    {
        ((node_type*)node)->unchain();
    }
    inline void insert_front(T* pi) 
    {
        ((node_type*)pi)->prev = ((node_type*)node)->prev;
        ((node_type*)pi)->next = node;
        
        node_type* prev = ((node_type*)pi)->prev;
        prev->next = pi;

        ((node_type*)node)->prev = pi;
    }


    T* node;
};

template <class T>
class eList  
{
    public :
    typedef eListIterator<T> iterator;
    typedef eListNode<T>     node_type;
    eList() 
    {
    }

    ~eList() 
    {
        // empty the whole list
        while (empty() == false) { pop_front(); }
    }

    inline void clear() 
    {
        listAnchor.init();
    }

    inline iterator begin() const
    {
        return (iterator)listAnchor.next;
    }
    inline iterator end() const
    {
        return (iterator)(T*)&listAnchor;
    }

    // insert operations
    inline iterator insert_front(iterator pos, T& i) 
    {
        pos.insert_front(&i);
        return (iterator)&i;  
    }

    inline void push_front(T & i) 
    {
        insert_front(begin(),i);
    }

    inline void push_back(T & i) 
    {
        insert_front(end(),i);
    }

    // delete operations
    inline iterator erase(iterator pos) 
    {
        iterator next = pos;
        ++next;
        pos.unchain();
        return next;
    }
    inline void pop_front() 
    {
        erase(begin());
    }
    inline void pop_back() 
    {
        iterator tmp = end();
        --tmp;
        erase(tmp);
    }
    inline bool empty() const
    {
        //return begin() == end();
        return iterator::equ(begin(),end());
    }
    
    unsigned size() 
    {
        unsigned n = 0;
        for (iterator it = begin(); it != end(); ++it) { n++; }
        return n;
    }

    eListNode<T> listAnchor;

};

template <class T>
class eSmartPtr  
{
    public:
    eSmartPtr() 
    {
        m_p = NULL;
    }
    ~eSmartPtr() 
    {
        release();
    }

    inline eSmartPtr& operator=(T* p) 
    {
        release();
        m_p = p;
        return *this;  
    } 
    inline T* operator ->() 
    { 
        return m_p;  
    }
    inline operator T*() 
    {   
        return m_p;  
    }

    private:
    inline void release()
    {
        if (m_p != NULL)
        {
            delete m_p;
        }
    }

    T* m_p;
};

#endif      // __ESTL_H__

