/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "method_parser.h"

#define macor_parse_params_2\
	PARSE_PARAM(1);\
	PARSE_PARAM(2);

#define declare_bind_2(Name, Const)\
		declare_bind(macro_typename2,\
				Name,\
				Const,\
				COMMA,\
				macro_args2,\
				macro_void_args2,\
				macro_params2\
				)

///bind class and parse member method with 2 arguments
namespace gce
{
	namespace reflection
	{
#ifdef _WINDOWS
		declare_bind_2(bind_2, );
		declare_bind_2(bind_2_const, const);
		declare_bind_static(macro_typename2,
				bind_2_static,
				COMMA,
				macro_args2,
				macro_void_args2,
				macro_params2
				)
#else
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename2,
				bind_2,
				,
				COMMA,
				macro_args2,
				macro_void_args2,
				macro_params2
				);
		declare_bind_base(T::, ((T*)ptr_)->*,
				macro_typename2,
				bind_2_const,
				const,
				COMMA,
				macro_args2,
				macro_void_args2,
				macro_params2
				);
		declare_bind_base(,,
				macro_typename2,
				bind_2_static,
				,
				COMMA,
				macro_args2,
				macro_void_args2,
				macro_params2
				);
#endif
/*
		template <typename R, typename T, typename A1, typename A2>
		struct bind_2_static : public bind_base
		{
			typedef R (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2)
			{
				*(R*)ret_buf = (fn_)((A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2);
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2_static<R*, T, A1, A2> : public bind_base
		{
			typedef R* (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				::memcpy(ret_buf, (R*)(fn_)((A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2), ret_type.get_size());
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2_static<R&, T, A1, A2> : public bind_base
		{
			typedef R& (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				R& r = (R&)(((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				*(void**)ret_buf = &r;
				return (void*)&r;
			}
		};
		template <typename T , typename A1, typename A2>
		struct bind_2_static<void, T, A1, A2> : public bind_base
		{
			typedef void (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				(fn_)((A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2);
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2_static<ret_object<R>, T, A1, A2> : public bind_base
		{
			typedef ret_object<R> (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				if(ret_type.get_data_type()==type_object_reflectable)
				{
					((reflect*)ret_buf)->clone((const void*)&(reflect&)((fn_)((A1)*(_arg<A1>*)a1 COMMA (A2)*(_arg<A2>*)a2)));
				}
				else
				{
					*(R*)ret_buf = (R&)(fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				}
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2_static<ret_object<R&>, T, A1, A2> : public bind_base
		{
			typedef ret_object<R&> (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
		};
		template <typename T , typename A1, typename A2>
		struct bind_2_static<ret_object<void>, T, A1, A2> : public bind_base
		{
			typedef ret_object<void> (*fn)(A1, A2);
			fn fn_;
			bind_2_static(fn f,T* p) : bind_base(p), fn_(f){}
		};
		*/
		/*
		template <typename R, typename T, typename A1, typename A2>
		struct bind_2 : public bind_base
		{
			typedef R (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2)
			{
				*(R*)ret_buf = (((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2<R*, T, A1, A2> : public bind_base
		{
			typedef R* (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				*(void**)ret_buf = (void*)(R*)(((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2<R&, T, A1, A2> : public bind_base
		{
			typedef R& (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				R& r = (R&)(((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				*(void**)ret_buf = &r;
				return (void*)&r;
			}
		};
		template <typename T , typename A1, typename A2>
		struct bind_2<void, T, A1, A2> : public bind_base
		{
			typedef void (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				(((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2<ret_object<R>, T, A1, A2> : public bind_base
		{
			typedef ret_object<R> (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
			virtual void* execute(const argument_type& ret_type, void* ret_buf , void* a1, void* a2)
			{
				if(ret_type.get_data_type()==type_object_reflectable)
				{
					((reflect*)ret_buf)->clone((const void*)&(reflect&)((((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2)));
				}
				else
				{
					*(R*)ret_buf = (R&)(((T*)ptr_)->*fn_)((A1)*(_arg<A1>*)a1, (A2)*(_arg<A2>*)a2);
				}
				return ret_buf;
			}
		};
		template <typename R, typename T , typename A1, typename A2>
		struct bind_2<ret_object<R&>, T, A1, A2> : public bind_base
		{
			typedef ret_object<R&> (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
		};
		template <typename T , typename A1, typename A2>
		struct bind_2<ret_object<void>, T, A1, A2> : public bind_base
		{
			typedef ret_object<void> (T::*fn)(A1, A2);
			fn fn_;
			bind_2(fn f,T* p) : bind_base(p), fn_(f){}
		};
		*/

		template<typename R, typename T, typename A1, typename A2>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2))
		{
			macro_init_method(T, false);
			macor_parse_params_2;
#ifdef _WINDOWS
			parse_bind(bind_2, , COMMA, macro_args2);
#else
			parse_bind_base(T, T::, bind_2, , COMMA, macro_args2);
#endif
		}
		template<typename R, typename T, typename A1, typename A2>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)(A1, A2) const)
		{
			macro_init_method(T, false);
			macor_parse_params_2;
#ifdef _WINDOWS
			parse_bind(bind_2_const, const, COMMA, macro_args2);
#else
			parse_bind_base(T, T::, bind_2_const, const, COMMA, macro_args2);
#endif
		}
		template<typename R, typename A1, typename A2>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)(A1, A2))
		{
			macro_init_method(placeholder, true);
			macor_parse_params_2;
#ifdef _WINDOWS
			parse_bind_static(bind_2_static, COMMA, macro_args2);
#else
			parse_bind_base(placeholder, , bind_2_static, , COMMA, macro_args2);
#endif
		}
	}
}