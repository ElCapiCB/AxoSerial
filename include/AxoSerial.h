#pragma once





#include <Windows.h>
#include <vector>
#include <string>






namespace axo{

	LPCWSTR CharToLPCWSTR(const char* str);

	class Serial {

	private:


		std::string m_COMM;
		DCB m_PARAMS;
		HANDLE m_Handle;
		std::vector<std::string> m_COMMS;

	public:


		Serial(DWORD BaudRate, BYTE ByteSize, BYTE StopBits, BYTE Parity);
		~Serial();


		int init(LPCWSTR Comm);
		int write(unsigned char _data);
		int write(unsigned char* _data, unsigned int _amount);
		int get(unsigned char* _data, unsigned int _nBytes);

		std::vector<std::string> getComms();
	};


}
