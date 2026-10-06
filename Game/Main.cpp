#include "Engine.h"

using namespace nu;

int main()
{
    SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();
    Engine::Get().GetAudio().Initialize();

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

        // RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetPS().Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN

    Engine::Get().GetAudio().Shutdown();

    Engine::Get().Shutdown();
    return 0;
}
