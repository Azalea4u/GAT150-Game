#pragma once
#include "RenderComponent.h"
#include "Renderer/Texture.h"
#include "FrameWork/Factory.h"

namespace kiko
{
	class SpriteRenderComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(SpriteRenderComponent)

		bool Initialize() override;
		void Update (float dt) override;
		void Draw(class Renderer& renderer) override;

		virtual float GetRadius() override { return m_texture->GetSize().Length() * 0.5f; }

	public:
		std::string textureName;
		res_t<Texture> m_texture;
	};

}