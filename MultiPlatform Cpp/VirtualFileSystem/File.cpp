#pragma once
#include "File.h"
#include <string>

#if defined(PLATFORM_Linux)
#include "File_PlatformSpecific_Linux.hpp"
#elif defined(PLATFORM_Windows)
#include "File_PlatformSpecific_Windows.hpp"
#endif


File::File(const std::string & name, std::shared_ptr<FileSystemItem> ptr) : FileSystemItem(name, ptr), m_FileHandle(0)
{
}

std::string File::GetExtension() const
{
	auto index = m_Name.rfind('.');
	if (index != std::string::npos)
	{
		return m_Name.substr(index, m_Name.size() - index);
	}
	return "";
}