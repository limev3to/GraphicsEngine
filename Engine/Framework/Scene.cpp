#include "pch.h"
#include "Framework/Scene.h"
#include "Framework/Actor.h"
#include "Core/Factory.h"
#include "Components/ColliderComponent.h"

namespace nu {

	void Scene::AddActor(std::unique_ptr<Actor> actor) { 
		actor->m_scene = this;
		m_pendingActors.push_back(std::move(actor));
	}

	void Scene::RemoveAllActors(bool force) {
		//m_actors.clear();
		std::erase_if(m_actors, [force](auto& actor) { return !actor->GetPersistent() || force; });
	}

	void Scene::Update(float dt) {
		// update actors
		for (auto& actor : m_actors) {
			actor->Update(dt);
		}

		UpdateCollisions();

		for (auto& actor : m_actors) {
			if (actor->GetDestroyed())
			{
				actor->OnDestroy();
			}
		}

		// remove destroyed actors
		std::erase_if(m_actors, [](auto& actor) { return actor->m_destroyed; });

		// add pending actors
		for (auto& actor : m_pendingActors) {
			actor->Start();
			m_actors.push_back(std::move(actor));
		}
		m_pendingActors.clear();
	}


	bool Scene::Load(const std::string& sceneName)
	{
		json::document_t document;
		if (json::Load(sceneName, document)) {

			if (JSON_HAS_NAME(document, "actors")) {

				for (auto& actorValue : JSON_GET_NAME(document, "actors").GetArray()) {

					// get actor type
					std::string typeName;
					JSON_READ_NAME(actorValue, "type", typeName);

					std::cout << "Loading actor type: " << typeName << std::endl;

					// create actor of type
					auto actor = Factory::Instance().Create<Actor>(typeName);
					
					// could not create actor (actor is null)
					if (!actor) {
						std::cout << "Could not create actor type: " << typeName << std::endl;
						continue;
					}

					// read actor json
					actor->Read(actorValue);

					bool prototype = false;
					JSON_READ(actorValue, prototype);

					if (prototype)
					{
						std::string name;
						JSON_READ(actorValue, name);
						Factory::Instance().RegisterPrototype<Actor>(name, std::move(actor));
					}
					else
					{
						AddActor(std::move(actor));
					}
				}
			}
		}

		return true;
	}


	void Scene::Draw(const class Renderer& renderer) {
		for (auto& actor : m_actors) {
			actor->Draw(renderer);
		}
	}


	void Scene::UpdateCollisions() {
		
		for (auto& actorA : m_actors) {

			for (auto& actorB : m_actors) {
				
				if (actorA == actorB || actorA->m_destroyed || actorB->m_destroyed) continue;

				auto colliderA = actorA->GetComponent<ColliderComponent>();
				auto colliderB = actorB->GetComponent<ColliderComponent>();

				if (!colliderA || !colliderB) continue;

				if (colliderA->CheckCollision(*colliderB))
				{
					actorA->OnCollision(actorB.get());
					actorB->OnCollision(actorA.get());
				}

				//float distance = (actorA->m_transform.position - actorB->m_transform.position).Length();
				//if (distance <= actorA->GetRadius() + actorB->GetRadius()) {
				//	actorA->OnCollision(actorB.get());
				//	actorB->OnCollision(actorA.get());
				//}
			}
		}
	}

}