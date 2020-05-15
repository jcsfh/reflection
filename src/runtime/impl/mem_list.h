/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

#include "../base/spin_lock.h"

namespace gce
{
	namespace runtime
	{
		///memory recorder
		class mem_list : public gce::Object<mem_list>
		{
			struct mem_node
			{
				uint seq;
				void* p;
				size_t size;
				alloc_trace_data trace_data;

				mem_node* prev;
				mem_node* next;

				void init()
				{
					memset(this, 0, sizeof(*this));
				}
			};

		public:
			mem_list();
			~mem_list();

			void* add(void* p, size_t size, const alloc_trace_data* ptr_trace_data);
			void update(void* p, size_t size, const alloc_trace_data* ptr_trace_data);
			void erase(void* p);
			void release();

			void print_leak() const;

		private:
			mem_node* _tail;
			spin_lock _spin_lock;
		};
	}
}