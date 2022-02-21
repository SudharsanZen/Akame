#include"Core/Reflection/ReflectionMeta.h"
#include"Core/Serialization/SceneSerialization.h"
ReflectionMeta::ReflectionMeta(SceneDeserializer* deserializer)
{
	m_deser = deserializer;
}
ReflectionMeta::ReflectionMeta()
{
	m_deser= nullptr;
}
void ReflectionMeta::JsonToTypeCmpx(nlohmann::json& json, Entity* val)
{
	if (m_deser != nullptr)
	{
		Entity old_id(json["index"],json["version"]);
		*val=m_deser->GetNewEntity(old_id);
		return;
	}
	val->index = json["index"];
	val->version = json["version"];
}

void ReflectionMeta::JsonToTypeCmpx(nlohmann::json& json, Quaternion* quater)
{
	quater->quaternion.x = json["x"];
	quater->quaternion.y = json["y"];
	quater->quaternion.z = json["z"];
	quater->quaternion.w = json["w"];
}

void ReflectionMeta::JsonToTypeCmpx(nlohmann::json& json, glm::vec3* vec3)
{
	vec3->x = json["x"];
	vec3->y = json["y"];
	vec3->z = json["z"];

}

void ReflectionMeta::JsonToTypeCmpx(nlohmann::json& json, glm::vec4* vec4)
{
	vec4->x = json["x"];
	vec4->y = json["y"];
	vec4->z = json["z"];
	vec4->w = json["w"];
}

void ReflectionMeta::JsonToTypeCmpx(nlohmann::json& json, glm::quat* quat)
{
	quat->x = json["x"];
	quat->y = json["y"];
	quat->z = json["z"];
	quat->w = json["w"];
}

void ReflectionMeta::JsonToTypeCmpx(nlohmann::json& json, glm::mat4* mat)
{
	for (int i = 0; i < 16; i++)
		(*mat)[i / 4][i % 4]=json[i];
}

void ReflectionMeta::typeToJsonCmpx(nlohmann::json& json, Quaternion* quat)
{
	typeToJsonCmpx(json, &quat->quaternion);
}

void ReflectionMeta::typeToJsonCmpx(nlohmann::json& json, glm::vec3* vec3)
{
	nlohmann::json obj;
	obj["x"] = vec3->x;
	obj["y"] = vec3->y;
	obj["z"] = vec3->z;
	json = obj;
}

void ReflectionMeta::typeToJsonCmpx(nlohmann::json& json, glm::vec4* vec4)
{
	nlohmann::json obj;
	obj["x"] = vec4->x;
	obj["y"] = vec4->y;
	obj["z"] = vec4->z;
	obj["w"] = vec4->w;
	json = obj;
}

void ReflectionMeta::typeToJsonCmpx(nlohmann::json& json, glm::quat* quat)
{
	nlohmann::json obj;
	obj["x"] = quat->x;
	obj["y"] = quat->y;
	obj["z"] = quat->z;
	obj["w"] = quat->w;
	json = obj;
}

void ReflectionMeta::typeToJsonCmpx(nlohmann::json& json, glm::mat4* mat)
{
	nlohmann::json j_mat;
	for (int i = 0; i < 16; i++)
		j_mat[i] = (*mat)[i / 4][i % 4];
	json = j_mat;
}

void ReflectionMeta::typeToJsonCmpx(nlohmann::json& json, Entity* ptr)
{
	nlohmann::json ent;
	ent["index"] = ptr->index;
	ent["version"] = ptr->version;
	json = ent;
}

std::string ReflectionMeta::to_string()
{
	return m_json_object.dump();
}

void ReflectionMeta::print_formatted() 
{ 
	std::cout << m_json_object.dump(4) << std::endl; 
}

void ReflectionMeta::from_string(std::string searialized_class)
{
	m_json_object = nlohmann::json::parse(searialized_class);
}

