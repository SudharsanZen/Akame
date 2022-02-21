#define AK_PRIVATE_GETTER_SETTER
#include "Core/Serialization/SceneSerialization.h"
#include"Core/SceneEntityListSystem.h"
#include"Core/OS/OS_Interface.h"
#include<fstream>
#include"Core/Log/Log.h"
#include"assets\MeshManager.h"
#include"Physics/Physics.h"
#define TYPENAME(x) typeid(x).name()
std::string dirnameOf(const std::string& fname)
{
	size_t pos = fname.find_last_of("\\/");
	return (std::string::npos == pos)? "": fname.substr(0, pos);
}
std::string filenameOf(const std::string& fname)
{
	size_t slash = fname.find_last_of("\\/");
	size_t dot = fname.rfind(".");
	return (std::string::npos == dot || std::string::npos == slash) ? "" : fname.substr(slash+1,dot-slash-1);
}
SceneSerializer::SceneSerializer(Scene& scene) :m_scene(scene), m_exporter(scene)
{
	m_exporter.SetExportBinary(false);
	m_exporter.m_serializer_mode = true;
}

void SceneSerializer::SaveTo(std::string file_path)
{
	AK_ASSERT(file_path != "" && "invalid file path!");
	std::filesystem::path path(file_path);

	std::string directory = dirnameOf(file_path);
	std::string filename = filenameOf(file_path);
	AK_ASSERT((directory != "" && std::filesystem::is_directory(directory) || filename == "") && "invalid directory/filename!");

	m_exporter.SaveToFilePath(directory+"/", filename, ".ascene");
}

void SceneSerializer::Save()
{
	static constexpr char* filter = "Akame Scene File(*ascene)\0*ascene\0";
	
	std::shared_ptr<SceneEntityListSystem> m_s_list=m_scene.GetSystem<SceneEntityListSystem>();
	

	std::string file_path = SaveFileDialog(filter, "ascene");
	m_exporter.m_folderPath = AssetManager::assetRootPath;
	for (auto& entity : m_s_list->entities)
		m_exporter.ExportEntity(entity);
	if(file_path!="")
		SaveTo(file_path);
}

Entity& SceneDeserializer::GetNewEntity(Entity& old_eid)
{
	static Entity inv_id = INVALID_ENTITY;
	if (old_eid == inv_id)
		return inv_id;
	auto p_old_new = m_old_new_entity_map.find(old_eid);
	AK_ASSERT(p_old_new != m_old_new_entity_map.end() && "can't find the old id in the map!");

	return p_old_new->second;
}

void SceneDeserializer::DeserealizeTransforms(nlohmann::json &ser_data, Entity& eid)
{
	ReflectionMeta ref(this);
	ref.m_json_object = ser_data;
	Transform& td = m_scene.GetComponent<Transform>(eid);
	ref.write(td);	
}
template<typename _compType>
void SceneDeserializer::AddComponent(nlohmann::json& ser_data, Entity& eid)
{
	AK_ASSERT("Undefined Component!");
}
template<>
void SceneDeserializer::AddComponent<Mesh>(nlohmann::json& ser_data, Entity& eid)
{
	Mesh& mesh = m_scene.AddComponent<Mesh>(eid);
	mesh.IsModel(true);
	std::string local_path = ser_data["fileLocation:"];
	std::string local_path_rel = m_rel_path + "/" + local_path;
	std::string true_path = AssetManager::assetRootPath + local_path;
	if (m_is_edf)
		true_path = AssetManager::assetRootPath + local_path_rel;
	else
		true_path = AssetManager::assetRootPath+local_path;
		
	mesh.SetModelPath(true_path);
	if (MeshManager::IsAvailable(true_path))
	{
		auto meshInfo = MeshManager::GetMeshIndexData(true_path);
		mesh.end_i = meshInfo.end_i;
		mesh.start_i = meshInfo.start_i;
		mesh.maxInd = meshInfo.end_v;
		mesh.minInd = meshInfo.start_v;
		mesh.numOfIndices = meshInfo.numOfIndices;
		mesh.numOfVertices = meshInfo.numOfVertices;
		mesh.count = meshInfo.numOfIndices;
		return;
	}
	
	size_t v_size = ser_data["maxVert:"];
	size_t i_size = ser_data["maxIndex:"];
	std::vector<vert> vertex(v_size);
	std::vector<unsigned int> index(i_size);

	std::ifstream file(true_path, std::ios::in | std::ios::binary);
	for (size_t i = 0; !file.eof() && i < v_size; i++)
	{
		file.read((char*)&vertex[i], (sizeof(vert)));
	}
	for (size_t i = 0; !file.eof() && i < i_size; i++)
	{
		file.read((char*)&index[i], (sizeof(unsigned int)));
	}



	mesh.CreateMesh(vertex, index);
	auto loc = file.tellg();
	file.seekg(0, std::ios::end);
	auto endloc = file.tellg();
	std::cout << "\nfile pointer:" << loc << "\nfile end:" << endloc << std::endl;
	file.close();
	MeshIndexData info;
	info.end_i = mesh.end_i;
	info.start_i = mesh.start_i;
	info.end_v = mesh.maxInd;
	info.start_v = mesh.minInd;
	info.numOfIndices = mesh.numOfIndices;
	info.numOfVertices = mesh.numOfVertices;
	mesh.needsUpdate = true;
	MeshManager::SetMeshIndexData(true_path, info);
}
template<>
void SceneDeserializer::AddComponent<SkeletalMesh>(nlohmann::json& ser_data, Entity& eid)
{
	SkeletalMesh& mesh = m_scene.AddComponent<SkeletalMesh>(eid);
	mesh.IsModel(true);
	std::string local_path = ser_data["fileLocation:"];
	std::string local_path_rel = m_rel_path + "/" + local_path;
	std::string true_path = AssetManager::assetRootPath + local_path;
	if (m_is_edf)
		true_path = AssetManager::assetRootPath + local_path_rel;
	else
		true_path = AssetManager::assetRootPath+local_path;
	Entity old_anim_cont;
	ReflectionMeta helper;
	helper.JsonToTypeCmpx(ser_data["animCont"],&old_anim_cont);
	mesh.SetAnimControllerID(GetNewEntity(old_anim_cont));
	

	mesh.SetModelPath(true_path);
	if (MeshManager::IsAvailable(true_path))
	{
		auto meshInfo = MeshManager::GetMeshIndexData(true_path);
		mesh.end_i = meshInfo.end_i;
		mesh.start_i = meshInfo.start_i;
		mesh.maxInd = meshInfo.end_v;
		mesh.minInd = meshInfo.start_v;
		mesh.numOfIndices = meshInfo.numOfIndices;
		mesh.numOfVertices = meshInfo.numOfVertices;
		mesh.count = meshInfo.numOfIndices;
		return;
	}
	size_t v_size = ser_data["maxVert:"];
	size_t i_size = ser_data["maxIndex:"];
	std::vector<sk_vert> vertex(v_size);
	std::vector<unsigned int> index(i_size);
	
	std::ifstream file(true_path, std::ios::in | std::ios::binary);
	file.read((char*)&vertex[0], v_size * (sizeof(sk_vert)));
	file.read((char*)&index[0], i_size * (sizeof(unsigned int)));

	mesh.CreateMesh(vertex, index);

	file.close();
	MeshIndexData info;
	info.end_i = mesh.end_i;
	info.start_i = mesh.start_i;
	info.end_v = mesh.maxInd;
	info.start_v = mesh.minInd;
	info.numOfIndices = mesh.numOfIndices;
	info.numOfVertices = mesh.numOfVertices;
	mesh.needsUpdate = true;
	MeshManager::SetMeshIndexData(true_path, info);
	
}
template<>
void SceneDeserializer::AddComponent<Material>(nlohmann::json& ser_data, Entity& eid)
{
	std::string mat_name = ser_data["SHADER_NAME"];
	Material new_mat(mat_name);
	if (!ser_data.at("texMap").is_null())
	{
		auto texmap = ser_data.at("texMap").get<std::map<std::string, std::string>>();
		for (auto& uniform_tex_loc : texmap)
		{
			new_mat.setTexture2D(uniform_tex_loc.first, uniform_tex_loc.second);
		}
	}
	if (!ser_data.at("floatMap").is_null())
	{
		auto floatmap = ser_data.at("floatMap").get<std::map<std::string, float>>();

		for (auto& uniform_float_val : floatmap)
		{
			new_mat.setValue(uniform_float_val.first, uniform_float_val.second);
		}
	}
	m_scene.AddComponent<Material>(eid)=new_mat;
}
template<>
void SceneDeserializer::AddComponent<AnimationController>(nlohmann::json& ser_data, Entity& eid)
{
	AnimationController &anim = m_scene.AddComponent<AnimationController>(eid);
	anim.currTime =ser_data["currTime"];
	anim.clipDuration =ser_data["clipDuration"];
	anim.normalizedTime =ser_data["normalizedTime"];
	anim.timePerTick = ser_data["timePerTick"];
	for (auto& bone_state : ser_data["bonesAnimStates"])
	{
		ReflectionMeta ref;
		AnimationController::AnimState state;
		ref.m_json_object = bone_state["bone_state"];
		ref.write(state);
		anim.bonesAnimStates[bone_state["bone_name"]] = state;
	}
	anim.boneList = std::make_shared<std::vector<BoneInfo>>();
	for (auto& bone_info: ser_data["bone_info_list"])
	{
		ReflectionMeta ref(this);
		ref.m_json_object = bone_info;
		BoneInfo info;
		ref.write(info);
		anim.boneList->push_back(info);
	}
	anim.boneMap = std::make_shared<std::map<std::string,BoneInfo>>();
	auto map = ser_data.at("bone_info_map").get<std::map<std::string, nlohmann::json>>();
	for (auto& bone_map : map)
	{
		ReflectionMeta ref(this);
		BoneInfo info;
		ref.m_json_object = bone_map.second;
		ref.write(info);
		(*(anim.boneMap))[bone_map.first] = info;
	}

}
template<>
void SceneDeserializer::AddComponent<physics::RigidBody3D>(nlohmann::json& ser_data, Entity& eid)
{
	physics::RigidBody3D &rbody = m_scene.AddComponent<physics::RigidBody3D>(eid);
	physics::ColliderShape shape;
	ReflectionMeta helper;
	helper.JsonToTypeCmpx(ser_data["colliderShape"]["dim"],&shape.dim);
	helper.JsonToTypeCmpx(ser_data["velocity"],&rbody.velocity);

	shape.colShape = ser_data["colliderShape"]["colShape"];
	rbody.setRigidBodyType(ser_data["type"],shape);
}
template<>
void SceneDeserializer::AddComponent<Lights>(nlohmann::json& ser_data, Entity& eid)
{
	Lights& light = m_scene.AddComponent<Lights>(eid);
	ReflectionMeta ref;
	ref.m_json_object = ser_data["ak_serial"];
	ref.write(light);
	light.setType(ser_data["type"]);
}
SceneDeserializer::SceneDeserializer(Scene& scene):m_scene(scene)
{
	m_is_edf = false;
}

void SceneDeserializer::LoadFrom(std::string path_str)
{
	std::filesystem::path path = path_str;
	AK_ASSERT(std::filesystem::is_regular_file(path) && "file path is incorrect!");
	if (path.extension() == ".edf")
	{
		m_is_edf = true;
		std::filesystem::path folderPath;
		std::filesystem::path assetRootPath = AssetManager::assetRootPath;

		m_fileName = filenameOf(path_str);
		m_folderPath = dirnameOf(path_str);

		folderPath = m_folderPath;

		m_rel_path = std::filesystem::relative(folderPath, assetRootPath).generic_string();

		AK_ASSERT(std::filesystem::is_directory(assetRootPath) && "assign a proper path for base asset folder!");
		AK_ASSERT(std::filesystem::is_directory(folderPath) && "folder path is wrong!");
		AK_ASSERT(std::filesystem::is_directory(std::filesystem::path(AssetManager::assetRootPath + m_rel_path)) && "reconstructed folder path is invalid!");
	}
	else
		m_is_edf = false;
	std::ifstream ser(path_str);
	AK_ASSERT(ser.good());
	nlohmann::json j_scene;
	ser >> j_scene;
	auto find = [](nlohmann::json& j_obj, std::string key)
	{
		return j_obj.find(key) != j_obj.end();
	};
	auto get = [](nlohmann::json& j_obj, std::string key)
	{
		return j_obj.find(key);
	};
	//phase one initialize new entity IDs for every old entityIDs in the serialized json object
	if (find(j_scene,"entities"))
	{
		for (auto& ent : j_scene["entities"])
		{
			Entity new_eid = m_scene.CreateEntity();
			Entity old_eid(ent["index"], ent["version"]);
			m_old_new_entity_map[old_eid] = new_eid;
			if (find(ent, "Components"))
			{
				if (find(ent["Components"], TYPENAME(Transform)))
					m_scene.AddComponent<Transform>(new_eid);
				if (find(ent["Components"], TYPENAME(EntityDescriptor)))
				{
					EntityDescriptor &ed=m_scene.GetComponent<EntityDescriptor>(new_eid);
					auto j_ed=get(ent["Components"],TYPENAME(EntityDescriptor));
					ReflectionMeta ref;
					ref.m_json_object = (*j_ed);
					ref.write(ed);
				}
				
			}
		}
		for (auto& ent : j_scene["entities"])
		{
			Entity old_eid(ent["index"], ent["version"]);
			Entity new_eid=GetNewEntity(old_eid);

			if(find(ent, "Components"))
			{
				auto &comp = ent["Components"];
				if (find(comp, TYPENAME(Transform)))
					DeserealizeTransforms(*get(comp,TYPENAME(Transform)), new_eid);
				if (find(comp, TYPENAME(Mesh)))
					AddComponent<Mesh>(*get(comp,TYPENAME(Mesh)),new_eid);
				if (find(comp, TYPENAME(SkeletalMesh)))
					AddComponent<SkeletalMesh>(*get(comp, TYPENAME(SkeletalMesh)), new_eid);
				if (find(comp, TYPENAME(Material)))
					AddComponent<Material>(*get(comp, TYPENAME(Material)), new_eid);
				if (find(comp, TYPENAME(AnimationController)))
					AddComponent<AnimationController>(*get(comp, TYPENAME(AnimationController)), new_eid);
				if (find(comp, TYPENAME(Lights)))
					AddComponent<Lights>(*get(comp, TYPENAME(Lights)), new_eid);
				if (find(comp, TYPENAME(physics::RigidBody3D)))
					AddComponent<physics::RigidBody3D>(*get(comp, TYPENAME(physics::RigidBody3D)), new_eid);
			}
		}
	}
}

void SceneDeserializer::Load(bool resetScene)
{
	static constexpr char* filter = "Akame Scene File(*ascene)\0*ascene\0";
	
	std::string file_path = OpenFileDialog(filter, "ascene");
	if (file_path != "")
	{
		if (resetScene)
		{
		
			m_scene.physicsSys->mPhysics->cleanPhysics();;
			m_scene.InitScene();
		}
		LoadFrom(file_path);
	}
}

