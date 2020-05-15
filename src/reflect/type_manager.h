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

#include "rtype.h"
#include "macro/method_parser.h"

#define macro_throw_param(i) StdThrow(std::runtime_error, "bad function call. parameter(%d) doesn't match", i);

///invoking function macro
#define macro_invoke(Index, Comma, Typenames, Args, ArgParams, InvokeParams, Params)\
			template<typename T, typename R Comma Typenames>\
			struct caller##Index\
			{\
				static R _do_invoke(rmethod& method, bind_base* ptr_bind_base Comma ArgParams)\
				{\
					__check_type<R Comma Args>(method);\
					ret_object<R> ret;\
					void* ptr_ret = ptr_bind_base->execute(*method._return_type_, &ret Comma Params);\
					return (R)*(ret_object<R>*)(void*)ptr_ret;\
				}\
				static R _invoke(T* p, const std::string& method_name Comma ArgParams)\
				{\
					bind_base* ptr_bind_base;\
					rmethod& method = __get_method<T>(p, method_name, ptr_bind_base);\
					return _do_invoke(method, ptr_bind_base Comma InvokeParams);\
				}\
			};\
			template<typename T Comma Typenames>\
			struct caller##Index<T, void Comma Args>\
			{\
				static void _do_invoke(rmethod& method, bind_base* ptr_bind_base Comma ArgParams)\
				{\
					__check_type<void Comma Args>(method);\
					ptr_bind_base->execute(*method._return_type_, nullptr Comma Params);\
				}\
				static void _invoke(T* p, const std::string& method_name Comma ArgParams)\
				{\
					bind_base* ptr_bind_base;\
					rmethod& method = __get_method<T>(p, method_name, ptr_bind_base);\
					_do_invoke(method, ptr_bind_base Comma InvokeParams);\
				}\
			};\
			template<typename R, typename T Comma Typenames>\
			static R invoke(T* p, const std::string& method_name Comma ArgParams) STD_THROW\
			{\
				return caller##Index<T, R Comma Args>::_invoke(p, method_name Comma InvokeParams);\
			}\
			template<typename R, typename T Comma Typenames>\
			static R invoke(const std::string& method_name Comma ArgParams) STD_THROW\
			{\
				return caller##Index<T, R Comma Args>::_invoke(nullptr, method_name Comma InvokeParams);\
			}\
			template<typename R, typename T Comma Typenames>\
			static R call_ref(T* p, const std::string& method_name Comma ArgParams) STD_THROW\
			{\
				ref_parser<T> instance(p);\
				return caller##Index<ref_parser<T>, R Comma Args>::_invoke(&instance, method_name Comma InvokeParams);\
			}

namespace gce
{
	namespace reflection
	{
		class rdata;
		class class_r;

		typedef std::map<std::string, class_r> map_uuid_class_r;
		typedef std::map<std::string, std::string> map_name_uuid;
		typedef std::map<std::string, bool> map_class;

		class type_info;

		///manage all types
		class DSL_EXPORT type_manager : public gce::Inception<type_manager, true, true>
		{
			friend class gce::Inception<type_manager, true, true>;
		public:
			static void _incept();
			static void _dispose();

		private:
#ifdef _WINDOWS
			type_manager(){}
#endif

			static uint __check_type(const rmethod& method
				, const variant& a1
				, const variant& a2
				, const variant& a3
				, const variant& a4
				, const variant& a5
				, const variant& a6
				, const variant& a7
				, const variant& a8
				, const variant& a9);

			template<typename R>
			static void __check_return_type(const rmethod& method)
			{
				if(!method._return_type_->_inexact_equal<R>())
				{
					StdThrow(std::runtime_error, "bad function call. return type(%s) doesn't match", _typeid(typeid(R)).name());
				}
			}

			template<uint i, typename A>
			static void __check_arg_type(const rmethod& method)
			{
				if(0==i || i>=method._params.size()+1)
				{
					StdThrow(std::runtime_error, "the number of parameters for %s doesn't match. input number is %d while required is %u", method._name.c_str(), i, (uint)method._params.size());
				}
				if(!method._params[i-1]->_inexact_equal<A>())
				{
					macro_throw_param(i);
				}
			}

			template<uint i>
			static void __check_param_type_dynamically(const rmethod& method, const variant& a)
			{
				meta_type mt;
				meta_type::_create_default_type(a.get_type(), mt);
				if(a.get_type()!=type_void && !method._params[i-1]->_inexact_equal(mt, false))
				{
					macro_throw_param(i);
				}
			}

			template<typename R>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
			}

			template<typename R, macro_typename1>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
			}

			template<typename R, macro_typename2>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
			}

			template<typename R, macro_typename3>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
			}

			template<typename R, macro_typename4>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
				__check_arg_type<4, A4>(method);
			}

			template<typename R, macro_typename5>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
				__check_arg_type<4, A4>(method);
				__check_arg_type<5, A5>(method);
			}

			template<typename R, macro_typename6>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
				__check_arg_type<4, A4>(method);
				__check_arg_type<5, A5>(method);
				__check_arg_type<6, A6>(method);
			}

			template<typename R, macro_typename7>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
				__check_arg_type<4, A4>(method);
				__check_arg_type<5, A5>(method);
				__check_arg_type<6, A6>(method);
				__check_arg_type<7, A7>(method);
			}

			template<typename R, macro_typename8>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
				__check_arg_type<4, A4>(method);
				__check_arg_type<5, A5>(method);
				__check_arg_type<6, A6>(method);
				__check_arg_type<7, A7>(method);
				__check_arg_type<8, A8>(method);
			}

			template<typename R, macro_typename9>
			static void __check_type(const rmethod& method)
			{
				__check_return_type<R>(method);
				__check_arg_type<1, A1>(method);
				__check_arg_type<2, A2>(method);
				__check_arg_type<3, A3>(method);
				__check_arg_type<4, A4>(method);
				__check_arg_type<5, A5>(method);
				__check_arg_type<6, A6>(method);
				__check_arg_type<7, A7>(method);
				__check_arg_type<8, A8>(method);
				__check_arg_type<9, A9>(method);
			}

			static rmethod& __get_bind_method(rtype* rtype_, const std::string& method_name, void** pp, const rtype* owner_);
			static rmethod& __get_method(void* instance_, rtype* rtype_, const std::string& method_name, const rtype* owner_, bind_base*& ptr_bind_base);
			static rmethod& __get_method(reflect* reflect_, class_r* class_r_, const std::string& method_name, const rtype* owner_, bind_base*& ptr_bind_base);

			template<typename T>
			static rmethod& __get_method(T* p, const std::string& method_name, bind_base*& ptr_bind_base)
			{
				return __get_method((reflect*)p, T::_class_r_, method_name, &type_info::typeof<T>().get_type(), ptr_bind_base);
			}

		public:/*
			template<typename T, typename R, typename A1, typename A2>
			struct caller
			{
				static R _invoke(T* p, const std::string& method_name, const A1& a1, const A2& a2)
				{
					bind_base* ptr_bind_base;
					rmethod& method = __get_method<T>(p, method_name, ptr_bind_base);
					__check_type<R, A1, A2>(method);
					ret_object<R> ret;
					void* ptr_ret = ptr_bind_base->execute(*method._return_type_, &ret, &(A1&)a1, &(A2&)a2);
					return (R)*(ret_object<R>*)(void*)ptr_ret;
				}
			};
			template<typename T, typename A1, typename A2>
			struct caller<T, void, A1, A2>
			{
				static void _invoke(T* p, const std::string& method_name, const A1& a1, const A2& a2)
				{
					bind_base* ptr_bind_base;
					rmethod& method = __get_method<T>(p, method_name, ptr_bind_base);
					__check_type<void, A1, A2>(method);
					ptr_bind_base->execute(*method._return_type_, nullptr, &(A1&)a1, &(A2)a2);
				}
			};
			template<typename T, typename R, typename A1, typename A2>
			static R invoke(T* p, const std::string& method_name, const A1& a1, const A2& a2) STD_THROW
			{
				return caller<T, R, A1, A2>::_invoke(p, method_name, a1, a2);
			}
			template<typename T, typename R, typename A1, typename A2>
			static R invoke(const std::string& method_name, const A1& a1, const A2& a2) STD_THROW
			{
				return caller<T, R, A1, A2>::_invoke(nullptr, method_name, a1, a2);
			}
			*/

			///declare callable methods
			macro_invoke(0, , , , , , );
			macro_invoke(1, COMMA, macro_typename1, macro_args1, const A1& a1, a1, &(A1&)a1);
			macro_invoke(2, COMMA, 
				macro_typename2, 
				macro_args2,
				const A1& a1 COMMA const A2& a2, 
				a1 COMMA a2,
				&(A1&)a1 COMMA &(A2&)a2);
			macro_invoke(3, COMMA, 
				macro_typename3, 
				macro_args3,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3, 
				a1 COMMA a2 COMMA a3,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3);
			macro_invoke(4, COMMA, 
				macro_typename4, 
				macro_args4,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4,
				a1 COMMA a2 COMMA a3 COMMA a4,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3 COMMA &(A4&)a4);
			macro_invoke(5, COMMA, 
				macro_typename5, 
				macro_args5,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5,
				a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3 COMMA &(A4&)a4 COMMA &(A5&)a5);
			macro_invoke(6, COMMA, 
				macro_typename6, 
				macro_args6,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6,
				a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3 COMMA &(A4&)a4 COMMA &(A5&)a5 COMMA &(A6&)a6);
			macro_invoke(7, COMMA, 
				macro_typename7, 
				macro_args7,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6 COMMA const A7& a7,
				a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6 COMMA a7,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3 COMMA &(A4&)a4 COMMA &(A5&)a5 COMMA &(A6&)a6 COMMA &(A7&)a7);
			macro_invoke(8, COMMA, 
				macro_typename8, 
				macro_args8,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6 COMMA const A7& a7 COMMA const A8& a8,
				a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6 COMMA a7 COMMA a8,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3 COMMA &(A4&)a4 COMMA &(A5&)a5 COMMA &(A6&)a6 COMMA &(A7&)a7 COMMA &(A8&)a8);
			macro_invoke(9, COMMA, 
				macro_typename9, 
				macro_args9,
				const A1& a1 COMMA const A2& a2 COMMA const A3& a3 COMMA const A4& a4 COMMA const A5& a5 COMMA const A6& a6 COMMA const A7& a7 COMMA const A8& a8 COMMA const A9& a9, 
				a1 COMMA a2 COMMA a3 COMMA a4 COMMA a5 COMMA a6 COMMA a7 COMMA a8 COMMA a9,
				&(A1&)a1 COMMA &(A2&)a2 COMMA &(A3&)a3 COMMA &(A4&)a4 COMMA &(A5&)a5 COMMA &(A6&)a6 COMMA &(A7&)a7 COMMA &(A8&)a8 COMMA &(A9&)a9);

		public:
			static bool is_derived_of(const std::string& derived, const std::string& base);

			static class_r& create_class(const std::string& name
				, const std::string& parent1=""
				, const std::string& parent2=""
				, const std::string& parent3=""
				, const std::string& parent4=""
				, const std::string& parent5=""
				, const std::string& parent6=""
				, const std::string& parent7=""
				, const std::string& parent8=""
				, const std::string& parent9="");
			static class_r& get_class(const std::string& name);

			static reflect* create_instance(const std::string& uuid);

			template<typename T>
			static void register_type()
			{
				type_info::typeof<T>();
			}

			static void* call_method(const std::string& type_name
				, const std::string& method_name
				, void* instance_
				, void* a1=nullptr
				, void* a2=nullptr
				, void* a3=nullptr
				, void* a4=nullptr
				, void* a5=nullptr
				, void* a6=nullptr
				, void* a7=nullptr
				, void* a8=nullptr
				, void* a9=nullptr);

			static void* _call_method(rtype* rtype_
				, const std::string& method_name
				, void* instance_
				, void* a1=nullptr
				, void* a2=nullptr
				, void* a3=nullptr
				, void* a4=nullptr
				, void* a5=nullptr
				, void* a6=nullptr
				, void* a7=nullptr
				, void* a8=nullptr
				, void* a9=nullptr);
			
			static rtype* _register_type(const std::string& uuid);
			static rtype* _register_type(const std::string& name, std::string* ptr_uuid/*in out*/);
			static bool _is_type_existed(const std::string& name);
			static std::string _get_existed_class_name(const std::string& raw_name);
			static void _add_class_name(const std::string& raw_name, const std::string& name);

		private:
			friend class reflect;
			friend class type_info;
			friend class class_r;
			friend bool ::register_rtype(const std::string& name, gce::reflection::rdata*& _rdata);

			static void __clean_type_manager();
			static void __do_register_built_in();

			static map_uuid_class_r* __get_map_uuid_rtype();
			static map_name_uuid* __get_map_name_uuid();
			static map_class* __get_map_class_name();

		protected:
			enum find_type
			{
				type,
				method,
				member,
			};

			static rtype* _get_type_by_uuid(const std::string& uuid);
			static bool _is_exist(rtype* class_r_, find_type _type, const std::string& name);
			static void _find_rtype(rtype* class_r_, find_type _type, const std::string& name, rparent& _rparent, const rtype* owner_);
			static rmember& _find_member(class_r* class_r_, const std::string& name, reflect*& reflect_, ref_data*& ref_data_, void** pp);

			static void _add_parent(const std::string& parent, rtype* rtype_);

		public:
			static rtype* _get_type_by_name(const std::string& name);
		};

		template<typename R, typename T>
		R rmethod::invoke(T* p) STD_THROW
		{
			return type_manager::invoke<R, T>(p, _name);
		}

		template<typename R, typename T, macro_typename1>
		R rmethod::invoke(T* p, macro_const_params1) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args1>(p, _name, macro_in_args1);
		}

		template<typename R, typename T, macro_typename2>
		R rmethod::invoke(T* p, macro_const_params2) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args2>(p, _name, macro_in_args2);
		}

		template<typename R, typename T, macro_typename3>
		R rmethod::invoke(T* p, macro_const_params3) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args3>(p, _name, macro_in_args3);
		}

		template<typename R, typename T, macro_typename4>
		R rmethod::invoke(T* p, macro_const_params4) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args4>(p, _name, macro_in_args4);
		}

		template<typename R, typename T, macro_typename5>
		R rmethod::invoke(T* p, macro_const_params5) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args5>(p, _name, macro_in_args5);
		}

		template<typename R, typename T, macro_typename6>
		R rmethod::invoke(T* p, macro_const_params6) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args6>(p, _name, macro_in_args6);
		}

		template<typename R, typename T, macro_typename7>
		R rmethod::invoke(T* p, macro_const_params7) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args7>(p, _name, macro_in_args7);
		}

		template<typename R, typename T, macro_typename8>
		R rmethod::invoke(T* p, macro_const_params8) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args8>(p, _name, macro_in_args8);
		}

		template<typename R, typename T, macro_typename9>
		R rmethod::invoke(T* p, macro_const_params9) STD_THROW
		{
			return type_manager::invoke<R, T, macro_args9>(p, _name, macro_in_args9);
		}
	}
}