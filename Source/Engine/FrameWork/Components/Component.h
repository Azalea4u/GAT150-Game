#pragma once
#include "FrameWork/Object.h"

namespace kiko
{
	class Component : public Object
	{
	public:
		virtual void Update(float dt) = 0;

		friend class Actor;

		Actor* GetOwner() const { return m_owner; }
	public:
		class Actor* m_owner = nullptr;
	};
}