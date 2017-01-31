#pragma once
#include "windows.h"
#include "File.h"
#include <vector>
#include <iostream>
#include <sstream>

bool File::Open(FILE_ACCESS_MODE accessMode, FILE_ACCESS_FLAGS accessFlags) 
{
	auto path = GetPhysicalFilePath();
	const char* cstr = path.c_str();  
	m_FileHandle = CreateFileA(GetPhysicalFilePath().c_str(),              
		accessMode,          // open for writing/reading
		FILE_SHARE_READ,                      
		nullptr,                   // default security
		accessFlags,             
		FILE_ATTRIBUTE_NORMAL,  // normal file
		nullptr);                  // no attr. template


	if (m_FileHandle != FILE_HANDLE_INVALID)
	{
		return true;
	}
	return false;
}

std::vector<uint8_t> File::Read(const size_t amountBytes) const 
{
	std::vector<uint8_t> data;
	if (amountBytes <= 0)
		return data;

	if (m_FileHandle == FILE_HANDLE_INVALID)
	{
		std::cout << "File::Read() >> You first have to succesfully call File::Open()" << std::endl;
		return data;
	}

	data.resize(amountBytes);
	LPDWORD nrOfBytesReadPtr = nullptr;

	if (!ReadFile(m_FileHandle, &data[0], amountBytes, nrOfBytesReadPtr, nullptr))
	{
		std::cout << "File::Read() >> Reading the file failed" << std::endl;
	}

	return data;
}

std::string File::ReadText(const size_t amountBytes) const 
{
	std::string data;
	if (amountBytes <= 0)
		return data;

	if (m_FileHandle == FILE_HANDLE_INVALID)
	{
		std::cout << "File::Read() >> You first have to succesfully call File::Open()" << std::endl;
		return data;
	}

	data.resize(amountBytes);
	LPDWORD nrOfBytesReadPtr = nullptr;

	if (!ReadFile(m_FileHandle, &data[0], amountBytes, nrOfBytesReadPtr, nullptr))
	{
		std::cout << "File::Read() >> Reading the file failed" << std::endl;
	}

	return data;
}

bool File::Write(const uint8_t* buffer, size_t size) const
{
	if (m_FileHandle == FILE_HANDLE_INVALID)
	{
		std::cout << "File::Write() >> You first have to succesfully call File::Open()" << std::endl;
		return false;
	}

	DWORD amountBytesWritten;
#pragma warning(disable: 4800) // forcing value to bool 'true' or 'false' (performance warning)
	bool writeBool = WriteFile(
		m_FileHandle,           // open file handle
		buffer,      // start of data to write
		size,  // number of bytes to write
		&amountBytesWritten, // number of bytes that were written
		nullptr);            // no overlapped structure
#pragma warning(default: 4800) // forcing value to bool 'true' or 'false' (performance warning)
	return writeBool;
}

void File::Close() const
{
	CloseHandle(m_FileHandle);
}

