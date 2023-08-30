#include "Font.h"
#include "SDL2-2.28.1/include/SDL_ttf.h"
#include <Core/Logger.h>

namespace kiko
{
	Font::Font(const std::string& filename, int fontSize)
	{
		Load(filename, fontSize);
	}

	Font::~Font()
	{
		if (m_ttfFont != nullptr)
		{
			TTF_CloseFont(m_ttfFont);
		}
	}

	bool Font::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);
		int fontSize = va_arg(args, int);
		va_end(args);

		return Load(filename, fontSize);
	}

	bool Font::Load(const std::string& filename, int fontSize)
	{
		// Use filename.c_str() to get the C-style string.
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);
		if (!m_ttfFont)
		{
			WARNING_LOG("Font failed to load: " << filename);
			return false;
		}

		return (m_ttfFont);
	}
}