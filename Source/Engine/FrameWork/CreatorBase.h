#pragma once
#include "Singleton.h"
#include <memory>
#include <map>
#include <string>

namespace kiko
{
	class CreatorBase
	{
	public:
		virtual ~CreatorBase() = default;

		virtual std::unique_ptr<class Object> Create() = 0;
	};

	class Creator : public CreatorBase
	{
	public:
		template<typename T>
		std::unique_ptr<class Object> Create()
		{
			return std::make_unique<T>();
		}
	};


}