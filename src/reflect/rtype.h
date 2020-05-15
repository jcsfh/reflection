/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include <vector>
#include <string>

#include "variant.h"
#include "template_meta/reflect.h"
#include "meta.h"
#include "macro/macro_base.h"

namespace gce
{
	namespace reflection
	{
		enum
		{
			Public,
			Private,
			Protected,
		};

		///member for reflection
		struct rmember
		{
			int _index;
			char _accessible;
			bool _is_static;
			std::string _name;
			union
			{
				uint _offset;
				void* _address;
			};
			rtype* _type_;

			rmember()
				: _index(-1)
				, _accessible(Public)
				, _is_static(false)
				, _address(0)
				, _type_(nullptr)
			{
			}

			rmember(rmember &&other)
			{
				*this = std::move(other);
			}

			rmember& operator = (rmember &&other)
			{
				if(this!=&other)
				{
					_accessible = other._accessible;
					_address = other._address;
					_name = std::move(other._name);
					_type_ = std::move(other._type_);
				}
				return *this;
			}

			inline std::string name() const
			{
				return _name;
			}

			inline const class_r& get_class() const
			{
				return *(const class_r*)_type_;
			}

			inline bool is_static() const
			{
				return _is_static;
			}
		};

		///method for reflection
		struct DSL_EXPORT rmethod
		{
			char _accessible;
			bool _is_static;
			bool _is_virtual;
			std::string _name;
			rtype* _return_type_;
			std::vector<rtype*> _params;
			void* _bind_addr_;
			rtype* _owner_;

			rmethod()
				: _accessible(Public)
				, _is_static(false)
				, _is_virtual(false)
				, _return_type_(nullptr)
				, _bind_addr_(nullptr)
				, _owner_(nullptr)
			{
			}

			~rmethod()
			{
				delete (char*)_bind_addr_;
			}

			inline bool is_static() const
			{
				return _is_static;
			}

			inline std::string name() const
			{
				return _name;
			}

			inline const class_r& return_type() const
			{
				return *(const class_r*)_return_type_;
			}

			inline const std::vector<class_r*>& params() const
			{
				return *__to_type<std::vector<class_r*>*>((void*)&_params);
			}

			variant invoke_v(reflect* instance_
				, const variant& a1=_default_variant
				, const variant& a2=_default_variant
				, const variant& a3=_default_variant
				, const variant& a4=_default_variant
				, const variant& a5=_default_variant
				, const variant& a6=_default_variant
				, const variant& a7=_default_variant
				, const variant& a8=_default_variant
				, const variant& a9=_default_variant) STD_THROW;

			variant invoke_v(
				const variant& a1=_default_variant
				, const variant& a2=_default_variant
				, const variant& a3=_default_variant
				, const variant& a4=_default_variant
				, const variant& a5=_default_variant
				, const variant& a6=_default_variant
				, const variant& a7=_default_variant
				, const variant& a8=_default_variant
				, const variant& a9=_default_variant) STD_THROW
			{
				return invoke_v((reflect*)_owner_, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			}

			std::string invoke(reflect* instance_
				, const std::string& a1=""
				, const std::string& a2=""
				, const std::string& a3=""
				, const std::string& a4=""
				, const std::string& a5=""
				, const std::string& a6=""
				, const std::string& a7=""
				, const std::string& a8=""
				, const std::string& a9="") STD_THROW;

			std::string invoke(
				const std::string& a1=""
				, const std::string& a2=""
				, const std::string& a3=""
				, const std::string& a4=""
				, const std::string& a5=""
				, const std::string& a6=""
				, const std::string& a7=""
				, const std::string& a8=""
				, const std::string& a9="") STD_THROW
			{
				return invoke((reflect*)_owner_, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			}

			template<typename R, typename T>
			R invoke(T* p) STD_THROW;

			template<typename R, typename T, macro_typename1>
			R invoke(T* p, macro_const_params1) STD_THROW;

			template<typename R, typename T, macro_typename2>
			R invoke(T* p, macro_const_params2) STD_THROW;

			template<typename R, typename T, macro_typename3>
			R invoke(T* p, macro_const_params3) STD_THROW;

			template<typename R, typename T, macro_typename4>
			R invoke(T* p, macro_const_params4) STD_THROW;

			template<typename R, typename T, macro_typename5>
			R invoke(T* p, macro_const_params5) STD_THROW;

			template<typename R, typename T, macro_typename6>
			R invoke(T* p, macro_const_params6) STD_THROW;

			template<typename R, typename T, macro_typename7>
			R invoke(T* p, macro_const_params7) STD_THROW;

			template<typename R, typename T, macro_typename8>
			R invoke(T* p, macro_const_params8) STD_THROW;

			template<typename R, typename T, macro_typename9>
			R invoke(T* p, macro_const_params9) STD_THROW;
		};

		struct rparent
		{
			rtype* rtype_;
			int offset;
			int ref_data_offset;
			int reflect_offset;
			std::map<std::string, rparent> list_next_parent;

			rparent()
				: rtype_(nullptr)
				, offset(0)
				, ref_data_offset(0)
				, reflect_offset(0)
			{
			}

			rparent(const rparent &other)
			{
				*this = other;
			}

			rparent(rparent &&other)
			{
				*this = std::move(other);
			}

			void shallow_copy(const rparent &other)
			{
				if(this!=&other)
				{
					rtype_ = other.rtype_;
					offset = other.offset;
					ref_data_offset = other.ref_data_offset;
					reflect_offset = other.reflect_offset;
				}
			}

			rparent& operator = (const rparent &other)
			{
				if(this!=&other)
				{
					shallow_copy(other);
					list_next_parent = other.list_next_parent;
				}
				return *this;
			}

			rparent& operator = (rparent &&other)
			{
				if(this!=&other)
				{
					shallow_copy(other);
					list_next_parent = std::move(other.list_next_parent);
				}
				return *this;
			}
		};

		typedef std::map<std::string, rparent> map_classes;
		typedef std::map<std::string, gce::reflection::rmethod> map_methods;
		typedef std::map<std::string, gce::reflection::rmember> map_members;

		typedef map_classes& class_res;
		typedef map_methods& rmethods;
		typedef map_members& rmembers;

		class class_data
		{
		protected:
			std::map<std::string, rparent> _parents;
			std::map<std::string, rmember> _members;

		public:
			mutable std::map<std::string, rmethod>* _methods_;
			mutable std::map<uint, rmember*> _index_members;

		public:
			class_data()
				: _methods_(nullptr)
			{
			}

			inline const class_res parents() const
			{
				return (const class_res)_parents;
			}

			inline bool has_method() const
			{
				return _methods_!=nullptr;
			}

			inline const rmethods methods() const
			{
				return (const rmethods)*_methods_;
			}

			const rmembers members() const
			{
				return (const rmembers)_members;
			}
		};

		///type for reflection
		class rtype : public rdata, public class_data
		{
		public:
			rtype()
			{
			}

			void clone(const void* ptr)
			{
				*this = *(rtype*)ptr;
			}

		protected:
			rtype& operator=(const rtype& other)
			{
				return *this;
			}

			mutable ref_data _ref_data;

		public:
			friend class type_manager;
		};
	}
}