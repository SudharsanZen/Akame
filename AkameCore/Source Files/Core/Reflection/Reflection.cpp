#include "..\..\..\Header Files\Core\Reflection\Reflection.h"
#include"glad/glad.h"
#include"glfw/glfw3.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<Core/Editor/ImGuiUtils.h>
#include<vector>
#define IS_NPOSE(x) (x == std::string::npos)
std::vector<std::string> splitStrings(std::string str)
{
	std::vector<std::string> val_strs;
	std::size_t end;
	while (str != "")
	{
		end = str.find(',');
		val_strs.push_back(str.substr(0,end));
		if (end == std::string::npos)
			str = "";
		else
			str = str.substr(end+1);
	}
	return val_strs;
}
std::string remCurls(std::string curly_str)
{
	size_t i_brack = 0;

	std::size_t start = curly_str.find('{');
	size_t end = std::string::npos;
	size_t str_pose = start;
	while (str_pose < curly_str.size())
	{
		if ('{' == curly_str[str_pose])
			i_brack++;
		if ('}' == curly_str[str_pose])
		{
			i_brack--;
			if (i_brack == 0)
			{
				end = str_pose;
				break;
			}

		}
		str_pose++;
	}
	if (start == std::string::npos || end == std::string::npos)
		throw("can't find ]");
	return curly_str.substr(start + 1, end - start - 1);
}

std::string remBoxBrack(std::string box_str)
{
	size_t i_brack=0;
	
	std::size_t start = box_str.find('[');
	size_t end = std::string::npos;
	size_t str_pose = start;
	while (str_pose < box_str.size())
	{
		if ('[' == box_str[str_pose])
			i_brack++;
		if (']' == box_str[str_pose])
		{
			i_brack--;
			if (i_brack == 0)
			{
				end=str_pose;
				break;
			}
		
		}
			str_pose++;
	}
	if (start == std::string::npos || end == std::string::npos)
		throw("can't find ]");
	return box_str.substr(start + 1, end - start - 1);
}

std::vector<std::string> splitSerialAttributes(std::string class_str)
{
	std::vector<std::string> list;
	std::size_t start=std::string::npos, end= std::string::npos;

	while (class_str != "")
	{
		size_t i_brack=0;
		start = class_str.find('[');
		size_t str_pose = start;
		while (str_pose < class_str.size())
		{
			if ('[' == class_str[str_pose])
				i_brack++;
			if (']' == class_str[str_pose])
			{
				i_brack--;
				if (i_brack == 0)
				{
					end = str_pose;
					break;
				}

			}
			str_pose++;
		}
		if (start == std::string::npos || end == std::string::npos)
			return list;
		list.push_back(class_str.substr(start,end-start+1));
		class_str = class_str.substr(end+1);
	}
	return list;
}
void ReflectionUIHandler::DrawTitle(std::string name)
{
	DrawHeadingWithSeperator(name);
}
void ReflectionUIHandler::DrawUIField(std::string name, int *val)
{
	ImGui::DragInt(("##"+name).c_str(), val);
}

void ReflectionUIHandler::DrawUIField(std::string name, float *val)
{
	ImGui::DragFloat(("##" + name).c_str(),val);
}

void ReflectionUIHandler::DrawUIField(std::string name, double* val)
{
	ImGui::InputDouble(("##" + name).c_str(),val);
}

void ReflectionUIHandler::DrawUIField(std::string name, glm::vec3* val)
{

	std::string names[] = {"x","y","z"};
	
	dragFloat3(names,(float*)val,80);
}

void ReflectionUIHandler::DrawUIField(std::string name, glm::vec4* val)
{
	
	std::string names[] = { "x","y","z","w"};

	dragFloat4(names, (float*)val, 80);
}

void ReflectionUIHandler::DrawUIField(std::string name, bool* val)
{

	ImGui::Checkbox(("##" + name).c_str(),val);
}

void ReflectionUIHandler::DrawUIField(std::string name, Quaternion* val)
{
	DrawUIField(name,&val->quaternion);
}

void ReflectionUIHandler::DrawUIField(std::string name, glm::quat* val)
{
	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	if (ImGui::BeginTable("rotation", 4, flags))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::TextDisabled("w: %f", (*val).w);
		ImGui::TableSetColumnIndex(1);
		ImGui::TextDisabled("x: %f", (*val).x);
		ImGui::TableSetColumnIndex(2);
		ImGui::TextDisabled("y: %f", (*val).y);
		ImGui::TableSetColumnIndex(3);
		ImGui::TextDisabled("z: %f", (*val).z);
		ImGui::EndTable();
	}
}

void ReflectionUIHandler::DrawUIField(std::string name, std::string* val)
{
	char valc[100];
	strcpy(valc,val->c_str());

	ImGui::InputText(("##"+name).c_str(),valc,100);
	*val = valc;
}

void ReflectionMeta::stringToType(std::string str_val, Quaternion* val)
{
	stringToType(str_val,&val->quaternion);
	
}

void ReflectionMeta::stringToType(std::string str_val, glm::vec3* val)
{
	std::string val_str = remCurls(str_val);
	auto str_list = splitStrings(val_str);
	stringToType(str_list[0], &val->x);
	stringToType(str_list[1], &val->y);
	stringToType(str_list[2], &val->z);
}

void ReflectionMeta::stringToType(std::string str_val, glm::vec4* val)
{
	std::string val_str = remCurls(str_val);
	auto str_list = splitStrings(val_str);
	stringToType(str_list[0], &val->x);
	stringToType(str_list[1], &val->y);
	stringToType(str_list[2], &val->z);
	stringToType(str_list[3], &val->w);
}

void ReflectionMeta::stringToType(std::string str_val, glm::quat* val)
{
	std::string val_str = remCurls(str_val);
	auto str_list = splitStrings(val_str);
	stringToType(str_list[0], &val->x);
	stringToType(str_list[1], &val->y);
	stringToType(str_list[2], &val->z);
	stringToType(str_list[3], &val->w);
}

void ReflectionMeta::typeToString(std::string str_name, Quaternion* quat)
{
	typeToString(str_name,&quat->quaternion);
}

void ReflectionMeta::typeToString(std::string str_name, glm::vec3* vec3)
{
	std::stringstream ss;
	ss << "{" << vec3->x << "," << vec3->y << "," << vec3->z<< "}";
	m_id_value_pair[str_name] = ss.str();
}

void ReflectionMeta::typeToString(std::string str_name, glm::vec4* vec4)
{
	std::stringstream ss;
	ss << "{" << vec4->x << "," << vec4->y << "," << vec4->z << "," << vec4->w << "}";
	m_id_value_pair[str_name] = ss.str();
}

void ReflectionMeta::typeToString(std::string str_name, glm::quat* quat)
{
	std::stringstream ss;
	ss << "{" << quat->x << "," << quat->y << "," << quat->z << "," << quat->w << "}";
	m_id_value_pair[str_name] = ss.str();
}

void ReflectionMeta::from_string(std::string searialized_class)
{
	size_t  start = searialized_class.find_first_of('{');
	size_t  end = searialized_class.find_last_of('}');
	std::string str =searialized_class.substr(start+1,end-start-1);
	auto str_list = splitSerialAttributes(str);
	m_id_value_pair.clear();
	for (auto str : str_list)
	{
		std::string name_val_pair = remBoxBrack(str);
		std::size_t colon_pose=name_val_pair.find(':');
		if (IS_NPOSE(colon_pose))
			throw("can't find ':', invalid format!");
		std::string id_name = name_val_pair.substr(0,colon_pose);
		std::string val_str = name_val_pair.substr(colon_pose+1);
		m_id_value_pair[id_name] = val_str;
	}

}
