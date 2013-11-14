#ifndef REMOTING_H
#define REMOTING_H

#ifdef REMOTING_SOCKET_NEWSTYLE
class EclrRemotingSocket;
#endif
class ESocket;
class CRemotingDeamon
{
    public:
    enum RemotingPorts
    {
        // for unassigned port numbers see : http://www.iana.org/assignments/port-numbers
        ClrServerPort           = 0xA08C,           //  41100,
        ClrServerPort0          = ClrServerPort+0,
        ClrServerPort1          = ClrServerPort+1,
        ClrServerPort2          = ClrServerPort+2,
        ClrServerPort3          = ClrServerPort+3,

        MsDebugProxyPort        = ClrServerPort+10, // 41110
        DownloadTestPort        = ClrServerPort+11, // 41111

        PerformanceMonitorPort  = ClrServerPort+20, // 41120,
        ClrLogServerPort        = ClrServerPort+21, // 41121,
        ClrLogClientPort        = ClrServerPort+22, // 41122,
        Null
    };

#ifdef REMOTING_SOCKET_NEWSTYLE
    static bool addSocket(EclrRemotingSocket* pSocket);
#else
    static bool addSocket(ESocket* pSocket);
#endif
    static bool start();
	static void Process();
    static void Ctors();
    private:
    static void threadProc();
#ifdef REMOTING_SOCKET_NEWSTYLE
#else
	static void internProcess();
    static eVector<ESocket*, 16>    s_socketVec;
#endif
};

#endif //#ifndef REMOTING_H
