/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

///bind classes
#define declare_bind_base(Define, Call, Typename, Name, Const, Comma, FuncParams, Args, Converts)\
		template <typename R, typename T Comma Typename>\
		struct Name : public bind_base\
		{\
			typedef R (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
			virtual void* execute(const argument_type& ret_type, void* ret_buf Comma Args)\
			{\
				*(R*)ret_buf = (Call fn_)(Converts);\
				return ret_buf;\
			}\
		};\
		template <typename R, typename T Comma Typename>\
		struct Name<R*, T Comma FuncParams> : public bind_base\
		{\
			typedef R* (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
			virtual void* execute(const argument_type& ret_type, void* ret_buf Comma Args)\
			{\
				*(void**)ret_buf = (void*)(R*)(Call fn_)(Converts);\
				return ret_buf;\
			}\
		};\
		template <typename R, typename T Comma Typename>\
		struct Name<R&, T Comma FuncParams> : public bind_base\
		{\
			typedef R& (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
			virtual void* execute(const argument_type& ret_type, void* ret_buf Comma Args)\
			{\
				R& r = (R&)(Call fn_)(Converts);\
				*(void**)ret_buf = &r;\
				return (void*)&r;\
			}\
		};\
		template <typename T Comma Typename>\
		struct Name<void, T Comma FuncParams> : public bind_base\
		{\
			typedef void (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
			virtual void* execute(const argument_type& ret_type, void* ret_buf Comma Args)\
			{\
				(Call fn_)(Converts);\
				return ret_buf;\
			}\
		};\
		template <typename R, typename T Comma Typename>\
		struct Name<ret_object<R>, T Comma FuncParams> : public bind_base\
		{\
			typedef ret_object<R> (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
			virtual void* execute(const argument_type& ret_type, void* ret_buf Comma Args)\
			{\
				if(ret_type.get_data_type()==type_object_reflectable || ret_type.get_data_type()==type_object_copyable)\
				{\
					ret_object<R> r = (Call fn_)(Converts);\
					((copyable*)ret_buf)->clone((const void*)&(R&)r);\
				}\
				else\
				{\
					ret_object<R> r = (Call fn_)(Converts);\
					*(R*)ret_buf = (R&)r;\
				}\
				return ret_buf;\
			}\
		};\
		template <typename R, typename T Comma Typename>\
		struct Name<ret_object<R&>, T Comma FuncParams> : public bind_base\
		{\
			typedef ret_object<R&> (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
		};\
		template <typename T Comma Typename>\
		struct Name<ret_object<void>, T Comma FuncParams> : public bind_base\
		{\
			typedef ret_object<void> (Define*fn)(FuncParams) Const;\
			fn fn_;\
			Name(fn f,T* p) : bind_base(p), fn_(f){}\
		};

#define declare_bind(Typename, Name, Const, Comma, FuncParams, Args, Converts)\
	declare_bind_base(T::, ((T*)ptr_)->*, Typename, Name, Const, Comma, FuncParams, Args, Converts)

#define declare_bind_static(Typename, Name, Comma, FuncParams, Args, Converts)\
	declare_bind_base(, , Typename, Name, , Comma, FuncParams, Args, Converts)

#define parse_bind_base(T, Define, Name, Const, Comma, FuncParams)\
			auto type_ = (gce::reflection::rtype*)&gce::reflection::type_info::typeof<R>().get_type();\
			if(type_->get_data_type()==type_invalid && !type_->is_pointer() && !type_->is_reference())\
			{\
				StdThrow(std::runtime_error, "return type not supported. it must be reflectable! (%s)", _typeid(typeid(R)).name());\
			}\
			else if(type_->get_data_type()==type_object_reflectable || type_->get_data_type()==type_object_copyable)\
			{\
				typedef ret_object<R> (Define*F)(FuncParams) Const;\
				method._bind_addr_ = new Name<ret_object<R>, T Comma FuncParams>((F)fn, ((T*)nullptr));\
			}\
			else\
			{\
				method._bind_addr_ = new Name<R, T Comma FuncParams>(fn, ((T*)nullptr));\
			}

#define parse_bind(Name, Const, Comma, FuncParams)\
	parse_bind_base(T, T::, Name, Const, Comma, FuncParams)

#define parse_bind_static(Name, Comma, FuncParams)\
	parse_bind_base(placeholder, , Name, , Comma, FuncParams)

namespace gce
{
	namespace reflection
	{
#define BIND_THROW StdThrow(std::runtime_error, "method not implemented");

		struct placeholder
		{
			virtual ~placeholder(){}
		};

		///return object wrapping
		template<typename R>
		struct ret_object
		{
			char buf[sizeof(R)];

			operator R()
			{
				return *__to_type<R*>(buf);
			}

			ret_object()
			{
				__call_constructor((R*)buf);
			}

			~ret_object()
			{
				__call_destructor((R*)buf);
			}
		};

		template<typename R> struct ret_object<R&>
		{
			char buf[sizeof(R*)];
			operator R&()
			{
				return *__to_type<R*>(buf);
			}
		};
		template<typename R> struct ret_object<R*>
		{
			char buf[sizeof(R*)];
			operator R*()
			{
				return *(R**)__to_type<R*>(buf);
			}
		};
		template<typename R> struct ret_object<const R&>
		{
			char buf[sizeof(R*)];
			operator R&()
			{
				return *__to_type<R*>(buf);
			}
		};
		template<typename R> struct ret_object<const R*>
		{
			char buf[sizeof(R*)];
			operator const R*()
			{
				return *(const R**)__to_type<R*>(buf);
			}
		};
		template<> struct ret_object<void>{};

		///argumet object wrapping
		template<typename A>
		struct _arg
		{
			char buf[sizeof(A)];
			operator A&()
			{
				return *__to_type<A*>(buf);
			}
		};
		template<typename A>
		struct _arg<A&>
		{
			char buf[sizeof(A)];
			operator A&()
			{
				return *__to_type<A*>(buf);
			}
		};

		struct bind_base
		{
			void* ptr_;

			bind_base(void* ptr)
				: ptr_(ptr)
			{
			}

			virtual void* execute(const argument_type& ret_type, void* ret_buf){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3, void* a4){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3, void* a4, void* a5){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8){BIND_THROW}
			virtual void* execute(const argument_type& ret_type, void* ret_buf, void* a1, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9){BIND_THROW}
		};

		///bind class and parse member method with 0 argument
		declare_bind(,
					bind_0,
					,
					,
					,
					,
					
					);

		declare_bind(,
					bind_0_const,
					const,
					,
					,
					,
					
					);

		declare_bind_static(,
				bind_0_static,
				,
				,
				,
				
				)

		template<typename R, typename T>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)())
		{
			macro_init_method(T, false);
			parse_bind(bind_0, , , );
		}
		template<typename R, typename T>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (T::*fn)() const)
		{
			macro_init_method(T, false);
			parse_bind(bind_0_const, const, , );
		}
		template<typename R>
		static void __parse_method(const std::string& _name, const gce::reflection::class_data& _type, bool is_virtual,
			R (*fn)())
		{
			macro_init_method(placeholder, true);
			parse_bind_static(bind_0_static, , );
		}
	}
}