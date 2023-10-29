// AxoSerial.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include "AxoSerial.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>



LPCWSTR axo::CharToLPCWSTR(const char* str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    if (size == 0) {
        // Error de conversión
        return NULL;
    }

    wchar_t* wstr = new wchar_t[size];
    if (MultiByteToWideChar(CP_UTF8, 0, str, -1, wstr, size) == 0) {
        // Error de conversión
        delete[] wstr;
        return NULL;
    }

    return wstr;
}

axo::Serial::Serial(DWORD BaudRate, BYTE ByteSize, BYTE StopBits, BYTE Parity): m_COMM("") {

    

    m_PARAMS.BaudRate = BaudRate;
    m_PARAMS.ByteSize = ByteSize;
    m_PARAMS.StopBits = StopBits;
    m_PARAMS.Parity = Parity;

}

axo::Serial::~Serial(){
    CloseHandle(m_Handle);
}

int axo::Serial::init(LPCWSTR Comm){

    std::wstring path = L"\\\\.\\";

    path.append(Comm);

    m_Handle = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (m_Handle == INVALID_HANDLE_VALUE) {
        std::cerr << "Error al abrir el puerto serie." << std::endl;
        return 0;
    }



    m_PARAMS.DCBlength = sizeof(m_PARAMS);
    if (!GetCommState(m_Handle, &m_PARAMS)) {
        std::cerr << "Error al obtener la configuración del puerto serie." << std::endl;
        CloseHandle(m_Handle);
        return 0;
    }



    if (!SetCommState(m_Handle, &m_PARAMS)) {
        std::cerr << "Error al configurar la comunicación del puerto serie." << std::endl;
        CloseHandle(m_Handle);
        return 0;
    }

    return 1;
}

int axo::Serial::write(unsigned char _data){

    unsigned char *temp = &_data;

    if (WriteFile(m_Handle, temp, 1, NULL, NULL) == FALSE) {
        std::cerr << "No fue posible escribir al puerto." << std::endl;
        return 0;
    }

    return 1;


}

int axo::Serial::get(unsigned char* _data, unsigned int _nBytes){

    DWORD availableBytes = 0;

   // if (PeekNamedPipe(m_Handle, NULL, 0, NULL, &availableBytes, NULL) && availableBytes > 0) {
    int res = ReadFile(m_Handle, _data, _nBytes, NULL, NULL);
        if (!res) {
            std::cerr << "No fue posible leer al puerto." << std::endl;
            return 0;
        }
    //}
    return 1;


}

int axo::Serial::available()
{

    COMSTAT status;
    int stat = ClearCommError(m_Handle, NULL, &status);
    if (stat) {
        return status.cbInQue > 0;
    }

    return false;

}

std::vector<std::string> axo::Serial::getComms(){
    HKEY hKey;
    std::vector<std::string> ports;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DEVICEMAP\\SERIALCOMM", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD maxValueNameLength;
        DWORD maxValueDataLength;

        if (RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &maxValueNameLength, &maxValueDataLength, NULL, NULL) == ERROR_SUCCESS) {
            DWORD valueIndex = 0;
            DWORD valueNameLength = maxValueNameLength + 1;
            DWORD valueDataLength = maxValueDataLength + 1;

            wchar_t* valueName = new wchar_t[valueNameLength];
            BYTE* valueData = new BYTE[valueDataLength];

            while (RegEnumValue(hKey, valueIndex, valueName, &valueNameLength, NULL, NULL, valueData, &valueDataLength) == ERROR_SUCCESS) {
                // Convierte el valor de datos a un string
                std::wstring wValue(reinterpret_cast<wchar_t*>(valueData));
                std::string portName(wValue.begin(), wValue.end());

                ports.push_back(portName);

                valueIndex++;
                valueNameLength = maxValueNameLength + 1;
                valueDataLength = maxValueDataLength + 1;
            }

            delete[] valueName;
            delete[] valueData;

            // Cierra la clave de registro
            RegCloseKey(hKey);
        }
        else {
            std::cerr << "No se pudo obtener información de la clave de registro." << std::endl;
        }
    }
    else {
        std::cerr << "No se pudo abrir la clave de registro." << std::endl;
    }

    return ports;
}


int axo::Serial::write(unsigned char* _data, unsigned int _amount)
{

   
    if (WriteFile(m_Handle, _data, _amount, NULL, NULL) == FALSE) {
        std::cerr << "No fue posible escribir al puerto." << std::endl;
        return 0;
    }

    return 1;

}