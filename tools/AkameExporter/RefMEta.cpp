#include"Refmeta.h"

void ReflectionMeta::JsonToTypeCmpx(json::json &json, Entity* val)
{
	val->index= json["index"];
	val->version= json["version"];
}

void ReflectionMeta::JsonToTypeCmpx(json::json &json, Quaternion* quater)
{
	quater->quaternion.x=json["x"];
	quater->quaternion.y=json["y"];
	quater->quaternion.z=json["z"];
	quater->quaternion.w=json["w"];
}

void ReflectionMeta::JsonToTypeCmpx(json::json &json, glm::vec3* vec3)
{
	vec3->x = json["x"];
	vec3->y = json["y"];
	vec3->z = json["z"];

}

void ReflectionMeta::JsonToTypeCmpx(json::json &json, glm::vec4* vec4)
{
	vec4->x = json["x"];
	vec4->y = json["y"];
	vec4->z = json["z"];
	vec4->w = json["w"];
}

void ReflectionMeta::JsonToTypeCmpx(json::json &json, glm::quat* quat)
{
	quat->x = json["x"];
	quat->y = json["y"];
	quat->z = json["z"];
	quat->w = json["w"];
}

void ReflectionMeta::typeToJsonCmpx(json::json& json, Quaternion* quat)
{
	typeToJsonCmpx(json, &quat->quaternion);
}

void ReflectionMeta::typeToJsonCmpx(json::json& json, glm::vec3* vec3)
{
	json::json obj;
	obj["x"] = vec3->x;
	obj["y"] = vec3->y;
	obj["z"] = vec3->z;
	json = obj;
}

void ReflectionMeta::typeToJsonCmpx(json::json& json, glm::vec4* vec4)
{
	json::json obj;
	obj["x"] = vec4->x;
	obj["y"] = vec4->y;
	obj["z"] = vec4->z;
	obj["w"] = vec4->w;
	json = obj;
}

void ReflectionMeta::typeToJsonCmpx(json::json& json, glm::quat* quat)
{
	json::json obj;
	obj["x"] = quat->x;
	obj["y"] = quat->y;
	obj["z"] = quat->z;
	obj["w"] = quat->w;
	json = obj;
}

void ReflectionMeta::typeToJsonCmpx(json::json& json, Entity* ptr)
{
	json::json ent;
	ent["index"] = ptr->index;
	ent["version"] = ptr->version;
	json = ent;
}

std::string ReflectionMeta::to_string()
{
	return m_json_object.dump();
}

void ReflectionMeta::from_string(std::string searialized_class)
{
	m_json_object = json::json::parse(searialized_class);
}

