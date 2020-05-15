/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

template<typename A>
struct check_reference
{
	enum { result = false };
};

template<typename A>
struct check_reference<A&>
{
	enum { result = true };
};

template<typename A>
struct check_pointer
{
	enum { result = false };
};

template<typename A>
struct check_pointer<A*>
{
	enum { result = true };
};

template<typename A>
struct check_const
{
	enum { result = false };
};

template<typename A>
struct check_const<const A>
{
	enum { result = true };
};

template<typename A>
struct check_const<const A*>
{
	enum { result = true };
};

template<typename A>
struct check_const<const A&>
{
	enum { result = true };
};

template<typename A>
struct check_const<A* const>
{
	enum { result = true };
};