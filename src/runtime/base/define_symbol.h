/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

#ifdef _WINDOWS

#define snprintf _snprintf

#ifndef sleep_ms
#define sleep_ms ::Sleep
#endif

#else

#ifndef nullptr
#define nullptr (0)
#endif

#ifndef sleep_ms
extern void sleep_ms(uint x);
#endif
#endif

#define safe_delete_ex(p)\
	if(p!=nullptr)\
			{\
			delete[] p;\
			p = nullptr;\
			}

#define safe_delete(p)\
	if(p!=nullptr)\
			{\
			delete p;\
			p = nullptr;\
			}

#define MEMBER_OFFSET(Class, Member) (((int)(long)&((Class*)nullptr)->Member))

#ifndef Max
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef Min
#define Min(a,b)            (((a) < (b)) ? (a) : (b))
#endif