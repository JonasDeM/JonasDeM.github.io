solution "VirtuaFileSystem"
	configurations {
		"DebugLinux",
		"ReleaseLinux",
		"DebugWindows",
		"ReleaseWindows",
}

	platforms {
		"x32",
		"x64",
		"Native", --for targets where bitness is not specified
	}
	language "C++"

location "../"
PROJ_DIR = path.getabsolute("..")

	project "VirtualFileSystemDemo"
		kind "ConsoleApp"

		files {
			path.join(PROJ_DIR,"*.cpp"),
			path.join(PROJ_DIR,"*.hpp"),
			path.join(PROJ_DIR,"*.h"),
		}

		configuration "DebugLinux"
			targetdir "../bin/DebugLinux"
			defines { "_DEBUG" }
			flags {"Symbols"}
			objdir "../obj/debug"
			defines{"PLATFORM_Linux"}
			targetextension ""

		configuration "DebugWindows"
			targetdir "../bin/DebugWindows"
			defines { "_DEBUG" }
			flags {"Symbols"}
			objdir "../obj/debug"
			defines{"PLATFORM_Windows"}
			targetextension ".exe"

		configuration "ReleaseWindows"
			targetdir "../bin/ReleaseWindows"
			defines { "NDEBUG" }
			objdir "../obj/debug"
			defines{"PLATFORM_Windows"}
			targetextension ".exe"

		configuration "ReleaseLinux"
			targetdir "../bin/ReleaseLinux"
			defines { "NDEBUG" }
			objdir "../obj/debug"
			defines{"PLATFORM_Linux"}
			targetextension ""


		configuration{}

		defines{"_CONSOLE"}

