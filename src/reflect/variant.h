/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "template_meta/meta_type.h"

namespace gce
{
	namespace reflection
	{
		///for argument wrapping
		struct DSL_EXPORT variant
		{
			union
			{
				void*	val_void_ptr;
				bool	val_bool;
				int16	val_int16;
				int32	val_int32;
				int64	val_int64;
				int		val_int;
				long	val_long;
				float	val_float;
				double	val_double;
				char	val_char;
				wchar	val_wchar;
				char	val_str[sizeof(std::string)];
				char	val_wstr[sizeof(std::wstring)];
			};

			void* get_val_address() const
			{
				return (void*)&val_wstr;
			}

			variant()
				: _type(type_invalid)
				, _sub_type(type_invalid)
				, _param(nullptr)
			{
			}

			~variant()
			{
				set_type(type_invalid);
			}

			variant(const variant& other)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = other;
			}

			variant(const void* ptr)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = ptr;
			}

			variant(const char* str)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = std::string(str);
			}

			variant(const wchar* wstr)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = std::wstring(wstr);
			}

			variant(const std::string& str)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = str;
			}

			variant(const std::wstring& wstr)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = wstr;
			}

			variant(bool val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(int16 val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(int32 val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(int64 val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(long val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(float val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(double val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(char val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant(wchar val)
				: _type(type_invalid)
				, _param(nullptr)
			{
				*this = val;
			}

			variant& operator=(const variant& other);

			variant& operator=(const void* ptr)
			{
				val_void_ptr = (void*)ptr;
				return *this;
			}

			variant& operator=(const char* str)
			{
				*this = std::string(str);
				return *this;
			}

			variant& operator=(const wchar* wstr)
			{
				*this = std::wstring(wstr);
				return *this;
			}

			inline std::string* to_string_pointer() const
			{
				if(_sub_type==type_stl_string)
				{
					return __to_type<std::string*>((char*)val_void_ptr);
				}
				return __to_type<std::string*>(val_str);
			}

			inline std::wstring* to_wstring_pointer() const
			{
				if(_sub_type==type_stl_wstring)
				{
					return __to_type<std::wstring*>((char*)val_void_ptr);
				}
				return __to_type<std::wstring*>(val_wstr);
			}

			variant& operator=(const std::string& str)
			{
				set_type(type_stl_string);
				*to_string_pointer() = str;
				return *this;
			}

			variant& operator=(const std::wstring& wstr)
			{
				set_type(type_stl_wstring);
				*to_wstring_pointer() = wstr;
				return *this;
			}

			variant& operator=(bool val)
			{
				set_type(type_bool);
				val_bool = val;
				return *this;
			}

			variant& operator=(int16 val)
			{
				set_type(type_int16);
				val_int16 = val;
				return *this;
			}

			variant& operator=(int32 val)
			{
				set_type(type_int32);
				val_int32 = val;
				return *this;
			}

			variant& operator=(int64 val)
			{
				set_type(type_int64);
				val_int64 = val;
				return *this;
			}

			variant& operator=(long val)
			{
				set_type(type_long);
				val_long = val;
				return *this;
			}

			variant& operator=(float val)
			{
				set_type(type_float);
				val_float = val;
				return *this;
			}

			variant& operator=(double val)
			{
				set_type(type_double);
				val_double = val;
				return *this;
			}

			variant& operator=(char val)
			{
				set_type(type_char);
				val_char = val;
				return *this;
			}

			variant& operator=(wchar val)
			{
				set_type(type_wchar);
				val_wchar = val;
				return *this;
			}

			operator std::string&() const
			{
				return *to_string_pointer();
			}

			operator std::wstring&() const
			{
				return *to_wstring_pointer();
			}

			operator int16() const
			{
				return val_int16;
			}

			operator int32() const
			{
				return val_int32;
			}

			operator int64() const
			{
				return val_int64;
			}

			operator long() const
			{
				return val_long;
			}

			operator float() const
			{
				return val_float;
			}

			operator double() const
			{
				return val_double;
			}

			operator char() const
			{
				return val_char;
			}

			operator wchar() const
			{
				return val_wchar;
			}

			void set_type(data_type type);

			inline data_type get_type() const
			{
				return _type;
			}

			inline void set_sub_type(data_type sub_type)
			{
				_sub_type = sub_type;
			}

			std::string to_string() const STD_THROW;
			void make_val(data_type type, const std::string& str);

		protected:
			data_type _type;
			data_type _sub_type;

		public:
			void* _param;
		};

		static variant _default_variant;
	}
}