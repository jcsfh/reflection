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
typedef __int64             int64;
typedef unsigned __int64    uint64;
#else
typedef unsigned long long  uint64;
typedef long long           int64;
#endif

typedef unsigned int        uint;
typedef unsigned long       ulong;
typedef unsigned short int  ushort;
typedef wchar_t             wchar;
typedef unsigned char       uchar;

typedef unsigned char       byte;

typedef short int           int16;
typedef unsigned short int  uint16;
typedef int                 int32;
typedef unsigned int        uint32;

#define BYTE_PER_BIT        (8)

#ifndef HANDLE
typedef void* HANDLE;
#endif

#ifdef PLATFORM_64
typedef int64	long_t;
typedef uint64	ulong_t;
#else
typedef int		long_t;
typedef uint	ulong_t;
#endif

#ifdef UNICODE
typedef std::wstring string_type;
typedef std::wstringstream tstringstream;
#else
typedef std::string string_type;
typedef std::stringstream tstringstream;
#endif

#ifdef _WINDOWS
#define thread_local_varial(type) __declspec( thread ) type
#else
#define thread_local_varial(type) __thread type
#endif



static_assert(sizeof(int16)==2, "int16 assert failed");
static_assert(sizeof(uint16)==2, "uint16 assert failed");
static_assert(sizeof(int32)==4, "int32 assert failed");
static_assert(sizeof(uint32)==4, "uint32 assert failed");
static_assert(sizeof(int64)==8, "int64 assert failed");
static_assert(sizeof(uint64)==8, "uint64 assert failed");