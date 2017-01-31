#pragma once

#if defined(PLATFORM_Linux)
#include <fcntl.h>
enum FILE_ACCESS_FLAGS {
	Create = O_CREAT | O_EXCL,
	OpenExisting = 0,
	Truncate = O_TRUNC
};
#elif defined(PLATFORM_Windows)
#include "windows.h"
enum FILE_ACCESS_FLAGS {
	Create = CREATE_NEW,
	OpenExisting = OPEN_EXISTING,
	Truncate = TRUNCATE_EXISTING
};
#endif
