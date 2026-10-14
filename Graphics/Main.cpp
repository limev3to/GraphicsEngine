#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

using namespace nu;

namespace nu {
    struct Color4 {
        float r, g, b, a;
    };
}

struct Vertex
{
    Vector3 position;
    Color4 color;
};

std::vector<Vertex> vertices =
{
    Vertex{ Vector3{ -0.5f, -0.5f, 0.0f}, Color4{ 1, 0, 0, 1 } }, // Bottom-Left
    Vertex{ Vector3{ 0.5f, -0.5f, 0.0f}, Color4{ 0, 1, 0, 1 } }, // Bottom-Right
    Vertex{ Vector3{ -0.5f, 0.5f, 0.0f}, Color4{ 0, 1, 1, 1 } }, // Top-Left
    Vertex{ Vector3{ 0.5f, 0.5f, 0.0f}, Color4{ 0, 0, 1, 1 } }, // Top-Right
};

struct vertexUniform
{
    glm::mat4 mvp;
};

vertexUniform uniform;
glm::vec3 cameraPosition{ 0, 0, 5 };

int main()
{
    SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();
    Engine::Get().GetAudio().Initialize();

    auto vb = std::make_shared<VertexBuffer>();
    vb->Create<Vertex>(vertices, Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer(), 1);
    auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer(), 1);

    auto pipeline = std::make_shared<Pipeline>();
    pipeline->AddVertexBuffer(sizeof(Vertex));
    // position
    pipeline->AddVertexAttribute(
        0,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
        offsetof(Vertex, position));
    // color
    pipeline->AddVertexAttribute(
        1,
        SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
        offsetof(Vertex, color));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    //float scale = 1;
    //Color4 color{ 0, 0, 0, 1 };

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
        
        // ENGINE
        Engine::Get().Update();
        float dt = Engine::Get().GetTime().GetDeltaTime();
        float time = Engine::Get().GetTime().GetTime();

        //scale = sin(Engine::Get().GetTime().GetTime());
        //color.r = sin(Engine::Get().GetTime().GetTime() * 3.0f) * 0.5f + 0.5f;

        glm::vec3 direction{ 0 };
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) direction.x = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) direction.x = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_E)) direction.y = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_Q)) direction.y = +1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) direction.z = -1;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) direction.z = +1;

        cameraPosition += direction * 4.0f * dt;

        // MODEL MATRIX
        // 1 0 0 0
        // 0 1 0 0
        // 0 0 1 0
        // 0 0 0 1

        auto mxt = glm::translate(glm::vec3{ 0.0f, 0.0f, 0.0f });
        auto mxr = glm::eulerAngleYXZ(0.0f, 0.0f, 0.0f);  //, time * 3.0f);
        auto mxs = glm::scale(glm::vec3( 1 ));

        auto model = mxt * mxr * mxs;

        // VIEW MATRIX
        
        auto view = glm::lookAtRH(cameraPosition, cameraPosition + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

        // PROJECTION MATRIX

        float aspectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 20.0f);

        uniform.mvp = projection * view * model;

        //uniform.model = mxs * mxr * mxt;
        //uniform.model = glm::mat4(1.0f);
        //uniform.model = glm::translate(uniform.model, glm::vec3(0.5f, 0.0f, 0.0f));
        //uniform.model = glm::rotate(uniform.model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //uniform.model = glm::scale(uniform.model, glm::vec3(0.5f, 0.5f, 0.5f));

        // RENDER
        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexBuffer(*vb);
        Engine::Get().GetRenderer().SetVertexUniform(0, &uniform, sizeof(uniform));
        //Engine::Get().GetRenderer().SetFragmentUniform(0, &color, sizeof(color));
        Engine::Get().GetRenderer().Draw(vb->GetVertexCount());
        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN

    Engine::Get().GetAudio().Shutdown();

    Engine::Get().Shutdown();
    return 0;
}