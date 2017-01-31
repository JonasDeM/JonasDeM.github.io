#pragma once
#include "FileSystemItem.h"
#include <memory>
#include <vector>
#include "FileAccesMode.h"
#include "FileHandle.h"
#include "FileAccessFlags.h"

class File : public FileSystemItem
{
public:
	File(const std::string & name, std::shared_ptr<FileSystemItem> ptr);


	virtual bool Open( FILE_ACCESS_MODE accessMode, FILE_ACCESS_FLAGS accessFlags);
	virtual std::vector<uint8_t> Read(const size_t amountBytes) const;
	virtual std::string ReadText(const size_t amountBytes) const;
	virtual bool Write(const uint8_t * buffer, size_t size) const; 
	virtual void Close() const;

	std::string GetExtension() const;
protected:

	FILE_HANDLE m_FileHandle = FILE_HANDLE_INVALID;

};

