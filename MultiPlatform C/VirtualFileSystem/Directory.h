#pragma once

#include "FileSystemItem.h"
#include "File.h"
#include <iostream>
#include <string>

class Directory : public FileSystemItem
{
public:
	Directory(const std::string & name, std::shared_ptr<FileSystemItem> ptr);


	
	friend class FileSystem;
protected:
	std::vector<std::shared_ptr<FileSystemItem>> m_Items;

	std::shared_ptr<File> RecursiveFileSearch(const std::string & fileToSearch) const;
	std::shared_ptr<Directory> RecursiveDirectorySearch(const std::string & dirToSearch) const;
	void RecursiveExtensionSearch(std::vector<std::shared_ptr<File>>& file_table, const std::string & extToSearch) const;
	void FillDirectories();
	const std::vector<std::shared_ptr<FileSystemItem>> & GetItems() const { return m_Items; };
};

