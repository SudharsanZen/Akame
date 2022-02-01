#include<filesystem>
#include "FileOpenSaveDialogBox.h"
#include "Core\Editor\ContentBrowsing\ContentBrowser.h"
#include"glad/glad.h"
#include"glfw/glfw3.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<imgui/misc/cpp/imgui_stdlib.h>
#include<sstream>
#include<Math/GlmMath.h>
#include<Assets/AssetManager.h>
#include<Windows.h>
long long int folderIcoTexID;
long long int unknownfileIcoTexID;
bool isFlagSet(FileOpenSaveDialogBoxFlag base_flag,FileOpenSaveDialogBoxFlag flag)
{
    return ((base_flag & flag) == flag);
}

void FileOpenSaveDialogBox::setCurrentPath(std::string newPath)
{
    currPath = newPath;
}
FileOpenSaveDialogBox::FileOpenSaveDialogBox(std::string windowName, FileOpenSaveDialogBoxFlag mode, std::string path)
{
    m_mode = mode;
    open = true;
    this->windowTitle = windowName;

    this->currPath = path;
    unknownfileIcoTexID = AssetManager::createTexture(AssetManager::assetRootPath + "\\EngineAssets\\Icons\\file.png");;
    folderIcoTexID = AssetManager::createTexture(AssetManager::assetRootPath + "\\EngineAssets\\Icons\\folder1.png");
}
bool FileOpenSaveDialogBox::Open()
{
    return open;
}
void FileOpenSaveDialogBox::DrawContentHierarchyPannel()
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick |
        ImGuiTreeNodeFlags_SpanAvailWidth;
    drwID = 0;
    ImGuiTreeNodeFlags curr = base_flags;
   
    {


        static bool align_label_with_current_x_position = false;
        static bool test_drag_and_drop = false;

        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

        DWORD diskMask = GetLogicalDrives();
        for (DWORD i = 0; i < 26; i++)
        {
            if ((diskMask &(((DWORD)1) << i)) != 0)
            {
                std::stringstream ss;
                ss<<(char)((int)'a' + i)<<":/";
                //std::cout << ss.str()<<std::endl;
                ImGui::PushID(i);
                if (ImGui::TreeNodeEx(ss.str().c_str(), ImGuiTreeNodeFlags_DefaultOpen | curr))
                {
                    currPath = (ImGui::IsItemClicked()) ? ss.str() : currPath;
                    DrawHierarchyNode(ss.str(), base_flags);
                    ImGui::TreePop();
                }
                currPath = (ImGui::IsItemClicked()) ? ss.str() : currPath;
                ImGui::PopID();
            }
        }

        
    }
}
void FileOpenSaveDialogBox::DrawHierarchyNode(std::string path, ImGuiTreeNodeFlags const& base_flags)
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
            try
            {
                if (!testPath(folder.path().string()))
                    node_flags |= ImGuiTreeNodeFlags_Bullet;
            }
            catch (...)
            {
                continue;
            }
            bool node_open = ImGui::TreeNodeEx(folder.path().string().c_str(), node_flags, fname.c_str());
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
void FileOpenSaveDialogBox::DrawContentPanel()
{
    float iconSize = 100;
    std::shared_ptr<Texture> folderIcon = AssetManager::GetTexture(folderIcoTexID);
    std::shared_ptr<Texture> unknownFileIcon = AssetManager::GetTexture(unknownfileIcoTexID);

    int max_col = glm::ceil((ImGui::GetWindowWidth() / (iconSize + iconSize * 0.3f)));

    if (ImGui::BeginTable("FOSDB##11", max_col))
    {

        int i_col = max_col, i_row = -1;
        int curr_i=0;
        int curr_col=0;
        for (const auto& entry : std::filesystem::directory_iterator(currPath))
        {
            curr_col = curr_i % max_col;
            if (curr_col==0)
            {
                ImGui::TableNextRow();
            }
            
            ImGui::TableSetColumnIndex(curr_col);

            std::string name = entry.path().filename().string();
            bool showFiles = (entry.is_directory() || (!entry.is_directory() && !isFlagSet(m_mode, ak_Folder_mode)));
            if(showFiles)
            {
                ImGui::Spacing();
                ImGui::SameLine(ImGui::GetColumnWidth(curr_col) / 2.0f - iconSize / 2.0f);
                ImGui::PushID(curr_i);
                curr_i++;
            }
            if (entry.is_directory())
            {

                if (ImGui::ImageButton((ImTextureID)folderIcon->textureID, ImVec2(iconSize, iconSize), ImVec2(1, 1), ImVec2(0, 0), 0))
                {
                    if (currPath[currPath.size() - 1] == '/')
                        currPath += name;
                    else
                        currPath += "/" + name;
                    selectedfileName = "";
                }



            }
            else if(showFiles)
            {
                if (ImGui::ImageButton((ImTextureID)unknownFileIcon->textureID, ImVec2(iconSize, iconSize), ImVec2(1, 1), ImVec2(0, 0), 0))
                {
                    selectedfileName = name;
                }


            }
            if (showFiles)
            {
                ImGui::PopID();
                ImGui::Spacing();
                float font_size = ImGui::CalcTextSize(name.c_str()).x;
                ImGui::SameLine(ImGui::GetColumnWidth(curr_col) / 2.0f - font_size / 2.0f);
                ImGui::Text(name.c_str());
            }
         
        }
        ImGui::EndTable();
    }
}
void FileOpenSaveDialogBox::DrawUI()
{
    static float currWidth = 400;
    static bool spliterClicked = false;
    float bHeight = 100;
    if (ImGui::Begin(windowTitle.c_str(), &open))
    {
        float mainWindowHeight = ImGui::GetWindowHeight();
       
        if(ImGui::BeginChild("##contentBrowserWindows",ImVec2(ImGui::GetWindowWidth(), mainWindowHeight - bHeight)))
        {
            
            float height = ImGui::GetWindowHeight() - 50;
            float width = ImGui::GetWindowWidth();
            ImGui::BeginChild("##FoldersHierarchy", ImVec2(currWidth, height), true);
                DrawContentHierarchyPannel();
            ImGui::EndChild();

            ImGui::SameLine();
            //spliter button

            if (ImGui::Button("##content|", ImVec2(10, height)) || spliterClicked)
            {

                ImGuiIO io = ImGui::GetIO();

                spliterClicked = !(io.MouseReleased[0] && spliterClicked);

                currWidth += io.MouseDelta.x;

            }

            ImGui::SameLine();


            ImGui::BeginChild("##ContentBrowserChild", ImVec2(width - currWidth, height));
                ImGui::Separator();
                    ImGui::Text((currPath + "/" + selectedfileName).c_str());
                ImGui::Separator();
                DrawContentPanel();
            ImGui::EndChild();



            ImGui::EndChild();
        }

        if (ImGui::BeginChild("##bottombar"))
        {
            ImGui::Text("Path:"); ImGui::SameLine(); ImGui::InputText("", &selectedfileName); ImGui::SameLine();
            //std::string buttonName = (isFlagSet(m_mode,ak_file));
            ImGui::EndChild();
        }

        ImGui::End();
    }
}
std::string FileOpenSaveDialogBox::getSelectedFilePath()
{
    return currPath + selectedfileName;
}

std::string FileOpenSaveDialogBox::getSelectedFolderPath()
{
    return currPath;
}
