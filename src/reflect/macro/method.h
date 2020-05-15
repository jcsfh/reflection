/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

///register methods before main function
#define method_base(Name, Line, Class, is_virtual, Variable)\
		class DSL_EXPORT class_member_func_##Name##Line : public gce::Inception<class_member_func_##Name##Line, false, true>\
		{\
		public:\
			static void _incept()\
			{\
				derived_type::_do_incept();\
				gce::reflection::__parse_method(#Name, Class get_class(), (bool)is_virtual, &derived_type::Name);\
			}\
			static void _dispose(){}\
		}declare_variable(Name, Variable);

#define method_define(Return, Name, Args, Variable)\
	method_base(Name, __LINE__, derived_type::, (std::string(#Return).find("virtual")!=std::string::npos), Variable)\
	Return Name Args

#define method(Return, Name, Args) method_define(Return, Name, Args, )
///only for template class
#define method_t(Return, Name, Args) method_define(Return, Name, Args, __##Name##_##Line)

///only for registering the third part's method
#define register_method(Class, Name) method_base(Name, __LINE__, Class::, false, )
#define register_method_t(Class, Name) method_base(Name, __LINE__, Class::, false, __##Name##Line)