#include "Player.h"
#include "Input/InputSystem.h"
#include "FrameWork/Framework.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(Player);

	bool Player::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteAnimRenderComponent = GetComponent<SpriteAnimRenderComponent>();

		return false;
	}

	void Player::OnDestory()
	{
		Actor::OnDestory();
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);

		bool onGround = (groundCount > 0);
		vec2 velocity = m_physicsComponent->velocity;

		//movement
		float dir = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1;

		if (dir)
		{
		velocity.x += speed * dir * ((onGround) ? 1 : 0.25f) * dt;
		velocity.x = Clamp(velocity.x, -maxSpeed, maxSpeed);
		m_physicsComponent->SetVelocity(velocity);
		//if (onGround) m_physicsComponent->ApplyForce(forward * speed * dir); else m_physicsComponent->ApplyForce(forward * speed * dir * 0.25f);
		}

		// jump
		if ((onGround) &&
			(kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) 
			&& (!kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)))
		{		
			kiko::vec2 up = kiko::vec2{ 0, -1 };
			m_physicsComponent->SetVelocity(velocity + (up * jump));
		}
		m_physicsComponent->SetGravityScale((onGround) ? 1.0f : 1.5f);

		//if (velocity.y > 0) m_physicsComponent->SetGravityScale(1.0f);
		//else m_physicsComponent->SetGravityScale(1.5f);

		//need help where after going to the other side of the screen, all ground physics disappear
		// Screen wrapping logic
		float screenWidth = (float)kiko::g_renderer.GetWidth();

		if (transform.position.x < 0)
			transform.position.x += screenWidth;
		else if (transform.position.x > screenWidth)
			transform.position.x -= screenWidth;

		// animation
		// check if player is moving
		if (std::fabs(velocity.x) > 0.5f)
		{
			if (dir != 0) m_spriteAnimRenderComponent->flipH = (dir < 0);
			m_spriteAnimRenderComponent->SetSequence("run");
		}
		else
		{
			m_spriteAnimRenderComponent->SetSequence("idle");
		}


		// attack animation
		if ((onGround) && (kiko::g_inputSystem.GetMouseButtonDown(0)) 
			&& (!kiko::g_inputSystem.GetPreviousMouseButtonDown(0)))
		{
			m_spriteAnimRenderComponent->SetSequence("attack");
			/*
			// Get the mouse position
			kiko::Vector2 mousePos = kiko::g_inputSystem.GetMousePosition();
			// Calculate the distance between the player and the enemy
			kiko::Vector2 direction = transform.position - targetedEnemy->transform.position;
			float distance = direction.Length();

			// Define a range for mouse hover (adjust this value as needed)
			float hoverRange = 50.0f;
			// Check if the mouse is hovering over the enemy within the range
			if (distance <= hoverRange)
			{
				
			}*/
			
		}
		else if ((!onGround) && (kiko::g_inputSystem.GetMouseButtonDown(0))
			&& (!kiko::g_inputSystem.GetPreviousMouseButtonDown(0)))
		{
			m_spriteAnimRenderComponent->SetSequence("air_attack");
		}

	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Enemy" && (!kiko::g_inputSystem.GetMouseButtonDown(0)))
		{
			health--;

			if (health == 3)
				m_scene->GetActorByName("Hearts")->GetComponent<SpriteAnimRenderComponent>()->SetSequence("hearts_3");
			else if (health == 2)
				m_scene->GetActorByName("Hearts")->GetComponent<SpriteAnimRenderComponent>()->SetSequence("hearts_2");
			else if (health == 1)
				m_scene->GetActorByName("Hearts")->GetComponent<SpriteAnimRenderComponent>()->SetSequence("hearts_1");
			else if (health == 0)
				m_scene->GetActorByName("Hearts")->GetComponent<SpriteAnimRenderComponent>()->SetSequence("death");
			m_spriteAnimRenderComponent->UpdateSource();

			// Compute knockback direction: from the enemy towards the player.
			kiko::vec2 knockbackDirection = transform.position - other->transform.position;
			knockbackDirection.Normalize();

			// Apply the knockback force.
			//m_physicsComponent->ApplyForce(knockbackDirection * knockbackForce);
			m_physicsComponent->SetVelocity(knockbackDirection * knockbackForce);

			std::cout << "Player health: " << health << std::endl;

			if (health == 0)
			{
				destoryed = true;
				std::cout << "Player is dead" << std::endl;
			}
		}

		if (other->tag == "Ground")
			groundCount++;
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground")
			groundCount--;
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, maxSpeed);
		READ_DATA(value, jump);
		READ_DATA(value, health);
		READ_DATA(value, knockbackForce);
		 
		READ_DATA(value, tag);
	}
}