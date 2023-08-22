#pragma once
#include "FrameWork/Actor.h"
#include "Input/InputSystem.h"

namespace kiko
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon)

		/*
		Weapon(float speed, float turnRate, const kiko::Transform& transform) :
			Actor{ transform },
			m_speed{ speed },
			m_turnRate{ turnRate }
		{
			lifespan = 3.0f;
		}
		*/

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(kiko::Actor* other);

	private:
		float speed = 0;
		float m_turnRate = 0;

		class PhysicsComponent* m_physicsComponent = nullptr;
	};


}