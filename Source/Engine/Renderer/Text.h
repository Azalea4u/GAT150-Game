#pragma once
#include "Font.h"
#include "Core/Core.h"
#include <string>
#include <memory>

struct SDL_Texture;

namespace kiko
{
	class Renderer;

	class Text
	{
	public:
		Text() = default;
		Text(std::shared_ptr<Font> font) : m_font{ font } {}
		~Text();

		void Create(Renderer& renderer, const std::string& text, const Color& color);
			void Draw(Renderer& renderer, int x, int y);
			void Draw(Renderer& renderer, const Transform& transform);

	private:
		std::shared_ptr<Font> m_font;
		struct SDL_Texture* m_texture = nullptr;
	};
}