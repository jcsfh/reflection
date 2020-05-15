// reflect.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include "../../../src/runtime/runtime.h"

#ifdef _DEBUG
#ifdef PLATFORM_64
#pragma comment(lib, "../../../libs/windows/x64/Debug/gcrt.lib")
#else
#pragma comment(lib, "../../../libs/windows/Debug/gcrt.lib")
#endif
#else
#ifdef PLATFORM_64
#pragma comment(lib, "../../../libs/windows/x64/release/gcrt.lib")
#else
#pragma comment(lib, "../../../libs/windows/release/gcrt.lib")
#endif
#endif