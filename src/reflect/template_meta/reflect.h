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

template<typename T, typename Base>
struct dynamic_caster
{
	static void* cast(T* p)
	{
		try
		{
			return dynamic_cast<Base*>(p);
		}
		catch(...)
		{
			return nullptr;
		}
	}
};

namespace gce
{
	namespace reflection
	{
		class type_info;
		class class_data;
		class rtype;
		class class_r;
		struct serializer_interface;

		///copyable base class
		struct copyable
		{
			virtual ~copyable(){}
			virtual void clone(const void* ptr) = 0;

			template<typename T>
			static bool is_copyable()
			{
				static T t;
				return dynamic_caster<T, copyable>::cast(&t)!=nullptr;
			}
		};

		///reflectable base class
		class DSL_EXPORT reflect : public gce::Object<reflect>, virtual public copyable
		{
		protected:
			reflect();

		public:
			~reflect();

			static void parse_namespace(const std::string& _raw_name, std::string& _name, std::vector<std::string>& _namespaces);
			static void parse_name(std::string& _name, std::vector<std::string>& _namespaces);
			static std::string get_type_name(const std::string& raw_name);

			struct type_data
			{
				std::string name;
				modifier mdf;
				std::vector<std::shared_ptr<type_data> > next_data;
				std::vector<std::shared_ptr<type_data> > sub_data;

				type_data();
				~type_data();
				void clear();

				type_data(type_data&& other)
				{
					*this = std::move(other);
				}

				type_data& operator=(type_data&& other)
				{
					if(this!=&other)
					{
						mdf = other.mdf;

						name = std::move(other.name);
						next_data = std::move(other.next_data);
						sub_data = std::move(other.sub_data);
					}
					return *this;
				}
			};

			inline void set_serializer(serializer_interface* ptr_serializer)
			{
				_ptr_serializer = ptr_serializer;
			}

		protected:
			static std::string _get_type_name(std::vector<std::shared_ptr<type_data> >& _list_template_data);
			static void _parse_template(const std::string& raw_name, std::vector<std::shared_ptr<type_data> >& _list_template_data);

			static void _combine_name(std::string& ret, type_data& _template_data, bool flag);

		protected:
			friend class type_info;
			friend class type_manager;
			friend class class_r;
			friend struct serializer;

		public:
			mutable class_r* _ptr_class_r;
			mutable class_r* _ptr_derived_class_r;
			mutable void* _ptr_to_derived;
			mutable reflect* _ptr_static_instance;

		protected:
			mutable reflect* _ptr_derived_to_reflect;
			mutable serializer_interface* _ptr_serializer;

		public:
			template<typename T>
			static bool is_reflectable(const T* p)
			{
				static T t;
				return dynamic_caster<T, reflect>::cast(&t)!=nullptr;
			}

			template<typename T>
			static bool is_reflectable()
			{
				static T t;
				return is_reflectable(&t);
			}

			void* get_derived() const
			{
				void* p = nullptr;
				if(this->_ptr_to_derived!=nullptr)
				{
					p = this->_ptr_to_derived;
				}
				else
				{
					p = this->_to_derived_pointer();
				}
				return p;
			}

			virtual reflect* create_instance() const = 0;
			virtual void destroy_instance(reflect*& p) const = 0;
			virtual uint get_derived_size() const = 0;

		protected:
			virtual void* _to_derived_pointer() const = 0;
			virtual reflect* _create_instance(void* buf, void* derived_, class_r* assigned_) const{return nullptr;};
			virtual void _destroy_instance(reflect* p) const{};
			virtual void _destroy_instance(void* p) const{};
		};

		class reflect_stub : public reflect
		{
		protected:
			void clone(const void* ptr)
			{
			}

			void* _to_derived_pointer() const
			{
				return (void*)this;
			}

			reflect* create_instance() const
			{
				return (reflect*)this;
			}

			void destroy_instance(reflect*& p) const
			{
			}
			
			uint get_derived_size() const
			{
				return sizeof(*this);
			}
		};

		class DSL_EXPORT ref_data
		{
		public:
			ref_data();
			~ref_data();

			void _create_flag(const rtype& _rtype) const;
			bool& _flag(uint index) const;

			mutable reflect* _reflect_;
			mutable int _ref_data_offset;
			mutable int _reflect_offset;
			mutable std::shared_ptr<bool> _sp_set_flags_;

			friend class class_r;
		};

		class rdata : public ref_data, public reflect_stub, public argument_type
		{
		public:
			rdata()
				: _type_info(nullptr)
			{
			}

			void clone(const void* ptr)
			{
				StdThrow(std::runtime_error, "do not implement");
			}

			std::string _uuid;
			std::vector<std::string> _namespaces;
			std::string _name;
			std::string _raw_name;

		public:
			type_info* _type_info;

			friend class type_info;
			friend class type_manager;
			friend class class_r;

		public:
			const std::string& get_uuid() const
			{
				return _uuid;
			}

			const std::string& get_name() const
			{
				return _name;
			}

			const std::string& get_raw_name() const
			{
				return _raw_name;
			}
		};
	}
}