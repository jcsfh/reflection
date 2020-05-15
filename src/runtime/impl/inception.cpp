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
#include "../base/init_loader.h"

#include <map>

namespace gce
{
	std::set<std::string>*& get_set_typeinfo()
	{
#pragma push_macro("new")
#undef new
		static auto _set_typeinfo = new std::set<std::string>;
#pragma pop_macro("new")
		return _set_typeinfo;
	}

	bool __set_type(const ::type_info& ti)
	{
		if(nullptr==get_set_typeinfo())
		{
			return false;
		}

		std::string name = _typeid(ti).name();
		if(get_set_typeinfo()->find(name)!=get_set_typeinfo()->end())
		{
			return true;
		}
		else
		{
			get_set_typeinfo()->insert(name);
			return false;
		}
	}

	bool __remove_type(const ::type_info& ti)
	{
		if(nullptr==get_set_typeinfo())
		{
			return false;
		}

		std::string name = _typeid(ti).name();
		auto it = get_set_typeinfo()->find(name);
		if(it!=get_set_typeinfo()->end())
		{
			get_set_typeinfo()->erase(it);
			return true;
		}
		else
		{
			return false;
		}
	}

	void __register(std::function<void()> dispose)
	{
		gce::runtime::init_loader::register_cleaner_callback(dispose);
	}
}