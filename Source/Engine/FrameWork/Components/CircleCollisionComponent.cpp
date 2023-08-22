#include "CircleCollisionComponent.h"
#include "FrameWork/Actor.h"

namespace kiko
{
	CLASS_DEFINITION(CircleCollisionComponent)

	//CircleCollisionComponent::Register register_class;

	void CircleCollisionComponent::Update(float dt)
	{
	}

	bool CircleCollisionComponent::CheckCollision(CollisionComponent* collision)
	{
		float distance = m_owner->transform.position.Distance(collision->m_owner->transform.position);
		float radius = m_radius + collision->m_radius;

		return (distance <= radius);
	}

	void CircleCollisionComponent::Read(const json_t& value)
	{
		//
	}
}