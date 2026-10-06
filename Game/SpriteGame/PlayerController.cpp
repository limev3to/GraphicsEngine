#include "PlayerController.h"
#include "Components/PhysicsComponent.h"
#include "Components/SpriteAnimatorRendererComponent.h"
#include "Core/Factory.h"
#include "Framework/Scene.h"
#include "Damager.h"
#include "Engine.h"
#include "SpriteGame.h"

using namespace nu;

FACTORY_REGISTER(PlayerController)

void PlayerController::Start() {

	CharacterBase::Start();
	
	m_physicsComponent = GetComponent<PhysicsComponent>();
	assert(m_physicsComponent);
	m_rendererComponent = GetComponent<SpriteAnimatorRendererComponent>();
	assert(m_rendererComponent);
}

void PlayerController::Update(float dt) {

	
	Vector2 velocity = m_physicsComponent->GetVelocity();

	switch (m_state)
	{
	case CharacterBase::State::Move:
	{
		float dir = 0.0f;
		if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_A)) dir = -1.0f;
		if (nu::Engine::Get().GetInput().GetKeyDown(SDL_SCANCODE_D)) dir = +1.0f;
		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_SPACE))
		{
			m_rendererComponent->Play("jump");
			velocity.y = -800.0f;
		}
		if (dir != 0.0f) {
			velocity.x = dir * 175.0f;

			if (m_rendererComponent->IsAnimationDone()) m_rendererComponent->Play("run");
			m_rendererComponent->SetFlipH(dir < 0.0f);
		}
		else {
			if (m_rendererComponent->IsAnimationDone()) m_rendererComponent->Play("idle");
		}

		if (nu::Engine::Get().GetInput().GetKeyPressed(SDL_SCANCODE_RSHIFT)) {
			m_state = State::Attack;
			m_rendererComponent->Play("attack");

			auto damager = nu::Factory::Instance().Create<nu::Actor>("DamagerPrototype");


			if (m_rendererComponent->GetFlipH() == true) {
				Vector2 pos = { -100.0f, 0.0f };
				damager->SetPosition(GetTransform().position + pos);
			}
			else {
				Vector2 pos = { 100.0f, 0.0f };
				damager->SetPosition(GetTransform().position + pos);
			}

			damager->SetTag("PlayerDamager");
			damager->SetScale(2.0f);
			m_scene->AddActor(std::move(damager));

		}
	}
		break;
	case CharacterBase::State::Attack:
	{
		if (m_rendererComponent->IsAnimationDone()) {
			m_state = State::Move;
			m_rendererComponent->Play("idle");
		}
	}
		break;
	case CharacterBase::State::Death:
		((SpriteGame*)m_scene->GetGame())->OnPlayerDead();
		break;
	default:
		break;
	}

	m_physicsComponent->SetVelocity(velocity);
	nu::Engine::Get().GetRenderer().SetCamera(m_physicsComponent->GetPosition());

	CharacterBase::Update(dt);
}

void PlayerController::OnCollision(nu::Actor* other) {
	
	if (nu::EqualsIgnoreCase(other->GetTag(), "EnemyDamager")) {

		m_rendererComponent->Play("hit");
		Damager* damager = dynamic_cast<Damager*>(other);

		if (damager) {
			m_health -= damager->GetDamage();
			damager->SetDestroyed();
		}

		if (m_health <= 0) {
			m_rendererComponent->Play("death");
			m_state = State::Death;
		}
		// remove damager
		other->SetDestroyed();
	}
}

void PlayerController::Read(const nu::json::value_t& value) {
	CharacterBase::Read(value);
}
