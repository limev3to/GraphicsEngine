#pragma once
#include "Framework/Component.h"
#include "Serialization/Json.h"

namespace nu {

	class ColliderComponent : public Component {

	public:
		virtual bool CheckCollision(const ColliderComponent& other) = 0;

		virtual void Read(const json::value_t& value) override { Component::Read(value); }
	};

}