#pragma once
#include "Core/Core.h"
#include "FrameWork/Actor.h"

class Enemy : public kiko::Actor
{
public:
	Enemy() = default;
	Enemy(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr < kiko::Model> model) :
		Actor(transform, model),
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
		m_fireTime = 2.0f;
		m_fireRate = m_fireTime;
	}

	void Update(float dt) override; 
	// virtual makes it a Pure virtual function
	void OnCollision(Actor* other) override;


protected:
	float m_speed = 0;
	float m_turnRate = 0;

	float m_fireTime = 0;
	float m_fireRate = 0;

	int m_eLives = 0;
};