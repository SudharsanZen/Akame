#pragma once
#include"Core/Scene.h"
#include"json/single_include/nlohmann/json.hpp"
#include"Core/Reflection/ReflectionMeta.h"
#include"Core/AkameCore.h"
class ECS;
class ModelExporter
{
	
	int m_mesh_count;
	
	std::string m_folderPath;
	std::string m_fileName;

	Scene& m_scene;
	std::shared_ptr<ECS> m_ecs;
	
	nlohmann::json m_entity_arr;
	
	bool m_export_binary;
	bool m_serializer_mode;
	AKAME_API void SetExportBinary(bool exp_stat);
	AKAME_API void makeFolder(std::string path);
	
	AKAME_API nlohmann::json EntitySerializer(Entity eid);
	AKAME_API nlohmann::json Serialize(Transform &data);
	AKAME_API nlohmann::json Serialize(BehaviourComponent &data);
	AKAME_API nlohmann::json Serialize(SkeletalMesh &data);
	AKAME_API nlohmann::json Serialize(Mesh &data);
	AKAME_API nlohmann::json Serialize(Material &data);
	AKAME_API nlohmann::json Serialize(Lights &data);
	AKAME_API nlohmann::json Serialize(AnimationController &data);
	AKAME_API nlohmann::json Serialize(EntityDescriptor &data);
	AKAME_API nlohmann::json Serialize(physics::RigidBody3D &data);
	
	template <typename _compType>
	AKAME_API void ComponentSerializerHelper(Entity eid, nlohmann::json& components);
	AKAME_API void ExportEntity(Entity eid);
	AKAME_API bool EntityHasChild(Entity eid);
	AKAME_API void SaveToFilePath(std::string path,std::string filname,std::string ext);
	friend class SceneSerializer;

public:
	AKAME_API ModelExporter(Scene& scene);

	AKAME_API void ExportEntity(Entity eid,std::string path,std::string fileName="entity");
	AKAME_API void ExportAnimationClips(Model& model);

};

