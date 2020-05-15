/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "rtype.h"

namespace gce
{
	namespace reflection
	{
		class type_info;
	}
}

template<typename T>
struct type_parser_base_impl
{
	static gce::reflection::type_info& __parse_type(gce::reflection::type_info& _ti);
	static gce::reflection::type_info& __parse_and_register_type(gce::reflection::type_info& _ti, const std::string& _name);
};

template<typename T>
struct type_parser_base
{
	static gce::reflection::type_info& __parse_type(gce::reflection::type_info& _ti);
	static gce::reflection::type_info& parse_type(gce::reflection::type_info& _ti, const std::string& _name="");
};

template<>
struct type_parser_base<void>
{
	static gce::reflection::type_info& __parse_type(gce::reflection::type_info& _ti)
	{
		return type_parser_base_impl<void>::__parse_type(_ti);
	}

	static gce::reflection::type_info& parse_type(gce::reflection::type_info& _ti, const std::string& _name="")
	{
		return type_parser_base_impl<void>::__parse_and_register_type(_ti, _name);
	}
};

template<typename T, uint Size=0> struct type_parser : public type_parser_base<T>
{
	static gce::reflection::type_info& parse_type();
};

template<typename T, uint Size> struct type_parser<T*, Size> : public type_parser_base<T>
{
	static gce::reflection::type_info& parse_type();
};
template<typename T, uint Size> struct type_parser<T&, Size> : public type_parser_base<T>
{
	static gce::reflection::type_info& parse_type();
};
template<typename T, uint Size> struct type_parser<const T*, Size> : public type_parser_base<T>
{
	static gce::reflection::type_info& parse_type();
};
template<typename T, uint Size> struct type_parser<const T&, Size> : public type_parser_base<T>
{
	static gce::reflection::type_info& parse_type();
};
template<typename T, uint Size> struct type_parser<const T* const, Size> : public type_parser_base<T>
{
	static gce::reflection::type_info& parse_type();
};

namespace gce
{
	namespace reflection
	{
		///upgrading for stardard ::type_info
		class DSL_EXPORT type_info : public gce::Inception<type_info, true, true>
		{
		public:
			static void _incept();
			static void _dispose();

			type_info()
				: rtype_(nullptr)
			{
			}

		public:
			template<typename T, uint Size>
			static const type_info& _typeof()
			{
				return type_parser<T, Size>::parse_type();
			}

			template<typename T>
			static const type_info& typeof() STD_THROW
			{
				return _typeof<T, 0>();
			}

			static const type_info& get_type(const std::string& uuid) STD_THROW;
			static const type_info& typeof(const std::string& name) STD_THROW;

			inline const rtype& get_type() const
			{
				return *rtype_;
			}

		private:
			static rtype* __register_type(const std::string& name, std::string* ptr_uuid);

		public:
			template<typename T>
			static void __register_type(data_type type)
			{
				static type_info _ti;

				std::string name = reflect::get_type_name(_typeid(typeid(T)).name());
				std::string uuid;
				_ti.rtype_ = __register_type(name, &uuid);
				_parse_template<T>::parse_modifier(_ti.rtype_);

				_ti.rtype_->_type_info = &_ti;
				_ti.rtype_->_uuid = uuid;
				if(_ti.rtype_->_name.empty())
				{
					_ti.rtype_->_name = name;
				}

				type_parser<T>::__parse_type(_ti);
				_ti.rtype_->_data_type = type;
			}

		public:
			rtype* rtype_;
			friend class reflect;
			friend class type_manager;
		};
	}
}

#include "macro/macro_base.h"
#include "type_manager.h"

template<typename T>
gce::reflection::type_info& type_parser_base_impl<T>::__parse_type(gce::reflection::type_info& _ti)
{
	if(nullptr==_ti.rtype_->_ptr_rtti)
	{
		_ti.rtype_->_ptr_rtti = (::type_info*)&typeid(T);
		_ti.rtype_->_raw_name = _ti.rtype_->_ptr_rtti->name();
	}
	return _ti;
}

template<typename T>
gce::reflection::type_info& type_parser_base_impl<T>::__parse_and_register_type(gce::reflection::type_info& _ti, const std::string& _name)
{
	std::string name = _name;
	if(name.empty())
	{
		name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	}
	_ti.rtype_ = gce::reflection::type_manager::_get_type_by_name(name);

	if(nullptr==_ti.rtype_ || nullptr==_ti.rtype_->_ptr_rtti)
	{
		std::string uuid;
		_ti.rtype_ = gce::reflection::type_manager::_register_type(name, &uuid);
		_ti.rtype_->_type_info = &_ti;
		_ti.rtype_->_uuid = uuid;
		type_parser<T>::__parse_type(_ti);
		_ti.rtype_->_name = name;

		gce::reflection::meta::parse_type<T>(*_ti.rtype_);
		gce::ref_parser<T>::parse(*_ti.rtype_);

		if(gce::reflection::type_invalid==_ti.rtype_->_data_type)
		{
			StdThrow(std::runtime_error, "type not supported (%s)", _typeid(typeid(T)).name());
		}
	}
	else if(0==_ti.rtype_->_total_size)
	{
		_ti.rtype_->_total_size = _ti.rtype_->_size;
	}
	return _ti;
}

template<typename T>
gce::reflection::type_info& type_parser_base<T>::__parse_type(gce::reflection::type_info& _ti)
{
	type_parser_base_impl<T>::__parse_type(_ti);

	if(!_ti.rtype_->_is_array)
	{
		_ti.rtype_->_size = sizeof(T);
		_ti.rtype_->_total_size = _ti.rtype_->_size;
	}
	return _ti;
}

template<typename T>
gce::reflection::type_info& type_parser_base<T>::parse_type(gce::reflection::type_info& _ti, const std::string& _name)
{
	return type_parser_base_impl<T>::__parse_and_register_type(_ti, _name);
}

template<typename T, uint Size>
gce::reflection::type_info& type_parser<T, Size>::parse_type()
{
	static gce::reflection::type_info _ti;
	std::string name;
	if(Size>0)
	{
		char buf[32];
		::sprintf(buf, "%u", Size);
		std::string raw_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
		name = raw_name+" [";
		name += buf;
		name += "]";
	}
	return type_parser_base<T>::parse_type(_ti, name);
}

template<typename T, uint Size>
gce::reflection::type_info& type_parser<T*, Size>::parse_type()
{
	static gce::reflection::type_info _ti;
	std::string raw_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	std::string name;
	if(name.empty())
	{
		name = raw_name+"*";
	}
	return type_parser_base<T*>::parse_type(_ti, name);
}

template<typename T, uint Size>
gce::reflection::type_info& type_parser<T&, Size>::parse_type()
{
	static gce::reflection::type_info _ti;
	std::string raw_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	std::string name;
	if(name.empty())
	{
		name = raw_name+"&";
	}
	return type_parser_base<T&>::parse_type(_ti, name);
}

template<typename T, uint Size>
gce::reflection::type_info& type_parser<const T*, Size>::parse_type()
{
	static gce::reflection::type_info _ti;
	std::string raw_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	std::string name;
	if(name.empty())
	{
		name = std::string("const ")+raw_name+"*";
	}
	return type_parser_base<const T*>::parse_type(_ti, name);
}

template<typename T, uint Size>
gce::reflection::type_info& type_parser<const T&, Size>::parse_type()
{
	static gce::reflection::type_info _ti;
	std::string raw_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	std::string name;
	if(name.empty())
	{
		name = std::string("const ")+raw_name+"&";
	}
	return type_parser_base<const T&>::parse_type(_ti, name);
}

template<typename T, uint Size>
gce::reflection::type_info& type_parser<const T* const, Size>::parse_type()
{
	static gce::reflection::type_info _ti;
	std::string raw_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	std::string name;
	if(name.empty())
	{
		name = std::string("const ")+raw_name+"* const";
	}
	return type_parser_base<const T* const>::parse_type(_ti, name);
}