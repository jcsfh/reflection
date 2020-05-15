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

#include "../type_info.h"
#include "../rclass.h"
#include "../reflect.h"
#include "../template_meta/ref_parser.h"

#include <map>
#include <set>

namespace gce
{
	namespace reflection
	{
#ifndef _WINDOWS
		type_manager _type_manager;
#endif

		map_uuid_class_r* type_manager::__get_map_uuid_rtype()
		{
			static map_uuid_class_r* ptr_map = nullptr;
			if(nullptr==ptr_map)
			{
				ptr_map = new map_uuid_class_r;
			}
			return ptr_map;
		}

		map_name_uuid* type_manager::__get_map_name_uuid()
		{
			static map_name_uuid* ptr_map = nullptr;
			if(nullptr==ptr_map)
			{
				ptr_map = new map_name_uuid;
			}
			return ptr_map;
		}

		map_class* type_manager::__get_map_class_name()
		{
			static map_class* ptr_map = nullptr;
			if(nullptr==ptr_map)
			{
				ptr_map = new map_class;
			}
			return ptr_map;
		}

		std::map<std::string, std::string>* __get_set_class_rtype()
		{
			static std::map<std::string, std::string>* ptr_set = nullptr;
			if(nullptr==ptr_set)
			{
#pragma push_macro("new")
#undef new
				ptr_set = new std::map<std::string, std::string>;
#pragma pop_macro("new")
			}
			return ptr_set;
		}

		std::string random_uuid()
		{
			char buf[37];
			const char *c = "89ab";
			char *p = buf;
			int n;
			for( n = 0; n < 16; ++n )
			{
				int b = rand()%255;
				switch( n )
				{
				case 6:
					sprintf(p, "4%x", b%15 );
					break;
				case 8:
					sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
					break;
				default:
					sprintf(p, "%02x", b);
					break;
				}

				p += 2;
				switch( n )
				{
				case 3:
				case 5:
				case 7:
				case 9:
					*p++ = '-';
					break;
				}
			}
			*p = 0;
			return buf;
		}

		void type_manager::__clean_type_manager()
		{
			delete __get_map_class_name();
			delete __get_set_class_rtype();
			delete __get_map_name_uuid();
			delete __get_map_uuid_rtype();
		}

		template<typename T>
		void __register_built_in(gce::reflection::data_type type)
		{
			///conference could not be resolved by template trait with visual studio
			type_info::typeof<T>();
			type_info::typeof<T*>();
			type_info::typeof<T&>();
			type_info::typeof<const T*>();
			type_info::typeof<const T&>();
			type_info::typeof<T*&>();
			type_info::typeof<const T* const>();
		}

		void type_manager::__do_register_built_in()
		{
			type_info::typeof<void>();
			type_info::typeof<void*>();
			type_info::typeof<const void*>();
			type_info::typeof<const void* const>();

			//__register_built_in<void>(type_void);
			__register_built_in<bool>(type_bool);
			__register_built_in<int>(type_int);
			__register_built_in<uint>(type_uint);
			__register_built_in<int16>(type_int16);
			__register_built_in<uint16>(type_uint16);
			__register_built_in<int32>(type_int32);
			__register_built_in<uint32>(type_uint32);
			__register_built_in<int64>(type_int64);
			__register_built_in<uint64>(type_uint64);
			__register_built_in<long>(type_long);
			__register_built_in<ulong>(type_ulong);
			__register_built_in<float>(type_float);
			__register_built_in<double>(type_double);
			__register_built_in<char>(type_char);
			__register_built_in<uchar>(type_uchar);
			__register_built_in<wchar>(type_wchar);

			__register_built_in<std::string>(type_stl_string);
			__register_built_in<std::wstring>(type_stl_wstring);
		}

		void type_manager::_incept()
		{
			gce::runtime::init_loader::register_cleaner_callback(__clean_type_manager);
			__do_register_built_in();
		}

		void type_manager::_dispose()
		{
		}

		rtype* type_manager::_register_type(const std::string& uuid)
		{
			if(uuid.empty())
			{
				return nullptr;
			}
			return &(*__get_map_uuid_rtype())[uuid];
		}

		rtype* type_manager::_register_type(const std::string& name, std::string* ptr_uuid)
		{
			if(name.empty())
			{
				if(ptr_uuid!=nullptr)
				{
					return _register_type(*ptr_uuid);
				}
				return nullptr;
			}

			std::string _name = name;
			meta::replace(_name, " ", "");

			auto it = __get_map_name_uuid()->find(_name);
			if(it!=__get_map_name_uuid()->end())
			{
				if(ptr_uuid!=nullptr)
				{
					*ptr_uuid = it->second;
				}
				return _register_type(it->second);
			}

			std::string uuid = random_uuid();
			(*__get_map_name_uuid())[_name] = uuid;
			if(ptr_uuid!=nullptr)
			{
				*ptr_uuid = uuid;
			}
			return _register_type(uuid);
		}

		rtype* type_manager::_get_type_by_uuid(const std::string& uuid)
		{
			auto it = __get_map_uuid_rtype()->find(uuid);
			if(it!=__get_map_uuid_rtype()->end())
			{
				return &it->second;
			}
			return nullptr;
		}

		rtype* type_manager::_get_type_by_name(const std::string& name)
		{
			std::string _name = name;
			meta::replace(_name, " ", "");

			auto it = __get_map_name_uuid()->find(_name);
			if(it!=__get_map_name_uuid()->end())
			{
				return _get_type_by_uuid(it->second);
			}
			return nullptr;
		}

		bool type_manager::_is_type_existed(const std::string& name)
		{
			return _get_type_by_name(name)!=nullptr;
		}

		std::string type_manager::_get_existed_class_name(const std::string& raw_name)
		{
			auto it = __get_set_class_rtype()->find(raw_name);
			if(it!=__get_set_class_rtype()->end())
			{
				return it->second;
			}
			else
			{
				return "";
			}
		}

		void type_manager::_add_class_name(const std::string& raw_name, const std::string& name)
		{
			(*__get_set_class_rtype())[raw_name] == name;
		}

		bool count_arg(const variant& a, uint& count)
		{
			if(a.get_type()!=type_invalid)
			{
				++count;
				return true;
			}
			return false;
		}

		uint type_manager::__check_type(const rmethod& method
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9)
		{
			uint arg_count = 0;
			count_arg(a1, arg_count);
			count_arg(a2, arg_count);
			count_arg(a3, arg_count);
			count_arg(a4, arg_count);
			count_arg(a5, arg_count);
			count_arg(a6, arg_count);
			count_arg(a7, arg_count);
			count_arg(a8, arg_count);
			count_arg(a9, arg_count);

			if(method._params.size()!=arg_count)
			{
				StdThrow(std::runtime_error, "the number of parameters for %s doesn't match. input number is %d while required is %u", method._name.c_str(), arg_count, (uint)method._params.size());
			}
			
			for(uint i=1; i<=arg_count; ++i)
			{
				switch(i)
				{
				case 1:
					__check_param_type_dynamically<1>(method, a1);
					break;
				case 2:
					__check_param_type_dynamically<2>(method, a2);
					break;
				case 3:
					__check_param_type_dynamically<3>(method, a3);
					break;
				case 4:
					__check_param_type_dynamically<4>(method, a4);
					break;
				case 5:
					__check_param_type_dynamically<5>(method, a5);
					break;
				case 6:
					__check_param_type_dynamically<6>(method, a6);
					break;
				case 7:
					__check_param_type_dynamically<7>(method, a7);
					break;
				case 8:
					__check_param_type_dynamically<8>(method, a8);
					break;
				case 9:
					__check_param_type_dynamically<9>(method, a9);
					break;
				}
			}
			return arg_count;
		}

		bool type_manager::_is_exist(rtype* class_r_, find_type _type, const std::string& name)
		{
			switch(_type)
			{
			case type:
				return class_r_->_name==name;
				break;
			case method:
				return class_r_->has_method() && class_r_->methods().find(name)!=class_r_->methods().end();
				break;
			case member:
				return class_r_->members().find(name)!=class_r_->members().end();
				break;
			}
			return false;
		}

		void type_manager::_find_rtype(rtype* class_r_, find_type _type, const std::string& name, rparent& _rparent, const rtype* owner_)
		{
			if(nullptr==class_r_)
			{
				StdThrow(std::runtime_error, "object is not reflectable");
			}

			if((nullptr==owner_ || class_r_==owner_) && _is_exist(class_r_, _type, name))
			{
				_rparent.rtype_ = class_r_;
				_rparent.reflect_offset = class_r_->_reflect_offset;
				_rparent.ref_data_offset = class_r_->_ref_data_offset;
			}
			else if(!class_r_->parents().empty())
			{
				///finde from parents
				struct parent
				{
					rparent* rparent_;
					int offset;
					bool flag;

					parent()
					{
						memset(this, 0, sizeof(*this));
					}
				};

				///traverse inheritance tree
				std::queue<parent> que_parent;
				for(auto it = class_r_->parents().begin(); it!=class_r_->parents().end(); ++it)
				{
					parent _parent;
					_parent.rparent_ = &it->second;
					_parent.offset = it->second.offset;
					_parent.flag = nullptr==owner_ || class_r_==owner_ || _parent.rparent_->rtype_==owner_;
					que_parent.push(_parent);
				}

				while(!que_parent.empty())
				{
					parent _parent = que_parent.front();
					que_parent.pop();

					if(_parent.flag && _is_exist(_parent.rparent_->rtype_, _type, name))
					{
						///matched
						_rparent.offset = _parent.offset;
						_rparent.rtype_ = _parent.rparent_->rtype_;
						_rparent.reflect_offset = _parent.rparent_->reflect_offset;
						_rparent.ref_data_offset = _parent.rparent_->ref_data_offset;
						break;
					}

					for(auto it = _parent.rparent_->list_next_parent.begin(); it!=_parent.rparent_->list_next_parent.end(); ++it)
					{
						parent _parent_sub;
						_parent_sub.rparent_ = &it->second;
						_parent_sub.offset = _parent.offset + it->second.offset;
						_parent_sub.flag = _parent.flag || nullptr==owner_ || _parent.rparent_->rtype_==owner_ || _parent_sub.rparent_->rtype_==owner_;
						que_parent.push(_parent_sub);
					}
				}
			}
		}

		rmethod& type_manager::__get_bind_method(rtype* class_r_, const std::string& method_name, void** pp, const rtype* owner_)
		{
			rparent _rparent;
			_find_rtype(class_r_, method, method_name, _rparent, owner_);
			if(_rparent.rtype_!=nullptr && _rparent.rtype_->has_method())
			{
				if(*pp!=nullptr)
				{
					///set pointer to the corresponding object
					*pp = (char*)*pp + _rparent.offset;
				}
				return _rparent.rtype_->methods()[method_name];
			}
			StdThrow(std::runtime_error, "bad function call. method(%s) doesn't exist", method_name.c_str());
		}

		rmethod& type_manager::__get_method(void* instance_, rtype* rtype_, const std::string& method_name, const rtype* owner_, bind_base*& ptr_bind_base)
		{
			rmethod& method = __get_bind_method(rtype_, method_name, (void**)&instance_, owner_);
			ptr_bind_base = (bind_base*)method._bind_addr_;
			ptr_bind_base->ptr_ = instance_;
			return method;
		}

		rmethod& type_manager::__get_method(reflect* reflect_, class_r* class_r_, const std::string& method_name, const rtype* owner_, bind_base*& ptr_bind_base)
		{
			rtype* rtype_ = class_r_;
			void* p = nullptr;
			if(reflect_!=nullptr)
			{
				///relocate to derived class
				rtype_ = reflect_->_ptr_derived_class_r;
				p = reflect_->get_derived();
			}
			return __get_method(p, rtype_, method_name, owner_, ptr_bind_base);
		}

		rtype* __find_type(const rparent& _rparent, rtype* _rtype_to_find)
		{
			if(_rparent.rtype_==_rtype_to_find)
			{
				return _rparent.rtype_;
			}

			for(auto it = _rparent.list_next_parent.begin(); it!=_rparent.list_next_parent.end(); ++it)
			{
				if(it->second.rtype_==_rtype_to_find)
				{
					return _rtype_to_find;
				}
				rtype* ptr_rtype = __find_type(it->second, _rtype_to_find);
				if(ptr_rtype!=nullptr)
				{
					return ptr_rtype;
				}
			}
			return nullptr;
		}

		bool type_manager::is_derived_of(const std::string& derived, const std::string& base)
		{
			rtype* _rtype_derived = _get_type_by_name(derived);
			if(nullptr==_rtype_derived)
			{
				return false;
			}

			rparent _rparent;
			_find_rtype((class_r*)_rtype_derived, type, base, _rparent, nullptr);
			return _rparent.rtype_!=nullptr;
		}

		rmember& type_manager::_find_member(class_r* class_r_, const std::string& name, reflect*& reflect_, ref_data*& ref_data_, void** pp)
		{
			rparent _rparent;
			_find_rtype(class_r_, member, name, _rparent, nullptr);
			if(_rparent.rtype_!=nullptr)
			{
				rmember& _rmember = _rparent.rtype_->members()[name];
				if(*pp!=nullptr)
				{
					///relocate the object that contains the member
					ref_data_ = (ref_data*)((char*)*pp + _rparent.offset + _rparent.ref_data_offset);
					reflect_ = (reflect*)((char*)*pp + _rparent.offset + _rparent.reflect_offset);
					*pp = (char*)*pp + _rparent.offset + _rmember._offset;
				}
				return _rmember;
			}
			StdThrow(std::runtime_error, "member(%s) doesn't exist", name.c_str());
		}

		void type_manager::_add_parent(const std::string& parent, rtype* class_r_)
		{
			if(!parent.empty())
			{
				rtype* _parent_type = type_manager::_get_type_by_name(parent);
				if(_parent_type!=nullptr)
				{
					rparent _rparent;
					_rparent.offset = class_r_->_size;
					_rparent.rtype_ = _parent_type;
					_rparent.ref_data_offset = _parent_type->_ref_data_offset;
					_rparent.reflect_offset = _parent_type->_reflect_offset;
					_rparent.list_next_parent = _parent_type->parents();
					class_r_->parents().insert(std::move(std::make_pair(parent, _rparent)));
					class_r_->_size += _parent_type->_size;
					class_r_->_total_size = class_r_->_size;
				}
				else
				{
					StdThrow(std::runtime_error, "base class(%s) doesn't exist", parent.c_str());
				}
			}
		}

		class_r& type_manager::create_class(const std::string& name
				, const std::string& parent1
				, const std::string& parent2
				, const std::string& parent3
				, const std::string& parent4
				, const std::string& parent5
				, const std::string& parent6
				, const std::string& parent7
				, const std::string& parent8
				, const std::string& parent9)
		{
			rtype* class_r_ = _get_type_by_name(name);
			if(class_r_!=nullptr)
			{
				StdThrow(std::runtime_error, "class(%s) has been existed", name.c_str());
			}

			std::string uuid;
			class_r_ = _register_type(name, &uuid);
			class_r_->_uuid = uuid;
			class_r_->_name = name;
			class_r_->_raw_name = name;
			reflect::parse_namespace(class_r_->_raw_name, class_r_->_name, class_r_->_namespaces);
			class_r_->_size = sizeof(reflect_stub) + sizeof(ref_data);
			class_r_->_ptr_class_r = (class_r*)class_r_;
			class_r_->_ptr_derived_class_r = (class_r*)class_r_;
			class_r_->_ptr_static_instance = class_r_->_ptr_class_r;
			class_r_->_data_type = type_object_reflectable;

			_add_parent(parent1, class_r_);
			_add_parent(parent2, class_r_);
			_add_parent(parent3, class_r_);
			_add_parent(parent4, class_r_);
			_add_parent(parent5, class_r_);
			_add_parent(parent6, class_r_);
			_add_parent(parent7, class_r_);
			_add_parent(parent8, class_r_);
			_add_parent(parent9, class_r_);
			return (class_r&)*class_r_;
		}

		class_r& type_manager::get_class(const std::string& name)
		{
			rtype* class_r_ = _get_type_by_name(name);
			if(class_r_!=nullptr)
			{
				return (class_r&)*class_r_;
			}
			StdThrow(std::runtime_error, "class(%s) doesn't exist. do you forget namespace(s)?", name.c_str());
		}

		void* type_manager::call_method(const std::string& type_name
				, const std::string& method_name
				, void* instance_
				, void* a1
				, void* a2
				, void* a3
				, void* a4
				, void* a5
				, void* a6
				, void* a7
				, void* a8
				, void* a9)
		{
			rtype* rtype_ = type_manager::_get_type_by_name(type_name);
			if(nullptr==rtype_)
			{
				StdThrow(std::runtime_error, "type(%s) hasn't been registered", type_name.c_str());
			}
			return _call_method(rtype_, method_name, instance_, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}

		inline variant make_param(void* a)
		{
			variant _variant;
			if(a!=nullptr)
			{
				_variant.set_type(type_void);
				_variant = a;
			}
			return _variant;
		}

		void* type_manager::_call_method(rtype* rtype_
				, const std::string& method_name
				, void* instance_
				, void* a1
				, void* a2
				, void* a3
				, void* a4
				, void* a5
				, void* a6
				, void* a7
				, void* a8
				, void* a9)
		{
			variant _a1 = make_param(a1);
			variant _a2 = make_param(a2);
			variant _a3 = make_param(a3);
			variant _a4 = make_param(a4);
			variant _a5 = make_param(a5);
			variant _a6 = make_param(a6);
			variant _a7 = make_param(a7);
			variant _a8 = make_param(a8);
			variant _a9 = make_param(a9);

			switch(rtype_->get_data_type())
			{
			///wrap std object
			case type_stl_sharedptr:
				{
					auto p = (std::shared_ptr<int64>*)instance_;
					ref_parser<std::shared_ptr<int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_vector:
				{
					auto p = (std::vector<int64>*)instance_;
					ref_parser<std::vector<int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_list:
				{
					auto p = (std::list<int64>*)instance_;
					ref_parser<std::list<int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_set:
				{
					auto p = (std::set<int64>*)instance_;
					ref_parser<std::set<int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_multiset:
				{
					auto p = (std::multiset<int64>*)instance_;
					ref_parser<std::multiset<int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_deque:
				{
					auto p = (std::deque<int64>*)instance_;
					ref_parser<std::deque<int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_map:
				{
					auto p = (std::map<int64, int64>*)instance_;
					ref_parser<std::map<int64, int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
			case type_stl_multimap:
				{
					auto p = (std::multimap<int64, int64>*)instance_;
					ref_parser<std::multimap<int64, int64> > instance(p);
					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				break;
//#ifdef USE_BOOST
//			case type_boost_unorderedmap:
//				{
//					auto p = (boost::unorderedmap<int64, int64>*)instance_;
//					ref_parser<boost::unorderedmap<int64, int64> > instance(p);
//					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
//					return *(void**)ret.get_val_address();
//				}
//				break;
//			case type_boost_unorderedmultimap:
//				{
//					auto p = (boost::unorderedmultimap<int64, int64>*)instance_;
//					ref_parser<boost::unorderedmultimap<int64, int64> > instance(p);
//					variant ret = ((class_r*)rtype_)->_invoke_v(&instance, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
//					return *(void**)ret.get_val_address();
//				}
//				break;
//#endif
			default:
				if(rtype_->get_data_type()==type_object_reflectable)
				{
					variant ret = ((class_r*)rtype_)->_invoke_v(instance_, rtype_, rtype_, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);
					return *(void**)ret.get_val_address();
				}
				else
				{
					StdThrow(std::runtime_error, "type(%s) not supported", rtype_->get_name().c_str());
				}
				break;
			}
			return nullptr;
		}
	}
}

bool register_rtype(const std::string& name, gce::reflection::rdata*& _rdata)
{
	std::string uuid;
	_rdata = gce::reflection::type_manager::_get_type_by_name(name);
	if(nullptr==_rdata)
	{
		_rdata = gce::reflection::type_manager::_register_type(name, &uuid);
		_rdata->_uuid = uuid;
		return false;
	}
	return true;
}