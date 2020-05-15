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

namespace gce
{
	namespace runtime
	{
		long memory_leak::_seq_break = 0;
		long memory_leak::_seq_count = 0;

#ifdef _WINDOWS

		memory_leak::memory_leak()
		{
			enble_memory_check = true;
			enble_memory_difference = false;
#ifdef _DEBUG
			_CrtMemCheckpoint(&m_s1);
#ifdef _AFX
			AfxEnableMemoryLeakDump(false);
#endif
#endif
		}

		int memory_leak::EnableDbgFlag(bool bEnbale)
		{
#ifdef _DEBUG
			int nFlag = 0;
			if(bEnbale)
			{
				nFlag = _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF;
			}
			else
			{
				nFlag = _CRTDBG_ALLOC_MEM_DF;
			}
			return ::_CrtSetDbgFlag ( nFlag );
#endif//_DEBUG
			return 0;
		}
#else
		memory_leak::memory_leak()
		{
		}
#endif

		memory_leak& memory_leak::globle_instance()
		{
			static memory_leak _memory_leak;
			return _memory_leak;
		}

		long memory_leak::set_break_alloc(long break_alloc)
		{
#ifdef _DEBUG
			_seq_break = break_alloc;
#endif//_DEBUG
			return _seq_break;
		}

		long memory_leak::SetBreakAlloc(long lBreakAlloc)
		{
#ifdef _DEBUG
#ifdef _WINDOWS
			return ::_CrtSetBreakAlloc(lBreakAlloc);
#endif
#endif//_DEBUG
			return 0;
		}

		void memory_leak::print_leak(uint seq, void* p, size_t size, alloc_trace_data& trace_data)
		{
#ifdef ALLOC_TRACE
			char buf[1024];

			if(trace_data.file_name!=nullptr)
			{
#ifdef PLATFORM_64
				sprintf(buf, "memory leak -> [sequence: %u  address: 0x%016llX  size: %llu bytes  file: %s  function: %s  line: %d]\n", seq, (uint64)p, (uint64)size, trace_data.file_name, trace_data.func_name, trace_data.line);
#else
				sprintf(buf, "memory leak -> [sequence: %u  address: 0x%08X  size: %u bytes  file: %s  function: %s  line: %d]\n", seq, (uint)p, (uint)size, trace_data.file_name, trace_data.func_name, trace_data.line);
#endif
			}
			else
			{
#ifdef PLATFORM_64
				sprintf(buf, "memory leak -> [sequence: %u  address: 0x%016llX  size: %llu bytes]\n", seq, (uint64)p, (uint64)size);
#else
				sprintf(buf, "memory leak -> [sequence: %u  address: 0x%08X  size: %u bytes]\n", seq, (uint)p, (uint)size);
#endif
			}
#ifdef _WINDOWS
			::OutputDebugStringA(buf);
#endif
			printf(buf);
#endif
		}

		void memory_leak::print_leak()
		{
#ifdef _DEBUG
#ifdef _WINDOWS
			_CrtMemCheckpoint(&m_s2);

			if(enble_memory_check)
			{
#ifndef _AFX
				::_CrtDumpMemoryLeaks();
#endif
			}

			if(enble_memory_difference)
			{
				_CrtMemState s3;
				if(_CrtMemDifference(&s3, &m_s1, &m_s2))
				{
					_CrtMemDumpStatistics(&s3);
				}
			}
#else
			get_mem_list().print_leak();
#endif
#endif
		}
	}
}