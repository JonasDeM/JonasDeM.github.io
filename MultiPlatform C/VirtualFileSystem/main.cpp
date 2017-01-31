//*******************************************
//Created By Jonas De Maeseneer
//Copyright: You can use it anywhere you want, I'd like to know if you use it
//But DO NOT claim you wrote it
//*******************************************

#pragma once
#include "FileSystem.h"
#include <iostream>
#include "File.h"
#include "FileAccesMode.h"

int main()
{
	string input = "";
	auto fs = FileSystem();
	// How to get a string/sentence with spaces
	cout << "Please Write the directory to mount" << endl;
	getline(cin, input);
	fs.MountDirectory(input);
	std::shared_ptr<File> filebyname = nullptr;

	//commands
	//********
	const string searchPathCommand =				"searchPath ";
	const string listFilesInDirectoryCommand =		"filesInDir ";
	const string listFilesWithExtensionCommand =	"extInDir ";
	const string mountZipCommand =					"mountZip ";
	const string openCommand =						"open ";
	const string openReadCommand =					"openRead ";
	const string readCommand =						"read ";
	const string writeCommand =						"write ";
	const string closeCommand =						"close";
	const string setSearchDirCommand =				"specifySearch ";
	const string resetSearchDirCommand =			"resetSearch";
	const string exitCommand =						"exit";

	//Example program loop
	//********************
	while (true)
	{
		cout << endl << '>';
		getline(cin, input);

		//get path of first file somewhere in this directory/subdirectory
		if (input.substr(0, searchPathCommand.size()).compare(searchPathCommand) == 0)
		{
			auto path = fs.GetPhysicalFilePath(input.substr(searchPathCommand.size()));
			if (path.empty())
			{
				cout << "file not found in current searchDirectory";
			}
			cout << path << endl;
			continue;
		}

		//fill file table with files in specific directory (not in its subdirectories)
		if (input.substr(0, listFilesInDirectoryCommand.size()).compare(listFilesInDirectoryCommand) == 0)
		{
			vector<shared_ptr<File>> file_table;
			fs.GetFilesInDirectory(file_table, input.substr(listFilesInDirectoryCommand.size()));
			cout << endl;
			for (auto f : file_table)
			{
				cout << f->GetName() << endl;
			}
			cout << endl;

			continue;
		}

		//fill file table with files with this extension IN SUBDIRECTORIES TOO
		if (input.substr(0, listFilesWithExtensionCommand.size()).compare(listFilesWithExtensionCommand) == 0)
		{
			vector<shared_ptr<File>> file_table_ext;
			fs.GetFilesWithExtension(file_table_ext, input.substr(listFilesWithExtensionCommand.size()));
			cout << endl;
			for (auto f : file_table_ext)
			{
				cout << f->GetName() << endl;
			}
			cout << endl;

			continue;
		}

		//Mount ZipArchive
		if (input.substr(0, mountZipCommand.size()).compare(mountZipCommand) == 0)
		{
			auto file = fs.GetFile(input.substr(mountZipCommand.size()));
			if (file != nullptr)
			{
				fs.MountZipArchive(file);
			}
			else cout << "File not Found" << endl;

			continue;
		}

		//find file by name + open
		if (input.substr(0, openCommand.size()).compare(openCommand) == 0)
		{
			if (filebyname != nullptr)//not really necesarry but ye
				filebyname->Close();

			filebyname = fs.GetFile(input.substr(openCommand.size()));
			if (filebyname != nullptr)
			{
				filebyname->Open(FILE_ACCESS_MODE::ReadWrite, FILE_ACCESS_FLAGS::OpenExisting);
			}
			else cout << "File not Found" << endl;

			continue;
		}

		//find file by name + open (with only read permission)
		if (input.substr(0, openReadCommand.size()).compare(openReadCommand) == 0)
		{
			if (filebyname != nullptr)//not really necesarry but ye
				filebyname->Close();

			filebyname = fs.GetFile(input.substr(openReadCommand.size()));
			if (filebyname != nullptr)
			{
				filebyname->Open(FILE_ACCESS_MODE::Read, FILE_ACCESS_FLAGS::OpenExisting);
			}

			else cout << "File not Found" << endl;

			continue;
		}

		//read the opened file
		if (input.substr(0, readCommand.size()).compare(readCommand) == 0)
		{
			if (filebyname != nullptr)
			{
				cout << filebyname->ReadText(stoi(input.substr(readCommand.size())));
			}

			continue;
		}

		//write over opened file
		if (input.substr(0, writeCommand.size()).compare(writeCommand) == 0)
		{
			auto newText = input.substr(writeCommand.size());
			if (filebyname != nullptr)
			{
				filebyname->Write(reinterpret_cast<const uint8_t*>(&newText[0]), newText.size());
			}

			continue;
		}

		//close file
		if (input.substr(0, closeCommand.size()).compare(closeCommand) == 0)
		{
			if (filebyname != nullptr)
			{
				filebyname->Close();
				filebyname = nullptr;
			}
			continue;
		}

		//sets more specific searchPath
		if (input.substr(0, setSearchDirCommand.size()).compare(setSearchDirCommand) == 0)
		{
			auto dir = fs.GetDirectory(input.substr(setSearchDirCommand.size()));
			if (dir != nullptr)
				fs.SetSearchDirectory(dir);
			else cout << "No such directory found" << endl;
			continue;
		}

		//REsets more specific searchPath
		if (input.substr(0, resetSearchDirCommand.size()).compare(resetSearchDirCommand) == 0)
		{
			fs.SetSearchDirectoryToMounted();
			continue;
		}

		if (input.compare(exitCommand) == 0)
			break;

		cout << "Invalid Command" << endl;
	}

	//other features
	//**************
	//find file by full path
	//std::shared_ptr<File> filebypath = fs.GetFileByFullPath(fs.GetPhysicalFilePath("test.zip"));

	//find directory by full path
	//std::shared_ptr<Directory> dirbypath = fs.GetDirectoryByFullPath("C:\\Users\\Jonas\\SkyDrive\\DAE\\Own\\ProjectEuler\\ProjectEuler\\Debug");
	//string s = dirbypath->GetName();


	return 0;
}
