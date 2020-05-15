/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#ifdef _WINDOWS
#pragma warning(disable:4275 4503 4227 4344 4661)
#endif

#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>

namespace gce
{
	namespace reflection
	{
		///data type
		enum data_type
		{
			type_invalid = 0,

			type_void,
			type_enum,
			type_bool,
			type_int,
			type_uint,
			type_int16,
			type_uint16,
			type_int32,
			type_uint32,
			type_int64,
			type_uint64,
			type_long,
			type_ulong,
			type_char,
			type_uchar,
			type_wchar,
			type_float,
			type_double,

			type_base_end = 32,

			type_stl_string,
			type_stl_wstring,

			type_simple_end = 40,

			type_container_start,

			type_stl_sharedptr,
			type_stl_vector,
			type_stl_list,
			type_stl_set,
			type_stl_multiset,
			type_stl_queue,
			type_stl_deque,
			type_stl_stack,
			type_stl_priority_queue,

			type_stl_map,
			type_stl_multimap,
			type_boost_unorderedmap,
			type_boost_unorderedmultimap,

			type_object_end = 80,

			type_object_reflectable,
			type_object_copyable,
			type_object_nonreflectable,

			type_extend_start = 100,

			type_hashset,
			type_hashmultiset,
			type_map,
			type_hashmap,
			type_hashmultimap,
		};

		///argument modifier
		enum arg_type
		{
			arg_object,
			arg_ref,
			arg_ptr,
		};

		struct modifier
		{
		public:
			modifier()
			{
				clear();
			}

			inline bool is_pointer() const
			{
				return arg_ptr==_arg_type;
			}

			inline bool is_reference() const
			{
				return arg_ref==_arg_type;
			}

			inline bool is_const() const
			{
				return _is_const;
			}

			inline arg_type get_arg_type() const
			{
				return (arg_type)_arg_type;
			}

			bool operator==(const modifier& other) const
			{
				return _arg_type==other._arg_type
					&& _is_const==other._is_const;
			}

			inline void clear()
			{
				_arg_type = arg_object;
				_is_const = false;
			}

			char _arg_type;
			bool _is_const;
		};

		class rtype;
		struct sub_type
		{
			std::vector<rtype*> list_sub_type_ptr;

			sub_type()
			{
			}

			sub_type(sub_type &&other)
			{
				*this = std::move(other);
			}

			sub_type& operator = (sub_type &&other)
			{
				if(this!=&other)
				{
					if(!other.list_sub_type_ptr.empty())
					{
						list_sub_type_ptr = std::move(other.list_sub_type_ptr);
					}
				}
				return *this;
			}
		};

		struct meta_type : public sub_type
		{
			::type_info* _ptr_rtti;
			data_type _data_type;
			uint16 _size;
			bool _is_array;
			uint16 _total_size;

			meta_type()
				: _ptr_rtti(nullptr)
				, _data_type(type_invalid)
				, _size(0)
				, _is_array(false)
				, _total_size(0)
			{
			}

			const ::type_info& get_rtti() const
			{
				return *_ptr_rtti;
			}

			meta_type(meta_type &&other)
			{
				*this = std::move(other);
			}

			meta_type& operator = (meta_type &&other)
			{
				if(this!=&other)
				{
					_data_type = other._data_type;
					_size = other._size;
					_ptr_rtti = other._ptr_rtti;

					(sub_type&)*this = std::move(other);
				}
				return *this;
			}

			inline bool equal(const ::type_info& ti) const
			{
				return _ptr_rtti==&ti;
			}

			inline data_type get_data_type() const
			{
				return _data_type;
			}

			inline uint16 get_size() const
			{
				return _size;
			}

			inline bool is_array() const
			{
				return _is_array;
			}

			inline uint16 get_total_size() const
			{
				return _total_size;
			}

			bool operator==(const meta_type& other) const
			{
				return _data_type==other._data_type 
					&& _size==other._size
					&& _is_array==other._is_array
					&& _total_size==other._total_size;
			}

		protected:
			static void _create_default_type(data_type type, meta_type& mt)
			{
				mt._data_type = type;
				switch(type)
				{
				case type_int:
				case type_uint:
					mt._size = sizeof(int);
					break;
				case type_int16:
				case type_uint16:
					mt._size = sizeof(int16);
					break;
				case type_int32:
				case type_uint32:
					mt._size = sizeof(int32);
					break;
				case type_int64:
				case type_uint64:
					mt._size = sizeof(int64);
					break;
				case type_long:
				case type_ulong:
					mt._size = sizeof(long);
					break;
				case type_float:
					mt._size = sizeof(float);
					break;
				case type_double:
					mt._size = sizeof(double);
					break;
				case type_bool:
				case type_char:
				case type_uchar:
					mt._size = sizeof(char);
					break;
				case type_wchar:
					mt._size = sizeof(wchar);
					break;
				case type_stl_string:
					mt._size = sizeof(std::string);
					break;
				case type_stl_wstring:
					mt._size = sizeof(std::wstring);
					break;
				default:
					break;
				}
				mt._total_size = mt._size;
			}
		};

		///argument type with meta type with modifier
		struct argument_type : public meta_type, public modifier
		{
			argument_type()
			{
			}

			argument_type(argument_type &&other)
			{
				*this = std::move(other);
			}

			argument_type& operator = (argument_type &&other)
			{
				if(this!=&other)
				{
					(meta_type&)*this = std::move(other);
					(modifier&)*this = other;
				}
				return *this;
			}

			bool operator==(const argument_type& other) const
			{
				return (meta_type&)*this==(meta_type&)other	&& (modifier&)*this==(modifier&)other;
			}

		protected:
			///compare type nonexactly
			inline bool _inexact_equal(const meta_type& _type, bool compare_size=true) const
			{
				if(!((meta_type&)*this==(meta_type&)_type))
				{
					if(
						!(_data_type>=type_enum&&_data_type<type_base_end
						&& _type._data_type>=type_enum&&_type._data_type<type_base_end
						)
						|| (compare_size && _size!=_type._size)
						)
					{
						return false;
					}
				}
				return true;
			}

			template<typename T>
			bool _inexact_equal() const;
		};
	}
}