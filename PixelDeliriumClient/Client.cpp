#include "UdpSocket.h"
#include <iostream>
UdpSocket sConnect;
Address addr;
WinSockDll winSockDll;
int Thread(){
	char* buffer = new char[512];
	while(true){
		ZeroMemory(buffer, 512);
		int res = sConnect.Receive(buffer, 512, &addr);
		if(res != WSAEINVAL)
			std::cout <<res << buffer << std::endl;
	}
	return 0;
}
int main(int argc, char* args[]){
	if(!winSockDll.Init()){
		std::cout << "Couldn't init winsock" << std::endl;
		system("pause");
		exit(1);
	}
	addr = Address("127.0.0.1", 2222);
	sConnect = UdpSocket();
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Thread, NULL, NULL, NULL);
	char* buffer = new char[512];
	while(true){
		ZeroMemory(buffer, 512);
		std::cin.getline(buffer, 512);
		sConnect.Send(buffer, addr);
	}
	system("pause");
}