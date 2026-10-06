#pragma once
#include "Framework/Object.h"
#include "Serialization/Json.h"

namespace nu {

	class Actor;

	class Component : public Object {
	public:
		Component() = default;
		virtual ~Component() = default;

		CLASS_PROTOTYPE(Component)

		virtual void Start() {}
		//virtual void Destroyed() {}
		virtual void OnDestroy() {}

		virtual void Update(float dt) {}

		Actor* GetOwner() const { return m_owner; }
		void SetOwner(Actor* owner) { m_owner = owner; }

		virtual void Read(const json::value_t& value) override { Object::Read(value); }

	protected:
		Actor* m_owner = nullptr;
	};
}
