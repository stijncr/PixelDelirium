#include "BitConverter.h"
byte* PDNet::CreateHeader(int gameId, short size, int sequenceId, int ackId){
	byte* buffer = Combine(ToByte(gameId), 4, ToByte(size), 2);
	buffer = Combine(buffer, 6, ToByte(sequenceId), 4);
	buffer = Combine(buffer, 10, ToByte(ackId), 4);
	return buffer;
}
byte* PDNet::CreatePosition(int x, int y){
	byte command = (byte)COMMAND_POSITION;
	byte* buffer = Combine(&command, 1, ToByte(x), 4);
	buffer = Combine(buffer, 5, ToByte(y), 4);
	return buffer;
}

byte* PDNet::ToByte(int value)
{
		byte* arrayOfbyte = new byte[4];
		for (int i = 0; i < 4; i++)
			arrayOfbyte[3 - i] = (value >> (i * 8));
		return arrayOfbyte;
}
byte* PDNet::ToByte(short value){
		byte* arrayOfbyte = new byte[2];
		for (int i = 0; i < 2; i++)
			arrayOfbyte[1 - i] = (value >> (i * 8));
		return arrayOfbyte;
}
byte* PDNet::ToByte(bool* param, int size, int* bufferLength){
	int BufferLength = (int)(size/8) + ((size%8)>0?1:0);
	if(bufferLength != 0) *bufferLength = BufferLength;
	byte* Buffer = new byte[BufferLength];
	ZeroMemory(Buffer, BufferLength);
	for(int i = 0; i < size; i++){
		Buffer[(int)(i/8)] |= param[i] << (i%8);
	}
	return Buffer;
}
byte* PDNet::ToByte(char* buffer, int size){
	byte* buf = new byte[size];
	for(int i = 0; i < size; i++){
		buf[i] = buffer[i];
	}
	return buf;
}
byte* PDNet::ToByte(std::string str){
	byte* buffer = new byte[str.length()+1];
	std::cout << str.length() << std::endl;
	for(unsigned int i = 0; i < str.length(); i++){
		buffer[i] = (byte)str[i];
	}
	buffer[str.length()] = 0;
	return buffer;
}

int PDNet::ToInt32(byte* buffer, int start){
	return ((buffer[start]<<24)|(buffer[start+1]<<16)|(buffer[start+2]<<8)|(buffer[start+3]));
}
short PDNet::ToInt16(byte* buffer, int start){
	return ((buffer[start]<<8)|(buffer[start+1]));
}
	
std::string PDNet::ToString(byte* buffer, int size, int start){
	int len = 0;
	if(size == 0) len = strlen(reinterpret_cast<const char*>(buffer));
	else len = size;
	std::string str2(buffer + start, buffer + start + len);
	return str2;
}
std::string PDNet::ToBinaryString(byte* buffer, int size, int start){
	byte* bits = new byte[size * 8];
	for(int j = 0; j < size; j++){
		byte Byte = buffer[start + j];
		for (int i = 0; i < 8; i++) {
			bits[(j * 8) + i] = (Byte >> i) & 1;
		}
	}
	std::stringstream ss;
	for(int i = 0; i < size *8; i++)
		ss << (int)bits[i];
	return ss.str();
}
	
char* PDNet::ToChar(byte* buffer, int size, int start){
	char* buf = new char[size];
	for(int i = 0; i < size; i++){
		buf[i] = buffer[start + i];
	}
	return buf;
}
std::string PDNet::reinterpret_byte(byte* buffer, int size, int start){
	return std::string(ToChar(buffer, size, start), size);
}
	
bool* PDNet::ToBoolean(byte* buffer, int size, int start){
	bool* bools = new bool[size*8];
	for (int i = 0; i < size*8; i++) {
		bools[i] = (buffer[start + (int)(i/8)] >> (i%8)) & 1;
	}
	return bools;
}	
byte* PDNet::Combine(byte* buffer1, int buffersize1, byte* buffer2, int buffersize2, int bufferstart1, int bufferstart2){
		byte* buffer = new byte[buffersize1 + buffersize2 + 1];
		for(int i = 0; i < buffersize1; i++){
			buffer[i] = buffer1[bufferstart1 + i];
		}
		for(int i = 0; i < buffersize2; i++){
			buffer[buffersize1 + i] = buffer2[bufferstart2 + i];
		}
		buffer[buffersize1 + buffersize2] = 0;
		return buffer;
	}