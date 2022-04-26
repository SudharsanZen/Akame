#include "Core\Editor\ContentBrowsing\ContentBrowser.h"
#include<filesystem>
#include<sstream>
#include<Math/GlmMath.h>
#include<Assets/AssetManager.h>
#pragma warning(push, 0)
#pragma warning( disable : 26495)
#pragma warning( disable : 6031)
#pragma warning( disable : 26812)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#pragma warning(pop)
long long int folderIconID;
long long int unknownfileIconID;
#define EXPOSE_OS_INTERFACE
#include"Core/OS/OS_Interface.h"
std::string getNameFromPath(std::string path)
{
   
    std::reverse(path.begin(),path.end());
    size_t i_1=path.find('/');
    size_t i_2=path.find('\\');
    if (i_1 == std::string::npos && i_2 == std::string::npos)
    {
        std::reverse(path.begin(), path.end());
        return path;
    }
    else
    {
        path = path.substr(0, glm::min(i_1, i_2));
        std::reverse(path.begin(), path.end());
        return path;
    }
   
   
}

std::string getExtention(std::string path)
{
    

    std::reverse(path.begin(), path.end());
    size_t i_1 = path.find('.');

    if (i_1 == std::string::npos)
    {
        return "";
    }
    else
    {
        path = path.substr(0,i_1);
        std::reverse(path.begin(), path.end());
        return path;
    }

}
void ContentBrowser::setCurrentPath(std::string newPath)
{
    currPath = newPath;
}
ContentBrowser::ContentBrowser(std::string windowName,std::string path)
{
    open = true;
    this->windowTitle = windowName;

    this->currPath = path;
    unknownfileIconID = AssetManager::createTexture(AssetManager::assetRootPath+"\\EngineAssets\\Icons\\file.png");;
    folderIconID =AssetManager::createTexture(AssetManager::assetRootPath+ "\\EngineAssets\\Icons\\folder1.png");
}
bool ContentBrowser::Open()
{
    return open;
}
void ContentBrowser::DrawContentHierarchyPannel()
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick | 
        ImGuiTreeNodeFlags_SpanAvailWidth;
    drwID = 0;
    ImGuiTreeNodeFlags curr = base_flags;
    if (currPath != AssetManager::assetRootPath)
        ImGui::SetNextTreeNodeOpen(true);
    else
        curr |= ImGuiTreeNodeFlags_Selected;
 
    bool assetNode_open = ImGui::TreeNodeEx("Assets", ImGuiTreeNodeFlags_DefaultOpen|curr);
    //ImGui::Image((void*)AssetManager::GetTexture(folderIconID)->textureID,ImVec2(5,5));
    if (ImGui::IsItemClicked())
    {
        currPath = AssetManager::assetRootPath;
    }
    if (assetNode_open)
    {

       
        static bool align_label_with_current_x_position = false;
        static bool test_drag_and_drop = false;

        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

        
        DrawHierarchyNode(AssetManager::assetRootPath,base_flags);


        ImGui::TreePop();
    }
}
void ContentBrowser::DrawHierarchyNode(std::string path,ImGuiTreeNodeFlags const& base_flags)
{
   
   
    auto testPath = [](std::string path)->bool
    {
        for (auto& p : std::filesystem::directory_iterator(path))
        {
            if (p.is_directory())
                return true;
        }
        return false;
    };
        for (auto& folder : std::filesystem::directory_iterator(path))
        {
            ImGuiTreeNodeFlags node_flags = base_flags;
            std::string fname = folder.path().filename().string();
            if (folder.path().string() == currPath)
                node_flags |= ImGuiTreeNodeFlags_Selected;
            std::string folderPath = folder.path().string();
            if (folder.is_directory())
            {
               
                if (!testPath(folder.path().string()))
                    node_flags|= ImGuiTreeNodeFlags_Bullet;
                bool node_open = ImGui::TreeNodeEx(folder.path().string().c_str(), node_flags,fname.c_str());
                if (ImGui::IsItemClicked())
                {
                    currPath = folder.path().string();
                    selectedfileName = "";
                }
                if (node_open)
                {
                    DrawHierarchyNode(folder.path().string(), base_flags);
                    ImGui::TreePop();
                }
               
            }
        }
   
}
void ContentBrowser::DrawContentPanel()
{
    float iconSize = 100;
    std::shared_ptr<Texture> folderIcon = AssetManager::GetTexture(folderIconID);
    std::shared_ptr<Texture> unknownFileIcon = AssetManager::GetTexture(unknownfileIconID);

    int max_col = static_cast<int>(glm::ceil((ImGui::GetWindowWidth() / (iconSize+iconSize*0.3f))));

    if (ImGui::BeginTable("DirectoryContent##11", max_col))
    {

        int i_col = max_col, i_row = -1;
        for (const auto& entry : std::filesystem::directory_iterator(currPath))
        {
            if (i_col == max_col)
            {
                i_row++;
                i_col = 0;
                ImGui::TableNextRow();
            }
            ImGui::TableSetColumnIndex(i_col);

            std::string name = entry.path().filename().string();
            ImGui::Spacing();
            ImGui::SameLine(ImGui::GetColumnWidth(i_col) / 2.0f -iconSize / 2.0f);
            ImGui::PushID(i_col + i_row * max_col);
            if (entry.is_directory())
            {
               
                if (ImGui::ImageButton((ImTextureID)static_cast<int64_t>(folderIcon->textureID), ImVec2(iconSize, iconSize), ImVec2(1, 1), ImVec2(0, 0),0))
                {
                    if (currPath[currPath.size() - 1] == '/')
                        currPath += name;
                    else
                        currPath += "/" + name;
                    selectedfileName = "";
                }
               
                

            }
            else
            {
                if (ImGui::ImageButton((ImTextureID)static_cast<int64_t>(unknownFileIcon->textureID), ImVec2(iconSize, iconSize), ImVec2(1, 1), ImVec2(0, 0),0))
                {
                    selectedfileName = name;
                }

               
            }
            ImGui::PopID();
            ImGui::Spacing();
            float font_size = ImGui::CalcTextSize(name.c_str()).x;
            ImGui::SameLine(ImGui::GetColumnWidth(i_col) / 2.0f-font_size/2.0f );
            ImGui::Text(name.c_str());

            i_col++;
        }
        ImGui::EndTable();
    }
}
void ContentBrowser::DrawUI()
{
   
   static float currWidth=400;
   static bool spliterClicked = false;
    if (ImGui::Begin(windowTitle.c_str(),&open))
    {
        float height = ImGui::GetWindowHeight()-50;
        float width = ImGui::GetWindowWidth();
        if (ImGui::BeginChild("ContentHierarchy", ImVec2(currWidth, height), true))
        {
            DrawContentHierarchyPannel();
            ImGui::EndChild();
        }
        ImGui::SameLine();
        //spliter button

        if (ImGui::Button("##content|", ImVec2(10, height))||spliterClicked)
        {
           
            ImGuiIO io=ImGui::GetIO();
            
            spliterClicked = !(io.MouseReleased[0] && spliterClicked);
         
            currWidth += io.MouseDelta.x;
            
        }
        
        ImGui::SameLine();


        if (ImGui::BeginChild("##ContentBrowserChild", ImVec2(width - currWidth, height)))
        {
            ImGui::Separator();
            ImGui::Text((currPath + "/" + selectedfileName).c_str());
            ImGui::Separator();

            if (ImGui::BeginChild("##cnt", ImVec2(ImGui::GetWindowWidth() - 30, ImGui::GetWindowHeight() - 10)))
            {
                DrawContentPanel();
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }
}
std::string ContentBrowser::getSelectedFileName()
{
    return currPath+selectedfileName;
}
