/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

#ifndef _WINDOWS

#if defined(_M_X64) || defined(_M_IA64) || defined(_M_AMD64)\
	|| defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)

namespace gce
{
	static inline uint64 interlocked_compare_exchange64(volatile uint64 *dest, uint64 exch, uint64 comp)
	{
		uint64 old;

		__asm __volatile__ ("lock; "
			"cmpxchgq"
			" %2, %0"
			: "=m" (*dest), "=a" (old)
			: "r" (exch), "m" (*dest), "a" (comp));  

		return(old);
	}

	static inline uint32 interlocked_exchange32(volatile uint32 *val, uint32 new_val)
	{
		uint32 ret;

		__asm__ __volatile__ ("1:; lock; "
			"cmpxchg"
			" %2, %0; jne 1b"
			: "=m" (*val), "=a" (ret)
			: "r" (new_val), "m" (*val), "a" (*val));

		return(ret);
	}

	static inline uint64 interlocked_exchange64(volatile uint64 *val, uint64 new_val)
	{
		uint64 ret;

		__asm__ __volatile__ ("1:; lock; "
			"cmpxchgq"
			" %2, %0; jne 1b"
			: "=m" (*val), "=a" (ret)
			: "r" (new_val), "m" (*val), "a" (*val));

		return(ret);
	}

	static inline long interlocked_exchange_add32(volatile uint32 *val, uint32 add)
	{
		uint32 ret;

		__asm__ __volatile__ ("lock; xaddl %0, %1"
			: "=r" (ret), "=m" (*val)
			: "0" (add), "m" (*val));

		return(ret);
	}

	static inline long interlocked_exchange_add64(volatile uint64 *val, uint64 add)
	{
		uint64 ret;

		__asm__ __volatile__ ("lock; xaddq %0, %1"
			: "=r" (ret), "=m" (*val)
			: "0" (add), "m" (*val));

		return(ret);
	}
}

#endif
#endif