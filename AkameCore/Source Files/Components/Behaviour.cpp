#include"Components/Behaviour/Behaviour.h"
#include"ECS.h"

Entity Behaviour::CreateEntity()
{
	Entity eid = ecs.lock()->CreateEntity();
	AddComponent<EntityDescriptor>(eid);
	return eid;

}

//AKAME_API void Behaviour::_serialize_data(ReflectionMeta& r) {  }

//AKAME_API void Behaviour::_draw_data(ReflectionUIHandler& r) { }

//AKAME_API void Behaviour::OnStart() {}

//AKAME_API void Behaviour::Update(float deltaTime) {}
