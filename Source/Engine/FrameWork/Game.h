#pragma once
#include <memory>

namespace kiko
{
	class Scene;

	class Game
	{
	public:
		Game() = default;
		virtual ~Game() = default;

		virtual bool Initalize() = 0;
		virtual void Shutdown() = 0;
		
		virtual void Update(float dt) = 0;
		virtual void Draw(class Renderer& renderer) = 0;

		int GetScore() const { return m_score; }

		int GetHighScore() const { return m_highScore; }
		void SetHighScore(int score) { m_highScore = score; }

		int AddPoints(int points) 
		{ 
			m_score += points;
			return m_score;
		}

		int GetLives() const { return m_lives; }
		void SetLives(int lives) { m_lives = lives; }

		int GetEnemyLives() const { return m_enemyLives; }
		void SetEnemyLives(int lives) { m_enemyLives = lives; }

		std::unique_ptr<Scene> m_scene;

	protected:
		int m_highScore = 0;
		int m_score = 0;
		int m_lives = 0;

		int m_enemyLives = 0;
	};
}