#pragma once

#include "Core/Random.h"
#include "Math/Vector2.h"
#include "Math/Transform.h"
#include "Math/MathUtils.h"
#include "Core/File.h"
#include "Serialization/Json.h"
#include "Core/StringUtils.h"
#include "Editor/Editor.h"


#include "Renderer/Text.h"
#include "Renderer/Texture.h"

#include "Core/Factory.h"

// Systems
#include "Renderer/Renderer.h"
#include "Input/Input.h"
#include "Core/GameTime.h"
#include "Resources/ResourceManager.h"

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
		Editor& GetEditor() { return *m_editor; }

	private:
		Engine() = default;

	private:
		Input m_input;
		Renderer m_renderer;
		Time m_time;
		std::unique_ptr<Editor> m_editor;

	};

}