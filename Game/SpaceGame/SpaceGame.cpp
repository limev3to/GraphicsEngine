#include "SpaceGame.h"
#include "Engine.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Assets.h"

#include <memory>
#include <iostream>

using namespace nu;

bool SpaceGame::Initialize()
{
    SetWorkingDirectory("SpaceGame");
    Game::Initialize();

    m_scene = std::make_unique<Scene>();

    m_scene->SetGame(this);

    m_scene->Load("data/scene.json");
   
    m_titleText = new Text(Resources().GetWithID<Font>("title_font", "fonts/Airspace.ttf", 64.0f));
    m_titleText->Create(Engine::Get().GetRenderer(), "Graveyard Mash", Color{ 1.0f, 1.0f, 1.0f });

    m_scoreText = new Text(Resources().GetWithID<Font>("game_font", "fonts/Airspace.ttf", 32.0f));
    m_livesText = new Text(Resources().Get<Font>("game_font", 32.0f));

    //Engine::Get().GetAudio().AddSound("laser", "audio/laser.wav");
    //Engine::Get().GetAudio().AddSound("explosion", "audio/explosion.wav");

    return false;
}

void SpaceGame::Update(float dt)
{
    switch (m_gameState)
    {
    case GameState::Title:
        
        if (Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {
            m_gameState = GameState::StartGame;
            Engine::Get().GetAudio().PlaySound("bgm");
        }

        break;
    case GameState::StartGame:
        
        m_score = 0;
        m_lives = 3;

        m_itemSpawnTime = 10.0f;

        m_spawnTime = 5.0f;
        m_stateTimer = 0.5f;
        m_gameState = GameState::StartLevel;
        break;

    case GameState::StartLevel:
        
        m_stateTimer -= dt;
        if (m_stateTimer <= 0) {
            m_scene->RemoveAllActors();
            SpawnPlayer();
            m_spawnTime = 5.0f;
            m_itemSpawnTimer = 10.0f;
            m_gameState = GameState::Game;
        }
        break;

    case GameState::Game:
        
        m_spawnTimer -= dt;
        if (m_spawnTimer <= 0.0f) {
            m_spawnTimer = m_spawnTime;
            SpawnEnemy();
            m_spawnCount++;
            if (m_spawnCount > 5) {
                m_spawnCount = 0;
                m_spawnTime -= 0.5f;
            }
        }

        m_itemSpawnTimer -= dt;
        if (m_itemSpawnTimer <= 0) {
            m_itemSpawnTimer = m_itemSpawnTime;
            SpawnItem();
        }
        break;

    case GameState::GameOver:

        m_stateTimer -= dt;
        if (m_stateTimer <= 0) {
            m_scene->RemoveAllActors();
            m_gameState = GameState::Title;
        }
        break;

    default:
        
        break;
    
    }

    Game::Update(dt);
}

void SpaceGame::Draw(nu::Renderer& renderer)
{
    renderer.DrawTexture(*nu::Resources().Get<Texture>("textures/background.png", nu::Engine::Get().GetRenderer()), 0, 0, 0, 5);

    switch (m_gameState)
    {
    case GameState::Title:
        // draw title
        m_titleText->Draw(renderer, 400, 400);
        break;
    case GameState::StartGame:
    case GameState::StartLevel:
    case GameState::Game:
        // draw score, lives
        m_scoreText->Create(renderer, "Score: " + std::to_string(m_score), { 1.0f, 1.0f, 1.0f });
        m_scoreText->Draw(renderer, 30, 30);

        m_livesText->Create(renderer, "Lives: " + std::to_string(m_lives), { 1.0f, 1.0f, 1.0f });
        m_livesText->Draw(renderer, (float)renderer.GetWidth() - 160, 30);

        break;
    case GameState::GameOver:
        // draw game over
        break;
    default:
        break;
    }

    Game::Draw(renderer);

}

void SpaceGame::OnPlayerDead() {

    std::cout << "Player DEAD" << std::endl;

    m_lives--;
    m_gameState = (m_lives == 0) ? GameState::GameOver : GameState::StartLevel;

    m_stateTimer = 2.0f;
}


void SpaceGame::SpawnPlayer() {
    //PlayerDesc playerDesc;
    //playerDesc.name = "Player";
    //playerDesc.tag = "Player";
    //playerDesc.model = assets::playerModel;
    //playerDesc.texture = Resources().Get<Texture>("textures/player.png", Engine::Get().GetRenderer());
    //playerDesc.transform = Transform{ Vector2{ 640.0f, 512.0f }, 0.0f, 1.0f };
    //playerDesc.velocity = Vector2{ 0.0f, 0.0f };
    //playerDesc.damping = 3.0f;
    //playerDesc.speed = 2000.0f;
    //std::unique_ptr<Player> player = std::make_unique<Player>(playerDesc);

    auto player = Factory::Instance().Create<Actor>("PlayerPrototype");
    m_scene->AddActor(std::move(player));
}

void SpaceGame::SpawnEnemy() {
    //EnemyDesc enemyDesc;
    //enemyDesc.name = "Enemy";
    //enemyDesc.tag = "Enemy";
    ////enemyDesc.model = assets::playerModel;
    //enemyDesc.texture = Resources().Get<Texture>("textures/enemy.png", Engine::Get().GetRenderer());
    //enemyDesc.transform = Transform{ Vector2{ nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetHeight())}, 0.0f, 1.0f };
    //enemyDesc.speed = RandomFloat(1000.0f, 2000.0f);
    //enemyDesc.damping = 3.0f;
    //std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(enemyDesc);

    auto enemy = Factory::Instance().Create<Actor>("EnemyPrototype");
    enemy->SetPosition({ RandomFloat(100.0f, 1820.0f), RandomFloat(100.0f, 980.0f) });
    m_scene->AddActor(std::move(enemy));
}

void SpaceGame::SpawnItem() {
    //ItemDesc itemDesc;
    //itemDesc.name = "Item";
    //itemDesc.tag = "Item";
    //itemDesc.model = assets::itemModel;
    //itemDesc.texture = Resources().Get<Texture>("textures/item.png", Engine::Get().GetRenderer());
    //itemDesc.transform = Transform{ Vector2{ nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetWidth()), nu::RandomFloat((float)nu::Engine::Get().GetRenderer().GetHeight())}, 0.0f, 0.5f };
    //itemDesc.speed = 0.0;
    //itemDesc.damping = 3.0f;
    //std::unique_ptr<Item> item = std::make_unique<Item>(itemDesc);

    auto item = Factory::Instance().Create<Actor>("ItemPrototype");
    item->SetPosition({ RandomFloat(100.0f, 1820.0f), RandomFloat(100.0f, 980.0f) });
    item->SetScale(0.5f);
    m_scene->AddActor(std::move(item));
}