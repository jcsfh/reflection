/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#include "../../runtime/runtime.h"

#include "../rtype.h"
#include "../type_info.h"
#include "../rclass.h"

#include <map>

namespace gce
{
	namespace reflection
	{
#ifndef _WINDOWS
		type_info _type_info;
#endif

		void type_info::_incept()
		{
		}

		void type_info::_dispose()
		{
		}

		rtype* type_info::__register_type(const std::string& name, std::string* ptr_uuid)
		{
			return type_manager::_register_type(name, ptr_uuid);
		}
		/*
		void type_info::_register_name(const std::string& name, reflect& reflect_)
		{
			if(!type_manager::_is_type_existed(name))
			{
				(*type_manager::__get_map_class_name())[name] = &reflect_;
			}
		}
		*/
		const type_info& type_info::get_type(const std::string& uuid) STD_THROW
		{
			auto it = type_manager::__get_map_uuid_rtype()->find(uuid);
			if(it!=type_manager::__get_map_uuid_rtype()->end())
			{
				if(it->second._type_info!=nullptr)
				{
					return *it->second._type_info;
				}
			}
			StdThrow(std::runtime_error, "uuid not found (%s)", uuid.c_str());
		}

		const type_info& type_info::typeof(const std::string& name) STD_THROW
		{
			auto it = type_manager::__get_map_name_uuid()->find(name);
			if(it!=type_manager::__get_map_name_uuid()->end())
			{
				return get_type(it->second);
			}
			StdThrow(std::runtime_error, "name not found (%s)", name.c_str());
		}
	}
}