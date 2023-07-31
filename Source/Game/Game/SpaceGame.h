#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"

class SpaceGame : public kiko::Game
{
public:
	enum eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		RoundTwo,
		PlayerDeadStart,
		PlayerDead,
		GameOver
	};

public:
	virtual bool Initalize() override;
	virtual void Shutdown() override;

	virtual void Update(float dt) override;
	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

private:
	eState m_state = eState::Title;
	float m_SpawnTimer = 0.0f;
	float m_SpawnTime = 3.0f;

	float m_stateTimer = 0;
	float m_gameTimer = 0;

	int m_round = 1;


	std::shared_ptr<kiko::Font> m_font;
	std::unique_ptr<kiko::Text> m_scoreText;
	std::unique_ptr<kiko::Text> m_timerText;
	std::unique_ptr<kiko::Text> m_titleText;
	std::unique_ptr<kiko::Text> m_gameoverText;
	std::unique_ptr<kiko::Text> m_highscoreText;

	std::unique_ptr<kiko::Text> m_nextRoundText;
};