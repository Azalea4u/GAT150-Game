#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"

#include "FrameWork/Framework.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"

bool SpaceGame::Initialize()
{
	// create text
	m_font = GET_RESOURCE(kiko::Font, "Fonts/ArcadeClassic.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(m_font);
	m_scoreText->Create(kiko::g_renderer, "SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

	m_highscoreText = std::make_unique<kiko::Text>(m_font);
	m_highscoreText->Create(kiko::g_renderer, "HIGH SCORE 0000", kiko::Color{ 1, 0, 1, 1 });

	m_titleText = std::make_unique<kiko::Text>(m_font);
	m_titleText->Create(kiko::g_renderer, "ASTEROIDs", kiko::Color{ 1, 1, 1, 1 });

	m_gameoverText = std::make_unique<kiko::Text>(m_font);
	m_gameoverText->Create(kiko::g_renderer, "GAME OVER", kiko::Color{ 1, 1, 1, 1 });

	m_timerText = std::make_unique<kiko::Text>(m_font);
	m_timerText->Create(kiko::g_renderer, "TIMER", kiko::Color{ 1, 1, 1, 1 });

	// load audio
	kiko::g_audioSystem.AddAudio("Audio/Laser", "Laser_Shoot.wav");

	// create scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/space_scene.json");
	m_scene->Initialize();

	// add events
	EVENT_SUBSCRIBE("AddPoints", SpaceGame::AddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", SpaceGame::OnPlayerDead);

	return false;
}

void SpaceGame::Shutdown()	{}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
		case SpaceGame::Title:
				m_scene->GetActorByName("Title")->active = true;
			if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
			{
				m_state = StartGame;
				auto actor = m_scene->GetActorByName("Background");
				if (actor) actor->active = false;

			}
			break;
		
		case SpaceGame::StartGame:
			//m_score = 0;
			m_lives = 3;
			m_state = StartLevel;

			break;
        
		case SpaceGame::StartLevel:
			m_scene->RemoveAll();

			//m_scene->Load("Scenes/space_scene.json");
			//m_scene->Initialize();
		{
			//create player
					
			auto player = INSTANTIATE(Player, "Player");
			player->transform = kiko::Transform{ {400, 300 }, 0, 1 };
			player->Initialize();
			m_scene->Add(std::move(player));
		}
			m_state = eState::Game;
			break;
		
		case SpaceGame::eState::Game:
			m_gameTimer += dt;
			m_SpawnTimer += dt;

			if (m_SpawnTimer >= m_SpawnTime)
			{
				
				m_SpawnTimer = 0;
				if (m_round == 1)
				{
					auto enemyArrow = INSTANTIATE(Enemy, "Enemy");
					enemyArrow->transform = kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 1 };
					enemyArrow->Initialize();
					m_scene->Add(std::move(enemyArrow));
				}
				else
				{
					auto enemyPlane = INSTANTIATE(Enemy, "Enemy");
					enemyPlane->transform = kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 1 };
					enemyPlane->Initialize();
					m_scene->Add(std::move(enemyPlane));
				}

				/*
				std::unique_ptr<kiko::Weapon> bonusPoints = std::make_unique<kiko::Weapon>(0.0f, kiko::Pi, kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 1 });
				bonusPoints->tag = "bonusPoints";
				bonusPoints->
				
				
				
				
				= this;

				// create components
				auto component = std::make_unique<kiko::SpriteRenderComponent>();
				component->m_texture = GET_RESOURCE(kiko::Texture, "pickup.png", kiko::g_renderer);
				bonusPoints->AddComponent(std::move(component));

				m_scene->Add(std::move(bonusPoints));
				*/

				auto bonusPoints = INSTANTIATE(Weapon, "bonusPoints");
				bonusPoints->transform = kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 1 };
				bonusPoints->Initialize();
				m_scene->Add(std::move(bonusPoints));
			}

			if (m_gameTimer >= 30 && m_round == 1)
			{
				m_round++;
				//m_state = eState::RoundTwo;
			}

			break;

		case eState::PlayerDeadStart:
			m_stateTimer = 3;

			if (m_lives == 0) m_state = eState::GameOver;
			else m_state = eState::PlayerDead;

			break;
		
		case SpaceGame::PlayerDead:
			m_stateTimer -= dt;
			if (m_stateTimer <= 0)
			{
				m_state = StartLevel;
			}

			break;

		case SpaceGame::GameOver:
			m_stateTimer -= dt;
			if (GetScore() >= GetHighScore())
			{
				SetHighScore(GetScore());
				m_highscoreText->Create(kiko::g_renderer, "HIGH SCORE " + std::to_string(GetHighScore()), kiko::Color{ 1, 1, 1, 1 });
			}

			if (m_stateTimer <= 0)
			{
				m_scene->RemoveAll();
				m_score = 0;
				m_gameTimer = 0;
				m_round = 1;
				m_state = Title;
			}

			break;

		default:
			break;
	}

	m_scoreText->Create(kiko::g_renderer, "SCORE " + std::to_string(m_score), kiko::Color{ 1, 1, 1, 1 });
	m_timerText->Create(kiko::g_renderer, std::to_string((int)m_gameTimer), kiko::Color{ 1, 1, 1, 1 });
	m_scene->Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer)
{
	if (m_state == eState::Title)
	{
		m_titleText->Draw(renderer, 350, 300);
	}

	if (m_state == eState::GameOver)
	{
		m_gameoverText->Draw(renderer, 350, 300);
	}

	m_highscoreText->Draw(renderer, 600, 40);
	m_timerText->Draw(renderer, 400, 40);
	m_scoreText->Draw(renderer, 40, 40);
	m_scene->Draw(renderer);
}

void SpaceGame::AddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}

void SpaceGame::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}
