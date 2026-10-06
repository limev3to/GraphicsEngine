#pragma once
#include "Framework/Actor.h"

struct ItemDesc : public nu::ActorDesc {
    float speed;
};

class Item : public nu::Actor {
public:
    Item() = default;

    CLASS_PROTOTYPE(Item)

    void Update(float dt) override;
    void OnCollision(Actor* other) override;

};