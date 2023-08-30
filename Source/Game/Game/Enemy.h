#pragma once
#include "FrameWork/Actor.h"
#include "FrameWork/Components/PhysicsComponent.h"

namespace kiko
{
	class Enemy : public Actor
	{
	public:
		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;

		void OnDestory() override;

	public:
		float speed = 0;
		float turnRate = 0;

		float fireTimer = 0;
		float fireRate = 0;

		int eLives = 0;

		class kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}