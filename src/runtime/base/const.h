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
	///const value define
	class DSL Const
	{
	public:
		const static short int16MaxValue = 0x7FFF;
		const static short int16MinValue = -int16MaxValue;

		const static ushort uint16MinValue = 0;
		const static ushort uint16MaxValue = -1;

		const static int32 int32MaxValue = 0x7FFFFFFF;
		const static int32 int32MinValue = -int32MaxValue;

		const static uint32 uint32MinValue = 0;
		const static uint32 uint32MaxValue = 0xFFFFFFFF;

		const static int64 int64MaxValue = 0x7FFFFFFFFFFFFFFF;
		const static int64 int64MinValue = -int64MaxValue;

		const static uint64 uint64MinValue = 0;
		const static uint64 uint64MaxValue = -1;

		const static int intMaxValue = sizeof(int)==4?int32MaxValue:int64MaxValue;
		const static int intMinValue = -intMaxValue;

		const static uint uintMaxValue = sizeof(int)==4?uint32MaxValue:uint64MaxValue;
		const static uint uintMinValue = 0;

		const static long longMaxValue = sizeof(long)==4?int32MaxValue:int64MaxValue;
		const static long longMinValue = -intMaxValue;

		const static ulong ulongMaxValue = sizeof(long)==4?uint32MaxValue:uint64MaxValue;
		const static ulong ulongMinValue = 0;

		const static uint INFINITETIME = intMaxValue;
		const static int WAITIMEOUTSEC = 5;
	};
}