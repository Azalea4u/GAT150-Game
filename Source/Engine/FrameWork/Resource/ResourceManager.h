#pragma once
#include "Resource.h"
#include <map>
#include <memory>
#include <string>
#include <cstdarg>

namespace kiko
{
	class ResourceManager
	{
	public:
		template<typename T, typename ... TArgs>
		//std::shared_ptr<T> Get(const std::string& filename, TArgs ... args);
		res_t<T> Get(const std::string& filename, TArgs ... args);

	private:
		std::map<std::string, std::shared_ptr<Resource>> m_resources;
	};

	template<typename T = Resource, typename ... TArgs>
	inline res_t<T> Get(const std::string& filename, TArgs ... args)
	{
		if (m_resources.find(filename) != m_resources.end())
		{
			return std::dynamic_pointer_cast<T>m_resources[filename];
		}

		res_t<T> resource = std::make_shared<T>();
		resource->Create(filename, args...);
		m_resources[filename] = resource;

		return resource;
	}

	extern ResourceManager g_resourceManager;
}