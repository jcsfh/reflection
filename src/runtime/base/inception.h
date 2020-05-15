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
	extern bool DSL __set_type(const ::type_info& ti);
	extern bool DSL __remove_type(const ::type_info& ti);
	extern void DSL __register(std::function<void()> dispose);

	///making the derived class having a likely-static constructor and destructor
	///unique indicates whether the likely-static constructor and destructor will be called only once, 
	///without the influence of multiple modules.
	///_incept is the subclass's static constructor and _dispose as destructor.
	template<class T, bool unique, bool dispose>
	class base_inception
	{
	public:
		base_inception()
		{
			if(&_obj==(base_inception<T, unique, dispose>*)this)
			{
				if(!_initialized)
				{
					_initialized = true;

					if(unique)
					{
						if(!__set_type(typeid(T)))
						{
							if(!dispose)
							{
								__register(T::_dispose);
							}
							T::_incept();
						}
					}
					else
					{
						if(!dispose)
						{
							__register(T::_dispose);
						}
						T::_incept();
					}
				}
			}
		}

		~base_inception()
		{
			if(&_obj==(base_inception<T, unique, dispose>*)this)
			{
				if(_initialized)
				{
					if(unique)
					{
						if(__remove_type(typeid(T)))
						{
							if(dispose)
							{
								T::_dispose();
							}
						}
					}
					else if(dispose)
					{
						T::_dispose();
					}
					_initialized = false;
				}
			}
		}

	protected:
		static bool _initialized;
		static base_inception<T, unique, dispose> _obj;
	};

	template<class T, bool unique, bool dispose>
	bool base_inception<T, unique, dispose>::_initialized = false;

	template<class T, bool unique, bool dispose>
	base_inception<T, unique, dispose> base_inception<T, unique, dispose>::_obj;

	template<class T, bool unique, bool dispose>
	class Inception : public base_inception<T, unique, dispose>
	{
	};

	template<class T, bool unique>
	class Inception<T, unique, true> : public base_inception<T, unique, true>
	{
	};

	template<class T, bool unique>
	class Inception<T, unique, false> : public base_inception<T, unique, false>
	{
	protected:
		static T _instance;
	};

	template<class T, bool unique>
	T Inception<T, unique, false>::_instance;


	extern std::set<std::string>*& get_set_typeinfo();

	namespace runtime
	{
		///the constructor of this class will be executed at first place, before any other constructors!
		class DSL_EXPORT pre_loader : public Inception<pre_loader, true, true>
		{
		public:
			static void _incept();
			static void _dispose();
		};
	}

	template<class T>
	class global_object : public Object<T>
	{
	public:
		global_object()
		{
			__p = new T;
			__register(std::bind(&global_object<T>::__dispose, *this));
		}

		T* operator->()
		{
			return __p;
		}

		T& instance()
		{
			return *__p;
		}

	private:
		void __dispose()
		{
			safe_delete(__p);
		}

	private:
		T* __p;
	};
}