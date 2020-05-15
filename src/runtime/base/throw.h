/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

#include <exception>

#define STD_THROW throw(std::exception)

const static int EXCEPTION_MSG_SIZE = 256;
const static int EXCEPTION_SIZE = 1024;

#define macro_throw(except)\
	char buf[EXCEPTION_SIZE];\
	::snprintf(buf, EXCEPTION_SIZE, "exception[%s], File[%s] Function[%s] Line[%d]\n", msg, __FILE__, __FUNCTION__, __LINE__);\
	throw except(buf);

///throw format with varaidic arguments
#ifdef _WINDOWS
#define StdThrow(except, format, ...)\
	char msg[EXCEPTION_MSG_SIZE];\
	::snprintf(msg, EXCEPTION_MSG_SIZE, format, __VA_ARGS__);\
	macro_throw(except)
#else
#define StdThrow(except, format, args...)\
	char msg[EXCEPTION_MSG_SIZE];\
	::snprintf(msg, EXCEPTION_MSG_SIZE, format, ##args);\
	macro_throw(except)
#endif