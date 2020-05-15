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

#define macor_parse_params_7\
	PARSE_PARAM(1);\
	PARSE_PARAM(2);\
	PARSE_PARAM(3);\
	PARSE_PARAM(4);\
	PARSE_PARAM(5);\
	PARSE_PARAM(6);\
	PARSE_PARAM(7);

#define declare_bind_7(Name, Const)\
	declare_bind(macro_typename7,\
				Name,\
				Const,\
				COMMA,\
				macro_args7,\
				macro_void_args7,\
				macro_params7\
				)

///bind class and parse member method with 7 arguments
namespace gce
{
	namespace reflection
	{
#ifdef _WINDOWS
		declare_bind_7(bind_7, );
		declare_bind_7(bind_7_const, const);
		declare_bind_static(macro_typename7,
				bind_7_static,
				COMMA,
				macro_args7,
				macro_void_args7,
				macro_params7
				)
#else
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename7,
				bind_7,
				,
				COMMA,
				macro_args7,
				macro_void_args7,
				macro_params7
				);
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename7,
				bind_7_const,
				const,
				COMMA,
				macro_args7,
				macro_void_args7,
				macro_params7
				);
		declare_bind_base(,,
				macro_typename7,
				bind_7_static,
				,
				COMMA,
				macro_args7,
				macro_void_args7,
				macro_params7
				);
#endif

		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5,
			typename A6,
			typename A7>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3, A4, A5, A6, A7))
		{
			macro_init_method(T, false);
			macor_parse_params_7;
#ifdef _WINDOWS
			parse_bind(bind_7, , COMMA, macro_args7);
#else
			parse_bind_base(T, T::, bind_7, , COMMA, macro_args7);
#endif
		}
		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5,
			typename A6,
			typename A7>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3, A4, A5, A6, A7) const)
		{
			macro_init_method(T, false);
			macor_parse_params_7;
#ifdef _WINDOWS
			parse_bind(bind_7_const, const, COMMA, macro_args7);
#else
			parse_bind_base(T, T::, bind_7_const, const, COMMA, macro_args7);
#endif
		}
		template <typename R,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5,
			typename A6,
			typename A7>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)(A1, A2, A3, A4, A5, A6, A7))
		{
			macro_init_method(placeholder, true);
			macor_parse_params_7;
#ifdef _WINDOWS
			parse_bind_static(bind_7_static, COMMA, macro_args7);
#else
			parse_bind_base(placeholder, , bind_7_static, , COMMA, macro_args7);
#endif
		}
	}
}