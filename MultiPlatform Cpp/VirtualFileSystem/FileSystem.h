#pragma once
#include <vector>
#include <memory>
#include "Directory.h"

using namespace std;

class FileSystem
{
public:
	void MountDirectory(const string & directory);
	void MountZipArchive(const shared_ptr<File> file_archive) const;
	void SetSearchDirectory(std::shared_ptr<Directory> dir);//default is the mounted
	void SetSearchDirectoryToMounted();

	//files and directory
	string GetPhysicalFilePath(const string & filename) const;
	void GetFilesInDirectory(vector<shared_ptr<File>>& file_table, const string & directory) const;
	void GetFilesWithExtension(vector<shared_ptr<File>>& file_table, const string & extension) const;
	std::shared_ptr<File> GetFile(const std::string & filename) const;
	std::shared_ptr<Directory> GetDirectory(const std::string &  directory) const;
	std::shared_ptr<File> GetFileByFullPath(const std::string & fullPath) const;
	std::shared_ptr<Directory> GetDirectoryByFullPath(const std::string & fullPath) const;

private:
	std::shared_ptr<Directory> m_SearchDirectory;
	std::shared_ptr<Directory> m_BaseDirectory;
};

