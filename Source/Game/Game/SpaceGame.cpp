#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"

#include "FrameWork/Scene.h"

#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/ModelManager.h"
#include "Weapon.h"

bool SpaceGame::Initalize()
{
	// create text
	m_font = std::make_shared<kiko::Font>("ArcadeClassic.ttf", 24);
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
	kiko::g_audioSystem.AddAudio("Laser", "Laser_Shoot.wav");

	m_scene = std::make_unique<kiko::Scene>();
	
	return false;
}

void SpaceGame::Shutdown()
{

}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
		case SpaceGame::Title:
			if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
				m_state = StartGame;

			break;
		
		case SpaceGame::StartGame:
			//m_score = 0;
			m_lives = 3;
			m_state = StartLevel;

			break;
        
		case SpaceGame::StartLevel:
		{
			m_scene->RemoveAll();

			//creates player
			std::unique_ptr<Player> player = std::make_unique<Player>(20.0f, kiko::Pi, kiko::Transform{ {400, 300 }, 0, 6}, kiko::g_modelManager.Get("Star_Shape.txt"));
			player->m_tag = "Player";
			player->m_game = this;
			player->SetDamping(0.9f);
			m_scene->Add(std::move(player));
			m_state = eState::Game;
		}
			break;
		
		case SpaceGame::eState::Game:
			m_gameTimer += dt;
			m_SpawnTimer += dt;

			if (m_SpawnTimer >= m_SpawnTime)
			{
				m_SpawnTimer = 0;
				if (m_round == 1)
				{
					// creates enemy
					std::unique_ptr<Enemy> enemyArrow = std::make_unique<Enemy>(kiko::randomf(75.0f, 150.f), kiko::Pi, kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 6 }, kiko::g_modelManager.Get("Arrow.txt"));
					enemyArrow->m_tag = "Enemy";
					enemyArrow->m_enemyType = "enemyArrow";
					enemyArrow->m_game = this;
					enemyArrow->m_game->SetEnemyLives(1);
					m_scene->Add(std::move(enemyArrow));
				}
				else
				{
					// creates enemy
					std::unique_ptr<Enemy> enemyPlane = std::make_unique<Enemy>(kiko::randomf(75.0f, 150.f), kiko::Pi, kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 6 }, kiko::g_modelManager.Get("Plane.txt"));
					enemyPlane->m_tag = "Enemy";
					enemyPlane->m_enemyType = "enemyPlane";
					enemyPlane->m_game = this;
					enemyPlane->m_game->SetEnemyLives(3);
					m_scene->Add(std::move(enemyPlane));

				}

				std::unique_ptr<Weapon> bonusPoints = std::make_unique<Weapon>(0.0f, kiko::Pi, kiko::Transform{ {kiko::random(800), kiko::random(600)}, 0, 3 }, kiko::g_modelManager.Get("Diamond.txt"));
				bonusPoints->m_tag = "bonusPoints";
				bonusPoints->m_game = this;
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
