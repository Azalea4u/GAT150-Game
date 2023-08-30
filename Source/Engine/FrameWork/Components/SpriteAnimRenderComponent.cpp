#include "SpriteAnimRenderComponent.h"
#include "Renderer/Renderer.h"
#include "FrameWork/Actor.h"
#include <FrameWork/Resource/ResourceManager.h>

namespace kiko
{
	CLASS_DEFINITION(SpriteAnimRenderComponent);

	bool SpriteAnimRenderComponent::Initialize()
	{
		SpriteRenderComponent::Initialize();

		SetSequence(defaultSequenceName);
		if (source.w == 0 && source.h == 0)
		{
			UpdateSource();
		}

		return true;
	}

	void SpriteAnimRenderComponent::Update(float dt)
	{
		frameTimer -= dt;
		if (frameTimer <= 0)
		{
			frameTimer = 1.0f / m_currentSequence->fps;
			frame++;
			if (frame > m_currentSequence->endFrame)
			{
				frame = (m_currentSequence->loop) ? m_currentSequence->startFrame : m_currentSequence->endFrame;
			}
		}

		UpdateSource();
	}

	void SpriteAnimRenderComponent::SetSequence(const std::string& name, bool update)
	{
		// prevent set sequence again
		if (m_currentSequence && m_currentSequence->name == name) return;

		if (m_sequences.find(name) != m_sequences.end())
		{
			// set new sequence
			m_currentSequence= &m_sequences[name];
			
			// set texture
			if (m_currentSequence->texture) m_texture = m_currentSequence->texture;
			// reset frame information
			frame = m_currentSequence->startFrame;
			frameTimer = 1.0f / m_currentSequence->fps;

			if (update) UpdateSource();
		}
	}

	void SpriteAnimRenderComponent::UpdateSource()
	{
		vec2 cellsize = m_texture->GetSize() / vec2(m_currentSequence->numColumns, m_currentSequence->numRows);
		int column = (frame - 1) % m_currentSequence->numColumns;
		int row = (frame - 1) / m_currentSequence->numColumns;

		// set source rect from current frame
		source.x = (int)(column * cellsize.x);
		source.y = (int)(row * cellsize.y);
		source.w = (int)cellsize.x;
		source.h = (int)cellsize.y;
	}

	void SpriteAnimRenderComponent::Read(const json_t& value)
	{
		SpriteRenderComponent::Read(value);

		// read in animation sequences
		if (HAS_DATA(value, sequences) && GET_DATA(value, sequences).IsArray())
		{
			for (auto& sequenceValue : GET_DATA(value, sequences).GetArray())
			{
				AnimSequence sequence;

				READ_NAME_DATA(sequenceValue, "name", sequence.name);
				READ_NAME_DATA(sequenceValue, "fps", sequence.fps);
				READ_NAME_DATA(sequenceValue, "numColumns", sequence.numColumns);
				READ_NAME_DATA(sequenceValue, "numRows", sequence.numRows);
				READ_NAME_DATA(sequenceValue, "startFrame", sequence.startFrame);
				READ_NAME_DATA(sequenceValue, "endFrame", sequence.endFrame);

				// read texture
				std::string textureName;
				READ_DATA(sequenceValue, textureName);
				sequence.texture = GET_RESOURCE(Texture, textureName, g_renderer);

				m_sequences[sequence.name] = sequence;
			}
		}

		if (!READ_DATA(value, defaultSequenceName))
		{
			// if default sequence not specified, use the first sequence in the sequences map
			defaultSequenceName = m_sequences.begin()->first;
		}
	}
}