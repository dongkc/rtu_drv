#ifndef __ESOCKET_H__
#define __ESOCKET_H__

class ESocket : public ClrAllocator
{
    public :
    ESocket();

    virtual bool Close() = 0;
    virtual int Send(const void* buf, int len) = 0;
    virtual int Recv(void* buf, int len) = 0;
    virtual ESocket* Accept() = 0;
    virtual void setNonBlocking() = 0;


    inline void addRef()
    {
        refCnt++;
    }
    inline unsigned getRefCnt()
    {
        return refCnt;
    }
    void release();

    inline bool hasAcceptSemantic()
    {
        return m_hasAcceptSemantic;
    }
    inline bool isNonBlocking()
    {
        return m_isNonBlocking;
    }


    protected:


    virtual ~ESocket();

    bool m_isNonBlocking;
    bool m_hasAcceptSemantic;

    inline void setAcceptSemantic()
    {
        m_hasAcceptSemantic = true;
    }
    inline void setNonBlockingSemantic()
    {
        m_isNonBlocking = true;
    }

    private :

    unsigned    refCnt;
    

};


#endif //#define __ESOCKET_H__

