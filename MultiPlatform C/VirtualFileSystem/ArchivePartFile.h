#pragma once
#include "File.h"
class ArchivePartFile :	public File
{
public:
	ArchivePartFile(const std::string & name, std::shared_ptr<FileSystemItem> ptr, size_t size, size_t startIndex);

	bool Open(FILE_ACCESS_MODE accessMode, FILE_ACCESS_FLAGS accessFlags) override;
	std::vector<uint8_t> Read(const size_t amountBytes) const override;
	std::string ReadText(const size_t amountBytes) const override;
	bool Write(const uint8_t * buffer, size_t size) const override;
	void Close() const override;
	std::string GetPartFileName() const;

protected:
	std::size_t m_Size;
	std::size_t m_StartIndex;

};

