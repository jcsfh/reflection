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
		///memory allocator interface, you can implement it by your own
		class DSL allocator_interface : public Object<allocator_interface>
		{
		public:
			virtual ~allocator_interface(){}
			virtual void initialize(size_t max_size) STD_THROW{}
			virtual bool enable_auto_increment(bool is_auto_increment, uint increment_buffer_size=0){return false;}
			virtual void* allocate(size_t size) = 0;
			virtual void* reallocate(void* p, size_t size) = 0;
			virtual void free(void* p) = 0;
			virtual void release(){}

			virtual bool test_created() const{return false;}
			virtual void print_debug_statistic_information() const{}
		};
	}
}