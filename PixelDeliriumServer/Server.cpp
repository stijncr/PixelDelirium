#include "UdpSocket.h"
#include <iostream>

UdpSocket sConnect;
Address addr, storeAddr;
Address* connections;
int Counter = 0;
int addrlen = sizeof(Address);
WinSockDll winSockDll;
int main(){
	if(!winSockDll.Init()){
		std::cout<<"Couldn't load winsock"<<std::endl;
		system("pause");
		return 1;
	}
	connections = (Address*)calloc(64, sizeof(Address));

	addr = Address("127.0.0.1", 2222);
	sConnect = UdpSocket();
	sConnect.Bind(addr);
	std::cout << "Server started..." << std::endl;

	char* buffer = new char[512];
	bool clientexist = false;
	while(true){
		clientexist = false;
		ZeroMemory(buffer, 512);
		sConnect.Receive(buffer, 512, &storeAddr);
		for(int i = 0; i < Counter; i++){
			if(storeAddr.Equals(connections[i])){
				std::cout << "Client["<<i<<"]: "<<buffer<<std::endl;
				clientexist = true;
			}
		}
		if(!clientexist){
			connections[Counter] = storeAddr;
			std::cout<<"New Client: "<<Counter<<std::endl;
			std::cout << "Client["<<Counter<<"]: "<< buffer << std::endl;
			Counter++;
		}
	}

	winSockDll.Cleanup();
	return 0;
}