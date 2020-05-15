/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

/////////////////////////////////////Debug//////////////////////////////////////
#ifdef DEBUG
#ifndef _DEBUG
#define _DEBUG
#endif
#endif

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

/////////////////////////////////////Windows//////////////////////////////////////
#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_MSC_VER)
#ifndef _WINDOWS
#define _WINDOWS
#endif
#endif

/////////////////////////////////////Linux//////////////////////////////////////
#if defined(__linux__) || defined(linux) || defined(__GNUC__) || defined(__unix__) || defined(unix) || defined(UNIX) || defined(AIX)
#ifndef LINUX
#define LINUX
#endif
#endif

/////////////////////////////////////x64//////////////////////////////////////
#if defined(_M_X64) || defined(_M_IA64) || defined(_M_AMD64)\
	|| defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#define PLATFORM_64
#ifdef _WINDOWS
#define WINDOWS_PLATFORM_64
#endif
#else
#endif

#ifdef WINDOWS_PLATFORM_64
#define USE_64_API
#endif

///////////////////////////////////////////////////////////////////////////
#ifdef _WINDOWS

#define DSL_EXPORT      __declspec( dllexport )
#define DSL_IMPORT      __declspec( dllimport )
#define DISABLE_VTABLE  __declspec( novtable )

#ifdef LIBRIAY_EXPORT
#define DSL             DSL_EXPORT
#else
#define DSL             DSL_IMPORT
#endif

#else

#define DSL
#define DSL_EXPORT
#define DSL_IMPORT
#define DISABLE_VTABLE

#endif


#define RUNTIME_HELPER