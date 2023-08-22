#pragma once
#include "Singleton.h"
#include "Core/Logger.h"
#include <memory>
#include <map>
#include <string>

#define CREATE_CLASS(classname) kiko::Factory::Instance().Create<kiko::classname>(#classname);
#define CREATE_CLASS_BASE(classbase, classname) kiko::Factory::Instance().Create<kiko::classbase>(classname);
#define INSTANTATE(classbase, classname) kiko::Factory::Instance().Create<kiko::classbase>(classname);

namespace kiko
{
	class CreatorBase
	{
	public:
		virtual ~CreatorBase() = default;

		virtual std::unique_ptr<class Object> Create() = 0;
	};

	template<typename T>
	class Creator : public CreatorBase
	{
	public:
		std::unique_ptr<class Object> Create()
		{
			return std::make_unique<T>();
		}
	};

	template<typename T>
	class PrototypeCreator : public CreatorBase
	{
	public:
		PrototypeCreator(std::unique_ptr<T> prototype)
			: m_prototype(std::move(prototype)) {}

		std::unique_ptr<class Object> Create()
		{
			return m_prototype->Clone();
		}

	private:
		std::unique_ptr<T> m_prototype;
	};

	class Factory : public Singleton<Factory>
	{
	public:
		template<typename T>
		void Register(const std::string& key);
		template<typename T>
		void RegisterPrototype(const std::string& key, std::unique_ptr<T> prototype);

		template<typename T>
		std::unique_ptr<T> Create(const std::string& key);

	private:
		std::map<std::string, std::unique_ptr<CreatorBase>> m_registery;
	};

	template<typename T>
	inline void Factory::Register(const std::string& key)
	{
		INFO_LOG("Class Registered: " << key);

		m_registery[key] = std::make_unique<Creator<T>>();
	}

	template<typename T>
	inline void Factory::RegisterPrototype(const std::string& key, std::unique_ptr<T> prototype)
	{
		INFO_LOG("Prototype Registered: " << key);

		m_registery[key] = std::make_unique<PrototypeCreator<T>>(std::move(prototype));
	}

	template<typename T>
	inline std::unique_ptr<T> Factory::Create(const std::string& key)
	{
		auto iter = m_registery.find(key);
		if (iter != m_registery.end())
		{
			return std::unique_ptr<T>(dynamic_cast<T*>(iter->second->Create().release()));
		}

		return std::unique_ptr<T>();
	}
}