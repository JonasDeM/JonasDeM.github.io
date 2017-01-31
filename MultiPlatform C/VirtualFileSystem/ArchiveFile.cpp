#include "ArchiveFile.h"

ArchiveFile::ArchiveFile(const std::string& name, std::shared_ptr<FileSystemItem> ptr) : File(name,ptr)
{
	m_Flags = FileSystemItemFlags::IsArchive;
}

#if defined(PLATFORM_Linux)
#include "ArchiveFile_PlatformSpecific_Linux.hpp"
#elif defined(PLATFORM_Windows)
#include "ArchiveFile_PlatformSpecific_Windows.hpp"
#endif