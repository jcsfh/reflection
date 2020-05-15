/*  
gce::reflection is a general C++ reflection engine.
Copyright (C) 2015

Version 0.5.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation;
*/

#pragma once

#include "../reflectable.h"

namespace gce
{
#pragma pack(1)
	///for third part
	template<typename T>
	struct ref : public T, virtual public gce::reflection::reflectable<struct ref<T> >
	{
		typedef T derived_type;

		ref(){}
		ref(const ref& other)
		{
			*this = other;
		}
		ref(const T& other)
		{
			*this = other;
		}
		ref& operator=(const ref& other)
		{
			*this = (T&)other;
			return *this;
		}
		ref& operator=(const T& other)
		{
			if((T*)this!=&other)
			{
				(T&)*this = other;
			}
			return *this;
		}
	};

	///for std class
	template<typename Ref, typename T>
	struct ref_parser_base : public gce::reflection::reflectable<Ref>
	{
		ref_parser_base(T* p=nullptr)
			: _instance_(p)
		{
		}
		mutable T* _instance_;

		ref_parser_base<Ref, T>& operator=(const ref_parser_base<Ref, T>& other)
		{
			return *this;
		}

		static void parse(gce::reflection::rtype& _rtype)
		{
			gce::reflection::reflectable<Ref>::_do_incept();
			_rtype._methods_ = gce::reflection::reflectable<Ref>::_class_r_->_methods_;
		}
	};

	template<typename Ref, typename C, typename T>
	struct ref_parser_std_1 : public ref_parser_base<Ref, C>
	{
		typedef ref_parser_base<Ref, C> base;
		typedef Ref derived_type;

		ref_parser_std_1(C* p=nullptr)
			: base(p)
		{
		}

		method_t(void*, construct, (void* buf))
		{
			gce::__call_constructor((T*)buf);
			return buf;
		}
		method_t(void, destruct, (void* buf))
		{
			gce::__call_destructor((T*)buf);
		}

		method_t(void, batch_construct, (void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				gce::__call_constructor((T*)values+i);
			}
		}
		method_t(void, batch_destruct, (void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				gce::__call_destructor((T*)values+i);
			}
		}

		method_t(size_t, size, ())
		{
			return base::_instance_->size();
		}

		method_t(bool, begin, (void* iterator, void*& val_))
		{
			auto& it = *(typename C::iterator*)iterator;
			gce::__call_constructor<typename C::iterator>(&it);
			it = base::_instance_->begin();
			return next(iterator, val_);
		}

		method_t(bool, next, (void* iterator, void*& val_))
		{
			auto& it = *(typename C::iterator*)iterator;
			if(it!=base::_instance_->end())
			{
				const T& val = *(it++);
				val_ = (void*)&val;
				return true;
			}
			else
			{
				gce::__call_destructor<typename C::iterator>(&it);
				return false;
			}
		}

		method_t(size_t, get_values, (std::vector<void*>& values))
		{
			size_t size = base::_instance_->size();
			values.reserve(size);

			for(auto it=base::_instance_->begin(); it!=base::_instance_->end(); ++it)
			{
				auto& obj = (const T&)*it;
				values.push_back((void*)&obj);
			}
			return size;
		}
	};

	template<typename Ref, typename C, typename T1, typename T2>
	struct ref_parser_std_2 : public ref_parser_base<Ref, C>
	{
		typedef gce::ref_parser_base<Ref, C> base;
		typedef Ref derived_type;

		ref_parser_std_2(C* p=nullptr)
			: base(p)
		{
		}

		method_t(void*, construct_1, (void* buf))
		{
			gce::__call_constructor((T1*)buf);
			return buf;
		}
		method_t(void, destruct_1, (void* buf))
		{
			gce::__call_destructor((T1*)buf);
		}

		method_t(void, batch_construct_1, (void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				gce::__call_constructor((T1*)values+i);
			}
		}
		method_t(void, batch_destruct_1, (void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				gce::__call_destructor((T1*)values+i);
			}
		}
		method_t(void, batch_construct_2, (void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				gce::__call_constructor((T2*)values+i);
			}
		}
		method_t(void, batch_destruct_2, (void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				gce::__call_destructor((T2*)values+i);
			}
		}

		method_t(size_t, size, ())
		{
			return base::_instance_->size();
		}

		method_t(bool, begin, (void* iterator, void*& val1_, void*& val2_))
		{
			auto& it = *(typename C::iterator*)iterator;
			gce::__call_constructor<typename C::iterator>(&it);
			it = base::_instance_->begin();
			return next(iterator, val1_, val2_);
		}

		method_t(bool, next, (void* iterator, void*& val1_, void*& val2_))
		{
			auto& it = *(typename C::iterator*)iterator;
			if(it!=base::_instance_->end())
			{
				val1_ = (void*)&it->first;
				val2_ = (void*)&it->second;
				++it;
				return true;
			}
			else
			{
				gce::__call_destructor<typename C::iterator>(&it);
				return false;
			}
		}

		method_t(size_t, get_values, (std::vector<void*>& values))
		{
			size_t size = base::_instance_->size()*2;
			values.reserve(size);

			for(auto it=base::_instance_->begin(); it!=base::_instance_->end(); ++it)
			{
				values.push_back((void*)&it->first);
				values.push_back((void*)&it->second);
			}
			return size;
		}
	};

	template<typename T>
	struct ref_parser<std::shared_ptr<T> > : public gce::ref_parser_base<ref_parser<std::shared_ptr<T> >, std::shared_ptr<T> >
	{
		typedef gce::ref_parser_base<ref_parser<std::shared_ptr<T> >, std::shared_ptr<T> > base;
		typedef ref_parser<std::shared_ptr<T> > derived_type;

		ref_parser(std::shared_ptr<T>* p=nullptr)
			: base(p)
		{
		}

		method_t(void*, fill, ())
		{
			*base::_instance_ = std::shared_ptr<T>(new T);
			return base::_instance_->get();
		}
	};

	template<typename T>
	struct ref_parser<std::vector<T> > : public gce::ref_parser_std_1<ref_parser<std::vector<T> >, std::vector<T>, T>
	{
	protected:
		///for std::vector<bool>
		void* _get_bool_ptr(typename std::vector<T>::iterator& it, uint index)
		{
#ifdef _WINDOWS
			void* p = (void*)((decltype((*(std::vector<bool>*)nullptr)[0])&)(*it))._Getptr();
			uint offset = (index % (BYTE_PER_BIT*sizeof(std::_Vbase))) / BYTE_PER_BIT;
			p = (char*)p + offset;
#else
			void* p = (void*)((typename std::vector<bool>::iterator&)it)._M_p;
			p = (char*)p + index/8;
#endif
			return p;
		}

	public:
		typedef gce::ref_parser_std_1<ref_parser<std::vector<T> >, std::vector<T>, T> base;
		typedef ref_parser<std::vector<T> > derived_type;

		ref_parser(std::vector<T>* p=nullptr)
			: base(p)
		{
		}

		method_t(size_t, get_values, (std::vector<void*>& values))
		{
			if(typeid(T)==typeid(bool))
			{
				size_t size = base::_instance_->size();
				values.reserve(size);

				uint index = 0;
				for(auto it=base::_instance_->begin(); it!=base::_instance_->end(); ++it)
				{
					void* p = _get_bool_ptr(it, index++);
					if(values.empty() || p!=values.back())
					{
						values.push_back(p);
					}
				}
				return size;
			}
			else
			{
				return base::get_values(values);
			}
		}

		method_t(bool, begin, (void* iterator, void*& val_))
		{
			if(typeid(T)==typeid(bool))
			{
				void* it_ = iterator;
				*(uint*)iterator = 0;
				it_ = (char*)iterator + sizeof(uint);

				auto& it = *(typename std::vector<T>::iterator*)it_;
				gce::__call_constructor<typename std::vector<T>::iterator>(&it);
				it = base::_instance_->begin();
				return next(iterator, val_);
			}
			else
			{
				return base::begin(iterator, val_);
			}
		}

		method_t(bool, next, (void* iterator, void*& val_))
		{
			if(typeid(T)==typeid(bool))
			{
				uint& index = *(uint*)iterator;
				iterator = (char*)iterator + sizeof(uint);

				auto& it = *(typename std::vector<T>::iterator*)iterator;
				if(it!=base::_instance_->end())
				{
					val_ = _get_bool_ptr(it, index);
					++it;
					++index;
					return true;
				}
				else
				{
					gce::__call_destructor<typename std::vector<T>::iterator>(&it);
					return false;
				}
			}
			else
			{
				return base::next(iterator, val_);
			}
		}

		method_t(void*, add, ())
		{
			base::_instance_->push_back(std::move(T()));
			if(typeid(T)==typeid(bool))
			{
				auto it = base::_instance_->end()-1;
				return _get_bool_ptr(it, (uint)base::_instance_->size()-1);
			}
			else
			{
				auto& obj = (const T&)base::_instance_->back();
				return (void*)&obj;
			}
		}
		method_t(void, resize, (size_t size))
		{
			base::_instance_->resize(size);
		}
		method_t(void, push_back, (const T& obj))
		{
			base::_instance_->push_back(obj);
		}
		method_t(void, pop_back, ())
		{
			base::_instance_->pop_back();
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

	template<typename T>
	struct ref_parser<std::list<T> > : public gce::ref_parser_std_1<ref_parser<std::list<T> >, std::list<T>, T>
	{
		typedef gce::ref_parser_std_1<ref_parser<std::list<T> >, std::list<T>, T> base;
		typedef ref_parser<std::list<T> > derived_type;

		ref_parser(std::list<T>* p=nullptr)
			: base(p)
		{
		}

		method_t(void*, add, ())
		{
			base::_instance_->push_back(std::move(T()));
			return &base::_instance_->back();
		}
		method_t(void, resize, (size_t size))
		{
			base::_instance_->resize(size);
		}
		method_t(void, push_back, (const T& obj))
		{
			base::_instance_->push_back(obj);
		}
		method_t(void, pop_back, ())
		{
			base::_instance_->pop_back();
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

	template<typename T>
	struct ref_parser<std::set<T> > : public gce::ref_parser_std_1<ref_parser<std::set<T> >, std::set<T>, T>
	{
		typedef gce::ref_parser_std_1<ref_parser<std::set<T> >, std::set<T>, T> base;
		typedef ref_parser<std::set<T> > derived_type;

		ref_parser(std::set<T>* p=nullptr)
			: base(p)
		{
		}

		method_t(void, batch_add, (void* keys_, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				base::_instance_->insert(std::move(*((T*)keys_+i)));
			}
		}

		method_t(void*, add, (void* key_))
		{
			base::_instance_->insert(std::move(*(T*)key_));
			return key_;
		}
		method_t(void, insert, (const T& obj))
		{
			base::_instance_->insert(obj);
		}
		method_t(void, erase, (const T& obj))
		{
			base::_instance_->erase(obj);
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

	template<typename T>
	struct ref_parser<std::multiset<T> > : public gce::ref_parser_std_1<ref_parser<std::multiset<T> >, std::multiset<T>, T>
	{
		typedef gce::ref_parser_std_1<ref_parser<std::multiset<T> >, std::multiset<T>, T> base;
		typedef ref_parser<std::multiset<T> > derived_type;

		ref_parser(std::multiset<T>* p=nullptr)
			: base(p)
		{
		}

		method_t(void, batch_add, (void* keys_, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				base::_instance_->insert(std::move(*((T*)keys_+i)));
			}
		}

		method_t(void*, add, (void* key_))
		{
			base::_instance_->insert(std::move(*(T*)key_));
			return key_;
		}
		method_t(void, insert, (const T& obj))
		{
			base::_instance_->insert(obj);
		}
		method_t(void, erase, (const T& obj))
		{
			base::_instance_->erase(obj);
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

	template<typename T>
	struct ref_parser<std::deque<T> > : public gce::ref_parser_std_1<ref_parser<std::deque<T> >, std::deque<T>, T>
	{
		typedef gce::ref_parser_std_1<ref_parser<std::deque<T> >, std::deque<T>, T> base;
		typedef ref_parser<std::deque<T> > derived_type;

		ref_parser(std::deque<T>* p=nullptr)
			: base(p)
		{
		}

		method_t(void*, add, ())
		{
			base::_instance_->push_back(std::move(T()));
			return &base::_instance_->back();
		}
		method_t(void, resize, (size_t size))
		{
			base::_instance_->resize(size);
		}
		method_t(void, push_back, (const T& obj))
		{
			base::_instance_->push_back(obj);
		}
		method_t(void, push_front, (const T& obj))
		{
			base::_instance_->push_front(obj);
		}
		method_t(void, pop_back, ())
		{
			base::_instance_->pop_back();
		}
		method_t(void, pop_front, ())
		{
			base::_instance_->pop_front();
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

	template<typename T1, typename T2>
	struct ref_parser<std::map<T1, T2> > : public gce::ref_parser_std_2<ref_parser<std::map<T1, T2> >, std::map<T1, T2>, T1, T2>
	{
		typedef gce::ref_parser_std_2<ref_parser<std::map<T1, T2> >, std::map<T1, T2>, T1, T2> base;
		typedef ref_parser<std::map<T1, T2> > derived_type;

		ref_parser(std::map<T1, T2>* p=nullptr)
			: base(p)
		{
		}

		method_t(void, batch_add, (void* keys_, void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				(*base::_instance_)[*((T1*)keys_+i)] = std::move(*((T2*)values+i));
			}
		}

		method_t(void*, add, (void* key_))
		{
			return (void*)&(*base::_instance_)[*(T1*)key_];
		}
		method_t(void, insert, (const T1& t1, const T2& t2))
		{
			base::_instance_->insert(std::make_pair(t1, t2));
		}

		method_t(void, erase, (const T1& t1))
		{
			base::_instance_->erase(t1);
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

	template<typename T1, typename T2>
	struct ref_parser<std::multimap<T1, T2> > : public gce::ref_parser_std_2<ref_parser<std::multimap<T1, T2> >, std::multimap<T1, T2>, T1, T2>
	{
		typedef gce::ref_parser_std_2<ref_parser<std::multimap<T1, T2> >, std::multimap<T1, T2>, T1, T2> base;
		typedef ref_parser<std::multimap<T1, T2> > derived_type;

		ref_parser(std::multimap<T1, T2>* p=nullptr)
			: base(p)
		{
		}

		method_t(void, batch_add, (void* keys_, void* values, size_t size))
		{
			for(size_t i=0; i<size; ++i)
			{
				base::_instance_->insert(std::move(std::make_pair(std::move(*((T1*)keys_+i)), std::move(*((T2*)values+i)))));
			}
		}

		method_t(void*, add, (void* key_))
		{
			auto it = base::_instance_->insert(std::move(std::make_pair(*(T1*)key_, T2())));
			return &it->second;
		}
		method_t(void, insert, (const T1& t1, const T2& t2))
		{
			base::_instance_->insert(std::make_pair(t1, t2));
		}
		method_t(void, erase, (const T1& t1))
		{
			base::_instance_->erase(t1);
		}
		method_t(void, clear, ())
		{
			base::_instance_->clear();
		}
	};

#pragma pack()
}

template<typename T>
void _parse_template<T>::parse_template_type(gce::reflection::rdata* ptr_data)
{
	gce::reflection::rdata* ptr_sub_type = nullptr;
	std::string sub_name = gce::reflection::reflect::get_type_name(_typeid(typeid(T)).name());
	if(!::register_rtype(sub_name, ptr_sub_type))
	{
		gce::ref_parser<T>::parse(*(gce::reflection::rtype*)ptr_sub_type);

		ptr_sub_type->_name = sub_name;
		template_type<T>::parse_template_type(ptr_sub_type);
		_parse_template<T>::parse_modifier(ptr_sub_type);
	}
	ptr_data->list_sub_type_ptr.push_back((gce::reflection::rtype*)ptr_sub_type);
}