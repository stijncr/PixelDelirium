#include "UdpSocket.h"



bool WinSockDll::Init(){
	#if PLATFORM == PLATFORM_WINDOWS
		int res = WSAStartup(MAKEWORD(2, 1), &_wsaData);
		if(res==0)return true;
		Error error;
		switch(res){
		case WSASYSNOTREADY:
			error.HandleError(Log, "The underlying network subsystem is not ready for network communication.");
			break;
		case WSAVERNOTSUPPORTED:
			error.HandleError(Log, "The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.");
			break;
		case WSAEINPROGRESS:
			error.HandleError(Log, "A blocking Windows Sockets 1.1 operation is in progress");
			break;
		case WSAEPROCLIM:
			error.HandleError(Log, "A limit on the number of tasks supported by the Windows Sockets implementation has been reached.");
			break;
		case WSAEFAULT:
			error.HandleError(Log, "The lpWSAData parameter is not a valid pointer.");
			break;
		}
		return false;
    #else
    return true;
    #endif
};
bool WinSockDll::Cleanup(){
	#if PLATFORM == PLATFORM_WINDOWS
		int res = WSACleanup();
		if(res==0)return true;
		Error error;
		switch(res){
		case WSANOTINITIALISED:
			error.HandleError(Log, "A successful WSAStartup call must occur before using this function.");
			break;
		case WSAENETDOWN:
			error.HandleError(Log, "The network subsystem has failed.");
			break;
		case WSAEINPROGRESS:
			error.HandleError(Log, "A blocking Winsock call is in progress, or the service provider is still processing a callback function.");
			break;
		}
		return false;
	#endif
};

Address::Address(){}
Address::Address(std::string address, u_short port){
	this->sin_family = AF_INET;
	this->sin_addr.s_addr = inet_addr(address.c_str());
	this->sin_port = htons(port);
	this->_addr = address;
	this->_port = port;
}
u_short Address::GetPort(){ 
	return _port; 
}
std::string Address::GetAddress(){ 
	return _addr; 
}
bool Address::Equals(Address address){
	if (address.sin_addr.s_addr == this->sin_addr.s_addr &&
		address.sin_port == this->sin_port) return true;
	return false;
}

UdpSocket::UdpSocket(){
	_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}
int UdpSocket::Bind(Address address){
	_addr = address;
	if(bind(_socket, (SOCKADDR*)&_addr, sizeof(_addr)) == SOCKET_ERROR)
		return WSAGetLastError();
	return 0;
}
int UdpSocket::Send(std::string msg, Address address, int flags){
	int error = sendto(_socket, msg.c_str(), msg.length(), flags, (SOCKADDR*)&address, sizeof(address));
	if(error==SOCKET_ERROR){
		error=WSAGetLastError();
		if(error==WSAECONNRESET)
			return -1;
	}
	return error;
}
int UdpSocket::Receive(char* buffer, int size, Address* address, int flags){
		int addrlen = sizeof(Address);
		int error = recvfrom(_socket, buffer, size, flags, (SOCKADDR*)address, &addrlen);
		if(error == SOCKET_ERROR){
			error = WSAGetLastError();
			if(error==WSAECONNRESET)
				return -1;
		}
		return error;
	}
int UdpSocket::CloseSocket(){
	int error = 0;
	if(_socket!=INVALID_SOCKET&&_socket!=SOCKET_ERROR){
#if PLATFORM == PLATFORM_WINDOWS
		error = closesocket(_socket); 
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		error  = close(_socket);
#endif
		_socket = INVALID_SOCKET;
	}
	return error;
}
bool UdpSocket::SetNonBlocking(){
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		int nonBlocking = 1;
		if ( fcntl( _socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
		{
		printf( "failed to set non-blocking socket\n" );
		return false;
		}
		return true;
	#elif PLATFORM == PLATFORM_WINDOWS
		DWORD nonBlocking = 1;
		if ( ioctlsocket( _socket, FIONBIO, &nonBlocking ) != 0 )
		{
		printf( "failed to set non-blocking socket\n" );
		return false;
		}
		return true;

	#endif
}
SOCKET UdpSocket::GetSocket(){ 
	return _socket; 
}