#include "Item.h"
#include "Player.h"
#include "Renderer/Renderer.h"
#include "Engine.h"
#include "SpaceGame.h"

#include <iostream>

FACTORY_REGISTER(Item)

void Item::Update(float dt) {

    //// particle system
    //nu::Particle particle;
    //particle.position = m_transform.position;
    //particle.color = { 0.0f, 1.0f, 0.0f };
    //particle.lifespan = nu::RandomFloat(0.5f, 1.5f);
    //particle.velocity = { nu::RandomFloat(-200.0f, 200.0f), nu::RandomFloat(-200.0f, 200.0f) };
    //nu::Engine::Get().GetPS().AddParticle(particle);

    Actor::Update(dt);
}

void Item::OnCollision(Actor* other) {
    if (other->GetName() == "Player") {
        SetDestroyed();
    }
}
