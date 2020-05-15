/*  
    gcrt is a general C++ runtime library.
    Copyright (C) 2014

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/ 

#pragma once

namespace gce
{
#pragma pack(push)
#pragma pack(1)
	///placeholder
	class Place
	{
	public:
		Place()
		{
			init();
		}

		inline bool is_valid() const
		{
			return (long_t)value== _place;
		}

		inline void init()
		{
			 _place = (long_t)value;
		}

		inline void reset()
		{
			 _place = 0;
		}

	protected:
		long_t  _place;

	public:
		const static int64 value = 0x68A3B972F5EFC851;
	};
#pragma pack(pop)

	///cheking pointer being valid(or dangling), non-throw version
	static bool runtime_check_no_throw(void* ptr_object)
	{
#ifdef RUNTIME_HELPER
		return nullptr!=ptr_object && runtime::compare_memory_unwinding<(ulong_t)Place::value>(ptr_object);
#else
		return true;
#endif
	}

	///cheking pointer being valid(or dangling), throw version
	static void runtime_check(void* ptr_object) STD_THROW
	{
#ifdef RUNTIME_HELPER
		if(!runtime_check_no_throw(ptr_object))
		{
#ifdef PLATFORM_64
			StdThrow(std::runtime_error, "invalid pointer is detected at 0x%016llX\n", (uint64)ptr_object);
#else
			StdThrow(std::runtime_error, "invalid pointer is detected at 0x%08X\n", (uint)ptr_object);
#endif
		}
#endif
	}

	///the top base class within this library. all the subclass can be applied to runtime_check at runtime.
	template<class Derived>
	class DISABLE_VTABLE Object
	{
	protected:
		Object(){}
		~Object(){}

		Object(const Object& other)
		{
			*this = other;
		}

	public:
		static void* _malloc(size_t size)
		{
#ifdef RUNTIME_HELPER
			Place* p = (Place*)gce::runtime::global_malloc(sizeof(Place)+size);
			p->init();
			return p+1;
#else
			return gce::runtime::global_malloc(size);
#endif
		}

		static void* _malloc(size_t size, const char *file_name, const char *func_name, int line)
		{
#ifdef RUNTIME_HELPER
			Place* p = (Place*)gce::runtime::global_malloc(sizeof(Place)+size, file_name, func_name, line);
			p->init();
			return p+1;
#else
			return gce::runtime::global_malloc(size, file_name, func_name, line);
#endif
		}

		static void _free(void* p)
		{
#ifdef RUNTIME_HELPER
			Place* place = ((Place*)p)-1;
			place->reset();
			p = place;
#endif
			gce::runtime::global_free(p);
		}

		inline bool runtime_check_no_throw() const
		{
			return gce::runtime_check_no_throw(_get_derived());
		}

		inline void runtime_check() const STD_THROW
		{
			gce::runtime_check(_get_derived());
		}

	public:
		void *operator new(size_t size)
		{
			return _malloc(size);
		}

		void *operator new(size_t size, void *_Where)
		{
			return _Where;
		}

		void *operator new[](size_t size)
		{
			return _malloc(size);
		}

		void operator delete(void *p)
		{
			if(p!=nullptr)
			{
				_free(p);
			}
		}

		void operator delete(void* p, void*)
		{
		}

		void operator delete[](void *p)
		{
			if(p!=nullptr)
			{
				_free(p);
			}
		}

#ifdef ALLOC_TRACE
		void* operator new(size_t size, const char *file_name, const char *func_name, int line)
		{
			return _malloc(size, file_name, func_name, line);
		}

		void* operator new[](size_t size, const char *file_name, const char *func_name, int line)
		{
			return _malloc(size, file_name, func_name, line);
		}

		void operator delete(void* p, const char *file_name, const char *func_name, int line)
		{
			_free(p);
		}

		void operator delete[](void* p, const char *file_name, const char *func_name, int line)
		{
			_free(p);
		}
#endif

	protected:
		inline Derived* _get_derived() const
		{
			return (Derived*)this;
		}

	public:
		inline void _destruct_derived()
		{
			_get_derived()->~Derived();
		}

		inline void Clone(const Derived &other)
		{
			::memcpy(_get_derived(), &other, sizeof(Derived));
		}

		inline bool Equals(const Derived &other) const
		{
			return ::memcmp(_get_derived(), &other, sizeof(Derived))==0;
		}

		inline uint64 HashCode() const
		{
			return (uint64)this;
		}

		inline std::string to_type_string() const
		{
			return _typeid(typeid(Derived)).name();
		}
	};

	template<class Derived>
	class vobject : public Object<Derived>
	{
	public:
		virtual ~vobject(){}

		template<class Base>
		bool is_derived_class_of(const Base* ptr_base)
		{
			bool ret_val = false;
			try
			{
				ret_val = dynamic_cast<Base*>(this)!=nullptr;
			}
			catch(...){}
			return ret_val;
		}

		template<class Base>
		bool is_derived_class()
		{
			return is_derived_class_of<Base>(nullptr);
		}
	};

#ifdef UNICODE
#define declare_to_string\
	inline string_type to_type_string() const\
	{\
	return gce::lib::encoding::string_to_wstring(_typeid(typeid(*this)).name());\
	}
#else
#define declare_to_string\
	inline string_type to_type_string() const\
	{\
	return std::string(_typeid(typeid(*this)).name());\
	}
#endif

	template <typename T> class make_final : public Object<T>
	{
#ifdef _WINDOWS
		friend typename T;
	private:
#else
	public:
#endif
		make_final(){}
		~make_final() {}
	};
}