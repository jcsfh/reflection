/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "placeholder.h"
#include "rtype.h"
#include "serializer.h"

#include "type_info.h"
#include "rclass.h"

class Nil{};

namespace gce
{
	namespace reflection
	{
		///reflectable base class, supporting 9 base classes
		template<class Derived, 
			typename B1=reflect_placeholder1, ///base class
			typename B2=reflect_placeholder2, 
			typename B3=reflect_placeholder3, 
			typename B4=reflect_placeholder4, 
			typename B5=reflect_placeholder5, 
			typename B6=reflect_placeholder6, 
			typename B7=reflect_placeholder7, 
			typename B8=reflect_placeholder8, 
			typename B9=reflect_placeholder9>
		class DSL_EXPORT reflectable : 
			virtual public reflect,
			virtual public gce::Inception<Derived, false, true>,
			virtual public B1, 
			virtual public B2, 
			virtual public B3, 
			virtual public B4, 
			virtual public B5, 
			virtual public B6, 
			virtual public B7, 
			virtual public B8, 
			virtual public B9
		{
		public:
			typedef reflectable<Derived, B1, B2, B3, B4, B5, B6, B7, B8, B9> base;
			typedef Derived derived_type;

		protected:
			reflectable()
			{
				reflect::_ptr_class_r = _class_r_;
				reflect::_ptr_derived_class_r = _class_r_;
				reflect::_ptr_static_instance = (reflect*)&_get_instance();
				reflect::_ptr_derived_to_reflect = this;

				if(_class_r_!=nullptr)
				{
					_ref_data._reflect_ = _class_r_->_reflect_;
					_ref_data._reflect_offset = _class_r_->_reflect_offset;
					_ref_data._ref_data_offset = _class_r_->_ref_data_offset;
					_ref_data._create_flag(*_class_r_);
				}
			}

			~reflectable()
			{
			}

			void clone(const void* ptr)
			{
				*(Derived*)(void*)(this) = *(Derived*)ptr;
			}

		public:
			static class_r& get_class()
			{
				return *_class_r_;
			}

			virtual void serialize(std::string& out) const STD_THROW
			{
				if(nullptr==_ptr_serializer)
				{
					StdThrow(std::runtime_error, "nullptr");
				}
				_ptr_serializer->serialize(*this, out);
			}

			virtual void deserialize(const std::string& in) STD_THROW
			{
				if(nullptr==_ptr_serializer)
				{
					StdThrow(std::runtime_error, "nullptr");
				}
				_ptr_serializer->deserialize(*this, in);
			}

			uint get_derived_size() const
			{
				return sizeof(Derived);
			}

			void* _to_derived_pointer() const
			{
				return dynamic_cast<Derived*>((base*)this);
			}

		protected:
			template<typename Placeholder, typename B>
			static void _parse_parent_template(void* p, class_res parent)
			{
				if(typeid(Placeholder)!=typeid(B))
				{
					try
					{
						rparent _rparent;
						const gce::reflection::type_info& ti = type_info::typeof<B>();
						_rparent.rtype_ = (gce::reflection::rtype*)&ti.get_type();
						Derived* derived = (Derived*)p;
						B* b_ = dynamic_cast<B*>(derived);
						_rparent.offset = (int)((uint64)b_-(uint64)derived);
						_rparent.ref_data_offset = (int)((uint64)&b_->_get_ref_data()-(uint64)b_);
						_rparent.reflect_offset = (int)((uint64)dynamic_cast<reflect*>(b_)-(uint64)b_);
						B1::_parse_parent((B*)derived, _rparent.list_next_parent);
						parent.insert(std::move(std::make_pair(ti.get_type().get_name(), _rparent)));
					}
					catch(...){}
				}
			}

		public:
			static void _parse_parent(void* p, class_res parent)
			{
				_parse_parent_template<reflect_placeholder1, B1>(p, parent);
				_parse_parent_template<reflect_placeholder2, B2>(p, parent);
				_parse_parent_template<reflect_placeholder3, B3>(p, parent);
				_parse_parent_template<reflect_placeholder4, B4>(p, parent);
				_parse_parent_template<reflect_placeholder5, B5>(p, parent);
				_parse_parent_template<reflect_placeholder6, B6>(p, parent);
				_parse_parent_template<reflect_placeholder7, B7>(p, parent);
				_parse_parent_template<reflect_placeholder8, B8>(p, parent);
				_parse_parent_template<reflect_placeholder9, B9>(p, parent);
			}

			static void _parse_parent()
			{
				Derived* derived_ = &_get_instance();
				_class_r_->_ref_data_offset = (int)((uint64)&derived_->_ref_data - (uint64)derived_);
				_class_r_->_reflect_offset = (int)((uint64)dynamic_cast<reflect*>(derived_) - (uint64)derived_);

				rparent parent;
				_parse_parent(derived_, _class_r_->parents());
			}

			static void _do_incept(bool check_name=false)
			{
				if(_class_r_!=nullptr)
				{
					return;
				}

				std::string raw_name = _typeid(typeid(Derived)).name();
				std::string name = type_manager::_get_existed_class_name(raw_name);
				if(check_name && !name.empty())
				{
					StdThrow(std::runtime_error, "Derived(%s): wrong type or it has existed.", name.c_str());
				}
				name = reflect::get_type_name(raw_name);

				const type_info& ti = type_info::typeof<Derived>();
				_class_r_ = (class_r*)&ti.get_type();
				_class_r_->_name = name;
				_class_r_->_ptr_static_instance = (reflect*)&_get_instance();
				_class_r_->_ptr_class_r = _class_r_;
				_class_r_->_ptr_derived_class_r = _class_r_;

				_class_r_->_methods_ = &_get_methods();

				_parse_parent();

				gce::runtime::init_loader::register_cleaner_callback(_dispose);
			}

			static void _incept()
			{
				_do_incept(true);
			}

			static void _dispose()
			{
				if(gce::Inception<Derived, false, true>::_initialized)
				{
					gce::Inception<Derived, false, true>::_initialized = false;
					delete &_get_instance();
					delete &_get_methods();
				}
			}

		protected:
			static rmethods _get_methods()
			{
				static map_methods* instance_ = new map_methods;
				return *instance_;
			}

			static Derived& _get_instance()
			{
				static Derived* instance_ = nullptr;
				static bool _is_created = false;
				if(!_is_created)
				{
					_is_created = true;
					instance_ = new Derived;
				}
				return *instance_;
			}

			template<typename T>
			static uint64 _parse_offset_or_addr(uint64 (T::*fn)() const)
			{
				return (T::_get_instance().*fn)();
			}

#pragma push_macro("new")
#undef new
		protected:
			void _set_derived(void* derived_) const
			{
				reflect::_ptr_derived_to_reflect = (reflect*)derived_;

				B1::_set_derived(derived_);
				B2::_set_derived(derived_);
				B3::_set_derived(derived_);
				B4::_set_derived(derived_);
				B5::_set_derived(derived_);
				B6::_set_derived(derived_);
				B7::_set_derived(derived_);
				B8::_set_derived(derived_);
				B9::_set_derived(derived_);
			}

			reflect* _create_instance(void* buf, void* derived_, class_r* assigned_) const
			{
				::new(buf) Derived;

				reflect* p = (reflect*)(Derived*)buf;
				p->_ptr_derived_class_r = assigned_;
				_set_derived(derived_);

				reflect::_ptr_to_derived = derived_;
				p->_ptr_to_derived = derived_;
				return p;
			}

			void _destroy_instance(reflect* p) const
			{
				if(p!=nullptr)
				{
					delete dynamic_cast<Derived*>(p);
				}
			}

			void _destroy_instance(void* p) const
			{
				if(p!=nullptr)
				{
					((Derived*)p)->~Derived();
				}
			}

		public:
			reflect* create_instance() const
			{
				return dynamic_cast<reflect*>(new Derived);
			}

			void destroy_instance(reflect*& p) const
			{
				_destroy_instance(p);
				p = nullptr;
			}

			void* operator new(size_t size)
			{
				return gce::Object<Derived>::_malloc(size);
			}

			void *operator new(size_t size, void *_Where)
			{
				return _Where;
			}

			void *operator new[](size_t size)
			{
				return gce::Object<Derived>::_malloc(size);
			}

			void operator delete(void *p)
			{
				if(p!=nullptr)
				{
					gce::Object<Derived>::_free(p);
				}
			}

			void operator delete(void* p, void*)
			{
			}

			void operator delete[](void *p)
			{
				if(p!=nullptr)
				{
					gce::Object<Derived>::_free(p);
				}
			}

#ifdef ALLOC_TRACE
			void* operator new(size_t size, const char *file_name, const char *func_name, int line)
			{
				return gce::Object<Derived>::_malloc(size, file_name, func_name, line);
			}

			void* operator new[](size_t size, const char *file_name, const char *func_name, int line)
			{
				return gce::Object<Derived>::_malloc(size, file_name, func_name, line);
			}

			void operator delete(void* p, const char *file_name, const char *func_name, int line)
			{
				gce::Object<Derived>::_free(p);
			}

			void operator delete[](void* p, const char *file_name, const char *func_name, int line)
			{
				gce::Object<Derived>::_free(p);
			}
#endif

#pragma pop_macro("new")

		private:
			inline bool runtime_check_no_throw() const
			{
				return false;
			}

			inline void runtime_check() const STD_THROW
			{
			}

		public:
			ref_data& _get_ref_data()
			{
				return _ref_data;
			}
			ref_data _ref_data;

		protected:
			static class_r* _class_r_;

			friend class reflect;
			friend class type_info;
			friend class type_manager;
		};

		template<class Derived, 
			typename B1, 
			typename B2, 
			typename B3, 
			typename B4, 
			typename B5, 
			typename B6, 
			typename B7, 
			typename B8, 
			typename B9>
			class_r* reflectable<Derived, B1, B2, B3, B4, B5, B6, B7, B8, B9>::_class_r_;

		///template reflectable base class, supporting 9 template arguments
		template<
			typename T1=reflect_placeholder1, 
			typename T2=reflect_placeholder2, 
			typename T3=reflect_placeholder3, 
			typename T4=reflect_placeholder4, 
			typename T5=reflect_placeholder5, 
			typename T6=reflect_placeholder6, 
			typename T7=reflect_placeholder7, 
			typename T8=reflect_placeholder8, 
			typename T9=reflect_placeholder9
		>
		class template_types
		{
			template<typename Placeholder, typename T>
			static void __parse_template_argument(rtype* _class_r_)
			{
				if(typeid(Placeholder)!=typeid(T))
				{
					try
					{
						const gce::reflection::type_info& ti = type_info::typeof<T>();
						_class_r_->list_sub_type_ptr.push_back((gce::reflection::rtype*)&ti.get_type());
					}
					catch(...)
					{
					}
				}
			}
		public:
			static void _parse_template_type(rtype* _class_r_)
			{
				__parse_template_argument<reflect_placeholder1, T1>(_class_r_);
				__parse_template_argument<reflect_placeholder2, T2>(_class_r_);
				__parse_template_argument<reflect_placeholder3, T3>(_class_r_);
				__parse_template_argument<reflect_placeholder4, T4>(_class_r_);
				__parse_template_argument<reflect_placeholder5, T5>(_class_r_);
				__parse_template_argument<reflect_placeholder6, T6>(_class_r_);
				__parse_template_argument<reflect_placeholder7, T7>(_class_r_);
				__parse_template_argument<reflect_placeholder8, T8>(_class_r_);
				__parse_template_argument<reflect_placeholder9, T9>(_class_r_);
			}
		};

		template<class Derived, 
			typename T=template_types<reflect_placeholder1>, 
			typename B1=reflect_placeholder1, 
			typename B2=reflect_placeholder2, 
			typename B3=reflect_placeholder3, 
			typename B4=reflect_placeholder4, 
			typename B5=reflect_placeholder5, 
			typename B6=reflect_placeholder6, 
			typename B7=reflect_placeholder7, 
			typename B8=reflect_placeholder8, 
			typename B9=reflect_placeholder9>
		class reflectable_template : 
			virtual public reflectable<Derived, B1, B2, B3, B4, B5, B6, B7, B8, B9>
		{
		public:
			typedef reflectable<Derived, B1, B2, B3, B4, B5, B6, B7, B8, B9> base;
			typedef Derived derived_type;

		public:
			void clone(const void* ptr)
			{
				if((copyable*)this!=ptr)
				{
					*(Derived*)(this) = *(Derived*)ptr;
				}
			}

			template<typename A>
			static void _set_template_list()
			{
				gce::reflection::type_info& ti = (gce::reflection::type_info&)type_info::typeof<A>();
				gce::reflection::rtype& type = (gce::reflection::rtype&)ti.get_type();
				type.list_sub_type_ptr = base::_class_r_->list_sub_type_ptr;
			}

			static void _incept()
			{
				base::_incept();
				T::_parse_template_type(base::_class_r_);

				_set_template_list<Derived>();
				_set_template_list<Derived*>();
				_set_template_list<Derived&>();
				_set_template_list<Derived*&>();
				_set_template_list<const Derived*>();
				_set_template_list<const Derived&>();
				_set_template_list<const Derived* const>();
			}
		};
	}
}