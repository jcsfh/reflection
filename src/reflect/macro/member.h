/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

template<typename T>
static void dispose_static(T& t)
{
#ifdef _WINDOWS
	t.~T();
#endif
}

///register memebers before main function
#define member_incept(Type, Full, Static, Name, Size, Class, Variable)\
	uint64 __get_offset_or_addr_##Name() const\
	{\
		if(Static)\
		{\
			return (uint64)&Name;\
		}\
		else\
		{\
			return (uint64)&Name - (uint64)this;\
		}\
	}\
	class DSL_EXPORT class_member_##Name : public gce::Inception<class_member_##Name, false, true>\
	{\
	public:\
		static void _incept()\
		{\
			if(Static)\
			{\
				gce::__register(derived_type::__dispose_##Name);\
			}\
			gce::reflection::rmember& member = ((gce::reflection::rmembers)derived_type::_class_r_->members())[#Name];\
			member._index = (int)derived_type::_class_r_->members().size()-1;\
			derived_type::_class_r_->_index_members[member._index] = &member;\
			member._name = #Name;\
			member._type_ = (gce::reflection::rtype*)&gce::reflection::type_info::typeof<Full>().get_type();\
			member._is_static = Static;\
			if(Static)\
			{\
				member._address = (void*)derived_type::_parse_offset_or_addr(&derived_type::__get_offset_or_addr_##Name);\
			}\
			else\
			{\
				member._offset = (uint)derived_type::_parse_offset_or_addr(&derived_type::__get_offset_or_addr_##Name);\
			}\
			if(Size>0)\
			{\
				member._type_->_is_array = true;\
				member._type_->_total_size = sizeof(Type)*Size;\
			}\
			if(member._type_->is_pointer() || member._type_->is_reference())\
			{\
				StdThrow(std::runtime_error, "the type [%s] is not supported!", _typeid(typeid(Type)).name());\
			}\
		}\
		static void _dispose(){}\
	}declare_variable(Name, Variable);

#define define_member_method(Type, Name, Variable)\
	inline Type& ref_##Name()\
	{\
		if(derived_type::_class_r_!=nullptr)\
		{\
			gce::reflection::rmember& member = ((gce::reflection::rmembers)derived_type::_class_r_->members())[#Name];\
			if(member._index<(int)derived_type::_class_r_->members().size())\
			{\
				derived_type::_ref_data._flag(member._index) = true;\
			}\
		}\
		return Name;\
	}\
	static void __dispose_##Name()\
	{\
	}\
	member_incept(Type, Type, false, Name, 0, , Variable)

#define define_array_member_method(Type, Name, Size, Variable)\
	inline Type& ref_##Name(uint index)\
	{\
		if(derived_type::_class_r_!=nullptr)\
		{\
			gce::reflection::rmember& member = ((gce::reflection::rmembers)derived_type::_class_r_->members())[#Name];\
			if(member._index<(int)derived_type::_class_r_->members().size() && !derived_type::_ref_data._flag(member._index))\
			{\
				derived_type::_ref_data._flag(member._index) = true;\
			}\
		}\
		return Name[index];\
	}\
	static void __dispose_##Name()\
	{\
	}\
	member_incept(Type, Type[Size], false, Name, Size, , Variable)

#define define_member(Modifier, Type, Name)\
	Modifier Type Name;\
	define_member_method(Type, Name, )

#define member(Type, Name)\
	typedef Type type_##Name;\
	define_member(, type_##Name, Name);

#define member_mutable(Type, Name)\
	typedef Type type_##Name;\
	define_member(mutable, type_##Name, Name);

#define define_member_array(Modifier, Type, Name, Size)\
	Type (Name)[Size];\
	define_array_member_method(Type, Name, Size, )

#define member_array(Type, Name, Size)\
	typedef Type type_##Name;\
	define_member_array(, type_##Name, Name, Size)

#define member_array_mutable(Type, Name, Size)\
	typedef Type type_##Name;\
	define_member_array(mutable, type_##Name, Name, Size)

#define member_static(Type, Name)\
	static Type Name;\
	static void __dispose_##Name()\
	{\
		dispose_static<Type>(Name);\
	}\
	typedef Type type_##Name;\
	member_incept(type_##Name, type_##Name, true, Name, 0, , )

#define member_static_array(Type, Name, Size)\
	static Type (Name)[Size];\
	static void __dispose_##Name()\
	{\
		for(uint i=0; i<Size; ++i)\
		{\
			dispose_static<Type>(Name[i]);\
		}\
	}\
	typedef Type type_##Name;\
	member_incept(type_##Name, type_##Name[Size], true, Name, Size, , )

///only for registering the third part's member
#define register_member(Type, Name)\
	typedef Type type_##Name;\
	define_member_method(type_##Name, Name, )

///only for registering the third part's member
#define register_member_array(Type, Name, Size)\
	typedef Type type_##Name;\
	define_array_member_method(type_##Name, Name, Size, )

///only for template class
#define member_t(Type, Name)\
	Type Name;\
	typedef Type type_##Name;\
	define_member_method(type_##Name, Name, __##Name##__LINE__)
