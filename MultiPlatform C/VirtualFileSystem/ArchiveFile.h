#pragma once
#include "File.h"

class ArchiveFile : public File
{
public:
	ArchiveFile(const std::string & name, std::shared_ptr<FileSystemItem> ptr);

	size_t GetByteIndex();
	void SetByteIndex(size_t byteIndex);

	friend class FileSystem;
	friend class Directory;
protected:
	
	std::vector<std::shared_ptr<FileSystemItem>> m_Items;
};
