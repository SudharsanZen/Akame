#include "..\..\Header Files\Assets\MeshManager.h"
std::map<std::filesystem::path, MeshIndexData> MeshManager::m_mesh_map = {};
MeshManager* MeshManager::GetMeshManager()
{
	static MeshManager mm;
	return &mm;
}
bool MeshManager::IsAvailable(std::string path_str)
{
	std::filesystem::path path = path_str;
	auto itr = m_mesh_map.find(path);
	return (itr != m_mesh_map.end());
}

MeshIndexData MeshManager::GetMeshIndexData(std::string path_str)
{
	std::filesystem::path path = path_str;
	auto itr = m_mesh_map.find(path);
	AK_ASSERT(itr != m_mesh_map.end() && "before fetching the Mesh info,please check if the mesh is already loaded using IsAvailable(path_str)!");
	return itr->second;
}

AKAME_API void MeshManager::SetMeshIndexData(std::string path_str, MeshIndexData mdata)
{
	std::filesystem::path path = path_str;
	auto itr = m_mesh_map.find(path);
	AK_ASSERT(itr == m_mesh_map.end() && "mesh is already loaded into the buffer!");
	m_mesh_map[path] = mdata;
}

