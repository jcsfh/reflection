/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "built-in.h"

///for std template class with two template arguments

template<typename A, typename B, typename T1, typename T2, char TYPE>
struct template_2
{
	enum { type = TYPE };

	static void parse_template_type(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_data_type = (gce::reflection::data_type)type;
		parse_concrete_type<B>::parse(ptr_data);

		_parse_template<A>::parse_modifier(ptr_data);

		_parse_template<T1>::parse_template_type(ptr_data);
		_parse_template<T2>::parse_template_type(ptr_data);
	}
};

#define declare_template_name_2(A, B)

#define declare_template_2(A, Type)\
template<typename T1, typename T2>\
struct template_type<A<T1, T2> > : public template_2<A<T1, T2>, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, A<T1 COMMA T2>);\
template<typename T1, typename T2>\
struct template_type<A<T1, T2>*> : public template_2<A<T1, T2>*, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, A<T1 COMMA T2>*);\
template<typename T1, typename T2>\
struct template_type<A<T1, T2>&> : public template_2<A<T1, T2>&, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, A<T1 COMMA T2>&);\
template<typename T1, typename T2>\
struct template_type<A<T1, T2>*&> : public template_2<A<T1, T2>*&, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, A<T1 COMMA T2>*&);\
template<typename T1, typename T2>\
struct template_type<const A<T1, T2>*> : public template_2<const A<T1, T2>*, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, const A<T1 COMMA T2>*);\
template<typename T1, typename T2>\
struct template_type<const A<T1, T2> > : public template_2<const A<T1, T2>, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, const A<T1 COMMA T2>);\
template<typename T1, typename T2>\
struct template_type<const A<T1, T2>&> : public template_2<const A<T1, T2>&, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, const A<T1 COMMA T2>&);\
template<typename T1, typename T2>\
struct template_type<const A<T1, T2>* const> : public template_2<const A<T1, T2>* const, A<T1, T2>, T1, T2, Type>{};\
declare_template_name_2(A, const A<T1 COMMA T2>* const);\



declare_template_2(std::map, gce::reflection::type_stl_map);
declare_template_2(std::multimap, gce::reflection::type_stl_multimap);

//#ifdef USE_BOOST
//declare_template_2(boost::unorderedmap, T1, T2, gce::reflection::type_boost_unorderedmap);
//declare_template_2(boost::unorderedmultimap, T1, T2, gce::reflection::type_boost_unorderedmultimap);
//#endif
