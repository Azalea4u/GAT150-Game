#pragma once
#include "Core/Core.h"
#include "FrameWork/Actor.h"

namespace kiko
{
	class Player : public Actor
	{

	public:
		CLASS_DECLARATION(Player);

		bool Initialize() override;
		void OnDestory() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;

	public:
		float speed = 0;
		float turnRate = 0; 

		class PhysicsComponent* m_physicsComponent = nullptr;
	};
}