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

#include "../base/leak.h"

namespace gce
{
	namespace runtime
	{
		mem_list::mem_list()
			: _tail(nullptr)
		{
		}

		mem_list::~mem_list()
		{
			release();
		}

		void* mem_list::add(void* p, size_t size, const alloc_trace_data* ptr_trace_data)
		{
			mem_node* node = (mem_node*)::malloc(sizeof(mem_node));
			node->init();

			node->p = p;
			node->size = size;
			if(ptr_trace_data!=nullptr)
			{
				node->trace_data = *ptr_trace_data;
			}

			_spin_lock.lock();

			if(_tail!=nullptr)
			{
				_tail->next = node;
				node->prev = _tail;
			}
			_tail = node;

			node->seq = ++memory_leak::_seq_count;
			assert(memory_leak::_seq_break!=node->seq);

			_spin_lock.unlock();

			return node;
		}

		void mem_list::update(void* p, size_t size, const alloc_trace_data* ptr_trace_data)
		{
			if(nullptr==p)
			{
				return;
			}
			mem_node* node = (mem_node*)p;

			node->size = size;
			if(ptr_trace_data!=nullptr)
			{
				node->trace_data = *ptr_trace_data;
			}

			_spin_lock.lock();
			node->seq = ++memory_leak::_seq_count;
			assert(memory_leak::_seq_break!=node->seq);
			_spin_lock.unlock();
		}

		struct alloc_header
		{
			void* p;
			ulong_t flag;

			inline void reset()
			{
				flag = reset_flag;
			}

#ifdef PLATFORM_64
			const static ulong_t reset_flag = 0xB7B7B7B7E7E7E7E7;
#else
			const static ulong_t reset_flag = 0xB7B7E7E7;
#endif
		};

		void mem_list::erase(void* p)
		{
			if(nullptr==p)
			{
				return;
			}
			mem_node* node = (mem_node*)p;
#ifdef ALLOC_TRACE
			auto alloc_header_ = (alloc_header*)node->p - 1;
			alloc_header_->reset();
#endif

			_spin_lock.lock();

			if(_tail==node)
			{
				_tail = node->prev;
			}

			if(node->prev!=nullptr)
			{
				node->prev->next = node->next;
			}
			if(node->next!=nullptr)
			{
				node->next->prev = node->prev;
			}

			_spin_lock.unlock();

			::free(node);
		}

		void mem_list::release()
		{
			while(_tail!=nullptr)
			{
				mem_node* node = _tail;
#ifdef ALLOC_TRACE
				auto alloc_header_ = (alloc_header*)node->p - 1;
				alloc_header_->reset();
#endif
				_tail = _tail->prev;
				::free(node);
			}
		}

		void mem_list::print_leak() const
		{
			if(nullptr==_tail)
			{
				return;
			}

			const char* info = "runtime detected memory leaks ------>\n";
			printf(info);
#ifdef _WINDOWS
			::OutputDebugStringA(info);
#endif

			mem_node* node = _tail;
			while(node!=nullptr && node->prev!=nullptr)
			{
				node = node->prev;
			}

			while(node!=nullptr)
			{
				memory_leak::print_leak(node->seq, node->p, node->size, node->trace_data);
				node = node->next;
			}
		}
	}
}