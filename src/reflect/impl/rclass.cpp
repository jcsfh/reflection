/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#include "../../runtime/runtime.h"

#include "../template_meta/reflect.h"
#include "../type_info.h"
#include "../serializer.h"
#include "../rclass.h"

namespace gce
{
	namespace reflection
	{
		reflect* class_r::create_instance() const
		{
			reflect* p = nullptr;
			if(this->_ptr_static_instance!=nullptr && this->_ptr_static_instance!=this)
			{
				///for static class
				p = this->_ptr_static_instance->create_instance();
			}
			else
			{
				///for dynamic class
				char* buf = new char[this->_size];
				p = _create_instance(buf, buf, (class_r*)this);
			}
			return p;
		}

		void class_r::destroy_instance(reflect*& p) const
		{
			if(this->_ptr_static_instance!=nullptr && this->_ptr_static_instance!=this)
			{
				this->_ptr_static_instance->destroy_instance(p);
			}
			else
			{
				_destroy_instance(p);
				delete (char*)p;
				p = nullptr;
			}
		}

#pragma push_macro("new")
#undef new
		reflect* class_r::_create_instance(void* buf, void* derived_, class_r* assigned_) const
		{
			static reflect_stub _reflect_stub;
			_reflect_stub._ptr_class_r = (class_r*)this;
			_reflect_stub._ptr_derived_class_r = assigned_;
			_reflect_stub._ptr_to_derived = derived_;
			::memcpy(buf, &_reflect_stub, sizeof(reflect_stub));

			ref_data _ref_data;
			_ref_data._reflect_ = (reflect_stub*)buf;
			_ref_data._reflect_offset = (int)((uint64)_ref_data._reflect_ - (uint64)buf);
			_ref_data._ref_data_offset = sizeof(reflect_stub);
			::memcpy((char*)buf+sizeof(reflect_stub), &_ref_data, sizeof(reflect_stub));
			this->_ref_data = _ref_data;

			this->_reflect_offset = _ref_data._reflect_offset;
			this->_ref_data_offset = _ref_data._ref_data_offset;

			((ref_data*)((char*)buf + _ref_data._ref_data_offset))->_create_flag(*this);
			
			///create parents
			for(auto it = this->parents().begin(); it!=this->parents().end(); ++it)
			{
				if(nullptr==it->second.rtype_->_ptr_static_instance)
				{
					StdThrow(std::runtime_error, "can not create instance_ of base class (%s)", it->second.rtype_->get_name().c_str());
				}
				it->second.rtype_->_ptr_static_instance->_create_instance((char*)buf+it->second.offset, derived_, assigned_);
				it->second.rtype_->_ptr_derived_class_r = assigned_;
			}

			///construct members
			for(auto it = this->members().begin(); it!=this->members().end(); ++it)
			{
				data_type type = it->second._type_->get_data_type();
				switch(type)
				{
				case type_stl_string:
					{
						__call_constructor((std::string*)((char*)buf+it->second._offset));
					}
					break;
				case type_stl_wstring:
					{
						__call_constructor((std::wstring*)((char*)buf+it->second._offset));
					}
					break;
				default:
					break;
				}
			}

			auto p = (reflect*)buf;
			p->_ptr_to_derived = derived_;
			p->_ptr_derived_to_reflect = p;
			return p;
		}
#pragma pop_macro("new")

		void class_r::_destroy_instance(void* p) const
		{
			auto reflect_ = (reflect*)p;
			p = reflect_->_to_derived_pointer();

			((ref_data*)((char*)p + _ref_data._ref_data_offset))->~ref_data();

			for(auto it = this->parents().begin(); it!=this->parents().end(); ++it)
			{
				it->second.rtype_->_ptr_static_instance->_destroy_instance((char*)p+it->second.offset);
			}

			for(auto it = this->members().begin(); it!=this->members().end(); ++it)
			{
				data_type type = it->second._type_->get_data_type();
				switch(type)
				{
				case type_stl_string:
					__call_destructor((std::string*)((char*)p+it->second._offset));
					break;
				case type_stl_wstring:
					__call_destructor((std::wstring*)((char*)p+it->second._offset));
					break;
				default:
					break;
				}
			}
		}

		void class_r::add_member(const std::string& name, const std::string& member_type) STD_THROW
		{
			rtype* ptr_member_type = type_manager::_get_type_by_name(member_type);
			if(nullptr==ptr_member_type)
			{
				StdThrow(std::runtime_error, "member type (%s) not existed", member_type.c_str());
			}
			if(this->members().find(name)!=this->members().end())
			{
				StdThrow(std::runtime_error, "member (%s) has existed", name.c_str());
			}

			rmember& member = this->members()[name];
			member._is_static = false;
			member._name = name;
			member._offset = this->_size;
			member._type_ = ptr_member_type;
			member._index = (int)this->members().size()-1;
			this->_size += ptr_member_type->_size;
			this->_index_members[member._index] = &member;
			this->_total_size = this->_size;
		}

		rmethod& class_r::get_method(const std::string& method_name) STD_THROW
		{
			void* p = nullptr;
			rmethod& method = type_manager::__get_bind_method((rtype*)this, method_name, (void**)&p, this);
			method._owner_ = this;
			return method;
		}

		void class_r::_set_value_v(reflect* instance_, ref_data* ref_data_, rmember& _rmember, void* p, const std::string& _name, const variant& _value)
		{
			data_type type = _rmember._type_->get_data_type();
			switch(type)
			{
			case type_stl_string:
				{
					*(std::string*)p = *_value.to_string_pointer();
				}
				break;
			case type_stl_wstring:
				{
					*(std::wstring*)p = *_value.to_wstring_pointer();
				}
				break;
			default:
				if(type<type_base_end)
				{
					::memcpy(p, _value.get_val_address(), _rmember._type_->get_size());
				}
				else
				{
					StdThrow(std::runtime_error, "member type (%s) not supported", _rmember._type_->get_name().c_str());
				}
				break;
			}

			if(ref_data_!=nullptr && !_rmember._is_static)
			{
				ref_data_->_flag(_rmember._index) = true;
			}
		}

		variant class_r::_get_value_v(rmember& _rmember, void* p, const std::string& name)
		{
			variant ret;
			data_type type = _rmember._type_->get_data_type();
			ret.set_type(type);
			switch(type)
			{
			case type_stl_string:
				{
					*ret.to_string_pointer() = *(std::string*)p;
				}
				break;
			case type_stl_wstring:
				{
					*ret.to_wstring_pointer() = *(std::wstring*)p;
				}
				break;
			default:
				if(type<type_base_end)
				{
					::memcpy(ret.get_val_address(), p, _rmember._type_->get_size());
				}
				else
				{
					StdThrow(std::runtime_error, "member type (%s) not supported", _rmember._type_->get_name().c_str());
				}
				break;
			}
			return ret;
		}

		rmember& class_r::_get_member(reflect*& instance_, ref_data*& ref_data_, const std::string& _name, void** pp)
		{
			class_r* class_r_ = this;
			if(instance_!=nullptr)
			{
				class_r_ = instance_->_ptr_class_r;
				*pp = instance_->_to_derived_pointer();
			}
			rmember& _rmember = type_manager::_find_member(class_r_, _name, instance_, ref_data_, pp);
			if(_rmember._is_static)
			{
				*pp = _rmember._address;
			}
			return _rmember;
		}

		void class_r::set_value_v(reflect* instance_, const std::string& name, const variant& _value) STD_THROW
		{
			void* p = nullptr;
			ref_data* ref_data_ = nullptr;
			rmember& _rmember = _get_member(instance_, ref_data_, name, &p);
			_set_value_v(instance_, ref_data_, _rmember, p, name, _value);
		}

		variant class_r::get_value_v(reflect* instance_, const std::string& name) STD_THROW
		{
			void* p = nullptr;
			ref_data* ref_data_ = nullptr;
			rmember& _rmember = _get_member(instance_, ref_data_, name, &p);
			return _get_value_v(_rmember, p, name);
		}

		void class_r::set_value(reflect* instance_, const std::string& _name, const std::string& _value) STD_THROW
		{
			void* p = nullptr;
			ref_data* ref_data_ = nullptr;
			rmember& _rmember = _get_member(instance_, ref_data_, _name, &p);

			variant _variant;
			_variant.make_val(_rmember._type_->get_data_type(), _value);
			_set_value_v(instance_, ref_data_, _rmember, p, _name, _variant);
		}
		
		std::string class_r::get_value(reflect* instance_, const std::string& _name) STD_THROW
		{
			void* p = nullptr;
			ref_data* ref_data_ = nullptr;
			rmember& _rmember = _get_member(instance_, ref_data_, _name, &p);

			variant _ret = _get_value_v(_rmember, p, _name);
			return _ret.to_string();
		}

		reflect* class_r::cast_to_type(reflect* p, const std::string& name)
		{
			if(nullptr==p)
			{
				return nullptr;
			}

			rparent _rparent;
			type_manager::_find_rtype(p->_ptr_derived_class_r, type_manager::type, name, _rparent, nullptr);
			if(nullptr==_rparent.rtype_)
			{
				return nullptr;
			}

			p = (reflect*)((char*)p->get_derived() + _rparent.offset + _rparent.reflect_offset);
			return p;
		}

		void make_param(rmethod& method, void** args, const variant& a, uint i)
		{
			if(a.get_type()!=type_invalid)
			{
				rtype* arg_ = method._params[i];
				if(arg_->_data_type<type_simple_end || arg_->is_pointer() || arg_->is_reference())
				{
					if(a.get_type()==type_void)
					{
						args[i] = *(void**)a.get_val_address();
					}
					else if(arg_->get_arg_type()!=arg_ptr)
					{
						args[i] = a.get_val_address();
					}
					else
					{
						((variant&)a)._param = (void*)a.get_val_address();
						args[i] = (void*)&a._param;
					}
				}
				else
				{
					StdThrow(std::runtime_error, "member type (%s) not supported", arg_->_name.c_str());
				}
			}
		}

		variant class_r::_invoke_v(rmethod& method, bind_base* ptr_bind_base, const std::string& method_name
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9) STD_THROW
		{
			if(method.return_type().get_data_type()>=type_simple_end)
			{
				StdThrow(std::runtime_error, "return type (%s) not supported", method.return_type().get_name().c_str());
			}

			uint arg_count = type_manager::__check_type(method, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			void* args[9];
			make_param(method, args, a1, 0);
			make_param(method, args, a2, 1);
			make_param(method, args, a3, 2);
			make_param(method, args, a4, 3);
			make_param(method, args, a5, 4);
			make_param(method, args, a6, 5);
			make_param(method, args, a7, 6);
			make_param(method, args, a8, 7);
			make_param(method, args, a9, 8);

			variant _ret_val;
			if(method._return_type_->get_arg_type()==arg_object)
			{
				_ret_val.set_type(method._return_type_->get_data_type());
			}

			switch(arg_count)
			{
			case 0:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address());
				break;
			case 1:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0]);
				break;
			case 2:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1]);
				break;
			case 3:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2]);
				break;
			case 4:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2], args[3]);
				break;
			case 5:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2], args[3], args[4]);
				break;
			case 6:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2], args[3], args[4], args[5]);
				break;
			case 7:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
				break;
			case 8:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]);
				break;
			case 9:
				ptr_bind_base->execute(*method._return_type_, _ret_val.get_val_address(), args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7], args[8]);
				break;
			}

			///make return value
			variant _ret;
			_ret.set_type(method._return_type_->get_data_type());

			if(method._return_type_->get_data_type()<type_simple_end)
			{
				if(method._return_type_->get_data_type()!=type_void || method._return_type_->get_arg_type()==arg_ptr)
				{
					if(method._return_type_->get_arg_type()==arg_object || (method._return_type_->get_data_type()==type_void && method._return_type_->get_arg_type()==arg_ptr))
					{
						if(method._return_type_->get_data_type()<type_base_end)
						{
							_ret.val_int64 = *(int64*)_ret_val.get_val_address();
						}
						else if(method._return_type_->get_data_type()==type_stl_string)
						{
							_ret = *(std::string*)_ret_val.get_val_address();
						}
						else if(method._return_type_->get_data_type()==type_stl_wstring)
						{
							_ret = *(std::wstring*)_ret_val.get_val_address();
						}
					}
					else
					{
						_ret.set_type(type_void);
						_ret.set_sub_type(method._return_type_->get_data_type());

						if(_ret_val.val_void_ptr!=nullptr)
						{
							if(method._return_type_->get_data_type()<type_base_end)
							{
								_ret.val_int64 = **(int64**)_ret_val.get_val_address();
							}
							else if(method._return_type_->get_data_type()==type_stl_string)
							{
								_ret.val_void_ptr = *(std::string**)_ret_val.get_val_address();
							}
							else if(method._return_type_->get_data_type()==type_stl_wstring)
							{
								_ret.val_void_ptr = *(std::wstring**)_ret_val.get_val_address();
							}
						}
						else
						{
							_ret.val_void_ptr = nullptr;
						}
					}
				}
			}
			return _ret;
		}

		variant class_r::_invoke_v(void* instance_
				, rtype* rtype_
				, rtype* owner_
				, const std::string& method_name
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9) STD_THROW
		{
			if(nullptr==instance_)
			{
				StdThrow(std::runtime_error, "nullptr found");
			}

			bind_base* ptr_bind_base = nullptr;
			rmethod& method = type_manager::__get_method(instance_, (class_r*)rtype_, method_name, owner_, ptr_bind_base);
			return _invoke_v(method, ptr_bind_base, method_name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}

		variant class_r::invoke_v(reflect* instance_, const std::string& method_name
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9) STD_THROW
		{
			if(nullptr==instance_)
			{
				StdThrow(std::runtime_error, "nullptr found");
			}
			return _invoke_v(instance_->get_derived()
				, instance_->_ptr_derived_class_r
				, instance_->_ptr_class_r
				, method_name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}

		variant rmethod::invoke_v(reflect* instance_
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9) STD_THROW
		{
			if(nullptr==_owner_)
			{
				StdThrow(std::runtime_error, "nullptr found");
			}
			return ((class_r*)_owner_)->invoke_v(instance_, _name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}

		variant make_variant(rmethod& method, uint index, const std::string& a)
		{
			variant _variant;
			if(index<method._params.size())
			{
				rtype* rtype_ = method._params[index];
				_variant.make_val(rtype_->get_data_type(), a);
			}
			return _variant;
		}

		std::string class_r::_invoke(void* instance_
				, rtype* rtype_
				, rtype* owner_
				, const std::string& method_name
				, const std::string& a1
				, const std::string& a2
				, const std::string& a3
				, const std::string& a4
				, const std::string& a5
				, const std::string& a6
				, const std::string& a7
				, const std::string& a8
				, const std::string& a9)
		{
			if(nullptr==instance_)
			{
				StdThrow(std::runtime_error, "nullptr found");
			}

			bind_base* ptr_bind_base = nullptr;
			rmethod& method = type_manager::__get_method(instance_, (class_r*)rtype_, method_name, owner_, ptr_bind_base);

			variant _a1 = make_variant(method, 0, a1);
			variant _a2 = make_variant(method, 1, a2);
			variant _a3 = make_variant(method, 2, a3);
			variant _a4 = make_variant(method, 3, a4);
			variant _a5 = make_variant(method, 4, a5);
			variant _a6 = make_variant(method, 5, a6);
			variant _a7 = make_variant(method, 6, a7);
			variant _a8 = make_variant(method, 7, a8);
			variant _a9 = make_variant(method, 8, a9);
			variant _ret = _invoke_v(method, ptr_bind_base, method_name, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8, _a9);

			return _ret.to_string();
		}

		std::string class_r::invoke(reflect* instance_, const std::string& method_name
				, const std::string& a1
				, const std::string& a2
				, const std::string& a3
				, const std::string& a4
				, const std::string& a5
				, const std::string& a6
				, const std::string& a7
				, const std::string& a8
				, const std::string& a9) STD_THROW
		{
			if(nullptr==instance_)
			{
				StdThrow(std::runtime_error, "nullptr found");
			}
			return _invoke(instance_->get_derived()
				, instance_->_ptr_derived_class_r
				, instance_->_ptr_class_r
				, method_name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}

		std::string rmethod::invoke(reflect* instance_
				, const std::string& a1
				, const std::string& a2
				, const std::string& a3
				, const std::string& a4
				, const std::string& a5
				, const std::string& a6
				, const std::string& a7
				, const std::string& a8
				, const std::string& a9) STD_THROW
		{
			if(nullptr==_owner_)
			{
				StdThrow(std::runtime_error, "nullptr found");
			}
			return ((class_r*)_owner_)->invoke(instance_, _name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
		}

		void class_r::serialize(const reflect* instance_, std::string& out) const STD_THROW
		{
			if(nullptr==_ptr_serializer)
			{
				StdThrow(std::runtime_error, "nullptr");
			}
			_ptr_serializer->serialize(*instance_, out);
		}

		void class_r::deserialize(reflect* instance_, const std::string& in) STD_THROW
		{
			if(nullptr==_ptr_serializer)
			{
				StdThrow(std::runtime_error, "nullptr");
			}
			_ptr_serializer->deserialize(*instance_, in);
		}
	}
}