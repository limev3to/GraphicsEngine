#include "Engine.h"
#include "Core/File.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Pipeline.h"
#include "Resources/ResourceManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

using namespace nu;

enum class LightType : uint32_t
{
    Directional,
    Point,
    Spot
};

int main()
{
    SetWorkingDirectory("Assets");

    // INITIALIZATION
    Engine::Get().Initialize();
    Engine::Get().GetAudio().Initialize();

    auto model = Resources().Get<Model>("models/spot.obj", Engine::Get().GetRenderer());
    auto texture = Resources().Get<Texture>("textures/spot_diffuse.png", Engine::Get().GetRenderer());

    auto vshader = Resources().Get<nu::Shader>("shaders/gouraud.vert", Engine::Get().GetRenderer(), 3);
    auto fshader = Resources().Get<nu::Shader>("shaders/gouraud.frag", Engine::Get().GetRenderer(), 1, 1);

    auto pipeline = std::make_shared<Pipeline>();

    pipeline->AddVertexBuffer(sizeof(Model::Vertex));
    pipeline->AddVertexAttribute(0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, position));
    pipeline->AddVertexAttribute(1, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2, offsetof(Model::Vertex, texcoord));
    pipeline->AddVertexAttribute(2, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, normal));
    pipeline->AddVertexAttribute(3, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(Model::Vertex, tangent));

    pipeline->Create(*vshader.get(), *fshader.get(),
        Engine::Get().GetRenderer().GetGPUDevice(),
        Engine::Get().GetRenderer().GetWindow());

    //auto vshader = Resources().Get<nu::Shader>("shaders/position.vert", Engine::Get().GetRenderer(), 1);
    //auto fshader = Resources().Get<nu::Shader>("shaders/color.frag", Engine::Get().GetRenderer(), 1);
    //auto pipeline = std::make_shared<Pipeline>();
    //pipeline->AddVertexBuffer(sizeof(Vertex));
    //// position
    //pipeline->AddVertexAttribute(
    //    0,
    //    SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
    //    offsetof(Vertex, position));
    //// color
    //pipeline->AddVertexAttribute(
    //    1,
    //    SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
    //    offsetof(Vertex, color));
    //pipeline->Create(*vshader.get(), *fshader.get(),
    //    Engine::Get().GetRenderer().GetGPUDevice(),
    //    Engine::Get().GetRenderer().GetWindow());

    // UNIFORMS
    glm::vec4 color{ 0, 0, 0, 1};
    struct VertexUniform {
        glm::mat4 mvp;
        glm::mat4 model;
        glm::vec3 cameraPosition;
        float padding1;
    };
    
    
    VertexUniform vertexUniform;

    struct LightUniform {
        glm::vec3 position;
        LightType type;
        glm::vec3 direction;
        float padding2;
        glm::vec3 color;
        float intensity;
        glm::vec3 ambient;
        float padding3;
    };

    LightUniform lightUniform{
        .position{ 10, 10, 10 },
        .type{ LightType::Point },
        .direction{ 0, -1, 0},
        .color{ 0, 0, 1},
        .intensity{ 1 },
        .ambient{ 0.1f }
    };

    struct MaterialUniform {
        glm::vec2 tiling;
        glm::vec2 offset;
    };

    MaterialUniform materialUniform{
        .tiling{ 1.0f, 1.0f },
        .offset{ 0.0f, 0.0f }
    };

    vertexUniform.cameraPosition = glm::vec3{ 0, 0, 5 };

    Transform transform;

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

        vertexUniform.cameraPosition += direction * 4.0f * dt;

        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_1)) lightUniform.type = LightType::Directional;
        if (Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_2)) lightUniform.type = LightType::Point;


        //materialUniform.offset.x += time * 0.2;
        //materialUniform.tiling = glm::vec3{ 2.0f };

        //lightUniform.lightIntensity = std::sin(time) * 0.5f + 0.5f;

        //TRANSFORM
        //transform.rotation.z = time * 400.0;
        //transform.rotation.x = time * 40.0;
        transform.rotation.y = time * 90.0;


        // VIEW MATRIX
        auto view = glm::lookAtRH(vertexUniform.cameraPosition, vertexUniform.cameraPosition + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

        // PROJECTION MATRIX
        float aspectRatio = Engine::Get().GetRenderer().GetWidth() / (float)Engine::Get().GetRenderer().GetHeight();
        auto projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 20.0f);

        // MVP
        vertexUniform.model = transform.GetMatrix();
        vertexUniform.mvp = projection * view * vertexUniform.model;

        // RENDER
        Color tint = Color();
        tint.r = 1.0f;
        tint.g = 1.0f;
        tint.b = 1.0f;

        Engine::Get().GetRenderer().BeginFrame();

        Engine::Get().GetRenderer().SetPipeline(*pipeline);
        Engine::Get().GetRenderer().SetVertexUniform(0, &vertexUniform, sizeof(vertexUniform));
        Engine::Get().GetRenderer().SetVertexUniform(1, &lightUniform, sizeof(lightUniform));
        Engine::Get().GetRenderer().SetVertexUniform(2, &materialUniform, sizeof(materialUniform));
        Engine::Get().GetRenderer().SetFragmentUniform(0, &tint, sizeof(tint));
        Engine::Get().GetRenderer().SetFragmentTexture(0, *texture);

        model->Draw(Engine::Get().GetRenderer());

        Engine::Get().GetRenderer().EndFrame();
    }

    // SHUTDOWN

    Engine::Get().GetAudio().Shutdown();

    Engine::Get().Shutdown();
    return 0;
}