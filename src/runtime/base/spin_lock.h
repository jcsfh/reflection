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
		///spin lock based on cmp-and-exchg
		class DSL spin_lock : public gce::Object<spin_lock>
		{
		public:
			spin_lock();
			~spin_lock();

			void lock();
			bool try_lock();
			void unlock();

		public:
			static uint32 atomic_compare_and_exchange(volatile uint32* target, uint32 exchange, uint32 comperand);

		private:
			volatile uint32 _lock_val;
		};
	}
}