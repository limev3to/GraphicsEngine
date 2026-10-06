#include "Player.h"
#include "SpaceGame.h"
#include "Engine.h"
#include "Bullet.h"
#include "Assets.h"
#include "Components/PhysicsComponent.h"

#include "Renderer/Renderer.h"
#include <memory>
#include <iostream>

FACTORY_REGISTER(Player)

void Player::Update(float dt) {
    // movement
    float thrust = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_W)) thrust = m_speed;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_S)) thrust = -m_speed;

    float rotate = 0.0f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) rotate = -.75f;
    if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) rotate = +.75f;


    auto physicsComponent = GetComponent<nu::PhysicsComponent>();
    if (physicsComponent)
    {
        nu::Vector2 forward{ 1, 0 };
        nu::Vector2 force = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;
        physicsComponent->ApplyForce(force);
        physicsComponent->ApplyTorque(rotate);

        nu::Vector2 position = physicsComponent->GetPosition();
        nu::Engine::Get().GetRenderer().SetCamera(position);

        //physicsComponent->SetPosition({ nu::Wrap(0.0f, 1280.0f, physicsComponent->GetPosition().x), nu::Wrap(0.0f, 1024.0f, physicsComponent->GetPosition().y) });
    }


    SetRotation(m_transform.rotation + rotate * dt);

    nu::Vector2 forward{ 1, 0 };
    nu::Vector2 velocity = forward.Rotate(m_transform.rotation * nu::DegToRad) * thrust;
    //AddVelocity(velocity * dt);

    // particle system
    if (thrust && nu::RandomInt(10) == 0)
    {
        nu::Particle particle;
        particle.position = m_transform.position;
        particle.sprite = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
        particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
        particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-200.0f, 200.0f) };

        nu::Engine::Get().GetPS().AddParticle(particle);
    }

    isBuffed -= dt;

    // fire
    if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE)) {

        nu::Transform b_transform = m_transform;
        b_transform.scale = 1.5f;

        auto bullet = nu::Factory::Instance().Create<Bullet>("BulletPrototype");

        bullet->SetPosition(b_transform.position);
        bullet->SetRotation(b_transform.rotation);

        if (isBuffed > 0) {

            bullet->SetLifespan(3.5f);
            bullet->SetSpeed(3000);
        }

        nu::Engine::Get().GetAudio().PlaySound("laser");

        m_scene->AddActor(std::move(bullet));
    }


    Actor::Update(dt);
}

void Player::OnCollision(Actor* other) {
    if (other->GetTag() == "Enemy") {
        SetDestroyed();
        nu::Engine::Get().GetAudio().PlaySound("explosion");
        ((SpaceGame*)m_scene->GetGame())->OnPlayerDead();
    }

    if (other->GetTag() == "Item") {
        other->SetDestroyed();
        SetBuffed(5.0f);
        nu::Engine::Get().GetAudio().PlaySound("itemPickup");
        ((SpaceGame*)m_scene->GetGame())->AddPoints(150);
    }
}

void Player::Read(const nu::json::value_t& value) {
    Actor::Read(value);

    JSON_READ_NAME(value, "speed", m_speed);
}

//BulletDesc desc;
//desc.name = "Bullet";
//desc.tag = "PlayerBullet";
//desc.transform = b_transform;
//desc.texture = nu::Resources().Get<nu::Texture>("textures/bullet.png", nu::Engine::Get().GetRenderer());
//desc.speed = 1000.0f;
//desc.lifespan = 2.0f;
//Bullet* bullet = new Bullet{ desc };
//m_scene->AddActor(std::move(std::make_unique<Bullet>(desc)));

//BulletDesc desc;
//desc.name = "Bullet";
//desc.tag = "PlayerBullet";
//desc.transform = b_transform;
//desc.texture = nu::Resources().Get<nu::Texture>("textures/bullet.png", nu::Engine::Get().GetRenderer());
//desc.speed = 3000.0f;
//desc.lifespan = 3.5f;
//Bullet* bullet = new Bullet{ desc };
//m_scene->AddActor(std::move(std::make_unique<Bullet>(desc)));