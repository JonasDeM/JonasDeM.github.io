#pragma once

#if defined(PLATFORM_Linux)
#include <fcntl.h>
enum FILE_ACCESS_MODE
{
	Read = O_RDONLY,
	Write = O_WRONLY,
	ReadWrite = O_RDWR,
};
#elif defined(PLATFORM_Windows)
#include "windows.h"
enum FILE_ACCESS_MODE
{
	Read = GENERIC_READ,
	Write = GENERIC_WRITE,
	ReadWrite = GENERIC_READ | GENERIC_WRITE
};
#endif
