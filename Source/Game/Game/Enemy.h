#pragma once
#include "Core/Core.h"
#include "FrameWork/Framework.h"

class Enemy : public kiko::Actor
{
public:
	Enemy() = default;
	Enemy(float speed, float turnRate, const kiko::Transform& transform) :
		Actor(transform),
		m_speed{ speed },
		m_turnRate{ turnRate }
	{
		m_fireTime = 2.0f;
		m_fireRate = m_fireTime;
	}

	bool Initialize() override;

	void Update(float dt) override; 
	void OnCollision(Actor* other) override;


protected:
	float m_speed = 0;
	float m_turnRate = 0;

	float m_fireTime = 0;
	float m_fireRate = 0;

	int m_eLives = 0;

	class kiko::PhysicsComponent* m_physicsComponent = nullptr;
};