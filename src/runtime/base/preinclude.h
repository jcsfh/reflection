/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

#ifdef _WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <tchar.h>
#include <winsock2.h>
#include <shlwapi.h>
#include <new.h>

#ifndef _WIN32_WCE
#pragma comment(lib, "Wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#pragma comment(lib, "ws2.lib")
#endif


#else


#include <cxxabi.h>
#include <stddef.h>
#include <stdlib.h>

#include <sstream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <cstdio>
#include <cstring>
#include <typeinfo>
#include <tr1/memory>

using namespace std;

#endif

#include <vector>
#include <set>

#include <utility>
#include <time.h>
#include <typeinfo>
#include <assert.h>

#include <stdio.h>
#include <stdarg.h>

#include <iostream>
#include <sstream>
#include <functional>

#ifdef _WIN32_WCE
#include "Mmsystem.h"
#pragma comment(lib, "Mmsystem.lib")
#else
#include <sys/stat.h>
#endif