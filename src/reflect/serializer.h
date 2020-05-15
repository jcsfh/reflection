/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#pragma once

namespace gce
{
	namespace reflection
	{
		class reflect;
		class class_r;

		///format simple data
		struct single_serializer
		{
			virtual void* create_context() const = 0;
			virtual void destroy_context(void* context_) const = 0;

			virtual void write_version(std::string& out, void* context_=nullptr) const{}
			virtual void write_index(int index, std::string& out, void* context_=nullptr) const = 0;
			virtual void write_array(const class_r& _class_r, bool is_array, std::string& out, uint16 count, void* context_=nullptr) const = 0;
			virtual void write_single(const void* p, const class_r& _class_r, std::string& out, void* context_=nullptr) const = 0;
			virtual void* write_member_count(std::string& out, uint16 count, void* context_=nullptr) const = 0;
			virtual void set_member_count(std::string& out, void* index, uint16 count, void* context_=nullptr) const = 0;
			
			virtual void read_version(const std::string& in, size_t& offset, void* context_=nullptr) const STD_THROW{}
			virtual void read_index(const std::string& in, size_t& offset, int& index, void* context_=nullptr) const = 0;
			virtual void read_array(const std::string& in, size_t& offset, bool& is_array, uint16& count, void* context_=nullptr) const = 0;
			virtual void read_single(const std::string& in, size_t& offset, const class_r& _class_r, void* p, void* context_=nullptr) const = 0;
			virtual void read_member_count(const std::string& in, size_t& offset, uint16& count, void* context_=nullptr) const = 0;
		};

		struct serializer_interface
		{
			serializer_interface()
				: _ptr_single_serializer(nullptr)
			{
			}

			virtual void serialize(const reflect& _instance, std::string& out) const STD_THROW = 0;
			virtual void deserialize(const reflect& _instance, const std::string& in) STD_THROW = 0;

			inline void set_single_serializer(single_serializer* ptr_single_serializer)
			{
				_ptr_single_serializer = ptr_single_serializer;
			}

		protected:
			single_serializer* _ptr_single_serializer;
		};

		extern DSL_EXPORT serializer_interface* __get_default_serializer();
		extern single_serializer* __get_default_single_serializer();
	}
}