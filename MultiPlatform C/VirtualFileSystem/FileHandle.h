
#pragma once

#if defined(PLATFORM_Linux)
typedef int FILE_HANDLE;
#define FILE_HANDLE_INVALID (-1)
#elif defined(PLATFORM_Windows)
#include "windows.h"
typedef HANDLE FILE_HANDLE;
#define FILE_HANDLE_INVALID INVALID_HANDLE_VALUE
#endif
