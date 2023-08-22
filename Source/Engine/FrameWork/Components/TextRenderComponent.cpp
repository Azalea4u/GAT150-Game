#include "TextRenderComponent.h"
#include "FrameWork/Resource/ResourceManager.h"
#include "Renderer/Text.h"
#include "FrameWork/Actor.h"

namespace kiko
{
	CLASS_DEFINITION(TextRenderComponent);

	TextRenderComponent::TextRenderComponent(const TextRenderComponent& other)
	{
		text = other.text;
		fontName = other.fontName;
		fontSize = other.fontSize;
		
		m_chnaged = true;
		m_text = std::make_unique<Text>(*other.m_text.get());
	}

	bool TextRenderComponent::Initialize()
	{
		if (!fontName.empty())
		{
			m_text = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, fontName, fontSize));
		}
		
		return true;
	}

	void TextRenderComponent::Update(float dt)
	{
	}

	void TextRenderComponent::Draw(Renderer& renderer)
	{
		// update text if changed
		if (m_chnaged)
		{
			m_chnaged = false;
			//create text using text string and color
			m_text->Create(renderer, text, Color(255, 255, 255, 255));
		}
		//draw text
		m_text->Draw(renderer, m_owner->transform);
	}

	void TextRenderComponent::SetText(const std::string& string)
	{
		// check if text has changed, if change set new string and update
		if (string != text)
		{
			text = string;
			m_chnaged = true;
		}
	}

	void TextRenderComponent::Read(const json_t& value)
	{
		READ_DATA(value, text);
		READ_DATA(value, fontName);
		READ_DATA(value, fontSize);
	}
}
