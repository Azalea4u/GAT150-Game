#include "SpriteRenderComponent.h"
#include "Renderer/Renderer.h"
#include "FrameWork/Actor.h"
#include <FrameWork/Resource/ResourceManager.h>

namespace kiko
{
	CLASS_DEFINITION(SpriteRenderComponent);

	bool SpriteRenderComponent::Initialize()
	{
		if (!textureName.empty()) m_texture = GET_RESOURCE(Texture, textureName, g_renderer);

		return true;
	}

	void SpriteRenderComponent::Update(float dt)
	{

	}

	void SpriteRenderComponent::Draw(Renderer& renderer)
	{
		renderer.DrawTexture(m_texture.get(), m_owner->transform);
	}

	void SpriteRenderComponent::Read(const json_t& value)
	{
		READ_DATA(value, textureName);
	}
}