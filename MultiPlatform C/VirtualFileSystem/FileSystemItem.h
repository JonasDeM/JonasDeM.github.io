#pragma once

#include <string>
#include <memory>
enum FileSystemItemFlags : char
{
	IsDirectory = 1 << 0,
	IsArchive = 1 << 1
};

class FileSystemItem : public std::enable_shared_from_this<FileSystemItem>
{
public:
	FileSystemItem(const std::string & name, std::shared_ptr<FileSystemItem> ptr);

	std::string GetPhysicalFilePath() const;
	FileSystemItemFlags GetFlags() const { return m_Flags; };
	const std::string & GetName() const { return m_Name; };

	static const char DIR_SEPERATOR;

protected:
	std::shared_ptr<FileSystemItem> m_ParentPtr;
	std::string m_Name;
	FileSystemItemFlags m_Flags = static_cast<FileSystemItemFlags>(0);
};
