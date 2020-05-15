/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

#include "rtype.h"

namespace gce
{
	namespace reflection
	{
		class reflect;
		struct rparent;

		class reflect_placeholder
		{
		public:
			virtual ~reflect_placeholder(){}
			static void _parse_parent(void* p, class_res parent){}
			void _set_derived(void* derived_) const{}

			ref_data& _get_ref_data()
			{
				static ref_data _ref_data;
				return _ref_data;
			}
		};
		class reflect_placeholder1 : public reflect_placeholder{};
		class reflect_placeholder2 : public reflect_placeholder{};
		class reflect_placeholder3 : public reflect_placeholder{};
		class reflect_placeholder4 : public reflect_placeholder{};
		class reflect_placeholder5 : public reflect_placeholder{};
		class reflect_placeholder6 : public reflect_placeholder{};
		class reflect_placeholder7 : public reflect_placeholder{};
		class reflect_placeholder8 : public reflect_placeholder{};
		class reflect_placeholder9 : public reflect_placeholder{};

		template<class B>
		class reflect_base : public B
		{
		};

		template<>
		class reflect_base<reflect_placeholder1>
		{
		};

		template<>
		class reflect_base<reflect_placeholder2>
		{
		};

		template<>
		class reflect_base<reflect_placeholder3>
		{
		};

		template<>
		class reflect_base<reflect_placeholder4>
		{
		};

		template<>
		class reflect_base<reflect_placeholder5>
		{
		};

		template<>
		class reflect_base<reflect_placeholder6>
		{
		};

		template<>
		class reflect_base<reflect_placeholder7>
		{
		};

		template<>
		class reflect_base<reflect_placeholder8>
		{
		};

		template<>
		class reflect_base<reflect_placeholder9>
		{
		};
	}
}