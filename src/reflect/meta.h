/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "template_meta/modifier.h"
#include "template_meta/template_1.h"
#include "template_meta/template_2.h"

namespace gce
{
	namespace reflection
	{
		class DSL_EXPORT meta
		{
		public:
			///parse type
			template<class T>
			static void parse_type(rdata& ret)
			{
				__parse_template_type<T>(ret);
			}

			static data_type name_to_type(const std::string& name);
			static std::string type_to_name(data_type dt);

			static void trim_left(std::string& str);
			static void trim_right(std::string& str);
			static void replace(std::string& str, const char* ptr_old, const char* ptr_new);
			static void split(const std::string& s, const std::string& delim, std::vector<std::string>& ret);
		};
	}
}