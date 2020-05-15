/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "base/predefine.h"
#include "base/preinclude.h"
#include "base/define_type.h"
#include "base/define_symbol.h"

#ifdef _WINDOWS
#pragma warning(disable:4996 4251 4800 4290 4307)
#endif

#define GLOBAL_DEFAULT_HEAP

#if defined(_DEBUG)
///enable tracing memory allocating, working only in Debug mode.
#define ALLOC_TRACE
#endif

#include "base/typeid.h"
#include "base/atomic64.h"
#include "base/alloc.h"
#include "base/const.h"
#include "base/throw.h"
#include "base/object.h"
#include "base/runtime_helper.h"

#if defined(_DEBUG) || !defined(EXCLUDE_ALLOC) || !defined(_WINDOWS)
#include "base/define_new.h"
#endif

#include "base/leak.h"
#include "base/inception.h"
#include "base/init_loader.h"
#include "base/spin_lock.h"
#include "base/alloc_interface.h"
#include "base/alloc_loader.h"