#define AK_PRIVATE_GETTER_SETTER
#include"ECS.h"
#include<string>
#include "ModelExporter.h"
#include"Components/EntityDescriptor.h"
#include"Core/Scene.h"
#include"Core/SceneEntityListSystem.h"
#include<fstream>
#include<direct.h>

template <typename _compType>
void ModelExporter::ComponentSerializerHelper(Entity eid,nlohmann::json &components)
{
	std::string typeName = typeid(_compType).name();
	if (m_scene.IsComponentAttached<_compType>(eid))
		components[typeName] = Serialize(m_scene.GetComponent<_compType>(eid));
}

void ModelExporter::makeFolder(std::string path)
{
	
	std::filesystem::path folder(path);

	if (!std::filesystem::is_directory(folder))
		if (mkdir(path.c_str()) == -1)
		{
			std::cerr << "can't create folder!";
			throw("can't create folder!");
		}
}

nlohmann::json ModelExporter::EntitySerializer(Entity eid)
{
	nlohmann::json eidSerial;
	eidSerial["index"] = eid.GetIndex();
	eidSerial["version"] = eid.GetVersion();
	nlohmann::json components;
	ComponentSerializerHelper<Transform>(eid, components);
	ComponentSerializerHelper<BehaviourComponent>(eid, components);
	ComponentSerializerHelper<EntityDescriptor>(eid, components);
	ComponentSerializerHelper<SkeletalMesh>(eid, components);
	ComponentSerializerHelper<Mesh>(eid, components);
	ComponentSerializerHelper<AnimationController>(eid, components);
	ComponentSerializerHelper<Lights>(eid, components);
	ComponentSerializerHelper<physics::RigidBody3D>(eid, components);
	ComponentSerializerHelper<Material>(eid, components);
	eidSerial["Components"] = components;
	return eidSerial;
}
nlohmann::json ModelExporter::Serialize(Transform& data)
{
	ReflectionMeta ref;
	ref.read(data);
	return ref.m_json_object;
}
nlohmann::json ModelExporter::Serialize(BehaviourComponent& data)
{
	return nlohmann::json();
}
nlohmann::json ModelExporter::Serialize(SkeletalMesh& data)
{
	nlohmann::json j_mesh;
	j_mesh["fileLocation:"] = m_fileName + "/SkMeshData/" + m_fileName + ".skmesh";

	j_mesh["maxVert:"] = data.maxInd - data.minInd + 1;
	j_mesh["maxIndex:"] = data.end_i - data.start_i + 1;

	m_mesh_count++;
	std::stringstream ss;

	
	makeFolder(m_folderPath + m_fileName);
	ss << m_folderPath << m_fileName << "/SkMeshData";
	makeFolder(ss.str());
	ss << "/" << m_mesh_count << ".skmesh";

	std::ofstream file(ss.str(), std::ios::out | std::ios::binary);
	auto& vertexData = data.getVertData();
	auto& indexData = data.getIndexData();
	for (size_t i_vert = data.minInd; i_vert <= data.maxInd; i_vert++)
	{
		file.write((char*)(&vertexData[i_vert]), sizeof(sk_vert));
	}
	for (size_t i_ind = data.start_i; i_ind <= data.end_i; i_ind++)
	{
		file.write((char*)(&indexData[i_ind]), sizeof(unsigned int));
	}
	file.close();
	AK_ASSERT(file.good() && "error while writing to file!");

	return j_mesh;
}
nlohmann::json ModelExporter::Serialize(Mesh& data)
{
	nlohmann::json j_mesh;
	j_mesh["fileLocation:"] = m_fileName+"/MeshData/"+m_fileName + ".mesh";

	j_mesh["maxVert:"] =data.maxInd-data.minInd+1;
	j_mesh["maxIndex:"] = data.end_i - data.start_i + 1;

	m_mesh_count++;
	std::stringstream ss;

	ss << m_folderPath << m_fileName<<"/MeshData";
	makeFolder(m_folderPath+m_fileName);
	makeFolder(ss.str());
	ss<<"/"<< m_mesh_count << ".mesh";
	
	std::ofstream file(ss.str(),std::ios::out|std::ios::binary);
	auto &vertexData =data.getVertData();
	auto& indexData = data.getIndexData();
	for (size_t i_vert = data.minInd; i_vert <= data.maxInd; i_vert++)
	{
		file.write((char*)(&vertexData[i_vert]),sizeof(vert));
	}
	for (size_t i_ind = data.start_i; i_ind <= data.end_i; i_ind++)
	{
		file.write((char*)(&indexData[i_ind]),sizeof(unsigned int));
	}
	file.close();
	AK_ASSERT(file.good() && "error while writing to file!");

	return j_mesh;
}
nlohmann::json ModelExporter::Serialize(Material& data)
{
	nlohmann::json j_material;
	j_material["SHADER_NAME"] = data.SHADER_NAME;
	nlohmann::json j_uniform_texture_map;
	nlohmann::json j_uniform_float_map;
	for (auto& str_2_texDet : (*data.uniformToTexDetails))
		j_uniform_texture_map[str_2_texDet.first] = str_2_texDet.second.location;
	for (auto& uniform_2_float_map : (*data.uniformTofloat))
		j_uniform_float_map[uniform_2_float_map.first] = uniform_2_float_map.second;

	j_material["texMap"] = j_uniform_texture_map;
	j_material["floatMap"] = j_uniform_float_map;
	return j_material;
}
nlohmann::json ModelExporter::Serialize(Lights& data)
{
	nlohmann::json j_lights;
	
	j_lights["type"] = data.getType();

	ReflectionMeta ref;
	ref.read(data);
	j_lights["ak_serial"] = ref.m_json_object;

	return j_lights;
}
nlohmann::json ModelExporter::Serialize(AnimationController& data)
{
	nlohmann::json j_anim_cont;
	j_anim_cont["currTime"] = data.currTime;
	j_anim_cont["clipDuration"] = data.clipDuration;
	j_anim_cont["normalizedTime"] = data.normalizedTime;
	j_anim_cont["timePerTick"] = data.timePerTick;

	nlohmann::json j_anim_state_map;
	for (auto& map : data.bonesAnimStates)
	{
		nlohmann::json j_anim_bone_state;
		j_anim_bone_state["bone_name"] = map.first;
		
		ReflectionMeta ref;
		ref.read(map.second);
		j_anim_bone_state["bone_state"] = ref.m_json_object;

		j_anim_state_map.push_back(j_anim_bone_state);
	}
	j_anim_cont["bonesAnimStates"] = j_anim_state_map;

	nlohmann::json j_bone_list;
	for (auto& boneinfo : (*data.boneList))
	{
		ReflectionMeta ref;
		ref.read(boneinfo);
		j_bone_list.push_back(ref.m_json_object);
	}
	j_anim_cont["boneList"] = j_bone_list;

	nlohmann::json j_bone_map;
	for (auto& boneinfomap : (*data.boneMap))
	{
		ReflectionMeta ref;
		ref.read(boneinfomap.second);
		j_bone_map[boneinfomap.first]=ref.m_json_object;
	}
	j_anim_cont["boneList"] = j_bone_map;

	return j_anim_cont;
}
nlohmann::json ModelExporter::Serialize(EntityDescriptor& data)
{
	ReflectionMeta ref;
	ref.read(data);
	return ref.m_json_object;
}
nlohmann::json ModelExporter::Serialize(physics::RigidBody3D& data)
{
	nlohmann::json j_rbody;
	j_rbody["type"] = data.rBodyType;
	j_rbody["colliderShape"]["colShape"] = data.colliderShape.getShapeType();
	ReflectionMeta helper;
	nlohmann::json vec;
	glm::vec3 vec3=data.colliderShape.getColliderDimensions();
	helper.typeToJsonCmpx(vec,&vec3);
	j_rbody["colliderShape"]["dim"] = vec;
	vec.clear();
	helper.typeToJsonCmpx(vec, &(data.velocity));
	j_rbody["velocity"] = vec;
	return j_rbody;
}
void ModelExporter::ExportEntity(Entity eid)
{
	m_entity_arr.push_back(EntitySerializer(eid));
	if (EntityHasChild(eid))
	{
		Transform& t = m_scene.GetComponent<Transform>(eid);
		for (auto child_eid : t.getChildList())
		{
			ExportEntity(child_eid);
		}
	}
}
bool ModelExporter::EntityHasChild(Entity eid)
{
	if (m_scene.IsComponentAttached<Transform>(eid))
	{
		Transform& t = m_scene.GetComponent<Transform>(eid);
		if (t.getChildList().size() > 0)
			return true;
	}
	return false;
}
ModelExporter::ModelExporter(Scene& scene) :m_scene(scene)
{
	m_entity_arr = nlohmann::json::array_t();
	m_mesh_count = 0;
	m_ecs = scene.GetECS();
}

void ModelExporter::ExportEntity(Entity start,std::string forlderPath,std::string fileName)
{
	m_fileName = fileName;
	m_folderPath = forlderPath;
	m_entity_arr.clear();
	std::ofstream file(forlderPath+fileName+".edf");
	ExportEntity(start);
	
	file << m_entity_arr;
	file.close();
}
