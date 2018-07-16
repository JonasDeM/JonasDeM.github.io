#pragma once
#include "windows.h"

void Directory::FillDirectories()
{
	WIN32_FIND_DATAA findFileData;
	std::string adjustedStr = GetPhysicalFilePath() + "\\*";
	auto hFind = FindFirstFileA(adjustedStr.c_str(), &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Unable to open directory " << m_Name << "(" << GetLastError() << ")" << std::endl;
		return;
	}

	do
	{
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strcmp(findFileData.cFileName, ".") == 0
				||
				strcmp(findFileData.cFileName, "..") == 0)
			{
				continue;
			}
			auto name = std::string(findFileData.cFileName);
			auto dirPtr = std::make_shared<Directory>(name, shared_from_this());
			dirPtr->FillDirectories();

			m_Items.push_back(dirPtr);
		}
		else
		{
			auto name = std::string(findFileData.cFileName);
			size_t indexPoint = name.rfind('.');
			if (	indexPoint != std::string::npos
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
	} while (FindNextFileA(hFind, &findFileData));


	FindClose(hFind);
}




