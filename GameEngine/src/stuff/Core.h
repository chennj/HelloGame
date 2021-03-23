#pragma once

#ifdef SO_PLATFORM_WINDOWS
	#ifdef SO_BUILD_DLL
		#define SOME_API __declspec(dllexport) 
	#else
		#define SOME_API __declspec(dllimport) 
	#endif
#else
	#error Hello only spport Windows!
#endif