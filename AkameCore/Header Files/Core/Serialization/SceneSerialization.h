#pragma once
#include"Core/scene.h"
#include"Core/Serialization/ModelExporter.h"
class SceneSerializer
{
	Scene& m_scene;
	ModelExporter m_exporter;
public:
	AKAME_API SceneSerializer(Scene& scene); 
	AKAME_API void SaveTo(std::string path_str);
	AKAME_API void Save();
};

class SceneDeserializer
{
	Scene& m_scene;
	std::map<Entity, Entity> m_old_new_entity_map;
	AKAME_API Entity& GetNewEntity(Entity& old_eid);
	friend class ReflectionMeta;
	void DeserealizeTransforms(nlohmann::json& ser_data,Entity& eid);
	bool m_is_edf;
	std::string m_fileName;
	std::string m_folderPath; 
	std::string m_rel_path;
	template<typename _compType>
	AKAME_API void AddComponent(nlohmann::json& ser_data, Entity& eid);
public:
	AKAME_API SceneDeserializer(Scene& scene);
	AKAME_API void LoadFrom(std::string path_str);
	AKAME_API void Load(bool resetScene);
};