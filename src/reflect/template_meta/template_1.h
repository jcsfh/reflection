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

///for std template class with one template argument

template<typename A, typename B, typename T, char TYPE>
struct template_1
{
	enum { type = TYPE };

	static void parse_template_type(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_data_type = (gce::reflection::data_type)type;
		parse_concrete_type<B>::parse(ptr_data);

		_parse_template<A>::parse_modifier(ptr_data);
		_parse_template<T>::parse_template_type(ptr_data);
	}
};

#define declare_template_name_1(A, B)

#define declare_template_1(A, Type)\
template<typename T>\
struct template_type<A<T> > : public template_1<A<T>, A<T>, T, Type>{};\
declare_template_name_1(A, A<T>);\
template<typename T>\
struct template_type<A<T>*> : public template_1<A<T>*, A<T>, T, Type>{};\
declare_template_name_1(A, A<T>*);\
template<typename T>\
struct template_type<A<T>&> : public template_1<A<T>&, A<T>, T, Type>{};\
declare_template_name_1(A, A<T>&);\
template<typename T>\
struct template_type<A<T>*&> : public template_1<A<T>*&, A<T>, T, Type>{};\
declare_template_name_1(A, A<T>*&);\
template<typename T>\
struct template_type<const A<T>*> : public template_1<const A<T>*, A<T>, T, Type>{};\
declare_template_name_1(A, const A<T>*);\
template<typename T>\
struct template_type<const A<T> > : public template_1<const A<T>, A<T>, T, Type>{};\
declare_template_name_1(A, const A<T>);\
template<typename T>\
struct template_type<const A<T>&> : public template_1<const A<T>&, A<T>, T, Type>{};\
declare_template_name_1(A, const A<T>&);\
template<typename T>\
struct template_type<const A<T>* const> : public template_1<const A<T>* const, A<T>, T, Type>{};\
declare_template_name_1(A, const A<T>* const);\

declare_template_1(std::shared_ptr, gce::reflection::type_stl_sharedptr);
declare_template_1(std::vector, gce::reflection::type_stl_vector);
declare_template_1(std::list, gce::reflection::type_stl_list);
declare_template_1(std::set, gce::reflection::type_stl_set);
declare_template_1(std::multiset, gce::reflection::type_stl_multiset);
declare_template_1(std::queue, gce::reflection::type_stl_queue);
declare_template_1(std::deque, gce::reflection::type_stl_deque);
declare_template_1(std::stack, gce::reflection::type_stl_stack);
declare_template_1(std::priority_queue, gce::reflection::type_stl_priority_queue);