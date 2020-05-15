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
		///cheking memory leak
		class DSL memory_leak : public make_final<memory_leak>
		{
		private:
			memory_leak();

		public:
			void print_leak();

			static memory_leak& globle_instance();

		public:
#ifdef _WINDOWS
			static int EnableDbgFlag(bool bEnbale=true);
#endif
			static long set_break_alloc(long seq);
			static long SetBreakAlloc(long lBreakAlloc);

			static void print_leak(uint seq, void* p, size_t size, alloc_trace_data& trace_data);

		public:
			bool enble_memory_check;
			bool enble_memory_difference;

			static long _seq_break;
			static long _seq_count;

#ifdef _WINDOWS
		private:
			_CrtMemState m_s1;
			_CrtMemState m_s2;
#endif
		};
	}
}