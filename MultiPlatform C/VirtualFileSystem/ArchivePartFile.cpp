#include "ArchivePartFile.h"
#include <iostream>
#include "ArchiveFile.h"


ArchivePartFile::ArchivePartFile(const std::string & name, std::shared_ptr<FileSystemItem> ptr, size_t size, size_t startIndex) : File(name,ptr)
, m_Size(size)
, m_StartIndex(startIndex)
{
}

bool ArchivePartFile::Open(FILE_ACCESS_MODE accessMode, FILE_ACCESS_FLAGS accessFlags)
{
	if (accessMode != FILE_ACCESS_MODE::Read)
	{
		std::cout << "Can only read from file in archive, change the accesmode to READ";
		return false;
	}
	else if (accessFlags != FILE_ACCESS_FLAGS::OpenExisting)
	{
		std::cout << "Can only read from file in archive, change the accesflag to OPENEXISTING";
		return false;
	}
	if (std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->Open(accessMode, accessFlags))
	{
		std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->SetByteIndex(m_StartIndex);
		return true;
	}
	
	return false;
}

std::vector<uint8_t> ArchivePartFile::Read(const size_t amountBytes) const
{
	if (amountBytes + std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->GetByteIndex() < m_StartIndex + m_Size)
	{
		return std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->Read(amountBytes);
	}
	else
	{
		return std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->Read(m_Size - std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->GetByteIndex() - 1);
	}
}

std::string ArchivePartFile::ReadText(const size_t amountBytes) const
{
	if (amountBytes + std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->GetByteIndex() < m_StartIndex + m_Size)
	{
		return std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->ReadText(amountBytes);
	}
	else
	{
		return std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->ReadText(m_Size -(m_StartIndex - std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->GetByteIndex()) - 1);
	}
}

bool ArchivePartFile::Write(const uint8_t* buffer, size_t size) const
{
	std::cout << "Cannot write to a file in an archive" << '\n';
	return false;
}

void ArchivePartFile::Close() const
{
	std::static_pointer_cast<ArchiveFile>(m_ParentPtr)->Close();
}

std::string ArchivePartFile::GetPartFileName() const
{
	if (m_Name.rfind('\\') != std::string::npos)
	{
		return m_Name.substr(m_Name.rfind('\\') +1);
	}
	else
	{
		return m_Name.substr(m_Name.rfind('/') + 1);
	}
	
}
