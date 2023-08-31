#include "PlatformGame.h"
#include "Player.h"

#include "FrameWork/Framework.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"

bool PlatformGame::Initialize()
{
	// load audio
	kiko::g_audioSystem.AddAudio("Laser", "Audio/Laser_Shoot.wav");

	// create scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/platform_scene.json");
	m_scene->Load("Scenes/tilemap.json");
	m_scene->Initialize();

	m_scene->GetActorByName("Game Over")->active = false;
	m_scene->GetActorByName("Title_Background")->active = true;
	m_scene->GetActorByName("World")->active = false;

	// add events
	EVENT_SUBSCRIBE("AddPoints", PlatformGame::AddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", PlatformGame::OnPlayerDead);

	return false;
}

void PlatformGame::Shutdown() {}

void PlatformGame::Update(float dt)
{
	kiko::Player* player = m_scene->GetActor<kiko::Player>();

	switch (m_state)
	{
	case PlatformGame::Title:
	{
		m_scene->GetActorByName("Player")->active = false;
		m_scene->GetActorByName("Bat")->active = false;
		m_scene->GetActorByName("Frog")->active = false;

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_state = StartGame;
			m_scene->GetActorByName("Title")->active = false;
		}
		break;
	}
		break;

	case PlatformGame::StartGame:
		m_scene->GetActorByName("Player")->active = true;
		m_scene->GetActorByName("Bat")->active = true;
		m_scene->GetActorByName("Frog")->active = true;
		player->health = 4;

		m_state = Game;

		break;

	case PlatformGame::StartLevel:
		break;

	case PlatformGame::eState::Game:
		if (player->destoryed)
		{
			m_state = GameOver;
		}
		break;

	case PlatformGame::eState::PlayerDeadStart:
		break;

	case PlatformGame::PlayerDead:
		break;

	case PlatformGame::GameOver:
		m_scene->GetActorByName("Game Over")->active = true;
		break;

	default:
		break;
	}

	m_scene->Update(dt);
}

void PlatformGame::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);
}

void PlatformGame::AddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}

void PlatformGame::OnPlayerDead(const kiko::Event& event)
{
	m_scene->GetActorByName("Game Over")->active = true;
	//m_state = eState::PlayerDeadStart;
	m_state = eState::GameOver;
}
