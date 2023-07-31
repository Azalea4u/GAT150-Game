#pragma once
#include "Core/Core.h"
#include "FrameWork/Actor.h"

class Player : public kiko::Actor
{
public:
	Player() = default;
	Player(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr < kiko::Model> model) :
		Actor{ transform, model },
		m_speed { speed },
		m_turnRate{ turnRate }
	{}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;
};