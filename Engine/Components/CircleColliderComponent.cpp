#include "pch.h"
#include "Components/CircleColliderComponent.h"
#include "Framework/Actor.h"
#include "Core/Factory.h"

namespace nu {

	FACTORY_REGISTER(CircleColliderComponent)

	bool CircleColliderComponent::CheckCollision(const ColliderComponent& other) {

		const CircleColliderComponent* circleCollider = dynamic_cast<const CircleColliderComponent*>(&other);
		if (!circleCollider) return false;

		float distance = GetOwner()->GetTransform().position.Distance(circleCollider->GetOwner()->GetTransform().position);

		return distance <= m_radius + circleCollider->GetRadius();
	}

	void CircleColliderComponent::Read(const json::value_t& value)
	{
		ColliderComponent::Read(value);

		JSON_READ_NAME(value, "radius", m_radius);
	}
}