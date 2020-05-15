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

#define macor_parse_params_3\
	PARSE_PARAM(1);\
	PARSE_PARAM(2);\
	PARSE_PARAM(3);

#define declare_bind_3(Name, Const)\
		declare_bind(macro_typename3,\
				Name,\
				Const,\
				COMMA,\
				macro_args3,\
				macro_void_args3,\
				macro_params3\
				)

///bind class and parse member method with 3 arguments
namespace gce
{
	namespace reflection
	{
#ifdef _WINDOWS
		declare_bind_3(bind_3, );
		declare_bind_3(bind_3_const, const);
		declare_bind_static(macro_typename3,
				bind_3_static,
				COMMA,
				macro_args3,
				macro_void_args3,
				macro_params3
				)
#else
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename3,
				bind_3,
				,
				COMMA,
				macro_args3,
				macro_void_args3,
				macro_params3
				);
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename3,
				bind_3_const,
				const,
				COMMA,
				macro_args3,
				macro_void_args3,
				macro_params3
				);
		declare_bind_base(,,
				macro_typename3,
				bind_3_static,
				,
				COMMA,
				macro_args3,
				macro_void_args3,
				macro_params3
				);
#endif

		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3))
		{
			macro_init_method(T, false);
			macor_parse_params_3;
#ifdef _WINDOWS
			parse_bind(bind_3, , COMMA, macro_args3);
#else
			parse_bind_base(T, T::, bind_3, , COMMA, macro_args3);
#endif
		}
		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3) const)
		{
			macro_init_method(T, false);
			macor_parse_params_3;
#ifdef _WINDOWS
			parse_bind(bind_3_const, const, COMMA, macro_args3);
#else
			parse_bind_base(T, T::, bind_3_const, const, COMMA, macro_args3);
#endif
		}
		template <typename R,
			typename A1,
			typename A2,
			typename A3>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)(A1, A2, A3))
		{
			macro_init_method(placeholder, true);
			macor_parse_params_3;
#ifdef _WINDOWS
			parse_bind_static(bind_3_static, COMMA, macro_args3);
#else
			parse_bind_base(placeholder, , bind_3_static, , COMMA, macro_args3);
#endif
		}
	}
}