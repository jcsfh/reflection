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

#include "../base/init_loader.h"
#include "mem_list.h"

namespace gce
{
	namespace runtime
	{
#ifndef _WINDOWS
		pre_loader __pre_loader;
		init_loader __init_loader;
#endif

		std::vector<std::function<void()> >& cleaner_callback_list()
		{
			static std::vector<std::function<void()> > __cleaner_callback_list;
			return __cleaner_callback_list;
		}

		void init_loader::register_cleaner_callback(std::function<void()> cleaner_callback)
		{
			cleaner_callback_list().push_back(cleaner_callback);
		}

		alloc_loader& alloc_loader::instance()
		{
			static alloc_loader _alloc_loader;
			return _alloc_loader;
		}

		void init_loader::_incept()
		{
			__init_socket();
			cleaner_callback_list();
		}

		void init_loader::_dispose()
		{
			///call cleaner functions
			while(!cleaner_callback_list().empty())
			{
				cleaner_callback_list().back()();
				cleaner_callback_list().pop_back();
			}
#ifdef _WINDOWS
			cleaner_callback_list().~vector();
#else
			cleaner_callback_list().clear();
#endif

			allocator_interface* ptr_allocator_interface = alloc_loader::instance().get_global_allocator();
#ifdef GLOBAL_DEFAULT_HEAP
			if(ptr_allocator_interface!=nullptr)
			{
				ptr_allocator_interface->release();
			}
#endif
			///delete allocator
			alloc_loader::instance().register_global_allocator(nullptr);
			delete ptr_allocator_interface;
		}

		void init_loader::__init_socket()
		{
#ifdef _WINDOWS
			static bool bStartup = false;
			if(!bStartup)
			{
				bStartup = true;
				WSADATA stWsaData;
				WSAStartup(MAKEWORD(2, 2), &stWsaData);
			}
#endif
		}
	}
}