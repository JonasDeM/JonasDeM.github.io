#pragma once

#include "FileSystemItem.h"

#if defined(PLATFORM_Linux)
const char FileSystemItem::DIR_SEPERATOR = '/';
#elif defined(PLATFORM_Windows)
const char FileSystemItem::DIR_SEPERATOR = '\\';
#endif

FileSystemItem::FileSystemItem(const std::string & name, std::shared_ptr<FileSystemItem> ptr) : m_ParentPtr(ptr), m_Name(name)
{}

std::string FileSystemItem::GetPhysicalFilePath() const
{
	std::string path = m_Name;
	auto ptr = m_ParentPtr;
	while (ptr != nullptr)
	{

		path.insert(0, ptr->m_Name + DIR_SEPERATOR);
		
		ptr = ptr->m_ParentPtr;
	}
	return path;
};