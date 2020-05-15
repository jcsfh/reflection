/*  
    gce::reflection is a general C++ reflection engine.
    Copyright (C) 2015

	Version 0.5.
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation;
*/

#include "../../runtime/runtime.h"

#include "../rtype.h"
#include "../type_info.h"
#include "../serializer.h"
#include "../type_manager.h"

namespace gce
{
	namespace reflection
	{
		ref_data::ref_data()
			: _reflect_(nullptr)
			, _ref_data_offset(0)
			, _reflect_offset(0)
		{
		}

		ref_data::~ref_data()
		{
		}

		void ref_data::_create_flag(const rtype& _rtype) const
		{
			if(!_rtype.members().empty() && nullptr==_sp_set_flags_)
			{
				bool* p = new bool[_rtype.members().size()];
				::memset(p, 0, _rtype.members().size());
				_sp_set_flags_ = std::shared_ptr<bool>(p);
			}
		}

		bool& ref_data::_flag(uint index) const
		{
			return (&(*_sp_set_flags_))[index];
		}

		reflect::reflect()
			: _ptr_class_r(nullptr)
			, _ptr_derived_class_r(nullptr)
			, _ptr_to_derived(nullptr)
			, _ptr_static_instance(nullptr)
			, _ptr_derived_to_reflect(nullptr)
		{
			_ptr_serializer = __get_default_serializer();
			_ptr_serializer->set_single_serializer(__get_default_single_serializer());
		}

		reflect::~reflect()
		{
		}

		inline std::queue<std::shared_ptr<reflect::type_data> >& __get_type_data_queue()
		{
			static std::queue<std::shared_ptr<reflect::type_data> > _que;
			return _que;
		}

		inline std::shared_ptr<reflect::type_data> __pop_type_data_queue()
		{
			if(!__get_type_data_queue().empty())
			{
				///using recycled object
				std::shared_ptr<reflect::type_data> p = __get_type_data_queue().front();
				__get_type_data_queue().pop();
				return p;
			}
			return std::shared_ptr<reflect::type_data>(new reflect::type_data);
		}

		reflect::type_data::type_data()
		{
		}

		reflect::type_data::~type_data()
		{
			for(size_t i=0; i<next_data.size(); ++i)
			{
				next_data[i]->clear();
				__get_type_data_queue().push(next_data[i]);
			}
			for(size_t i=0; i<sub_data.size(); ++i)
			{
				sub_data[i]->clear();
				__get_type_data_queue().push(sub_data[i]);
			}

			clear();
		}

		void reflect::type_data::clear()
		{
			name.clear();
			mdf.clear();
			sub_data.clear();
			next_data.clear();
		}

		void reflect::parse_namespace(const std::string& _raw_name, std::string& _name, std::vector<std::string>& _namespaces)
		{
			std::string raw_name;
			if(_name.empty())
			{
				raw_name = _raw_name;
			}
			else
			{
				raw_name = _name;
			}

			size_t index = raw_name.find('<');
			if(index!=std::string::npos)
			{
				_name = raw_name.substr(0, index);
			}
			else
			{
				_name = raw_name;
			}

			std::vector<std::string> ret;
			meta::split(_name, "::", ret);

			size_t size = ret.size();
			if(size>1)
			{
				if(index!=std::string::npos)
				{
					_name += raw_name.substr(index, -1);
				}

				meta::replace(_name, "class ", "");
				meta::replace(_name, "struct ", "");

				for(size_t i=0; i<size-1; ++i)
				{
					if(0==i)
					{
						std::vector<std::string> real;
						meta::split(ret[0], " ", real);
						if(!real.empty())
						{
							ret[0] = real[real.size()-1];
						}
						meta::trim_left(ret[0]);
					}

					if(!type_manager::_is_type_existed(ret[i]))
					{
						_namespaces.push_back(ret[i]);
					}
					else
					{
						StdThrow(std::runtime_error, "type already existed (%s)", ret[i].c_str());
					}
				}
			}

			meta::replace(_name, "class ", "");
			meta::replace(_name, "struct ", "");
			meta::trim_left(_name);
		}

		void reflect::parse_name(std::string& _name, std::vector<std::string>& _namespaces)
		{
			std::string raw_name = _name;
			parse_namespace(raw_name, _name, _namespaces);
		}

		///parse std container
		typedef std::map<std::string, int> map_type;
		static map_type* get_map_type()
		{
			static map_type* _map = new map_type;
			if(_map->empty())
			{
				_map->insert(std::make_pair("std::basic_string", 0));
				_map->insert(std::make_pair("std::basic_string", 0));

				_map->insert(std::make_pair("std::vector", 1));
				_map->insert(std::make_pair("std::list", 1));
				_map->insert(std::make_pair("std::set", 1));
				_map->insert(std::make_pair("std::multiset", 1));
				_map->insert(std::make_pair("std::queue", 1));
				_map->insert(std::make_pair("std::deque", 1));
				_map->insert(std::make_pair("std::stack", 1));
				_map->insert(std::make_pair("std::priority_queue", 1));

				_map->insert(std::make_pair("std::map", 2));
				_map->insert(std::make_pair("std::multimap", 2));
			}
			return _map;
		}

		struct DSL_EXPORT cleaner : public gce::Inception<cleaner, true, true>
		{
			static void _incept()
			{
				gce::runtime::init_loader::register_cleaner_callback([]()
				{
					delete get_map_type();
#ifdef _WINDOWS
					__get_type_data_queue().~queue();
#else
					while(!__get_type_data_queue().empty())
					{
						__get_type_data_queue().pop();
					}
#endif
				});
			}
			static void _dispose()
			{
			}
		}_cleaner;

		void parse_modifier(std::string& name, modifier& modifier)
		{
			if(name.empty())
			{
				return;
			}

			if(name.find("const")!=std::string::npos)
			{
				modifier._is_const = true;
				meta::replace(name, "const ", "");
				meta::replace(name, " const", "");
			}
			if(name.find("*")!=std::string::npos)
			{
				modifier._arg_type = arg_ptr;
				meta::replace(name, "*", "");
			}
			if(name.find("&")!=std::string::npos)
			{
				modifier._arg_type = arg_ref;
				meta::replace(name, "&", "");
			}

			meta::replace(name, "__ptr64", "");
		}

		void trim_name(std::string& str)
		{
			meta::replace(str, "class ", "");
			meta::replace(str, "struct ", "");
			meta::replace(str, "tr1::", "");
			meta::trim_left(str);
		}

		void reflect::_parse_template(const std::string& raw_name, std::vector<std::shared_ptr<type_data> >& _list_template_data)
		{
			std::string name = raw_name;
			while(!name.empty())
			{
				meta::trim_left(name);
				if(name.size()>1 && name[0]==':' && name[1]==':')
				{
					name = name.substr(2, -1);
				}

				_list_template_data.push_back(__pop_type_data_queue());
				type_data* ptr_template_data = &*_list_template_data.back();
				int layer = 0;
				int next_layer = -1;

				std::stack<type_data*> _stack;

				std::string temp;
				temp.reserve(name.size());
				size_t size = name.size();

				size_t i=0;
				for(i=0; i<size; ++i)
				{
					if('<'==name[i])
					{
						++layer;

						parse_modifier(temp, ptr_template_data->mdf);
						trim_name(temp);
						meta::trim_right(temp);

						ptr_template_data->name = temp;
						temp.clear();

						_stack.push(ptr_template_data);
						ptr_template_data->sub_data.push_back(__pop_type_data_queue());
						ptr_template_data = &*ptr_template_data->sub_data.front();
					}
					else if('>'==name[i])
					{
						parse_modifier(temp, ptr_template_data->mdf);
						meta::trim_right(temp);
						if(!temp.empty())
						{
							trim_name(temp);
							ptr_template_data->name = temp;
							temp.clear();
						}

						ptr_template_data = _stack.top();
						_stack.pop();

						if(--layer==0)
						{
							break;
						}

						if(next_layer==layer)
						{
							next_layer = -1;
							ptr_template_data = _stack.top();
							_stack.pop();
						}
					}
					else if(','==name[i])
					{
						parse_modifier(temp, ptr_template_data->mdf);
						meta::trim_right(temp);
						if(!temp.empty())
						{
							trim_name(temp);
							ptr_template_data->name = temp;
							temp.clear();
						}

						if(next_layer==layer)
						{
							next_layer = -1;
							ptr_template_data = _stack.top();
							_stack.pop();
						}
						
						type_data* ptr_parent = _stack.top();
						ptr_parent->sub_data.push_back(__pop_type_data_queue());
						ptr_template_data = &*ptr_parent->sub_data.back();
					}
					else if(i>0 && i<size-1 && '>'==name[i-1] && ':'==name[i] && ':'==name[i+1])
					{
						_stack.push(ptr_template_data);

						ptr_template_data->next_data.push_back(__pop_type_data_queue());
						ptr_template_data = &*ptr_template_data->next_data.back();
						next_layer = layer;
						++i;
					}
					else
					{
						temp += name[i];
					}
				}

				if(i>=size)
				{
					parse_modifier(name, ptr_template_data->mdf);
					trim_name(name);
					meta::trim_right(name);
					ptr_template_data->name = name;
					name.clear();
				}
				else
				{
					name = name.substr(i+1, -1);
					if(!name.empty() && name[0]!=':')
					{
						parse_modifier(name, ptr_template_data->mdf);
					}
					
					meta::trim_left(name);
					meta::trim_right(name);
				}
			}
		}

		void reflect::_combine_name(std::string& ret, type_data& _template_data, bool flag)
		{
			std::string name = _template_data.name;
			if(0==name.compare("std::basic_string"))
			{
				if(0==_template_data.sub_data[0]->name.compare("char"))
				{
					name = "std::string";
				}
				else
				{
					name = "std::wstring";
				}
			}

			if(_template_data.mdf.is_const())
			{
				name = std::string("const ") + name;
			}
			ret += name;
			size_t _max = _template_data.sub_data.size();

			auto it = get_map_type()->find(_template_data.name);
			if(it!=get_map_type()->end())
			{
				_max = Min((size_t)it->second, _max);
			}
			
			for(size_t i=0; i<_max; ++i)
			{
				if(0==i)
				{
					ret += "<";
				}
				_combine_name(ret, *_template_data.sub_data[i], false);
				if(i!=_max-1)
				{
					ret += ",";
				}
				else
				{
					ret += ">";
				}
			}

			if(!_template_data.next_data.empty())
			{
				ret += "::";
				ret += reflect::_get_type_name(_template_data.next_data);
			}

			if(_template_data.mdf.is_pointer())
			{
				ret += "*";
			}
			if(_template_data.mdf.is_reference())
			{
				ret += "&";
			}
		}

		std::string reflect::_get_type_name(std::vector<std::shared_ptr<type_data> >& _list_template_data)
		{
			std::string ret;
			ret.reserve(512);

			for(size_t i=0; i<_list_template_data.size(); ++i)
			{
				type_data& _template_data = *_list_template_data[i];
				if(i!=0)
				{
					if(!_template_data.name.empty() && _template_data.name[0]!='[')
					{
						ret += "::";
					}
				}
				_combine_name(ret, _template_data, true);
			}
			return ret;
		}

		std::string reflect::get_type_name(const std::string& raw_name)
		{
			std::string name = raw_name;
			trim_name(name);

			if(raw_name.find("<")!=std::string::npos)
			{
				name = type_manager::_get_existed_class_name(raw_name);
				if(name.empty())
				{
					std::vector<std::shared_ptr<type_data> > _list_template_data;
					_list_template_data.reserve(4);
					_parse_template(raw_name, _list_template_data);
					name = _get_type_name(_list_template_data);
					type_manager::_add_class_name(raw_name, name);
				}
			}
			return name;
		}
	}
}