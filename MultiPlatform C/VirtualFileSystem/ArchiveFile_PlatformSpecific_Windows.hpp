#pragma once
#include "ArchiveFile.h"
#include "windows.h"

size_t ArchiveFile::GetByteIndex()
{
	return SetFilePointer(
		m_FileHandle,
		0,
		nullptr,
		FILE_CURRENT);
}

void ArchiveFile::SetByteIndex(size_t byteIndex)
{
	SetFilePointer(
		m_FileHandle,
		byteIndex,
		nullptr,
		FILE_BEGIN);
}
