#pragma once
#include "ArchiveFile.h"
#include <sys/types.h>
#include <unistd.h>

size_t ArchiveFile::GetByteIndex()
{
	return lseek(m_FileHandle, 0, SEEK_CUR);
}

void ArchiveFile::SetByteIndex(size_t byteIndex)
{
	lseek(m_FileHandle, byteIndex, SEEK_SET);
}
