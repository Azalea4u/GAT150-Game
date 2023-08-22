#pragma once
#include "Core/Core.h"
#include "FrameWork/Framework.h"

class Player : public kiko::Actor
{
public:
	Player() = default;
	Player(float speed, float turnRate, const kiko::Transform& transform) :
		Actor{ transform },
		m_speed { speed },
		m_turnRate{ turnRate }
	{}

	bool Initialize() override;
	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;

	class kiko::PhysicsComponent* m_physicsComponent = nullptr;
};