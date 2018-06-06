

#ifdef WIN32
typedef int socklen_t;
#include <Winsock2.h>
#endif

#ifdef WINCE
typedef int socklen_t;
#include <Winsock2.h>
#endif

#include <string>


#define NO_FLAGS_SET 0

 class UDP 
  {
  public:
    UDP(unsigned short _portNo,  const std::string _destIP, const std::string _localIP);
    ~UDP();
    virtual int Initialize();
    int Disconnect();
	void InitializeAsyncEventAndSelect();
	int Send (char* data, unsigned int length);
	int Recv (char* data, unsigned int length);
	int RecvNonblocking(char* data, unsigned int length, unsigned int timeout);
    unsigned int GetPortNumber() const
    {
      return portNo;
    }
    SOCKET  GetSocket() const 
    { 
      return Socket;
    }
	std::string  GetIP() const 
    { 
      return destIP;
    }
  protected:
    unsigned short     portNo;
    SOCKET             Socket;
	HANDLE			   hWSAEvent;
	std::string        localIP, destIP;
	unsigned long      destAddr, localAddr;
	SOCKADDR_IN        destSockAddr, localSockAddr;
	int                slen;  //size of destSockAddr
  };

