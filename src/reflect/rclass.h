/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "variant.h"
#include "rtype.h"

namespace gce
{
	namespace reflection
	{
		struct bind_base;

		///class type to execute reflection
		class DSL_EXPORT class_r : public rtype
		{
		public:
			reflect* create_instance() const;
			void destroy_instance(reflect*& p) const;

			///add member
			void add_member(const std::string& name, const std::string& member_type) STD_THROW;
			rmember& _get_member(reflect*& instance_, ref_data*& ref_data_, const std::string& _name, void** pp);
			rmethod& get_method(const std::string& method_name) STD_THROW;
			void set_value_v(reflect* instance_, const std::string& _name, const variant& _value) STD_THROW;
			variant get_value_v(reflect* instance_, const std::string& _name) STD_THROW;

			void set_value(reflect* instance_, const std::string& _name, const std::string& _value) STD_THROW;
			std::string get_value(reflect* instance_, const std::string& _name) STD_THROW;

			reflect* cast_to_type(reflect* p, const std::string& name);

			variant invoke_v(reflect* instance_, const std::string& method_name
				, const variant& a1=_default_variant
				, const variant& a2=_default_variant
				, const variant& a3=_default_variant
				, const variant& a4=_default_variant
				, const variant& a5=_default_variant
				, const variant& a6=_default_variant
				, const variant& a7=_default_variant
				, const variant& a8=_default_variant
				, const variant& a9=_default_variant) STD_THROW;

			variant invoke_v(const std::string& method_name
				, const variant& a1=_default_variant
				, const variant& a2=_default_variant
				, const variant& a3=_default_variant
				, const variant& a4=_default_variant
				, const variant& a5=_default_variant
				, const variant& a6=_default_variant
				, const variant& a7=_default_variant
				, const variant& a8=_default_variant
				, const variant& a9=_default_variant) STD_THROW
			{
				return invoke_v(this, method_name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			}

			std::string invoke(reflect* instance_, const std::string& method_name
				, const std::string& a1=""
				, const std::string& a2=""
				, const std::string& a3=""
				, const std::string& a4=""
				, const std::string& a5=""
				, const std::string& a6=""
				, const std::string& a7=""
				, const std::string& a8=""
				, const std::string& a9="") STD_THROW;

			std::string invoke(const std::string& method_name
				, const std::string& a1=""
				, const std::string& a2=""
				, const std::string& a3=""
				, const std::string& a4=""
				, const std::string& a5=""
				, const std::string& a6=""
				, const std::string& a7=""
				, const std::string& a8=""
				, const std::string& a9="") STD_THROW
			{
				return invoke(this, method_name, a1, a2, a3, a4, a5, a6, a7, a8, a9);
			}

			void serialize(const reflect* instance_, std::string& out) const STD_THROW;
			void deserialize(reflect* instance_, const std::string& in) STD_THROW;

		protected:
			reflect* _create_instance(void* buf, void* derived_, class_r* assigned_) const;
			void _destroy_instance(void* p) const;

			void _set_value_v(reflect* instance_, ref_data* ref_data_, rmember& _rmember, void* p, const std::string& _name, const variant& _value);
			variant _get_value_v(rmember& _rmember, void* p, const std::string& _name);

			variant _invoke_v(rmethod& method, bind_base* ptr_bind_base, const std::string& method_name
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9) STD_THROW;

			variant _invoke_v(void* instance_
				, rtype* rtype_
				, rtype* owner_
				, const std::string& method_name
				, const variant& a1=_default_variant
				, const variant& a2=_default_variant
				, const variant& a3=_default_variant
				, const variant& a4=_default_variant
				, const variant& a5=_default_variant
				, const variant& a6=_default_variant
				, const variant& a7=_default_variant
				, const variant& a8=_default_variant
				, const variant& a9=_default_variant) STD_THROW;

			std::string _invoke(void* instance_
				, rtype* rtype_
				, rtype* owner_
				, const std::string& method_name
				, const std::string& a1=""
				, const std::string& a2=""
				, const std::string& a3=""
				, const std::string& a4=""
				, const std::string& a5=""
				, const std::string& a6=""
				, const std::string& a7=""
				, const std::string& a8=""
				, const std::string& a9="");

			friend class type_manager;
		};
	}
}