#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct ZipFileHeader
{
	int32_t signature = (0x04034b50);
	int16_t version;
	int16_t flags;
	int16_t compression;
	int16_t lastmodfiletime;
	int16_t lastmodfiledate;
	int32_t crc32;
	int32_t compressedsize;
	int32_t uncompressedsize;
	int16_t filenamelength;
	int16_t extrafieldlength;/*
		filename(variablesize)					   ;
		extrafield(variablesize)				   ;*/
};
#pragma pack(pop)
