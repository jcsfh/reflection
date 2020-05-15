/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#include "../../runtime/runtime.h"

#include "../serializer.h"
#include "../type_info.h"
#include "../rclass.h"

namespace gce
{
	namespace reflection
	{
#pragma pack(1)
		struct version
		{
			uint16 val;
		};
		struct array_data
		{
			bool is_array;
			uint16 count;
		};
#pragma pack()
		///default binary serializer
		class default_single_serializer : public single_serializer
		{
		protected:
			const static uint16 _default_version = 1;

			void write_version(std::string& out, void* context_=nullptr) const
			{
				out.resize(out.size()+sizeof(version));
				((version*)out.data())->val = _default_version;
			}

			void read_version(const std::string& in, size_t& offset, void* context_=nullptr) const STD_THROW
			{
				version* version_ = (version*)(in.data()+offset);
				if(version_->val!=_default_version)
				{
					StdThrow(std::runtime_error, "version (%u) not matched", (uint)version_->val);
				}
				offset += sizeof(version);
			}

			void write_index(int index, std::string& out, void* context_=nullptr) const
			{
				size_t size = out.size();
				out.resize(size+sizeof(int));
				*(int*)(out.data()+size) = index;
			}

			void read_index(const std::string& in, size_t& offset, int& index, void* context_=nullptr) const
			{
				index = *(int*)(in.data()+offset);
				offset += sizeof(int);
			}

			void write_array(const class_r& _class_r, bool is_array, std::string& out, uint16 count, void* context_=nullptr) const
			{
				if(is_array)
				{
					size_t size = out.size();
					out.resize(size+sizeof(array_data));
					auto p_ = (array_data*)(out.data()+size);
					p_->is_array = is_array;
					p_->count = count;
				}
				else
				{
					out += (char)0;
				}
			}

			void read_array(const std::string& in, size_t& offset, bool& is_array, uint16& count, void* context_=nullptr) const
			{
				is_array = *(bool*)(in.data()+offset);
				if(is_array)
				{
					auto p_ = (array_data*)(in.data()+offset);
					count = p_->count;
					offset += sizeof(array_data);
				}
				else
				{
					++offset;
				}
			}

			void write_single(const void* p, const class_r& _class_r, std::string& out, void* context_=nullptr) const
			{
				switch(_class_r.get_data_type())
				{
				case type_stl_string:
					{
						std::string& str = *(std::string*)p;
						out += str;
						out.resize(out.size()+1);
					}
					break;
				case type_stl_wstring:
					{
						std::wstring& str = *(std::wstring*)p;
						size_t size = out.size();
						size_t str_size = (str.size()+1)*sizeof(wchar);
						out.resize(out.size()+str_size);
						::memcpy((void*)(out.data()+size), str.data(), str_size);
					}
					break;
				default:
					{
						if(_class_r.get_data_type()<type_base_end && _class_r.get_data_type()>type_invalid)
						{
							size_t size = out.size();
							out.resize(size+_class_r.get_size());
							::memcpy((void*)(out.data()+size), p, _class_r.get_size());
						}
						else
						{
							StdThrow(std::runtime_error, "type (%s) not supported", _class_r.get_name().c_str());
						}
					}
					break;
				}
			}

			void read_single(const std::string& in, size_t& offset, const class_r& _class_r, void* p, void* context_=nullptr) const
			{
				switch(_class_r.get_data_type())
				{
				case type_stl_string:
					{
						char* sz = (char*)(in.data()+offset);
						auto str_ = (std::string*)p;
						*str_ = sz;
						offset += str_->size()+1;
					}
					break;
				case type_stl_wstring:
					{
						wchar* sz = (wchar*)(in.data()+offset);
						auto wstr_ = (std::wstring*)p;
						*wstr_ = sz;
						offset += (wstr_->size()+1)*sizeof(wchar);
					}
					break;
				default:
					{
						if(_class_r.get_data_type()<type_base_end && _class_r.get_data_type()>type_invalid)
						{
							::memcpy(p, in.data()+offset, _class_r.get_size());
							offset += _class_r.get_size();
						}
						else
						{
							StdThrow(std::runtime_error, "type (%s) not supported", _class_r.get_name().c_str());
						}
					}
					break;
				}
			}

			void* write_member_count(std::string& out, uint16 count, void* context_=nullptr) const
			{
				size_t size = out.size();
				out.resize(size+sizeof(uint16));
				uint16* p = (uint16*)(out.data()+size);
				*p = count;
				return (void*)size;
			}

			void set_member_count(std::string& out, void* index, uint16 count, void* context_=nullptr) const
			{
				*(uint16*)(out.data()+(size_t)index) = count;
			}

			void read_member_count(const std::string& in, size_t& offset, uint16& count, void* context_=nullptr) const
			{
				count = *(uint16*)(in.data()+offset);
				offset += sizeof(uint16);
			}

			void* create_context() const
			{
				return nullptr;
			}

			void destroy_context(void* context_) const
			{
			}
		};

		struct default_serializer : public serializer_interface
		{
			void serialize(const reflect& _instance, std::string& out) const STD_THROW
			{
				if(nullptr==_instance._ptr_class_r)
				{
					StdThrow(std::runtime_error, "nullptr found");
				}
				void* p = _instance.get_derived();
				ref_data* ref_data_ = (ref_data*)((char*)p + _instance._ptr_class_r->_ref_data_offset);
				out.reserve(128);

				void* context_ = _ptr_single_serializer->create_context();
				_ptr_single_serializer->write_version(out, context_);
				try
				{
					__serialize(p, ref_data_, *_instance._ptr_class_r, nullptr, out, true, context_);
				}
				catch(std::exception& e)
				{
					_ptr_single_serializer->destroy_context(context_);
					throw e;
				}
				_ptr_single_serializer->destroy_context(context_);
			}

			void deserialize(const reflect& _instance, const std::string& in) STD_THROW
			{
				if(nullptr==_instance._ptr_class_r)
				{
					StdThrow(std::runtime_error, "nullptr found");
				}

				void* context_ = _ptr_single_serializer->create_context();

				size_t offset = 0;
				_ptr_single_serializer->read_version(in, offset, context_);

				void* p = _instance.get_derived();
				ref_data* ref_data_ = (ref_data*)((char*)p + _instance._ptr_class_r->_ref_data_offset);
				try
				{
					__deserialize(p, ref_data_, *_instance._ptr_class_r, nullptr, in, offset, true, context_);
				}
				catch(std::exception& e)
				{
					_ptr_single_serializer->destroy_context(context_);
					throw e;
				}
				_ptr_single_serializer->destroy_context(context_);
			}

		private:
			inline ref_data* __get_sub_ref_data(const void* p, const class_r& _class_r) const
			{
				if(_class_r.get_data_type()!=type_object_reflectable)
				{
					return nullptr;
				}
				return (ref_data*)((char*)p + _class_r._ref_data_offset);
			}

			///serialize std container with one template argument
			void __serialize_container_1(const void* p, const ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, std::string& out, void* context_) const
			{
				class_r& _sub_class_r = *(class_r*)(_class_r.list_sub_type_ptr[0]);
				void* _index = _ptr_single_serializer->write_member_count(out, 0, context_);

				std::vector<void*> values;
				///get values by batch method, reducing the calling times.
				size_t size = (size_t)type_manager::_call_method((class_r*)&_class_r, "get_values", (void*)p, &values);
				size_t value_count = values.size();

				for(size_t i=0; i<value_count; ++i)
				{
					void* val_ = values[i];
					auto _ref_data_ = __get_sub_ref_data(val_, _sub_class_r);
					__serialize(val_, _ref_data_, _sub_class_r, rparent_, out, false, context_);
				}
				_ptr_single_serializer->set_member_count(out, _index, (uint16)size, context_);
			}

			///serialize std container with two template arguments
			void __serialize_container_2(const void* p, const ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, std::string& out, void* context_) const
			{
				class_r& _sub_class_r_1 = *(class_r*)(_class_r.list_sub_type_ptr[0]);
				class_r& _sub_class_r_2 = *(class_r*)(_class_r.list_sub_type_ptr[1]);
				void* _index = _ptr_single_serializer->write_member_count(out, 0, context_);

				std::vector<void*> values;
				///get values by batch method, reducing the calling times.
				size_t size = (size_t)type_manager::_call_method((class_r*)&_class_r, "get_values", (void*)p, &values);

				for(size_t i=0; i<size; )
				{
					void* val1_ = values.at(i++);
					void* val2_ = values.at(i++);

					auto _ref_data_ = __get_sub_ref_data(val1_, _sub_class_r_1);
					__serialize(val1_, _ref_data_, _sub_class_r_1, rparent_, out, false, context_);

					_ref_data_ = __get_sub_ref_data(val2_, _sub_class_r_2);
					__serialize(val2_, _ref_data_, _sub_class_r_2, rparent_, out, false, context_);
				}
				_ptr_single_serializer->set_member_count(out, _index, (uint16)size/2, context_);
			}

			void __serialize_single(const void* p, const ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, std::string& out, bool write_array, void* context_) const
			{
				switch(_class_r.get_data_type())
				{
				case type_stl_sharedptr:
					{
						auto _p_ = ((std::shared_ptr<int>*)p)->get();
						if(_p_!=nullptr)
						{
							class_r& _class_r_sub = *(class_r*)(_class_r.list_sub_type_ptr[0]);
							auto _ref_data_ = __get_sub_ref_data(_p_, _class_r_sub);
							__serialize(_p_, _ref_data_, _class_r_sub, rparent_, out, false, context_);
						}
						else
						{
							StdThrow(std::runtime_error, "shared_ptr can not be nullptr: %s", _class_r.get_name().c_str());
						}
					}
					break;
				case type_stl_vector:
					__serialize_container_1(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				case type_stl_list:
					__serialize_container_1(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				case type_stl_deque:
					__serialize_container_1(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				case type_stl_set:
					__serialize_container_1(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				case type_stl_multiset:
					__serialize_container_1(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				case type_stl_map:
					__serialize_container_2(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				case type_stl_multimap:
					__serialize_container_2(p, ref_data_, _class_r, rparent_, out, context_);
					break;
				default:
					if(_class_r.get_data_type()<type_simple_end && _class_r.get_data_type()>type_invalid)
					{
						_ptr_single_serializer->write_single(p, _class_r, out, context_);
						return;
					}
					StdThrow(std::runtime_error, "member type (%s) not supported", _class_r.get_name().c_str());
					break;
				}
			}

			void __serialize_class_r(const void* p, const ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, std::string& out, bool write_array, void* context_) const
			{
				if(_class_r.get_data_type()!=type_object_reflectable)
				{
					__serialize_single(p, ref_data_, _class_r, rparent_, out, write_array, context_);
				}
				else
				{
					void* _index = _ptr_single_serializer->write_member_count(out, 0, context_);
					uint16 _count = 0;

					///serialize members
					for(auto it=_class_r.members().begin(); it!=_class_r.members().end(); ++it)
					{
						if(!it->second._is_static && ref_data_->_sp_set_flags_!=nullptr && ref_data_->_flag(it->second._index))
						{
							++_count;
							_ptr_single_serializer->write_index(it->second._index, out, context_);

							auto _p_ = (char*)p+it->second._offset;
							class_r& _sub_class_r = *(class_r*)it->second._type_;
							auto _ref_data_ = __get_sub_ref_data(_p_, _sub_class_r);
							__serialize(_p_, _ref_data_, _sub_class_r, rparent_, out, true, context_);
						}
					}
					_ptr_single_serializer->set_member_count(out, _index, _count, context_);

					///serialize parents
					for(auto it=_class_r.parents().begin(); it!=_class_r.parents().end(); ++it)
					{
						void* _p_ = nullptr;
						rparent* _rparent_ = nullptr;
						if(rparent_!=nullptr)
						{
							_rparent_ = &rparent_->list_next_parent[it->first];
						}
						else
						{
							_rparent_ = &it->second;
						}
						_p_ = (char*)p+_rparent_->offset;
						class_r& _class_r_parent = *(class_r*)it->second.rtype_;
						auto _ref_data_ = (ref_data*)((char*)_p_ + _rparent_->ref_data_offset);
						__serialize(_p_, _ref_data_, _class_r_parent, _rparent_, out, false, context_);
					}
				}
			}

			void __serialize(const void* p, const ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, std::string& out, bool write_array, void* context_) const
			{
				if(_class_r.is_pointer())
				{
					StdThrow(std::runtime_error, "member (%s) cannot be pointer", _class_r.get_name().c_str());
				}

				if(_class_r.is_array())
				{
					uint16 count = _class_r.get_total_size()/_class_r.get_size();
					_ptr_single_serializer->write_array(_class_r, true, out, count, context_);

					for(uint16 i=0; i<count; ++i)
					{
						void* _p_ = (char*)p+i*_class_r.get_size();
						auto& _sub_class_r = *(class_r*)_class_r.list_sub_type_ptr[0];
						auto _ref_data_ = __get_sub_ref_data(_p_, _sub_class_r);
						__serialize_class_r(_p_, _ref_data_, _sub_class_r, rparent_, out, write_array, context_);
					}
				}
				else
				{
					if(write_array)
					{
						_ptr_single_serializer->write_array(_class_r, false, out, 0, context_);
					}
					__serialize_class_r(p, ref_data_, _class_r, rparent_, out, write_array, context_);
				}
			}

			//-----------------------------deserialize-------------------------------------
			///deserialize std container with one template argument
			template<bool Key>
			void __deserialize_container_1(const std::string& in, size_t& offset, void* p, ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, void* context_) const
			{
				class_r& _sub_class_r = *(class_r*)(_class_r.list_sub_type_ptr[0]);

				uint16 count = 0;
				_ptr_single_serializer->read_member_count(in, offset, (uint16&)count, context_);
				const size_t size = count;

				if(Key)
				{
					char* keys_ = new char[count * _sub_class_r.get_size()];
					///batch construct objects
					type_manager::_call_method((class_r*)&_class_r, "batch_construct", p, &keys_, (size_t*)&size);

					for(uint16 i=0; i<count; ++i)
					{
						void* val_ = keys_+i*_sub_class_r.get_size();
						auto _ref_data_ = __get_sub_ref_data(val_, _sub_class_r);
						__deserialize(val_, _ref_data_, _sub_class_r, rparent_, in, offset, false, context_);
					}
					///batch add objects to the container
					type_manager::_call_method((class_r*)&_class_r, "batch_add", p, &keys_, (size_t*)&size);

					type_manager::_call_method((class_r*)&_class_r, "batch_destruct", p, &keys_, (size_t*)&size);
					delete[] keys_;
				}
				else
				{
					type_manager::_call_method((class_r*)&_class_r, "resize", (void*)p, (void*)&size);

					std::vector<void*> values;
					type_manager::_call_method((class_r*)&_class_r, "get_values", (void*)p, &values);
					size_t value_count = values.size();

					for(size_t i=0; i<value_count; ++i)
					{
						void* val_ = values.at(i);
						auto _ref_data_ = __get_sub_ref_data(val_, _sub_class_r);
						__deserialize(val_, _ref_data_, _sub_class_r, rparent_, in, offset, false, context_);
					}
				}
			}

			///deserialize std container with two template arguments
			void __deserialize_container_2(const std::string& in, size_t& offset, void* p, ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, void* context_) const
			{
				class_r& _sub_class_r_1 = *(class_r*)(_class_r.list_sub_type_ptr[0]);
				class_r& _sub_class_r_2 = *(class_r*)(_class_r.list_sub_type_ptr[1]);

				uint16 count = 0;
				_ptr_single_serializer->read_member_count(in, offset, (uint16&)count, context_);


				char* keys_ = new char[count * _sub_class_r_1.get_size()];
				const size_t size = count;
				type_manager::_call_method((class_r*)&_class_r, "batch_construct_1", p, &keys_, (size_t*)&size);
				char* values_ = new char[count * _sub_class_r_2.get_size()];
				type_manager::_call_method((class_r*)&_class_r, "batch_construct_2", p, &values_, (size_t*)&size);

				for(uint16 i=0; i<count; ++i)
				{
					void* key_ = keys_+i*_sub_class_r_1.get_size();
					auto _ref_data_ = __get_sub_ref_data(key_, _sub_class_r_1);
					__deserialize(key_, _ref_data_, _sub_class_r_1, rparent_, in, offset, false, context_);

					void* value_ = values_+i*_sub_class_r_2.get_size();
					_ref_data_ = __get_sub_ref_data(value_, _sub_class_r_2);
					__deserialize(value_, _ref_data_, _sub_class_r_2, rparent_, in, offset, false, context_);
				}
				type_manager::_call_method((class_r*)&_class_r, "batch_add", p, &keys_, &values_, (size_t*)&size);

				type_manager::_call_method((class_r*)&_class_r, "batch_destruct_2", p, &values_, (size_t*)&size);
				delete[] values_;
				type_manager::_call_method((class_r*)&_class_r, "batch_destruct_1", p, &keys_, (size_t*)&size);
				delete[] keys_;
			}

			void __deserialize_single(const std::string& in, size_t& offset, void* p, ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, bool read_array, void* context_) const
			{
				switch(_class_r.get_data_type())
				{
				case type_stl_sharedptr:
					{
						auto _p_ = type_manager::_call_method((class_r*)&_class_r, "fill", (void*)p);
						class_r& _class_r_sub = *(class_r*)(_class_r.list_sub_type_ptr[0]);
						auto _ref_data_ = __get_sub_ref_data(_p_, _class_r_sub);
						__deserialize(_p_, _ref_data_, _class_r_sub, rparent_, in, offset, false, context_);
					}
					break;
				case type_stl_vector:
					__deserialize_container_1<false>(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				case type_stl_list:
					__deserialize_container_1<false>(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				case type_stl_deque:
					__deserialize_container_1<false>(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				case type_stl_set:
					__deserialize_container_1<true>(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				case type_stl_multiset:
					__deserialize_container_1<true>(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				case type_stl_map:
					__deserialize_container_2(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				case type_stl_multimap:
					__deserialize_container_2(in, offset, p, ref_data_, _class_r, rparent_, context_);
					break;
				default:
					if(_class_r.get_data_type()<type_simple_end && _class_r.get_data_type()>type_invalid)
					{
						_ptr_single_serializer->read_single(in, offset, _class_r, p, context_);
						return;
					}
					StdThrow(std::runtime_error, "member type (%s) not supported", _class_r.get_name().c_str());
					break;
				}
			}

			void __deserialize_class_r(const std::string& in, size_t& offset, void* p, ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, bool read_array, void* context_) const
			{
				if(_class_r.get_data_type()!=type_object_reflectable)
				{
					__deserialize_single(in, offset, p, ref_data_, _class_r, rparent_, read_array, context_);
				}
				else
				{
					uint16 count = 0;
					_ptr_single_serializer->read_member_count(in, offset, count, context_);
					if(count>0)
					{
						for(uint16 i=0; i<count; ++i)
						{
							int index = -1;
							_ptr_single_serializer->read_index(in, offset, index, context_);
							if(index>=(int)_class_r.members().size())
							{
								StdThrow(std::runtime_error, "invalid index(%d)", index);
							}

							if(nullptr==ref_data_->_sp_set_flags_)
							{
								ref_data_->_create_flag(_class_r);
							}
							ref_data_->_flag(index) = true;

							rmember* member_ = _class_r._index_members[index];
							auto _p_ = (char*)p+member_->_offset;
							class_r& _class_r_sub = *(class_r*)member_->_type_;
							auto _ref_data_ = __get_sub_ref_data(_p_, _class_r_sub);
							__deserialize(_p_, _ref_data_, _class_r_sub, rparent_, in, offset, true, context_);
						}
					}
					for(auto it=_class_r.parents().begin(); it!=_class_r.parents().end(); ++it)
					{
						void* _p_ = nullptr;
						rparent* _rparent_ = nullptr;
						if(rparent_!=nullptr)
						{
							_rparent_ = &rparent_->list_next_parent[it->first];
						}
						else
						{
							_rparent_ = &it->second;
						}
						_p_ = (char*)p+_rparent_->offset;
						class_r& _class_r_parent = *(class_r*)it->second.rtype_;
						auto _ref_data_ = (ref_data*)((char*)_p_ + _rparent_->ref_data_offset);
						__deserialize(_p_, _ref_data_, _class_r_parent, _rparent_, in, offset, false, context_);
					}
				}
			}

			void __deserialize(void* p, ref_data* ref_data_, const class_r& _class_r, rparent* rparent_, const std::string& in, size_t& offset, bool read_array, void* context_) const
			{
				if(_class_r.is_pointer())
				{
					StdThrow(std::runtime_error, "member (%s) cannot be pointer", _class_r.get_name().c_str());
				}

				bool is_array = false;
				uint16 count = 0;
				if(read_array)
				{
					_ptr_single_serializer->read_array(in, offset, is_array, count, context_);
				}

				if(is_array)
				{
					for(uint16 i=0; i<count; ++i)
					{
						void* _p_ = (char*)p+i*_class_r.get_size();
						auto& _sub_class_r = *(class_r*)_class_r.list_sub_type_ptr[0];
						auto _ref_data_ = __get_sub_ref_data(_p_, _sub_class_r);
						__deserialize_class_r(in, offset, _p_, _ref_data_, _sub_class_r, rparent_, read_array, context_);
					}
				}
				else
				{
					__deserialize_class_r(in, offset, p, ref_data_, _class_r, rparent_, read_array, context_);
				}
			}
		};

		
		serializer_interface* __get_default_serializer()
		{
			static default_serializer _serializer;
			return &_serializer;
		}

		single_serializer* __get_default_single_serializer()
		{
			static default_single_serializer _single_serializer_impl;
			return &_single_serializer_impl;
		}
	}
}