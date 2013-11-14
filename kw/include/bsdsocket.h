#ifndef __BSDSOCKET_H__
#define __BSDSOCKET_H__

#include "esocket.h"

//---------------------------------------------------------
//    IP addresses
//---------------------------------------------------------
class IpAddr 
{
      public:
      union {
            uint8   ip[4];
            uint32  ipl;
            };
            
      unsigned port;

      private:

      public:

      inline IpAddr(const uint32 a) : ipl(a), port(0) {}
      inline IpAddr() : ipl(0), port(0) {}

      inline IpAddr(const uint8 a, const uint8 b, const uint8 c, const uint8 d) {
            ip[0] = a;
            ip[1] = b;
            ip[2] = c;
            ip[3] = d;
            port = 0;
            }
      inline IpAddr(const IpAddr& ip) {
            ipl = ip.ipl;
            port = ip.port;
            }

      inline bool is_set() const {
            return (ipl != 0);
            }

      inline bool is_valid_mask() const {
            return (ip[3] & 1) == 0;
            }

      inline const IpAddr& operator=(const IpAddr& ip) {
            ipl = ip.ipl;
            port = ip.port;
            return *this;
            }

      inline bool operator==(const IpAddr& ip) const {
            return (ipl == ip.ipl);
            }

      inline bool operator!=(const IpAddr& ip) const {
            return (ipl != ip.ipl);
            }
      //
      // Construct the generic net mask to this IP address.
      //
      const IpAddr mask() const {
            if ((ip[0] & 0x80) == 0)
                  return IpAddr(255,0,0,0);     // class A
            if ((ip[0] & 0xc0) == 0x80)
                  return IpAddr(255,255,0,0);   // class B
            if ((ip[0] & 0xe0) == 0xc0)
                  return IpAddr(255,255,255,0); // class C
            return IpAddr();
            }

      //
      // Construct the broadcast address
      // from this IP address and a net mask
      //
      const IpAddr broadcast(const IpAddr& mask) const {
            return IpAddr(mask.ipl & ipl | ~mask.ipl);
            }

      int print(char* const buf) const {
            return sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
            }

      inline bool is_broadcast() const {
            return ipl == (uint32)-1;
            }
      inline bool is_multicast() const {
            return ((ipl & 0xf0000000) == 0xe0000000);
            }

};

typedef long socket_t;
class BsdSocket : public EclrRemotingSocket
{
    public :

    enum SocketType { Packet, Stream, Raw };
    enum SelectMode { Receiving, Sending };
    BsdSocket();
    virtual ~BsdSocket();

    // virtual adapt EclrSocket
    EclrRemotingSocket* Open(BlockingMode mode, bool& wouldblock);
    int Send(const void* buf, int len);
    int Recv(void* buf, int len);
    void Close();
    
    // obsolete 
    void setNonBlocking();
    bool isNonBlocking() { return true; }
    void setAcceptSemantic() {}
    void setNonBlockingSemantic() {}

    bool isOpen();
    bool Open(SocketType mode, int port = 0, int backlog = 1);
    bool Connect(const IpAddr& toAddr);
    int SendTo(const void* buf, int len, const IpAddr& to);
    int RecvFrom(void* buf, int len, IpAddr& from);
    bool Select(SelectMode mode, int timeout_ms);

    IpAddr      remoteIp;

    inline socket_t getSocket()
    {
        return soHndl;
    }
    bool setBroadcast();
    IpAddr getSockName();

    private:

    socket_t    soHndl;
};

#endif //#define __BSDSOCKET_H__

