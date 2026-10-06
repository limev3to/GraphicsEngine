#pragma once
#include "Renderer/Font.h"
#include "Math/Vector3.h"

struct SDL_Texture;

namespace nu {

	class Renderer;

	class Text {
	public:
		Text() = default;
		Text(res_t<Font> font) : m_font{ font } {}
		~Text();

		bool Create(Renderer& renderer, const std::string& text, const Color& color);
		void Draw(const Renderer& renderer, float x, float y);

	private:
		res_t<Font> m_font;
		SDL_Texture* m_texture{ nullptr };
	};
}
