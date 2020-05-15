/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

#pragma push_macro("new")
#undef new

namespace gce
{
	template<class T>
	static void __construct(T& obj)
	{
		::new(&obj) T();
	}

	template<class T, typename A1>
	static void __construct(T& obj, const A1& a1)
	{
		::new(&obj) T(a1);
	}

	template<class T, typename A1, typename A2>
	static void __construct(T& obj, const A1& a1, const A2& a2)
	{
		::new(&obj) T(a1, a2);
	}

	template<class T, typename A1, typename A2, typename A3>
	static void __construct(T& obj, const A1& a1, const A2& a2, const A3& a3)
	{
		::new(&obj) T(a1, a2, a3);
	}

	template<class T, typename A1, typename A2, typename A3, typename A4>
	static void __construct(T& obj, const A1& a1, const A2& a2, const A3& a3, const A4& a4)
	{
		::new(&obj) T(a1, a2, a3, a4);
	}

	template<class T, typename A1, typename A2, typename A3, typename A4, typename A5>
	static void __construct(T& obj, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
	{
		::new(&obj) T(a1, a2, a3, a4, a5);
	}

	template<class T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	static void __construct(T& obj, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
	{
		::new(&obj) T(a1, a2, a3, a4, a5, a6);
	}

	template<class T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
	static void __construct(T& obj, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
	{
		::new(&obj) T(a1, a2, a3, a4, a5, a6, a7);
	}

	template<class T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
	static void __construct(T& obj, const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
	{
		::new(&obj) T(a1, a2, a3, a4, a5, a6, a7, a8);
	}

#pragma push_macro("new")
#undef new
		template<typename T>
		static void __call_constructor(T* p)
		{
			::new(p) T();
		}

		template<typename T>
		static void __call_destructor(T* p)
		{
			p->~T();
		}
#pragma pop_macro("new")

	template<typename A>
	static A __to_type(const void* p)
	{
#ifdef _WINDOWS
		return (A)p;
#else
		union data
		{
			void* p;
			A a;
		};
		data _data;
		_data.p = (void*)p;
		return _data.a;
#endif
	}
}

#pragma pop_macro("new")

///declare local object with Object<> as base class
#define declare(Class, name, ...)\
	struct auto_class_##name\
	{\
		char temp_##name[sizeof(gce::Place)+sizeof(Class)];\
		auto_class_##name()\
		{\
			gce::__construct((gce::Place&)*(gce::Place*)gce::__to_type<gce::Place*>((void*)&temp_##name));\
		}\
		~auto_class_##name()\
		{\
			Class& name = *(Class*)(((gce::Place*)gce::__to_type<gce::Place*>((void*)&temp_##name))+1);\
			((gce::Object<Class>&)name)._destruct_derived();\
		}\
	};\
	auto_class_##name auto_##name;\
	Class& name = *(Class*)(((gce::Place*)gce::__to_type<gce::Place*>((void*)&auto_##name.temp_##name))+1);\
	gce::__construct(name, ##__VA_ARGS__);


///declare local object without inheritance from Object<>
#define declare_ex(Class, name, ...)\
	class object_##name : public Class, public gce::Object<object_##name>\
	{\
	public:\
		object_##name & operator=(const Class & other)\
		{\
			if(this!=&other)\
			{\
				(Class&)*this = other;\
			}\
			return *this;\
		}\
	};\
	struct auto_class_##name\
	{\
		char temp_##name[sizeof(gce::Place)+sizeof(Class)];\
		auto_class_##name()\
		{\
			gce::__construct((gce::Place&)*gce::__to_type<gce::Place*>((void*)&temp_##name));\
		}\
		~auto_class_##name()\
		{\
			object_##name& name = (object_##name&)*(Class*)(((gce::Place*)gce::__to_type<gce::Place*>((void*)&temp_##name))+1);\
			((gce::Object<Class>&)name)._destruct_derived();\
		}\
	};\
	auto_class_##name auto_##name;\
	object_##name& name = (object_##name&)*(Class*)(((gce::Place*)gce::__to_type<gce::Place*>((void*)&auto_##name.temp_##name))+1);\
	gce::__construct((Class&)name, ##__VA_ARGS__);
