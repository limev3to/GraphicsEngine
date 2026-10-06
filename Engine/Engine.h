#pragma once

#include "Core/Random.h"
#include "Math/Vector2.h"
#include "Math/Transform.h"
#include "Math/MathUtils.h"
#include "Core/File.h"
#include "Serialization/Json.h"
#include "Core/StringUtils.h"


#include "Renderer/Text.h"
#include "Renderer/Texture.h"

#include "Core/Factory.h"

// Systems
#include "Renderer/Renderer.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "Core/GameTime.h"
#include "Renderer/ParticleSystem.h"
#include "Resources/ResourceManager.h"
#include "Physics/Physics.h"

// framework
#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Framework/Game.h"

namespace nu {
	class Engine {

	public:
		static Engine& Get() { static Engine engine; return engine; }

		bool Initialize();
		void Shutdown();

		void Update();

		Input& GetInput() { return m_input; }
		Renderer& GetRenderer() { return m_renderer; }
		Time& GetTime() { return m_time; }
		ParticleSystem& GetPS() { return m_particleSystem; }
		Audio& GetAudio() { return m_audio; }


		Physics& GetPhysics() { return m_physics; }

	private:
		Engine() = default;

	private:
		Input m_input;
		Renderer m_renderer;
		Audio m_audio;
		ParticleSystem m_particleSystem;

		Time m_time;

		Physics m_physics;

	};

}