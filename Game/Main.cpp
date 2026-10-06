#include "Engine.h"
#include "SpriteGame/SpriteGame.h"


using namespace nu;


int main()
{
    SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();
    Engine::Get().GetAudio().Initialize();

    auto spriteGame = std::make_unique<SpriteGame>();

    spriteGame->Initialize();

    Engine::Get().GetAudio().AddSound("bgm", "audio/tech-space.wav");
    Engine::Get().GetAudio().AddSound("laser", "audio/laser.wav");
    Engine::Get().GetAudio().AddSound("explosion", "audio/explosion.wav");
    Engine::Get().GetAudio().AddSound("itemPickup", "audio/item_pickup.wav");
    Engine::Get().GetAudio().AddSound("enemyExplosion", "audio/enemy_explosion.wav");

    // MAIN LOOP
    bool quit = false;

    while (!quit) {

        // UPDATE
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }

        Engine::Get().GetAudio().Update();
        
        // engine
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();

        // GAME
        spriteGame->Update(dt);

        // RENDER
        Engine::Get().GetRenderer().SetColor(0.0f, 0.0f, 0.0f);
        Engine::Get().GetRenderer().Clear();
        spriteGame->Draw(Engine::Get().GetRenderer()); 

        Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());
        Engine::Get().GetRenderer().Present();
    }

    // SHUTDOWN

    Engine::Get().GetAudio().Shutdown();

    spriteGame.reset();
    Engine::Get().Shutdown();
    return 0;
}
