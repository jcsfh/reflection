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
		///this class is a interface to set your own allocator. it's a singleton.
		class DSL alloc_loader : public Object<alloc_loader>
		{
			friend class init_loader;

			alloc_loader()
				: __ptr_allocator_interface(nullptr)
			{
			}

		public:
			///register your own allocator in the entrance of main function.
			///Note: marco GLOBAL_DEFAULT_HEAP must be off in ../runtime.h
			inline void register_global_allocator(allocator_interface* ptr_allocator_interface) throw(std::exception)
			{
#ifdef GLOBAL_DEFAULT_HEAP
				if(__ptr_allocator_interface!=nullptr && ptr_allocator_interface!=nullptr)
				{
					StdThrow(std::runtime_error, "can not register global allocator when GLOBAL_DEFAULT_HEAP is on");
				}
#endif
				__ptr_allocator_interface = ptr_allocator_interface;
			}

			inline allocator_interface* get_global_allocator() const
			{
				return __ptr_allocator_interface;
			}

			static alloc_loader& instance();

		private:
			allocator_interface* __ptr_allocator_interface;
		};
	}
}