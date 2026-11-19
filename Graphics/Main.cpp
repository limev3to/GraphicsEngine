#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Framework/Scene.h"

#include "Components/CameraComponent.h"

#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

using namespace nu;

int main()
{
    SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();

    Scene scene;
    scene.Load("scenes/scene.json");

    auto& renderer = Engine::Get().GetRenderer();

    for (int i = 0; i < 5; i++)
    {
        auto cow = Factory::Instance().Create<Actor>("CowPrototype");
        cow->SetPosition({ (rand() % 41) - 20, -0.5, (rand() % 41) - 20 });
        cow->SetRotation({-90, rand() % 360, 0});
        scene.AddActor(std::move(cow));
    }

    for (int i = 0; i < 5; i++)
    {
        auto horse = Factory::Instance().Create<Actor>("HorsePrototype");
        horse->SetPosition({ (rand() % 41) - 20, -0.5, (rand() % 41) - 20 });
        horse->SetRotation({-90, rand() % 360, 0});
        scene.AddActor(std::move(horse));
    }

    for (int i = 0; i < 5; i++)
    {
        auto bull = Factory::Instance().Create<Actor>("BullPrototype");
        bull->SetPosition({ (rand() % 41) - 20, -0.5, (rand() % 41) - 20 });
        bull->SetRotation({-90, rand() % 360, 0});
        scene.AddActor(std::move(bull));
    }

    for (int i = 0; i < 5; i++)
    {
        auto donkey = Factory::Instance().Create<Actor>("DonkeyPrototype");
        donkey->SetPosition({ (rand() % 41) - 20, -0.5, (rand() % 41) - 20 });
        donkey->SetRotation({-90, rand() % 360, 0});
        scene.AddActor(std::move(donkey));
    }

    // MAIN LOOP
    bool quit = false;
    while (!quit) {

        // UPDATE
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            Engine::Get().GetEditor().ProcessEvent(event);

            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE) {
                quit = true;
            }
        }

        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        // SCENE
        scene.Update(dt);

        // EDITOR
        Engine::Get().GetEditor().BeginFrame();
        Engine::Get().GetEditor().Update(scene);

        // RENDER
        renderer.BeginFrame();

        scene.Draw(renderer);

        Engine::Get().GetRenderer().EndRenderPass();
        Engine::Get().GetEditor().Draw(renderer);

        renderer.EndFrame();
    }

    // SHUTDOWN
    Engine::Get().Shutdown();

    return 0;
}