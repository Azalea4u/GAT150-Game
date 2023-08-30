#pragma once
#include "Core/Core.h"
#include "FrameWork/Actor.h"
#include "FrameWork/Components/PhysicsComponent.h"

namespace kiko
{
	class Enemy : public Actor
	{

	public:
		CLASS_DECLARATION(Enemy)

		bool Initialize() override;
		void OnDestory() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;

	public:
		float speed = 0;
		float jump = 0;
		float knockbackForce = 0;
		int groundCount = 0;
		int health = 1;

		class PhysicsComponent* m_physicsComponent = nullptr;
		class SpriteAnimRenderComponent* m_spriteAnimRenderComponent = nullptr;
	};
}