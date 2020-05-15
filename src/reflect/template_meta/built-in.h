/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "meta_type.h"

#ifndef COMMA
#define COMMA ,
#endif

extern bool DSL_EXPORT register_rtype(const std::string& name, gce::reflection::rdata*& _rdata);

///for template meta parsers

template<typename T>
struct parse_concrete_type
{
	static void parse(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_size = sizeof(T);
		if(!ptr_data->_is_array)
		{
			ptr_data->_total_size = ptr_data->_size;
		}
		ptr_data->_ptr_rtti = (::type_info*)&typeid(T);
		ptr_data->_raw_name = _typeid(typeid(T)).name();
	}
};

template<>
struct parse_concrete_type<void>
{
	static void parse(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_ptr_rtti = (::type_info*)&typeid(void);
		ptr_data->_raw_name = _typeid(typeid(void)).name();
	}
};

///parse template class
template<typename T>
struct _parse_template
{
	static void parse_template_type(gce::reflection::rdata* ptr_data);

	static void parse_modifier(gce::reflection::modifier* ptr_modifier)
	{
		if(check_reference<T>::result)
		{
			ptr_modifier->_arg_type = gce::reflection::arg_ref;
		}
		else if(check_pointer<T>::result)
		{
			ptr_modifier->_arg_type = gce::reflection::arg_ptr;
		}
		else
		{
			ptr_modifier->_arg_type = gce::reflection::arg_object;
		}
		ptr_modifier->_is_const = check_const<T>::result;
	}
};

///implementation for parsing reflectable
template<typename A, typename B>
struct template_type_reflectable
{
	enum { type = gce::reflection::type_invalid };

	static void parse_template_type(gce::reflection::rdata* ptr_data)
	{
		_parse_template<A>::parse_modifier(ptr_data);
		if(gce::reflection::reflect::is_reflectable<B>())
		{
			ptr_data->_data_type = gce::reflection::type_object_reflectable;
		}
		else if(gce::reflection::reflect::is_copyable<B>())
		{
			ptr_data->_data_type = gce::reflection::type_object_copyable;
		}
		else
		{
			ptr_data->_data_type = gce::reflection::type_object_nonreflectable;
		}

		parse_concrete_type<B>::parse(ptr_data);
	}
};

template<>
struct template_type_reflectable<void, void>
{
	enum { type = gce::reflection::type_invalid };

	static void parse_template_type(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_data_type = gce::reflection::type_void;
		ptr_data->_ptr_rtti = (::type_info*)&typeid(void);
		ptr_data->_name = "void";
		ptr_data->_raw_name = _typeid(typeid(void)).name();
		ptr_data->_ptr_rtti = (::type_info*)&typeid(void);
	}
};

///parse reflectable
template<typename A>
struct template_type : public template_type_reflectable<A, A>{};
template<typename A>
struct template_type<A*> : public template_type_reflectable<A*, A>{};
template<typename A>
struct template_type<A* const&> : public template_type_reflectable<A* const&, A>{};
template<typename A>
struct template_type<A&> : public template_type_reflectable<A&, A>{};
template<typename A>
struct template_type<A*&> : public template_type_reflectable<A*&, A>{};
template<typename A>
struct template_type<const A*> : public template_type_reflectable<const A*, A>{};
template<typename A>
struct template_type<const A> : public template_type_reflectable<const A, A>{};
template<typename A>
struct template_type<const A&> : public template_type_reflectable<const A&, A>{};
template<typename A>
struct template_type<const A* const> : public template_type_reflectable<const A* const, A>{};

///parse array
template<typename A, typename B>
struct template_type_array
{
	static void parse_template_type(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_is_array = true;
		_parse_template<B>::parse_modifier(ptr_data);
		template_type<B>::parse_template_type(ptr_data);
		ptr_data->_raw_name = _typeid(typeid(A)).name();

		_parse_template<B>::parse_template_type(ptr_data);
	}
};

template<uint SIZE, typename T>
struct template_type<T[SIZE]> : public template_type_array<T[SIZE], T>{};
template<uint SIZE, typename T>
struct template_type<T*[SIZE]> : public template_type_array<T*[SIZE], T>{};
template<uint SIZE, typename T>
struct template_type<const T*[SIZE]> : public template_type_array<const T*[SIZE], T>{};
template<uint SIZE, typename T>
struct template_type<const T[SIZE]> : public template_type_array<const T[SIZE], T>{};

template<typename T, char TYPE>
struct template_type_base
{
	enum { type = TYPE };

	static void parse_template_type(gce::reflection::rdata* ptr_data)
	{
		ptr_data->_data_type = (gce::reflection::data_type)type;
		parse_concrete_type<T>::parse(ptr_data);
	}
};

///template traits
#define declare_template_class(T, Type)\
template<> struct template_type<T> : public template_type_base<T, Type>{};\
template<> struct template_type<T*> : public template_type_base<T, Type>{};\
template<> struct template_type<T&> : public template_type_base<T, Type>{};\
template<> struct template_type<T*&> : public template_type_base<T, Type>{};\
template<> struct template_type<const T*> : public template_type_base<T, Type>{};\
template<> struct template_type<const T> : public template_type_base<T, Type>{};\
template<> struct template_type<const T&> : public template_type_base<T, Type>{};\
template<> struct template_type<T* const&> : public template_type_base<T, Type>{};\
template<> struct template_type<const T* const> : public template_type_base<T, Type>{};

template<> struct template_type<void> : public template_type_base<void, gce::reflection::type_void>{};
template<> struct template_type<void*> : public template_type_base<void, gce::reflection::type_void>{};
template<> struct template_type<void* const&> : public template_type_base<void, gce::reflection::type_void>{};
template<> struct template_type<void**> : public template_type_base<void*, gce::reflection::type_void>{};
template<> struct template_type<void*&> : public template_type_base<void, gce::reflection::type_void>{};
template<> struct template_type<const void*> : public template_type_base<void, gce::reflection::type_void>{};
template<> struct template_type<const void* const> : public template_type_base<void, gce::reflection::type_void>{};

///base type
declare_template_class(bool, gce::reflection::type_bool);
declare_template_class(int16, gce::reflection::type_int16);
declare_template_class(uint16, gce::reflection::type_uint16);
declare_template_class(int32, gce::reflection::type_int32);
declare_template_class(uint32, gce::reflection::type_uint32);
declare_template_class(int64, gce::reflection::type_int64);
declare_template_class(uint64, gce::reflection::type_uint64);
declare_template_class(long, gce::reflection::type_long);
declare_template_class(ulong, gce::reflection::type_ulong);
declare_template_class(float, gce::reflection::type_float);
declare_template_class(double, gce::reflection::type_double);
declare_template_class(char, gce::reflection::type_char);
declare_template_class(uchar, gce::reflection::type_uchar);
declare_template_class(wchar, gce::reflection::type_wchar);

declare_template_class(std::string, gce::reflection::type_stl_string);
declare_template_class(std::wstring, gce::reflection::type_stl_wstring);

template<typename T>
static void __parse_template_type(gce::reflection::rdata& ret_type)
{
	_parse_template<T>::parse_modifier(&ret_type);
	template_type<T>::parse_template_type(&ret_type);
	gce::reflection::rdata::parse_name(ret_type._name, ret_type._namespaces);
}

namespace gce
{
	template<typename T>
	struct ref_parser
	{
		static void parse(gce::reflection::rtype& _rtype)
		{
		}
	};

	namespace reflection
	{
		class rdata;

		template<typename T>
		bool argument_type::_inexact_equal() const
		{
			rdata _data;
			_parse_template<T>::parse_modifier(&_data);
			template_type<T>::parse_template_type(&_data);

			if(!_inexact_equal(_data))
			{
				return false;
			}
			if(_arg_type==_data._arg_type)
			{
				return true;
			}

			if(
				((_arg_type==arg_object||_arg_type==arg_ref) && _data._arg_type==arg_ptr)
				|| ((_data._arg_type==arg_object||_data._arg_type==arg_ref) && _arg_type==arg_ptr)
				)
			{
				return false;
			}
			return true;
		}
	}
}