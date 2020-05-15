/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "method_parser.h"

#define macor_parse_params_1\
	PARSE_PARAM(1);

#define declare_bind_1(Name, Const)\
	declare_bind(macro_typename1,\
				Name,\
				Const,\
				COMMA,\
				macro_args1,\
				macro_void_args1,\
				macro_params1\
				)

///bind class and parse member method with one argument
namespace gce
{
	namespace reflection
	{
#ifdef _WINDOWS
		declare_bind_1(bind_1, );
		declare_bind_1(bind_1_const, const);
		declare_bind_static(macro_typename1,
				bind_1_static,
				COMMA,
				macro_args1,
				macro_void_args1,
				macro_params1
				)
#else
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename1,
				bind_1,
				,
				COMMA,
				macro_args1,
				macro_void_args1,
				macro_params1
				);
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename1,
				bind_1_const,
				const,
				COMMA,
				macro_args1,
				macro_void_args1,
				macro_params1
				);
		declare_bind_base(,,
				macro_typename1,
				bind_1_static,
				,
				COMMA,
				macro_args1,
				macro_void_args1,
				macro_params1
				);
#endif

		template<typename R, typename T, typename A1>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1))
		{
			macro_init_method(T, false);
			macor_parse_params_1;
#ifdef _WINDOWS
			parse_bind(bind_1, , COMMA, macro_args1);
#else
			parse_bind_base(T, T::, bind_1, , COMMA, macro_args1);
#endif
		}
		template<typename R, typename T, typename A1>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1) const)
		{
			macro_init_method(T, false);
			macor_parse_params_1;
#ifdef _WINDOWS
			parse_bind(bind_1_const, const, COMMA, macro_args1);
#else
			parse_bind_base(T, T::, bind_1_const, const, COMMA, macro_args1);
#endif
		}
		template<typename R, typename A1>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)(A1))
		{
			macro_init_method(placeholder, true);
			macor_parse_params_1;
#ifdef _WINDOWS
			parse_bind_static(bind_1_static, COMMA, macro_args1);
#else
			parse_bind_base(placeholder, , bind_1_static, , COMMA, macro_args1);
#endif
		}
	}
}