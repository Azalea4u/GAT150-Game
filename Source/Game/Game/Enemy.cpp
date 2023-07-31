#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "FrameWork/Scene.h"
#include "Renderer/Renderer.h"
#include "FrameWork/Emitter.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player)
	{
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;

		//turn towards player
		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		m_transform.rotation += turnAngle * dt;

		// checks if player is in front
		if (std::fabs(turnAngle) < kiko::DegreesToRadians(30.0f))
		{
			// I see you
		}
	}

	m_transform.position += forward * m_speed * dt;
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());


	// sets the enemy to fire at a certain rate
	if (m_fireRate)
	{
		m_fireRate -= dt;
		if (m_fireRate <= 0.0f)
		{
			// create weapon
			kiko::Transform transform{ m_transform.position, m_transform.rotation, 1 };
			std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(400.0f, transform, m_model);
			m_scene->Add(std::move(weapon));

			m_fireRate = m_fireTime;
		}
	}
}

void Enemy::OnCollision(Actor* other)
{
	if (other->m_tag == "Player" && m_enemyType == "enemyArrow")
	{
		m_game->AddPoints(100);
		m_destoryed = true;

		kiko::EmitterData data;
		data.burst = true;
		data.burstCount = 100;
		data.spawnRate = 200;
		data.angle = 0;
		data.angleRange = kiko::Pi;
		data.lifetimeMin = 0.5f;
		data.lifetimeMin = 1.5f;
		data.speedMin = 50;
		data.speedMax = 250;
		data.damping = 0.5f;

		data.color = kiko::Color{ 1, 0, 0, 1 };

		kiko::Transform transform{ m_transform.position, 0, 1 };
		auto emitter = std::make_unique<kiko::Emitter>(transform, data);
		emitter->m_lifespan = 0.1f;
		m_scene->Add(std::move(emitter));
		
	}
	else if (other->m_tag == "Player" && m_enemyType == "enemyPlane")
	{
		// enemyPlane has 3 lives and will be destroyed after 3 hits
		if (other->m_tag == "Player" && m_enemyType == "enemyPlane" && m_game->GetEnemyLives() == 3)
		{
			m_game->SetEnemyLives(2);
		}
		else if (other->m_tag == "Player" && m_enemyType == "enemyPlane" && m_game->GetEnemyLives() == 2)
		{
			m_game->SetEnemyLives(1);
		}
		else if (other->m_tag == "Player" && m_enemyType == "enemyPlane" && m_game->GetEnemyLives() == 1)
		{
			m_game->SetEnemyLives(0);
		}

		if (other->m_tag == "Player" && m_enemyType == "enemyPlane" && m_game->GetEnemyLives() == 0)
		{
			m_game->AddPoints(250);

			m_destoryed = true;
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.5f;
			data.lifetimeMin = 1.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 0, 0, 1 };
			kiko::Transform transform{ m_transform.position, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->m_lifespan = 0.1f;
			m_scene->Add(std::move(emitter));
		}

	}
}
