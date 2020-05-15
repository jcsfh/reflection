/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#define LIBRIAY_EXPORT
#include "../runtime.h"

#include "mem_list.h"
#include "../base/init_loader.h"

namespace gce
{
	namespace runtime
	{
		void pre_loader::_incept()
		{
			///set break sequence number here to trace the allocating
			memory_leak::globle_instance().set_break_alloc(0);
			memory_leak::globle_instance().enble_memory_check = false;

			get_mem_list();
		}

		void pre_loader::_dispose()
		{
			init_loader::_dispose();

			delete get_set_typeinfo();
			get_set_typeinfo() = nullptr;

			///print memory leak in the last destructor.
			get_mem_list().print_leak();
//#ifdef _WINDOWS
//			memory_leak::globle_instance().print_leak();
//#endif
		}
	}
}