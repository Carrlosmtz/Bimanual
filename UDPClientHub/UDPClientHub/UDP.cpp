///////////////////////////////////////////////////////////////////////////////
//	
// Methods for UDP connection
//
///////////////////////////////////////////////////////////////////////////////

#include "UDP.h"
#include <iostream>


#define MSG_WAITALL 0x8
#define NAGLE_OFF

  void DisplayError(const std::string&);

  void DisplayError(const std::string& errString)
  {
	  std::cout << "UDP Communication Error : " << errString << std::endl;
  }



  //////////////////////////////////////////////////////////////////////////////
  //
  // Method declarations for UDP class
  //
  //////////////////////////////////////////////////////////////////////////////

  //constructor, assign port number and destination IP address in the string format 
  UDP::UDP(unsigned short _portNo,  const std::string _destIP, const std::string _localIP) 
  {
    portNo = _portNo;
	destIP = _destIP;
	destAddr = inet_addr(_destIP.c_str());
	localIP = _localIP;
	localAddr = inet_addr(_localIP.c_str());
  }

  //close socket, returns 1 if succeeded, -1 if failed
  int UDP::Disconnect()
  {
    int retVal = 1;
    retVal = closesocket(Socket);
    if(retVal == -1)
      {
        //DisplayError("UDP::Disconnect, error during close\n");
        return 0;
      }
    return retVal;
  }
  
  //destructor, closes socket and cleanup WSA
  UDP::~UDP()
  {
    Disconnect();
    #ifdef WIN32
    if(WSACleanup() == SOCKET_ERROR){
     DisplayError("UDP::~UDP, WSACleanup failed\n");
    }
    #endif
  }
  
  //initializes WSADATA, UDP socket
  int UDP::Initialize()
  {

    WSADATA   Data;
    int status = WSAStartup(MAKEWORD(2,2), &Data);
    if(status != 0)
	{
       DisplayError("WSAStartup unsuccessful\n");
       return -1;
	}
	//setup destination address structure
	memcpy(&destSockAddr.sin_addr, &destAddr, sizeof(destAddr));
	destSockAddr.sin_family = AF_INET;
    destSockAddr.sin_port = htons(portNo);
	//get address size
	slen = sizeof(destSockAddr);

	//create socket
	Socket = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (Socket == INVALID_SOCKET)
	{
		int WSErrorCode = WSAGetLastError();
		std::cout << "WSErrorCode = " << WSErrorCode << std::endl;
		DisplayError("UDP::Initialize failed on socket creation \n");
		return -1;
	}

	memcpy(&localSockAddr.sin_addr, &localAddr, sizeof(localAddr));
	localSockAddr.sin_family = AF_INET;
	localSockAddr.sin_port = htons(portNo);
	slen = sizeof(destSockAddr);

	  
	//bind
	if (bind(Socket, (struct sockaddr *) &localSockAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		DisplayError("UDPServer: Initialize failed on binding\n");
		closesocket(Socket);
		return -1;
	}
    return 1;
  }
  
  void UDP::InitializeAsyncEventAndSelect()
  {
      hWSAEvent = WSACreateEvent();
      if (NULL == hWSAEvent)
      {
            printf("UDP AsyncEvent: Failed to create Event. Err = %u\r\n", WSAGetLastError());
      }
      else
      {
            if (SOCKET_ERROR == WSAEventSelect(Socket, hWSAEvent, FD_WRITE|FD_READ|FD_CLOSE)) //FD_READ: recv, FD_WRITE: send
            {
                  printf("AyncSocketComms: Failed to select. Err = %u\r\n", WSAGetLastError());
                  WSACloseEvent(hWSAEvent);
                  hWSAEvent = NULL;
            }
            else
            {
                  printf("UDP: Async socket ops ready.\r\n");
            }
      }
  }

  int UDP::Send(char* data, unsigned int length)
  {
      int numsnt = sendto(Socket, data, length, 0, (struct sockaddr *)&destSockAddr, slen);
	  if ( numsnt == SOCKET_ERROR)
	  {
		  int WSErrorCode = WSAGetLastError();
		  std::cout << "WSErrorCode = " << WSErrorCode<< std::endl;
		  DisplayError("UDP:: sendto error");
		  return -1;
	  }
	  return numsnt;
  }

  int UDP::Recv(char* data, unsigned int length)
  {
	  int numrec = recvfrom(Socket, data, length, 0, (struct sockaddr*)&destSockAddr, &slen);
	  if (numrec == SOCKET_ERROR)
	  {
		  int WSErrorCode = WSAGetLastError();
		  std::cout << "WSErrorCode = " << WSErrorCode<< std::endl;
		  DisplayError("UDP:: recvfrom error");
		  return -1;
	  }
	  return numrec;
  }

  int UDP::RecvNonblocking(char* data, unsigned int length, unsigned int timeout)
  {
	  int numrec= 0;
	  int flag = 0;
	  DWORD waitRes = 0;
	  DWORD lastErr = 0;

      if (NULL != hWSAEvent)
      {
          waitRes = WaitForSingleObject(hWSAEvent, timeout);
          if (WAIT_TIMEOUT == waitRes)
          {
                printf("UDP RecvNonblocking: Wait for socket read timed out.\r\n");
                numrec = SOCKET_ERROR;
				return numrec;
           }
		  
		  else if (WAIT_FAILED == waitRes)
          {
                printf("UDP RecvNonblocking: Wait for socket read event failed. Err = %d\r\n", GetLastError());
                numrec = SOCKET_ERROR;
				return numrec;
          }
          else
          {
                // __fallthrough to recv below.
          }
      }
           
      numrec = recvfrom(Socket, data, length, 0, (struct sockaddr*)&destSockAddr, &slen);
	  lastErr = WSAGetLastError();	  
      if ((numrec == SOCKET_ERROR) && (WSAEWOULDBLOCK == lastErr)) 
	  {
          //printf("recv returned would block. skipping\r\n");
		  //printf("numerc = %d \n", numrec);
		  return 0;  //SOCKET_ERROR =-1, if returned would stop receiving thread. so return something else
      }
 
      if (numrec == 0) 
	  {
		  printf("UDP RecvNonblocking: Connection closed by peer.\n");
          return -1;
      }
 
      if (numrec == SOCKET_ERROR) 
	  {
      printf("UDP RecvNonblocking: socket error %d.\n", lastErr);
      return -1;
	  }

      return numrec;
  }


	  
















	