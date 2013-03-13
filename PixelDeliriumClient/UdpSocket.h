#pragma once

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif
#if PLATFORM == PLATFORM_WINDOWS
#include <WinSock.h>
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <fcntl.h>

#endif
#include <string>
#include "Framework.h"
#pragma comment (lib, "ws2_32.lib")


class WinSockDll {
private:
	WSADATA _wsaData;
public:
	bool Init();
	bool Cleanup();
};
class Address : public SOCKADDR_IN {
private:
	u_short _port; 
	std::string _addr;
public:
	Address();
	Address(std::string address, u_short port);
	u_short GetPort();
	std::string GetAddress();
	bool Equals(Address address);
};
class UdpSocket
{
	SOCKET _socket;
	Address _addr;
public:
	UdpSocket();
	int Bind(Address address);
	int Send(std::string msg, Address address, int flags=0);
	int Receive(char* buffer, int size, Address* address, int flags=0);
	int CloseSocket();
	bool SetNonBlocking();
	SOCKET GetSocket();
};