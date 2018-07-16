#pragma once
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <string>
#include "Directory.h"
#include "ArchiveFile.h"

void Directory::FillDirectories()
{
	DIR *dir;
	struct dirent *entry;
	auto path = GetPhysicalFilePath();

	if (m_ParentPtr == nullptr)
	{
		const char * root = &DIR_SEPERATOR;
		chdir(root);//working directory to root, root on linux is /
	}
	
	if (!(dir = opendir(path.c_str())))
	{
		if (ENOENT == errno)
		{
			std::cout << "Directory doesn't exist";
			return;
		}
	}
	if (!(entry = readdir(dir)))
		return;

	do {
		if (entry->d_type == DT_DIR) 
		{
			if (strcmp(entry->d_name, ".") == 0
				||
				strcmp(entry->d_name, "..") == 0)
			{
				continue;
			}
			auto dirPtr = std::make_shared<Directory>(std::string(entry->d_name), shared_from_this());
			dirPtr->FillDirectories();
			m_Items.push_back(dirPtr);
		}
		else
		{
			auto name = std::string(entry->d_name);
			size_t indexPoint = name.rfind('.');
			if (indexPoint != std::string::npos
				&&
				name.substr(indexPoint).compare(".zip") == 0)
			{
				m_Items.push_back(std::make_shared<ArchiveFile>(name, shared_from_this()));
			}
			else
			{
				m_Items.push_back(std::make_shared<File>(name, shared_from_this()));
			}
		}
	} while (entry = readdir(dir));
	closedir(dir);

}
