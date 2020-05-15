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

#define macor_parse_params_9\
	PARSE_PARAM(1);\
	PARSE_PARAM(2);\
	PARSE_PARAM(3);\
	PARSE_PARAM(4);\
	PARSE_PARAM(5);\
	PARSE_PARAM(6);\
	PARSE_PARAM(7);\
	PARSE_PARAM(8);\
	PARSE_PARAM(9);

#define declare_bind_9(Name, Const)\
	declare_bind(macro_typename9,\
				Name,\
				Const,\
				COMMA,\
				macro_args9,\
				macro_void_args9,\
				macro_params9\
				)

///bind class and parse member method with 9 arguments
namespace gce
{
	namespace reflection
	{
#ifdef _WINDOWS
		declare_bind_9(bind_9, );
		declare_bind_9(bind_9_const, const);
		declare_bind_static(macro_typename9,
				bind_9_static,
				COMMA,
				macro_args9,
				macro_void_args9,
				macro_params9
				)
#else
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename9,
				bind_9,
				,
				COMMA,
				macro_args9,
				macro_void_args9,
				macro_params9
				);
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename9,
				bind_9_const,
				const,
				COMMA,
				macro_args9,
				macro_void_args9,
				macro_params9
				);
		declare_bind_base(,,
				macro_typename9,
				bind_9_static,
				,
				COMMA,
				macro_args9,
				macro_void_args9,
				macro_params9
				);
#endif

		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5,
			typename A6,
			typename A7,
			typename A8,
			typename A9>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3, A4, A5, A6, A7, A8, A9))
		{
			macro_init_method(T, false);
			macor_parse_params_9;
#ifdef _WINDOWS
			parse_bind(bind_9, , COMMA, macro_args9);
#else
			parse_bind_base(T, T::, bind_9, , COMMA, macro_args9);
#endif
		}
		template <typename R, typename T,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5,
			typename A6,
			typename A7,
			typename A8,
			typename A9>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2, A3, A4, A5, A6, A7, A8, A9) const)
		{
			macro_init_method(T, false);
			macor_parse_params_9;
#ifdef _WINDOWS
			parse_bind(bind_9_const, const, COMMA, macro_args9);
#else
			parse_bind_base(T, T::, bind_9_const, const, COMMA, macro_args9);
#endif
		}
		template <typename R,
			typename A1,
			typename A2,
			typename A3,
			typename A4,
			typename A5,
			typename A6,
			typename A7,
			typename A8,
			typename A9>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)(A1, A2, A3, A4, A5, A6, A7, A8, A9))
		{
			macro_init_method(placeholder, true);
			macor_parse_params_9;
#ifdef _WINDOWS
			parse_bind_static(bind_9_static, COMMA, macro_args9);
#else
			parse_bind_base(placeholder, , bind_9_static, , COMMA, macro_args9);
#endif
		}
	}
}