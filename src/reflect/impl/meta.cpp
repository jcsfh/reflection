/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#include "../../runtime/runtime.h"

#include "../rtype.h"
#include "../meta.h"
#include "../type_info.h"
#include "../type_manager.h"
#include <map>
#include <string>

namespace gce
{
	namespace reflection
	{
		void meta::trim_left(std::string& str)
		{
			long_t length = str.size();
			if(length==0)
			{
				return;
			}

			char* ptr_str_t = (char*)str.data();

			long_t left = 0;

			long_t i=0;
			for(i=0; i<length; ++i)
			{
				if(ptr_str_t[i]==' ')
				{
					++left;
				}
				else
				{
					break;
				}
			}

			::memmove((char*)ptr_str_t, (char*)ptr_str_t+left, sizeof(char)*(length-left));
			memset((char*)ptr_str_t+(length-left), 0, left);

			if(str.size()>=(size_t)left)
			{
				str.resize(str.size()-left);
			}
		}

		void meta::trim_right(std::string& str)
		{
			size_t size = str.size();
			if(0==size)
			{
				return;
			}

			char * back_ = &str.back();
			size_t i=0;
			while(*(back_--)==' ' && i++<size);
			if(i>0)
			{
				if(size>i)
				{
					str.resize(size-i);
				}
				else
				{
					str.clear();
				}
			}
		}

		void meta::replace(std::string& str, const char* ptr_old, const char* ptr_new)
		{
			std::string old_value;
			if(ptr_old!=nullptr)
			{
				old_value = ptr_old;
			}

			std::string new_value;
			if(ptr_new!=nullptr)
			{
				new_value = ptr_new;
			}

			for(string_type::size_type   pos(0);   pos!=string_type::npos;   pos+=new_value.length())   {   
				if(   (pos=str.find(old_value.c_str(),pos))!=string_type::npos   )   
					str.replace(pos,old_value.length(),new_value);   
				else   break;   
			}
		}

		void meta::split(const std::string& s, const std::string& delim, std::vector<std::string>& ret)
		{
			size_t last = 0;
			size_t index=s.find_first_of(delim,last);
			while (index!=std::string::npos)
			{
				std::string sub = s.substr(last,index-last);
				if(!sub.empty())
				{
					ret.push_back(sub);
				}
				last=index+1;
				index=s.find_first_of(delim,last);
			}

			if (index-last>0)
			{
				std::string sub = s.substr(last,index-last);
				if(!sub.empty())
				{
					ret.push_back(sub);
				}
			}
		}

		std::map<std::string, data_type>& base_data_type_map()
		{
			static std::map<std::string, data_type> map;
			return map;
		}

		///destroy map before printing leak
		void cleaner_callback()
		{
			base_data_type_map().~map();
		}

		data_type meta::name_to_type(const std::string& name)
		{
			rtype* rtype_ = type_manager::_get_type_by_name(name);
			if(rtype_!=nullptr)
			{
				return rtype_->get_data_type();
			}
			return gce::reflection::type_invalid;
		}

		std::string meta::type_to_name(data_type dt)
		{
			std::string name;
			switch(dt)
			{
			case type_void:
				name = "void";
				break;
			case type_bool:
				name = _typeid(typeid(bool)).name();
				break;
			case type_int:
				name = _typeid(typeid(int)).name();
				break;
			case type_uint:
				name = _typeid(typeid(uint)).name();
				break;
			case type_int16:
				name = _typeid(typeid(int16)).name();
				break;
			case type_uint16:
				name = _typeid(typeid(uint16)).name();
				break;
			case type_int32:
				name = _typeid(typeid(int32)).name();
				break;
			case type_uint32:
				name = _typeid(typeid(uint32)).name();
				break;
			case type_int64:
				name = _typeid(typeid(int64)).name();
				break;
			case type_uint64:
				name = _typeid(typeid(uint64)).name();
				break;
			case type_long:
				name = _typeid(typeid(long)).name();
				break;
			case type_ulong:
				name = _typeid(typeid(ulong)).name();
				break;
			case type_char:
				name = _typeid(typeid(char)).name();
				break;
			case type_uchar:
				name = _typeid(typeid(uchar)).name();
				break;
			case type_wchar:
				name = _typeid(typeid(wchar)).name();
				break;
			case type_float:
				name = _typeid(typeid(float)).name();
				break;
			case type_double:
				name = _typeid(typeid(double)).name();
				break;
			case type_stl_string:
				name = "std::string";
				break;
			case type_stl_wstring:
				name = "std::wstring";
				break;
			case type_stl_sharedptr:
				name = "std::shared_ptr";
				break;
			case type_stl_vector:
				name = "std::vector";
				break;
			case type_stl_list:
				name = "std::list";
				break;
			case type_stl_set:
				name = "std::set";
				break;
			case type_stl_multiset:
				name = "std::multiset";
				break;
			case type_stl_queue:
				name = "std::queue";
				break;
			case type_stl_deque:
				name = "std::deque";
				break;
			case type_stl_stack:
				name = "std::stack";
				break;
			case type_stl_priority_queue:
				name = "std::priority_queue";
				break;
			case type_stl_map:
				name = "std::map";
				break;
			case type_stl_multimap:
				name = "std::multimap";
				break;
			case type_boost_unorderedmap:
				name = "boost::unorderedmap";
				break;
			case type_boost_unorderedmultimap:
				name = "boost::unorderedmultimap";
				break;
			case type_object_reflectable:
				name = "reflectable";
				break;
			case type_object_copyable:
				name = "copyable";
				break;
			case type_object_nonreflectable:
				name = "nonreflectable";
				break;
			case type_hashset:
				name = "gce::container::hash_set";
				break;
			case type_hashmultiset:
				name = "gce::container::hash_multiset";
				break;
			case type_map:
				name = "gce::container::map";
				break;
			case type_hashmap:
				name = "gce::container::hash_map";
				break;
			case type_hashmultimap:
				name = "gce::container::hash_multimap";
				break;
			default:
				break;
			}
			return name;
		}
	}
}