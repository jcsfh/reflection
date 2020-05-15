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

#ifndef _WINDOWS
///sleep with milliseond
void sleep_ms(uint x)
{
	struct timespec req, rem;
	int err;
	req.tv_sec = x / 1000;
	req.tv_nsec = (x % 1000) * 1000000;
	while ((req.tv_sec != 0) || (req.tv_nsec != 0))
	{
		if (nanosleep(&req, &rem) == 0)
			break;
		err = errno;
		// Interrupted; continue
		if (err == EINTR) {
			req.tv_sec = rem.tv_sec;
			req.tv_nsec = rem.tv_nsec;
		}
		// Unhandleable error (EFAULT (bad pointer), EINVAL (bad timeval in tv_nsec), or ENOSYS (function not supported))
		break;
	}
}
#endif

namespace gce
{
	namespace runtime
	{
#if defined(_WINDOWS)

		uint32 spin_lock::atomic_compare_and_exchange(volatile uint32* target, uint32 exchange, uint32 comperand)
		{
			return ::InterlockedCompareExchange(target, exchange, comperand);
		}

#elif defined(SUPPROT_CPLUSPLUS_11)

		uint32 spin_lock::atomic_compare_and_exchange(volatile uint32* target, uint32 exchange, uint32 comperand)
		{
			//return std::atomic_compare_exchange_weak();
		}

#else

		uint32 spin_lock::atomic_compare_and_exchange(volatile uint32* target, uint32 exchange, uint32 comperand)
		{
			return ::__sync_val_compare_and_swap(target, comperand, exchange);
		}

#endif

		spin_lock::spin_lock()
			: _lock_val(0)
		{
		}

		spin_lock::~spin_lock()
		{
		}

		void spin_lock::lock()
		{
			uint i=0;
			while(atomic_compare_and_exchange(&_lock_val, 1, 0) != 0)
			{
				if(i%100==0)
				{
					sleep_ms(0);
				}
			}
		}

		bool spin_lock::try_lock()
		{
			return atomic_compare_and_exchange(&_lock_val, 1, 0) == 0;
		}

		void spin_lock::unlock()
		{
			atomic_compare_and_exchange(&_lock_val, 0, 1);
		}
	}
}