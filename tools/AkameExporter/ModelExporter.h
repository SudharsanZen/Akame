#pragma once
#include"Core/Scene.h"
#include"json/single_include/nlohmann/json.hpp"
#include"Core/Reflection/ReflectionMeta.h"
class ECS;
class ModelExporter
{
	void makeFolder(std::string path);
	int m_mesh_count;
	std::string m_folderPath;
	std::string m_fileName;
	Scene& m_scene;
	std::shared_ptr<ECS> m_ecs;
	nlohmann::json m_entity_arr;
	nlohmann::json EntitySerializer(Entity eid);
	nlohmann::json Serialize(Transform &data);
	nlohmann::json Serialize(BehaviourComponent &data);
	nlohmann::json Serialize(SkeletalMesh &data);
	nlohmann::json Serialize(Mesh &data);
	nlohmann::json Serialize(Material &data);
	nlohmann::json Serialize(Lights &data);
	nlohmann::json Serialize(AnimationController &data);
	nlohmann::json Serialize(EntityDescriptor &data);
	nlohmann::json Serialize(physics::RigidBody3D &data);
	template <typename _compType>
	void ComponentSerializerHelper(Entity eid, nlohmann::json& components);
	void ExportEntity(Entity eid);
	bool EntityHasChild(Entity eid);

public:
	ModelExporter(Scene& scene);

	void ExportEntity(Entity eid,std::string path,std::string fileName="entity");
	void ExportAnimationClips(Model& model);

};

