#include"Components/Behaviour/Behaviour.h"
#include"ECS.h"

Entity Behaviour::CreateEntity()
{
	Entity eid = ecs.lock()->CreateEntity();
	AddComponent<EntityDescriptor>(eid);
	return eid;

}
