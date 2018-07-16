#pragma once
#include <vector>
#include <memory>
#include "Directory.h"
#include "File.h"
#include "ArchivePartFile.h"
#include "ArchiveFile.h"

#if defined(PLATFORM_Linux)
#include "Directory_PlatformSpecific_Linux.hpp"
#elif defined(PLATFORM_Windows)
#include "Directory_PlatformSpecific_Windows.hpp"
#endif

Directory::Directory(const std::string & name, std::shared_ptr<FileSystemItem> ptr) : FileSystemItem(name,ptr)
{
	m_Flags = FileSystemItemFlags::IsDirectory;
}

std::shared_ptr<File> Directory::RecursiveFileSearch(const std::string & fileToSearch) const
{
	for (size_t i = 0; i < m_Items.size(); i++)
	{
		if (m_Items[i]->GetName().compare(fileToSearch) == 0)
		{
			return std::static_pointer_cast<File>(m_Items[i]);
		}
		else if (m_Items[i]->GetFlags() & FileSystemItemFlags::IsDirectory)
		{
			auto filePtr = std::static_pointer_cast<Directory>(m_Items[i])->RecursiveFileSearch(fileToSearch);
			if (filePtr != nullptr)
			{
				return filePtr;
			}
			else
			{
				continue;
			}
		}
		else if (m_Items[i]->GetFlags() & FileSystemItemFlags::IsArchive)
		{
			auto filePtr = std::static_pointer_cast<ArchiveFile>(m_Items[i]);
			for (size_t j = 0; j < filePtr->m_Items.size(); ++j)
			{
				if (std::static_pointer_cast<ArchivePartFile>(filePtr->m_Items[j])->GetPartFileName().compare(fileToSearch) == 0)
				{
					return std::static_pointer_cast<File>(filePtr->m_Items[j]);
				}
			}
		}
	}
	return nullptr;
}



std::shared_ptr<Directory> Directory::RecursiveDirectorySearch(const std::string& dirToSearch) const
{
	for (size_t i = 0; i < m_Items.size(); i++)
	{
		if (m_Items[i]->GetFlags() & FileSystemItemFlags::IsDirectory)
		{		
			if (m_Items[i]->GetName().compare(dirToSearch) == 0)
			{
				return std::static_pointer_cast<Directory>(m_Items[i]);
			}
			else
			{
				auto dirPtr = std::static_pointer_cast<Directory>(m_Items[i])->RecursiveDirectorySearch(dirToSearch);
				if (dirPtr != nullptr)
				{
					return dirPtr;
				}
				else
				{
					continue;
				}
			}
		}
	}
	return nullptr;
}


void Directory::RecursiveExtensionSearch(std::vector<std::shared_ptr<File>>& file_table, const std::string & extToSearch) const
{
	for (size_t i = 0; i < m_Items.size(); i++)
	{
		if (!(m_Items[i]->GetFlags() & FileSystemItemFlags::IsDirectory))
		{
			if (std::static_pointer_cast<File>(m_Items[i])->GetExtension().compare(extToSearch) == 0)
			{
				file_table.push_back(std::static_pointer_cast<File>(m_Items[i]));
			}
		}
		//TODO for archives
		else
		{
			std::static_pointer_cast<Directory>(m_Items[i])->RecursiveExtensionSearch(file_table, extToSearch);
		}
	}
}

