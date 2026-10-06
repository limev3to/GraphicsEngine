#include "FlyingEnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Core/Factory.h"
#include "Damager.h"
#include "Engine.h"
#include "SpriteGame.h"

using namespace nu;

FACTORY_REGISTER(FlyingEnemyController)

void FlyingEnemyController::Start() {
	
	CharacterBase::Start();

	m_physicsComponent = GetComponent<PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void FlyingEnemyController::Update(float dt) {

	Vector2 velocity = m_physicsComponent->GetVelocity();

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		auto player = m_scene->GetActorByName<Actor>("PlayerPrototype");

		if (player) {
			Vector2 position = GetTransform().position;
			Vector2 playerPosition = player->GetTransform().position;
			Vector2 direction = playerPosition - position;

			m_rendererComponent->SetFlipH(direction.x < 0.0f);

			if (direction.Length() < 100.0f) {
				m_state = State::Attack;
				m_hasAttacked = false;
				m_rendererComponent->Play("attack");
			}

			m_physicsComponent->ApplyForce(direction.Normalized() * 800.0f);
		}
	}
		break;
	case CharacterBase::State::Attack:
		
		if (!m_hasAttacked && m_rendererComponent->GetFrame() == 3) {
			m_hasAttacked = true;
			auto damager = nu::Factory::Instance().Create<Damager>("DamagerPrototype");
			damager->SetDamage(1.0f);
			damager->SetPosition(GetTransform().position + nu::Vector2{ (m_rendererComponent->GetFlipH()) ? -20.0f : 20.f, 0.0f });
			damager->SetScale(3.0f);
			damager->SetTag("EnemyDamager");
			m_scene->AddActor(std::move(damager));
		}

		if (m_rendererComponent->IsAnimationDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		
		break;
	case CharacterBase::State::Hit:
		if (m_rendererComponent->IsAnimationDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
		break;
	case CharacterBase::State::Death:
		m_rendererComponent->Play("death");
		SetDestroyed();
		break;
	default:
		break;
	}


	CharacterBase::Update(dt);
}

void FlyingEnemyController::OnCollision(nu::Actor* other) {

	if (nu::EqualsIgnoreCase(other->GetTag(), "PlayerDamager")) {

		m_state = State::Hit;
		m_rendererComponent->Play("hit");
		Damager* damager = dynamic_cast<Damager*>(other);

		if (damager) {
			m_health -= damager->GetDamage();
			damager->SetDestroyed();
		}

		if (m_health <= 0) {
			SpriteGame* game = dynamic_cast<SpriteGame*>(m_scene->GetGame());
			game->AddPoints(200);
			m_state = State::Death;
		}

		// remove damager
		other->SetDestroyed();
	}
}

void FlyingEnemyController::Read(const nu::json::value_t& value) {
	CharacterBase::Read(value);
}