#pragma once
#include "FileSystem.h"
#include <iostream>

#include "ArchiveFile.h"
#include "ArchivePartFile.h"
#include "ZipFile.h"

void FileSystem::MountDirectory(const string & directory)
{
	auto dir = directory;
	if (directory[0] == FileSystemItem::DIR_SEPERATOR)
	{
		if (directory.length() > 1)
		{
			dir = dir.substr(1);
		}
		else { dir = "."; }
	}

	m_BaseDirectory = std::make_shared<Directory>(string(dir), nullptr); //the pointer to its parent is nullptr, because this is the root as far as this program knows
	m_BaseDirectory->FillDirectories(); //didn't put this as private and in constructor from directory BECAUSE, i call shared_from_this() inside FillDirectories() and that requires that there already exists a shared ptr to the object.
	m_SearchDirectory = m_BaseDirectory;
}

void FileSystem::MountZipArchive(const shared_ptr<File> file_archive) const
{
	if (!(file_archive->GetFlags() & FileSystemItemFlags::IsArchive))
	{
		std::cout << "This file is not an archive" << std::endl;
		return;
	}

	file_archive->Open(FILE_ACCESS_MODE::Read, FILE_ACCESS_FLAGS::OpenExisting);
	do
	{
		ZipFileHeader * file_header;
		std::vector<uint8_t> currentData;

		currentData = file_archive->Read(sizeof(ZipFileHeader));
		if (currentData.size() == 0)
			break;
		
		file_header = reinterpret_cast<ZipFileHeader*>(&currentData[0]);

		if (file_header->signature != 0x4034b50)
			break;

		std::string filename = file_archive->ReadText(file_header->filenamelength);
		file_archive->Read(file_header->extrafieldlength);

		auto part_file = std::make_shared<ArchivePartFile>(
			filename
			, file_archive->shared_from_this()
			, file_header->compressedsize
			, std::static_pointer_cast<ArchiveFile>(file_archive)->GetByteIndex());
		file_archive->Read(file_header->compressedsize);

		std::static_pointer_cast<ArchiveFile>(file_archive)->m_Items.push_back(part_file);

	} while (true);

	file_archive->Close();
}


void FileSystem::SetSearchDirectory(std::shared_ptr<Directory> dir)
{
	m_SearchDirectory = dir;
}

void FileSystem::SetSearchDirectoryToMounted()
{
	m_SearchDirectory = m_BaseDirectory;
}

string FileSystem::GetPhysicalFilePath(const string & filename) const 
{
	auto filePtr = m_SearchDirectory->RecursiveFileSearch(filename);
	if (filePtr != nullptr)
	{
		return filePtr->GetPhysicalFilePath();
	}
	return "";
}

void FileSystem::GetFilesInDirectory(vector<shared_ptr<File>>& file_table, const string& directory) const
{
	shared_ptr<Directory> dirPtr;
	if (directory.compare(m_SearchDirectory->GetName()) == 0)
	{
		dirPtr = m_SearchDirectory;
	}
	else
	{
		dirPtr = m_SearchDirectory->RecursiveDirectorySearch(directory);
	}

	for (size_t i = 0; i < dirPtr->GetItems().size(); i++)
	{
		if (!(dirPtr->GetItems()[i]->GetFlags() & FileSystemItemFlags::IsDirectory))
		{
			file_table.push_back(std::static_pointer_cast<File>(dirPtr->GetItems()[i]));
		}
	}
}

void FileSystem::GetFilesWithExtension(vector<shared_ptr<File>>& file_table, const string& extension) const
{
	m_SearchDirectory->RecursiveExtensionSearch(file_table, extension);
}

std::shared_ptr<File> FileSystem::GetFile(const std::string& filename) const
{
	return m_SearchDirectory->RecursiveFileSearch(filename);
}

std::shared_ptr<Directory> FileSystem::GetDirectory(const std::string & directory) const
{
	return m_SearchDirectory->RecursiveDirectorySearch(directory);
}

//I did this one without recursion - as exercise
std::shared_ptr<File> FileSystem::GetFileByFullPath(const std::string& fullPath) const
{
	auto dirPtr = m_SearchDirectory;
	string path = fullPath;

	std::string nextItemName = path.substr(0, m_SearchDirectory->GetName().size());

	if (nextItemName.compare(dirPtr->GetName()) != 0)
	{
		return nullptr;
	}
	path = path.substr(nextItemName.size()+1); //+1 for seperator
	

	while (true)
	{
		if (path.find(FileSystemItem::DIR_SEPERATOR) != std::string::npos)
		{
			nextItemName = path.substr(0, path.find(FileSystemItem::DIR_SEPERATOR));
			bool found = false;
			for (size_t i = 0; i < dirPtr->GetItems().size(); i++)
			{
				if (dirPtr->GetItems()[i]->GetName().compare(nextItemName) == 0
					&& 
					(dirPtr->GetItems()[i]->GetFlags() & FileSystemItemFlags::IsDirectory ))
				{
					dirPtr = static_pointer_cast<Directory>(dirPtr->GetItems()[i]);
					path = path.substr(nextItemName.size()+1);//+1 for seperator
					found = true;
					break;
				}
			}
			if (!found)
			{
				return nullptr;
			}
		}
		else
		{
			for (size_t i = 0; i < dirPtr->GetItems().size(); i++)
			{
				if (dirPtr->GetItems()[i]->GetName().compare(path) == 0
					&&
					!(dirPtr->GetItems()[i]->GetFlags() & FileSystemItemFlags::IsDirectory))
				{
					return static_pointer_cast<File>(dirPtr->GetItems()[i]);
				}
			}
			return nullptr;
		}
	}
}

//I did this one without recursion - as exercise
std::shared_ptr<Directory> FileSystem::GetDirectoryByFullPath(const std::string& fullPath) const
{
	auto dirPtr = m_SearchDirectory;
	string path = fullPath;

	std::string nextItemName = path.substr(0, m_SearchDirectory->GetName().size());

	if (nextItemName.compare(dirPtr->GetName()) != 0)
	{
		return nullptr;
	}
	path = path.substr(nextItemName.size() + 1); //+1 for seperator


	while (true)
	{
		if (path.find(FileSystemItem::DIR_SEPERATOR) != std::string::npos)
		{
			nextItemName = path.substr(0, path.find(FileSystemItem::DIR_SEPERATOR));
			bool found = false;
			for (size_t i = 0; i < dirPtr->GetItems().size(); i++)
			{
				if (dirPtr->GetItems()[i]->GetName().compare(nextItemName) == 0
					&&
					(dirPtr->GetItems()[i]->GetFlags() & FileSystemItemFlags::IsDirectory))
				{
					dirPtr = static_pointer_cast<Directory>(dirPtr->GetItems()[i]);
					path = path.substr(nextItemName.size() + 1);//+1 for seperator
					found = true;
					break;
				}
			}
			if (!found)
			{
				return nullptr;
			}
		}
		else
		{
			for (size_t i = 0; i < dirPtr->GetItems().size(); i++)
			{
				if (dirPtr->GetItems()[i]->GetName().compare(path) == 0
					&&
					(dirPtr->GetItems()[i]->GetFlags() & FileSystemItemFlags::IsDirectory))
				{
					return static_pointer_cast<Directory>(dirPtr->GetItems()[i]);
				}
			}
			return nullptr;
		}
	}
}


