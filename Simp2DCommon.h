#pragma once
#if defined (_WIN32) && defined(__cplusplus)
	#if defined (_MSC_VER)
		#pragma comment(lib, "opengl32.lib")
	#else
		#warning "Please link opengl libriaries "opengl32.lib" 
	#endif // _MSC_VER
#else
	#error "For C++ Windows only!!!"
#endif


// i prefer to ANSI :>
#ifdef UNICODE
	#undef UNICODE
#endif
#ifdef _UNICODE
	#undef _UNICODE
#endif // _UNICODE


#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <vector>		// NOTE TO SELF: Migth be a good idea to implement ur own arraylist
#include <string>		


