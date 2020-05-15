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
#include "../base/define_new.h"

#include "mem_list.h"

namespace gce
{
	namespace runtime
	{
		thread_local_varial(bool) _local_enable_alloc = true;
		void local_enable_alloc(bool enable)
		{
			_local_enable_alloc = enable;
		}

		void* rt_malloc(size_t size)
		{
			return global_malloc(size);
		}

		void* rt_malloc(size_t size, const char* file_name, const char* func_name, int line)
		{
			return global_malloc(size, file_name, func_name, line);
		}

		void rt_free(void *p)
		{
			global_free(p);
		}

		struct alloc_header
		{
			void* p;
			ulong_t flag;

			inline void init()
			{
				::memset(this, 0, sizeof(*this));
				flag = default_flag;
			}

#ifdef PLATFORM_64
			const static ulong_t default_flag = 0xD7D7D7D7A7A7A7A7;
			const static ulong_t reset_flag = 0xB7B7B7B7E7E7E7E7;
#else
			const static ulong_t default_flag = 0xD7D7A7A7;
			const static ulong_t reset_flag = 0xB7B7E7E7;
#endif
		};

		void* global_malloc(size_t size)
		{
			return global_realloc(nullptr, size);
		}

		void* global_malloc(size_t size, const char* file_name, const char* func_name, int line)
		{
#ifdef ALLOC_TRACE
			alloc_trace_data trace_data;
			trace_data.file_name = (char*)file_name;
			trace_data.func_name = (char*)func_name;
			trace_data.line = line;
			return global_realloc(nullptr, size, &trace_data);
#else
			return global_realloc(nullptr, size, nullptr);
#endif
		}

		void* global_realloc(void* p, size_t size)
		{
			return global_realloc(p, size, nullptr);
		}

		void* global_realloc(void* p, size_t size, const alloc_trace_data* ptr_alloc_trace_data)
		{
#ifdef ALLOC_TRACE
			if(!_local_enable_alloc)
			{
				return system_realloc(p, size);
			}

			void* ptr_old = p;
			void* ptr_old_erase = nullptr;
			if(memory_leak::globle_instance().enble_memory_check)
			{
				if(ptr_old!=nullptr && compare_memory_unwinding<alloc_header::default_flag>(ptr_old))
				{
					ptr_old = ((alloc_header*)ptr_old) - 1;
					alloc_header* ptr_old_alloc_header = (alloc_header*)ptr_old;
					ptr_old_erase = ptr_old_alloc_header->p;
					p = ((alloc_header*)p) - 1;
				}
				size += sizeof(alloc_header);
			}
#endif
			allocator_interface* ptr_allocator_interface = alloc_loader::instance().get_global_allocator();
			if(ptr_allocator_interface!=nullptr)
			{
				p = ptr_allocator_interface->reallocate(p, size);
			}
			else
			{
				p = system_realloc(p, size);
			}

#ifdef ALLOC_TRACE
			if(memory_leak::globle_instance().enble_memory_check)
			{
				alloc_header* ptr_alloc_header = (alloc_header*)p;
				p = ((alloc_header*)p) + 1;

				if(p != ptr_old)
				{
					if(ptr_old_erase!=nullptr)
					{
						get_mem_list().erase(ptr_old_erase);
					}

					ptr_alloc_header->init();
					ptr_alloc_header->p = get_mem_list().add(p, size, ptr_alloc_trace_data);
				}
				else
				{
					get_mem_list().update(ptr_alloc_header->p, size, ptr_alloc_trace_data);
				}
			}
#endif
			return p;
		}

		void global_free(void *p)
		{
			if(nullptr==p)
			{
				return;
			}

#ifdef ALLOC_TRACE
			if(compare_memory_unwinding<alloc_header::reset_flag>(p))
			{
				return;
			}

			if(compare_memory_unwinding<alloc_header::default_flag>(p))
			{
				p = ((alloc_header*)p) - 1;
				alloc_header* ptr_alloc_header = (alloc_header*)p;
				get_mem_list().erase(ptr_alloc_header->p);
			}
#endif
			allocator_interface* ptr_allocator_interface = alloc_loader::instance().get_global_allocator();
			if(ptr_allocator_interface!=nullptr)
			{
				ptr_allocator_interface->free(p);
			}
			else
			{
				system_free(p);
			}
		}

		void* system_malloc(size_t size)
		{
			return ::malloc(size);
		}

		void* system_realloc(void* p, size_t size)
		{
			return ::realloc(p, size);
		}

		void system_free(void* p)
		{
			::free(p);
		}

		mem_list& get_mem_list()
		{
			static mem_list _mem_list;
			return _mem_list;
		}

		void* handle_null_pointer(size_t size) throw(std::bad_alloc())
		{
			void* p = nullptr;
			new_handler globalhandler = set_new_handler(0);
			set_new_handler(globalhandler);

			if (globalhandler)
			{
				(*globalhandler)();
				throw std::bad_alloc();
			}
			else
			{
				bool bFlag = true;
				while(bFlag)
				{
					bFlag = false;
					char buf[32];
					::printf("Memory is exhausted:\n1: try again\n2: exit process\n3: throw exception\nmake your chioce: ");
					::scanf("%s", buf);
					printf(buf);
					printf("\n");

					if(::strcmp(buf, "1")==0)
					{
						p = gce::runtime::system_realloc(nullptr, size);
						bFlag = (nullptr==p);
					}
					else if(::strcmp(buf, "2")==0)
					{
						::exit(0);
					}
					else
					{
						throw std::bad_alloc();
					}
				}
			}
			return p;
		}
	}
}