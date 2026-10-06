#include "Enemy.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"

#include <iostream>

FACTORY_REGISTER(Enemy)

void Enemy::Update(float dt) {

    Player* player = m_scene->GetActorByName<Player>("PlayerPrototype");
    if (player) {
        nu::Vector2 direction = player->GetTransform().position - m_transform.position;
        float rotation = direction.Angle();
        SetRotation(rotation * nu::RadToDeg);

        nu::Vector2 forward{ 1, 0 };
        forward = forward.Rotate(m_transform.rotation * nu::DegToRad);
        //AddVelocity(forward * m_speed * dt);

    }

    // particle system
    if (nu::RandomInt(30) == 0)
    {
        nu::Particle particle;
        particle.position = m_transform.position;
        particle.sprite = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
        particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
        particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-200.0f, 200.0f) };

        nu::Engine::Get().GetPS().AddParticle(particle);
    }

    Actor::Update(dt);
}

void Enemy::OnCollision(Actor* other) {
    if (other->GetTag() == "PlayerBullet") {
        SetDestroyed();
        other->SetDestroyed();

        ((SpaceGame*)m_scene->GetGame())->AddPoints(100);

        nu::Engine::Get().GetAudio().PlaySound("enemyExplosion");

        // create particle explosion
        for (int i = 0; i < 100; i++) {
            nu::Particle particle;
            particle.position = m_transform.position;
            particle.sprite = nu::Resources().Get<nu::Texture>("textures/particle.png", nu::Engine::Get().GetRenderer());
            particle.lifespan = nu::RandomFloat(0.5f, 2.0f);
            particle.velocity = { nu::RandomFloat(-600.0f, 600.0f), nu::RandomFloat(-600.0f, 600.0f) };

            nu::Engine::Get().GetPS().AddParticle(particle);
        }
    }
}

void Enemy::Read(const nu::json::value_t& value) {
    Actor::Read(value);

    JSON_READ_NAME(value, "speed", m_speed);
}
