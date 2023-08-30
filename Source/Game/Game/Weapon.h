#pragma once
#include "FrameWork/Actor.h"
#include "FrameWork/Components/PhysicsComponent.h"

namespace kiko
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon)

		bool Initialize() override;
		void OnDestory() override;
		void Update(float dt) override;

		void OnCollisionEnter(Actor* other);

	private:
		float speed = 0;
		float turnRate = 0;

		class PhysicsComponent* m_physicsComponent = nullptr;
	};


}