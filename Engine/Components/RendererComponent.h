#pragma once
#include "Framework/Component.h"

namespace nu {
	class RendererComponent : public Component {
	public:
		virtual void Draw(const class Renderer& renderer) = 0;
		
		virtual void Read(const json::value_t& value) override { Component::Read(value); }
	};
}