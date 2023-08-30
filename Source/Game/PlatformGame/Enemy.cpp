#include "Enemy.h"
#include "Player.h"

#include "Input/InputSystem.h"
#include "FrameWork/Framework.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(Enemy);

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		m_spriteAnimRenderComponent = GetComponent<SpriteAnimRenderComponent>();


		return false;
	}

	void Enemy::OnDestory()
	{
		Actor::OnDestory();
	}

	void Enemy::Update(float dt)
	{
		Actor::Update(dt);

		// animation
		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		// check if player is moving
		auto player = m_scene->GetActorByName("Player");
		if (player)
		{
			kiko::Vector2 direction = player->transform.position - transform.position;
			if (enemyType == "Frog")
			{
				direction.y = 0;
				if (std::fabs(direction.x) < 0.1f)
				{
					m_spriteAnimRenderComponent->SetSequence("idle");
				}
				else if (std::fabs(direction.x) > 0.1f)
				{
					if (direction.x < 0) m_spriteAnimRenderComponent->flipH = false;
					else m_spriteAnimRenderComponent->flipH = true;
					m_spriteAnimRenderComponent->SetSequence("jump");
				}
			}
			m_physicsComponent->ApplyForce(direction.Normalized() * 20);
		}

		// frog animation
		if (enemyType == "Frog")
		{
			
		}

		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.GetHeight());
	}


	void Enemy::OnCollisionEnter(Actor* other)
	{
		
		if (other->tag == "Player" && kiko::g_inputSystem.GetMouseButtonDown(0))
		{
			// Compute knockback direction: from the enemy towards the player.
			kiko::vec2 knockbackDirection = transform.position - other->transform.position;
			knockbackDirection.Normalize();

			// Apply the knockback force.
			m_physicsComponent->SetVelocity(knockbackDirection * knockbackForce);

			destoryed = true;
			std::cout<<"Enemy destoryed"<<std::endl;
		}

		if (other->tag == "Ground")
			groundCount++;
	}

	void Enemy::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground")
			groundCount--;
	}

	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, jump);
		READ_DATA(value, health);
		READ_DATA(value, enemyType);
		READ_DATA(value, tag);
	}



}