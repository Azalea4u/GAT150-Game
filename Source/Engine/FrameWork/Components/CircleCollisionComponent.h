#pragma once
#include "FrameWork/Components/CollisionComponent.h"

namespace kiko
{
	class CircleCollisionComponent : public CollisionComponent
	{
	public:
		CLASS_DECLARATION(CircleCollisionComponent)

		// Inherited via CollisionComponent
		virtual void Update(float dt) override;
		virtual bool CheckCollision(CollisionComponent* collision) override;

	};
}