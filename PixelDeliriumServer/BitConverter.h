#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#define GAME_ID 178956970
#define COMMAND_POSITION 1
#define COMMAND_INPUT 2
#define COMMAND_NORMALMSG 3
#define COMMAND_PRIVATEMSG 4
#define COMMAND_AUTOMSG 5
#define COMMAND_ERRORMSG 6
#define COMMAND_LOGIN 7
#define COMMAND_DISCONNECT 8
#define COMMAND_CLIENTINFO 9

namespace PDNet{
	byte* CreateHeader(int gameId, short size, int sequenceId, int ackId);
	byte* CreatePosition(int x, int y);


	byte* ToByte(int value);
	byte* ToByte(short value);
	byte* ToByte(bool* param, int size, int* bufferLength = 0);
	byte* ToByte(char* buffer, int size);
	byte* ToByte(std::string str);
	
	int ToInt32(byte* buffer, int start = 0);
	short ToInt16(byte* buffer, int start = 0);
	
	bool* ToBoolean(byte* buffer, int size, int start = 0);

	std::string ToString(byte* buffer, int size = 0, int start = 0);
	std::string ToBinaryString(byte* buffer, int size, int start = 0);
	
	std::string reinterpret_byte(byte* buffer, int size, int start = 0);
	char* ToChar(byte* buffer, int size, int start = 0);
	
	byte* Combine(byte* buffer1, int buffersize1, byte* buffer2, int buffersize2, int bufferstart1 = 0, int bufferstart2 = 0);
};