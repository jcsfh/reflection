/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

namespace gce
{
	namespace runtime
	{
		extern DSL void* rt_malloc(size_t size);
		extern DSL void* rt_malloc(size_t size, const char* file_name, const char* func_name, int line);
		extern DSL void rt_free(void *p);
	}
}

///overload new/delete
#ifdef _WINDOWS
static
#else
inline
#endif
void *operator new(size_t size)
{
	return gce::runtime::rt_malloc(size);
}

#ifdef _WINDOWS
static
#else
inline
#endif
void *operator new[](size_t size)
{
	return gce::runtime::rt_malloc(size);
}

#ifdef _WINDOWS
static
#else
inline
#endif
void operator delete(void *p)
{
	gce::runtime::rt_free(p);
}

#ifdef _WINDOWS
static
#else
inline
#endif
void operator delete[](void *p)
{
	gce::runtime::rt_free(p);
}

#ifdef ALLOC_TRACE

#ifdef _WINDOWS
static
#else
inline
#endif
void* operator new(size_t size, const char *file_name, const char *func_name, int line)
{
	return gce::runtime::rt_malloc(size, file_name, func_name, line);
}

#ifdef _WINDOWS
static
#else
inline
#endif
void* operator new[](size_t size, const char *file_name, const char *func_name, int line)
{
	return gce::runtime::rt_malloc(size, file_name, func_name, line);
}

#ifdef _WINDOWS
static
#else
inline
#endif
void operator delete(void* p, const char *file_name, const char *func_name, int line)
{
	gce::runtime::rt_free(p);
}

#ifdef _WINDOWS
static
#else
inline
#endif
void operator delete[](void* p, const char *file_name, const char *func_name, int line)
{
	gce::runtime::rt_free(p);
}

#define new new(__FILE__, __FUNCTION__, __LINE__)

#endif