/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#include "../../runtime/runtime.h"

#include "../variant.h"

namespace gce
{
	namespace reflection
	{
		variant& variant::operator=(const variant& other)
		{
			if(this!=&other)
			{
				switch(other.get_type())
				{
				case type_stl_string:
					*this = (std::string&)other;
					break;
				case type_stl_wstring:
					*this = (std::wstring&)other;
					break;
				default:
					::memcpy(this, &other, sizeof(*this));
					break;
				}
			}
			return *this;
		}

		void variant::set_type(data_type type)
		{
			if(_type==type)
			{
				return;
			}

			switch(_type)
			{
			case type_stl_string:
				__call_destructor((std::string*)val_str);
				break;
			case type_stl_wstring:
				__call_destructor((std::wstring*)val_wstr);
				break;
			default:
				break;
			}

			_type = type;
			switch(_type)
			{
			case type_stl_string:
				__call_constructor((std::string*)val_str);
				break;
			case type_stl_wstring:
				__call_constructor((std::wstring*)val_wstr);
				break;
			default:
				break;
			}
		}

		int64 string_to_int(const std::string& str)
		{
			if(str.empty())
			{
				return 0;
			}
#ifdef _WINDOWS
			return _atoi64(str.c_str());
#else
			return strtol(str.c_str(), nullptr, 10);
#endif
		}
		char string_to_char(const std::string& str)
		{
			if(str.empty())
			{
				return 0;
			}
			return str[0];
		}
		double string_to_double(const std::string& str)
		{
			if(str.empty())
			{
				return 0;
			}
			return atof(str.c_str());
		}

		static const uint max_size = 64;
		std::string int_to_string(int64 val)
		{
			std::string str;
			char buf[max_size];
			::snprintf(buf, max_size, "%lld", val);
			str = buf;
			return str;
		}
		std::string int_to_string(uint64 val)
		{
			std::string str;
			char buf[max_size];
			::snprintf(buf, max_size, "%llu", val);
			str = buf;
			return str;
		}
		std::string char_to_string(char val)
		{
			std::string str;
			char buf[max_size];
			::snprintf(buf, max_size, "%c", val);
			str = buf;
			return str;
		}
		std::string double_to_string(double val)
		{
			std::string str;
			char buf[max_size];
			::snprintf(buf, max_size, "%f", val);
			str = buf;
			return str;
		}

		std::string variant::to_string() const STD_THROW
		{
			if(get_type()==type_void)
			{
				if(_sub_type==type_stl_string)
				{
					return *__to_type<std::string*>((char*)val_void_ptr);
				}
				return "";
			}
			else if(get_type()!=type_invalid)
			{
				std::string _temp;
				if(get_type()<type_base_end)
				{
					switch(get_type())
					{
					case type_bool:
						{
							if(val_bool)
							{
								_temp = "true";
							}
							else
							{
								_temp = "false";
							}
						}
						break;
					case type_int:
						_temp = int_to_string((int64)val_int);
						break;
					case type_uint:
						_temp = int_to_string((uint64)val_int);
						break;
					case type_int16:
						_temp = int_to_string((int64)val_int16);
						break;
					case type_uint16:
						_temp = int_to_string((uint64)val_int16);
						break;
					case type_int32:
						_temp = int_to_string((int64)val_int32);
						break;
					case type_uint32:
						_temp = int_to_string((uint64)val_int32);
						break;
					case type_int64:
						_temp = int_to_string(val_int64);
						break;
					case type_uint64:
						_temp = int_to_string((uint64)val_int64);
						break;
					case type_long:
						_temp = int_to_string((int64)val_long);
						break;
					case type_ulong:
						_temp = int_to_string((uint64)val_long);
						break;
					case type_char:
						_temp = char_to_string(val_char);
						break;
					case type_uchar:
						_temp = char_to_string(val_char);
						break;
					case type_float:
						_temp = double_to_string(val_float);
						break;
					case type_double:
						_temp = double_to_string(val_double);
						break;
					default:
						break;
					}
					return _temp;
				}
				else if(get_type()==type_stl_string)
				{
					return *to_string_pointer();
				}
			}
			StdThrow(std::runtime_error, "variant type not supported");
		}

#ifdef _WINDOWS
#define strcmp_nocase strcmpi
#else
#define strcmp_nocase strcasecmp
#endif

		void variant::make_val(data_type type, const std::string& str)
		{
			if(type!=type_invalid)
			{
				set_type(type);
				if(type<type_base_end)
				{
					if(type==type_bool)
					{
						if(!str.empty() && strcmp_nocase(str.c_str(), "true")==0)
						{
							val_bool = true;
						}
						else
						{
							val_bool = false;
						}
					}
					else if(type<=type_ulong)
					{
						val_int64 = string_to_int(str);
					}
					else if(type>=type_char && type<=type_uchar)
					{
						val_char = string_to_char(str);
					}
					else if(type==type_float)
					{
						val_float = (float)string_to_double(str);
					}
					else if(type==type_double)
					{
						val_double = string_to_double(str);
					}
				}
				else if(type==type_stl_string)
				{
					*this = str;
				}
			}
		}
	}
}