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

#define macor_parse_params_5\
	PARSE_PARAM(1);\
	PARSE_PARAM(2);\
	PARSE_PARAM(3);\
	PARSE_PARAM(4);\
	PARSE_PARAM(5);

#define declare_bind_5(Name, Const)\
		declare_bind(macro_typename5,\
				Name,\
				Const,\
				COMMA,\
				macro_args5,\
				macro_void_args5,\
				macro_params5\
				)

///bind class and parse member method with 5 arguments
namespace gce
{
	namespace reflection
	{
#ifdef _WINDOWS
		declare_bind_5(bind_5, );
		declare_bind_5(bind_5_const, const);
		declare_bind_static(macro_typename5,
				bind_5_static,
				COMMA,
				macro_args5,
				macro_void_args5,
				macro_params5
				)
#else
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename5,
				bind_5,
				,
				COMMA,
				macro_args5,
				macro_void_args5,
				macro_params5
				);
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename5,
				bind_5_const,
				const,
				COMMA,
				macro_args5,
				macro_void_args5,
				macro_params5
				);
		declare_bind_base(,,
				macro_typename5,
				bind_5_static,
				,
				COMMA,
				macro_args5,
				macro_void_args5,
				macro_params5
				);
#endif

		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3, A4, A5))
		{
			macro_init_method(T, false);
			macor_parse_params_5;
#ifdef _WINDOWS
			parse_bind(bind_5, , COMMA, macro_args5);
#else
			parse_bind_base(T, T::, bind_5, , COMMA, macro_args5);
#endif
		}
		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3, A4, A5) const)
		{
			macro_init_method(T, false);
			macor_parse_params_5;
#ifdef _WINDOWS
			parse_bind(bind_5_const, const, COMMA, macro_args5);
#else
			parse_bind_base(T, T::, bind_5_const, const, COMMA, macro_args5);
#endif
		}
		template <typename R,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)(A1, A2, A3, A4, A5))
		{
			macro_init_method(placeholder, true);
			macor_parse_params_5;
#ifdef _WINDOWS
			parse_bind_static(bind_5_static, COMMA, macro_args5);
#else
			parse_bind_base(placeholder, , bind_5_static, , COMMA, macro_args5);
#endif
		}
	}
}