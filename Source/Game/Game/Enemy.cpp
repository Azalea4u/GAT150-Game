#include "Enemy.h"
#include "Enemy.h"
#include "Player.h"
#include "Weapon.h"
#include "SpaceGame.h"
#include "Renderer/Renderer.h"
#include "FrameWork/Framework.h"

namespace kiko
{
	CLASS_DEFINITION(Enemy);


	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();

		return false;
	}

	void Enemy::OnDestory()
	{
		Actor::OnDestory();
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			kiko::Vector2 direction = player->transform.position - transform.position;
			m_physicsComponent->ApplyForce(direction.Normalized() * speed);			
		}

		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Player" && enemyType == "enemyArrow")
		{
			m_game->AddPoints(100);
			destoryed = true;

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

			kiko::Transform transform{ this->transform.position, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->lifespan = 0.1f;
			m_scene->Add(std::move(emitter));

		}
		else if (other->tag == "Player" && enemyType == "enemyPlane")
		{
			// enemyPlane has 3 lives and will be destroyed after 3 hits
			if (other->tag == "Player" && enemyType == "enemyPlane" && m_game->GetEnemyLives() == 3)
			{
				m_game->SetEnemyLives(2);
			}
			else if (other->tag == "Player" && enemyType == "enemyPlane" && m_game->GetEnemyLives() == 2)
			{
				m_game->SetEnemyLives(1);
			}
			else if (other->tag == "Player" && enemyType == "enemyPlane" && m_game->GetEnemyLives() == 1)
			{
				m_game->SetEnemyLives(0);
			}

			if (other->tag == "Player" && enemyType == "enemyPlane" && m_game->GetEnemyLives() == 0)
			{
				m_game->AddPoints(250);

				destoryed = true;
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

				kiko::Transform transform{ this->transform.position, 0, 1 };
				auto emitter = std::make_unique<kiko::Emitter>(transform, data);
				emitter->lifespan = 0.1f;
				m_scene->Add(std::move(emitter));
			}

		}
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
		READ_DATA(value, fireRate);
		READ_DATA(value, tag);
	}
}