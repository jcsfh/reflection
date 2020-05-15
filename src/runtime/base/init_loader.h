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
	namespace runtime
	{
		///initialize global environment
		class DSL_EXPORT init_loader : public Inception<init_loader, true, true>
		{
		public:
			static void _incept();
			static void _dispose();

			static void register_cleaner_callback(std::function<void()> cleaner_callback);

		private:
			static void __init_socket();
		};
	}
}