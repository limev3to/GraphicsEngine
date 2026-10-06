#pragma once
#include "Framework/Actor.h"
#include <vector>
#include <string>
#include <memory>

namespace nu {

	class Scene {
	public: 
		void AddActor(std::unique_ptr<Actor> actor);
		void RemoveAllActors(bool force = false);

		bool Load(const std::string& sceneName);

		void Update(float dt);
		void Draw(const class Renderer& renderer);

		template<typename T = Actor> 
		T* GetActorByName(const std::string& name);
		
		void SetGame(class Game* game) { m_game = game; }
		class Game* GetGame() { return m_game; }

	private:
		void UpdateCollisions();

	private:
		std::vector<std::unique_ptr<Actor>> m_actors;
		std::vector<std::unique_ptr<Actor>> m_pendingActors;

		class Game* m_game{ nullptr };
	};

	template<typename T>
	inline T* Scene::GetActorByName(const std::string& name)
	{
		for (auto& actor : m_actors) {

			T* actorT = dynamic_cast<T*>(actor.get());
			
			if (actorT && actorT->m_name == name) {
				return actorT;
			}

		}

		return nullptr;
	}

}