/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

struct _typeid
{
	///formating typeid's name
	_typeid(const ::type_info& type)
	{
#ifdef _WINDOWS
		_str_ = (char*)type.name();
#else
		int result;
		_str_ = abi::__cxa_demangle(type.name(), 0, 0, &result);
#endif
	}

	~_typeid()
	{
#ifndef _WINDOWS
		::free(_str_);
#endif
	}

	inline const char* name() const
	{
		return _str_;
	}

private:
	char* _str_;
};