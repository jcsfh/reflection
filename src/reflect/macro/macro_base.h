/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

///macros for substitute
#define macro_typename1 typename A1
#define macro_args1 A1

#define macro_typename2 typename A1 COMMA typename A2
#define macro_args2 A1 COMMA A2

#define macro_typename3 typename A1 COMMA typename A2 COMMA typename A3
#define macro_args3 A1 COMMA A2 COMMA A3

#define macro_typename4 typename A1 COMMA typename A2 COMMA typename A3 COMMA typename A4
#define macro_args4 A1 COMMA A2 COMMA A3 COMMA A4

#define macro_typename5 typename A1 COMMA typename A2 COMMA typename A3 COMMA typename A4 COMMA typename A5
#define macro_args5 A1 COMMA A2 COMMA A3 COMMA A4 COMMA A5

#define macro_typename6 typename A1 COMMA typename A2 COMMA typename A3 COMMA typename A4 COMMA typename A5 COMMA typename A6
#define macro_args6 A1 COMMA A2 COMMA A3 COMMA A4 COMMA A5 COMMA A6

#define macro_typename7 typename A1 COMMA typename A2 COMMA typename A3 COMMA typename A4 COMMA typename A5 COMMA typename A6 COMMA typename A7
#define macro_args7 A1 COMMA A2 COMMA A3 COMMA A4 COMMA A5 COMMA A6 COMMA A7

#define macro_typename8 typename A1 COMMA typename A2 COMMA typename A3 COMMA typename A4 COMMA typename A5 COMMA typename A6 COMMA typename A7 COMMA typename A8
#define macro_args8 A1 COMMA A2 COMMA A3 COMMA A4 COMMA A5 COMMA A6 COMMA A7 COMMA A8

#define macro_typename9 typename A1 COMMA typename A2 COMMA typename A3 COMMA typename A4 COMMA typename A5 COMMA typename A6 COMMA typename A7 COMMA typename A8 COMMA typename A9
#define macro_args9 A1 COMMA A2 COMMA A3 COMMA A4 COMMA A5 COMMA A6 COMMA A7 COMMA A8 COMMA A9



#define macro_void_args1 void* a1
#define macro_params1 (A1)*(_arg<A1>*)a1
#define macro_const_params1 const A1& a1
#define macro_in_args1 a1

#define macro_void_args2 void* a1 COMMA void* a2
#define macro_params2 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2
#define macro_const_params2 const A1& a1 COMMA const A2& a2
#define macro_in_args2 a1 COMMA a2

#define macro_void_args3 void* a1 COMMA void* a2 COMMA void* a3
#define macro_params3 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3
#define macro_const_params3 const A1& a1 COMMA const A2& a2 COMMA const A3& a3
#define macro_in_args3 a1 COMMA a2 COMMA a3

#define macro_void_args4 void* a1 COMMA void* a2 COMMA void* a3 COMMA void* a4
#define macro_params4 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3 COMMA (A4)*(_arg<A4>*)a4
#define macro_const_params4 const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4
#define macro_in_args4 a1 COMMA a2 COMMA a3 COMMA a4

#define macro_void_args5 void* a1 COMMA void* a2 COMMA void* a3 COMMA void* a4 COMMA void* a5
#define macro_params5 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3 COMMA (A4)*(_arg<A4>*)a4 COMMA (A5)*(_arg<A5>*)a5
#define macro_const_params5 const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5
#define macro_in_args5 a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5

#define macro_void_args6 void* a1 COMMA void* a2 COMMA void* a3 COMMA void* a4 COMMA void* a5 COMMA void* a6
#define macro_params6 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3 COMMA (A4)*(_arg<A4>*)a4 COMMA (A5)*(_arg<A5>*)a5 COMMA (A6)*(_arg<A6>*)a6
#define macro_const_params6 const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6
#define macro_in_args6 a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6

#define macro_void_args7 void* a1 COMMA void* a2 COMMA void* a3 COMMA void* a4 COMMA void* a5 COMMA void* a6 COMMA void* a7
#define macro_params7 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3 COMMA (A4)*(_arg<A4>*)a4 COMMA (A5)*(_arg<A5>*)a5 COMMA (A6)*(_arg<A6>*)a6 COMMA (A7)*(_arg<A7>*)a7
#define macro_const_params7 const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6 COMMA const A7& a7
#define macro_in_args7 a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6 COMMA a7

#define macro_void_args8 void* a1 COMMA void* a2 COMMA void* a3 COMMA void* a4 COMMA void* a5 COMMA void* a6 COMMA void* a7 COMMA void* a8
#define macro_params8 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3 COMMA (A4)*(_arg<A4>*)a4 COMMA (A5)*(_arg<A5>*)a5 COMMA (A6)*(_arg<A6>*)a6 COMMA (A7)*(_arg<A7>*)a7 COMMA (A8)*(_arg<A8>*)a8
#define macro_const_params8 const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6 COMMA const A7& a7 COMMA const A8& a8
#define macro_in_args8 a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6 COMMA a7 COMMA a8

#define macro_void_args9 void* a1 COMMA void* a2 COMMA void* a3 COMMA void* a4 COMMA void* a5 COMMA void* a6 COMMA void* a7 COMMA void* a8 COMMA void* a9
#define macro_params9 (A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2 COMMA (A3)*(_arg<A3>*)a3 COMMA (A4)*(_arg<A4>*)a4 COMMA (A5)*(_arg<A5>*)a5 COMMA (A6)*(_arg<A6>*)a6 COMMA (A7)*(_arg<A7>*)a7 COMMA (A8)*(_arg<A8>*)a8 COMMA (A9)*(_arg<A9>*)a9
#define macro_const_params9 const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6 COMMA const A7& a7 COMMA const A8& a8 COMMA const A9& a9
#define macro_in_args9 a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6 COMMA a7 COMMA a8 COMMA a9

#define macro_init_method(T, Static)\
	((gce::reflection::rmethods)_type.methods()).erase(_name);\
	gce::reflection::rmethod& method = ((gce::reflection::rmethods)_type.methods())[_name];\
	method._name = _name;\
	method._is_static = Static;\
	method._is_virtual = is_virtual;\
	method._return_type_ = (gce::reflection::rtype*)&gce::reflection::type_info::typeof<R>().get_type();\
	method._owner_ = (rtype*)&gce::reflection::type_info::typeof<T>().get_type();

#define PARSE_PARAM(i)	method._params.push_back((gce::reflection::rtype*)&gce::reflection::type_info::typeof<A##i>().get_type());

#ifdef _WINDOWS
#define declare_tempate(Class)
#define declare_variable(Name, Variable) Variable
#else
#define declare_tempate(Class) typedef Class derived_type;
#define declare_variable(Name, Variable) __##Name##__LINE__
#endif