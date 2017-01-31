#pragma once
#include "File.h"
#include <vector>
#include <iostream>

//#include <sys/stat.h>
#include <unistd.h>

bool File::Open(FILE_ACCESS_MODE accessMode, FILE_ACCESS_FLAGS accessFlags)
{
	auto path = GetPhysicalFilePath();
	m_FileHandle = open(path.c_str(), accessFlags|accessMode);
	return m_FileHandle != FILE_HANDLE_INVALID;
}

std::vector<uint8_t> File::Read(const size_t amountBytes) const // can max read 2^32 per read
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


	if (!read(m_FileHandle, &data[0], amountBytes) < 0)
	{
		std::cout << "File::Read() >> Reading the file failed" << std::endl;
	}

	return data;
}

std::string File::ReadText(const size_t amountBytes) const // can max read 2^32 per read
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


	if (!read(m_FileHandle, &data[0], amountBytes) < 0)
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

	int result;
	result = write(m_FileHandle, buffer, size);
	return result != -1;
}

void File::Close() const
{
	if (close(m_FileHandle) != 0)
	{
		std::cout << "Closing the file went wrong" << std::endl;
	}
}


