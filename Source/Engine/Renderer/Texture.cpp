#include "Texture.h"
#include "Renderer.h"
#include "Core/Logger.h"
#include "SDL2-2.28.1/include/SDL_image.h"

namespace kiko
{
	Texture::~Texture()
	{
		if (m_texture) SDL_DestroyTexture(m_texture);
	}

	bool Texture::Create(std::string filename, ...)
	{
		// va_list - type to hold information about variable arguments
		va_list args;

		// va_start - initialize va_list
		va_start(args, filename);
		
		// va_arg - retrieve next argument
		Renderer& renderer = va_arg(args, Renderer);

		// va_end - clean up the list
		va_end(args);

		return Load(filename, renderer);
	}

	bool Texture::Load(const std::string filename, class Renderer& renderer)
	{
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			WARNING_LOG("Failed to load texture: %s" << filename.c_str());
			return false;
		}
		
		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);
		SDL_FreeSurface(surface);
		if (!m_texture)
		{
			WARNING_LOG("Failed to create texture: %s" << filename.c_str());
			return false;
		}

		return true;
	}

	vec2 Texture::GetSize()
	{
		ASSERT_LOG(m_texture != NULL, "Texture is null");

		// SDL_Point - structure that defines a two dimensional point
		SDL_Point point;
		SDL_QueryTexture(m_texture, nullptr , nullptr, &point.x, &point.y);

		return vec2(point.x, point.y);
	}

}