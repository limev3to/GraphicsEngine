#include "EnemyController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Core/Factory.h"
#include "Damager.h"
#include "Engine.h"
#include "SpriteGame.h"

using namespace nu;

FACTORY_REGISTER(EnemyController)

void EnemyController::Start() {

	CharacterBase::Start();

	m_physicsComponent = GetComponent<PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void EnemyController::Update(float dt) {
	
	Vector2 velocity = m_physicsComponent->GetVelocity();

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		float dir = 0.0f;
		auto player = m_scene->GetActorByName("PlayerPrototype");

		if (player) {

			Vector2 position = GetTransform().position;
			Vector2 playerPosition = player->GetTransform().position;
			Vector2 direction = playerPosition - position;

			if (direction.Length() < 100.0f) {
				m_state = State::Attack;
				m_hasAttacked = false;
				m_rendererComponent->Play("attack");
			}

			if (playerPosition.x < position.x) dir = -1.0f;
			else dir = 1.0f;
		}

		if (dir != 0.0f) {
			velocity.x = dir * 50.0f;
			m_rendererComponent->Play("run");
			m_rendererComponent->SetFlipH(dir < 0.0f);
		}
		else {
			m_rendererComponent->Play("idle");
		}
	}
		break;
	case CharacterBase::State::Attack:
		if (!m_hasAttacked && m_rendererComponent->GetFrame() == 3) {
			m_hasAttacked = true;
			auto damager = nu::Factory::Instance().Create<Damager>("DamagerPrototype");
			damager->SetDamage(2.0f);
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

	m_physicsComponent->SetVelocity(velocity);

	CharacterBase::Update(dt);
}

void EnemyController::OnCollision(nu::Actor* other) {
	
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

void EnemyController::Read(const nu::json::value_t& value) {
	CharacterBase::Read(value);
}
