#pragma once
#include "Framework/Actor.h"

struct EnemyDesc : public nu::ActorDesc {
    float speed;
};

class Enemy : public nu::Actor {
public:
    Enemy() = default;
    Enemy(const EnemyDesc& enemyDesc) : 
        Actor(enemyDesc),
        m_speed { enemyDesc.speed }
    { }

    CLASS_PROTOTYPE(Enemy)

    void Update(float dt) override;
    void OnCollision(Actor* other) override;

    void Read(const nu::json::value_t& value) override;

private:
    int m_ammo = 0;
    float m_speed = 800.0f;

};