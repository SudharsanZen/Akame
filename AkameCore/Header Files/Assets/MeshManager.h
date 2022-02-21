#pragma once
#include"Components/Rendering/Mesh.h"
#include<map>
#include<string>
#include<filesystem>
#include"Core/AkameCore.h"
struct MeshIndexData
{
	//IBO indices range [start_i,end_i]
	size_t start_i = 0;
	size_t end_i = 0;
	//vertex array range [start_v,end_v]
	size_t start_v=0;
	size_t end_v=0;

	size_t numOfIndices;
	size_t numOfVertices;
};
class MeshManager
{
private:
	//maps the physical path of a .mesh file to a MeshIndexData that locates that specific mesh's, vertex/Index buffer data inside
	//the Mesh Class's static vertex/index buffer data.  
	static std::map<std::filesystem::path,MeshIndexData> m_mesh_map;
public:
	static AKAME_API MeshManager* GetMeshManager();
	static AKAME_API bool IsAvailable(std::string path_str);
	static AKAME_API MeshIndexData GetMeshIndexData(std::string path_str);
	static AKAME_API void SetMeshIndexData(std::string path_str,MeshIndexData);
};

