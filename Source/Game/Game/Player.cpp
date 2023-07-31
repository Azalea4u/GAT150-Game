#include "Player.h"
#include "SpaceGame.h"
#include "FrameWork/Scene.h"
#include "Input/InputSystem.h"
#include "Weapon.h"

void Player::Update(float dt)
{
	Actor::Update(dt);

	//movement
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * dt;

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;


	//fire weapon
	if ((kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE)) && (!kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)))
	{
		// create weapon
		kiko::Transform transform1{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(10.0f), 1};
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>( 400.0f, transform1, m_model );
		weapon->m_tag = "Player";
		m_scene->Add(std::move(weapon));

		kiko::Transform transform2{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(10.0f), 1};
		weapon = std::make_unique<Weapon>(400.0f, transform2, m_model);
		weapon->m_tag = "Player";
		m_scene->Add(std::move(weapon));
	}

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	AddForce(forward * thrust * m_speed);

	//m_transform.position += forward * m_speed * thrust * dt;
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.GetHeight());

	if ((kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T))) kiko::g_time.SetTimeScale(0.5f);
	else kiko::g_time.SetTimeScale(1.0f);
}

void Player::OnCollision(Actor* other)
{
	//Player* p = dynamic_cast<Player*>(other);
	
	if (other->m_tag == "Enemy")
	{
		m_game->SetLives(m_game->GetLives() - 1);
		m_destoryed = true;
		dynamic_cast<SpaceGame*> (m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
	}
}
