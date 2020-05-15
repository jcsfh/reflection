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
		///trace new/delete data struct
		struct alloc_trace_data
		{
			char* file_name;
			char* func_name;
			int line;

			alloc_trace_data()
			{
				::memset(this, 0, sizeof(*this));
			}
		};

		extern DSL void* global_malloc(size_t size);
		extern DSL void* global_malloc(size_t size, const char* file_name, const char* func_name, int line);
		extern DSL void* global_realloc(void* p, size_t size);
		extern DSL void* global_realloc(void* p, size_t size, const alloc_trace_data* ptr_alloc_trace_data);
		extern DSL void global_free(void *p);

		extern DSL void* system_malloc(size_t size);
		extern DSL void* system_realloc(void* p, size_t size);
		extern DSL	void system_free(void* p);

		template<class T>
		static T* global_malloc_t(size_t count)
		{
			return (T*)global_malloc(count*sizeof(T));
		}

		template<class T>
#ifdef _WINDOWS
		static T* global_realloc_t(T* p, size_t count)
#else
		inline T* global_realloc(T* p, size_t count)
#endif
		{
			if(count<0)
			{
				return p;
			}
			return (T*)global_realloc((void*)p, count*sizeof(T));
		}

		template<ulong_t cval>
		static bool compare_memory_unwinding(void* p)
		{
			if(nullptr==p)
			{
				return false;
			}

			const static char n1 = cval&0x000000FF;
			const static char n2 = (cval&0x0000FF00)>>8;
			const static char n3 = (cval&0x00FF0000)>>16;
			const static char n4 = (cval&0xFF000000)>>24;

#ifdef PLATFORM_64
			const static char n5 = (cval&0x000000FF00000000)>>32;
			const static char n6 = (cval&0x0000FF0000000000)>>40;
			const static char n7 = (cval&0x00FF000000000000)>>48;
			const static char n8 = (cval&0xFF00000000000000)>>56;
#endif
			return
#ifdef PLATFORM_64
				(n8==*(char*)(p=(char*)p-sizeof(char)))
				&& (n7==*(char*)(p=(char*)p-sizeof(char)))
				&& (n6==*(char*)(p=(char*)p-sizeof(char)))
				&& (n5==*(char*)(p=(char*)p-sizeof(char)))
				&&
#endif
				(n4==*(char*)(p=(char*)p-sizeof(char)))
				&& (n3==*(char*)(p=(char*)p-sizeof(char)))
				&& (n2==*(char*)(p=(char*)p-sizeof(char)))
				&& (n1==*(char*)(p=(char*)p-sizeof(char)))
				;
		}

		class mem_list;
		extern mem_list& get_mem_list();
		extern DSL void* handle_null_pointer(size_t size) throw(std::bad_alloc());

		extern DSL void local_enable_alloc(bool enable);
	}
}