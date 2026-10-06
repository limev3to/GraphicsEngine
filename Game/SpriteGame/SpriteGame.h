#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"
#include "Resources/ResourceManager.h"

class SpriteGame : public nu::Game {

public:
	enum class GameState {
		Title,
		StartGame,
		StartLevel,
		Game,
		GameOver
	};

public :
	SpriteGame() = default;
	SpriteGame(nu::Scene* scene) :
		Game( scene )
	{ }

	bool Initialize() override;

	void Update(float dt) override;
	void Draw(class nu::Renderer& renderer) override;

	void OnPlayerDead();
	void AddPoints(int points) { m_score += points; }
	int GetPoints() { return m_score; }

private: 
	void SpawnPlayer();
	void SpawnEnemy();
	void SpawnItem();

private:
	int m_score = 0;
	int m_lives = 0;

	float m_stateTimer = 0.0f;

	float m_spawnTimer = 0.0f;
	float m_spawnTime = 5.0f;
	int m_spawnCount = 0;

	int m_enemyCount = 0;

	float m_itemSpawnTimer = 0.0f;
	float m_itemSpawnTime = 10.0f;

	GameState m_gameState = GameState::Title;
	 
	nu::Text* m_titleText{ nullptr };
	nu::Text* m_gameOverText{ nullptr };

	nu::Text* m_scoreText{ nullptr };		
	nu::Text* m_livesText{ nullptr };	
};