#include "Core\Editor\EditorUI.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<iostream>
#include"Core/Input.h"
#include<sstream>
#include"Components/EntityDescriptor.h"
#include"Rendering/System/LightSystem.h"
void Editor::initImGui()
{
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(context.lock().get(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

ImGuiIO& Editor::initGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	return ImGui::GetIO();
}

Editor::Editor(Window &window,Scene &scene):io(initGui()),scene(scene)
{
	context = window.mainWindow;
	initImGui();
}
void Editor::DrawNode(Transform const& t, EntityDescriptor &edt, ImGuiTreeNodeFlags const& base_flags)
{
	
	EntityID selectedEntity(-1, -1);
    int numOfChild = t.child.size();
    static int selection_mask = (1 << 2);
    int node_clicked = -1;
    ImGuiTreeNodeFlags node_flags = base_flags;
	std::stringstream str;
	str << " id: "<<edt.eid.index<<"-"<<edt.eid.version;
	const bool is_selected = (selected.find(edt.eid)!=selected.end());
    if (is_selected)
        node_flags |= ImGuiTreeNodeFlags_Selected;
    if (numOfChild>0)
    {
        //tree nodes
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)edt.eid.index, node_flags, (edt.name+str.str()).c_str(), edt.eid.index);
		if (ImGui::IsItemClicked())
		{
			selectedEntity = edt.eid;
		}
           
        if (node_open)
        {
            for (auto child : t.child)
            {
                Transform& t = scene.ecs->GetComponent<Transform>(child);
                EntityDescriptor& edt = scene.ecs->GetComponent<EntityDescriptor>(child);
                DrawNode(t,edt,base_flags);
            }
			ImGui::TreePop();
        }

		
    }
    else
    {
      //leaves
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)edt.eid.index, node_flags, (edt.name + str.str()).c_str(), edt.eid.index);
		if (ImGui::IsItemClicked())
			selectedEntity = edt.eid;

		
    }
	if (selectedEntity != EntityID(-1, -1))
	{
		if (ImGui::GetIO().KeyCtrl)
		{
			if (selected.find(edt.eid) == selected.end())
				selected.insert(edt.eid);
			else
			{
				selected.erase(edt.eid);
			}
		}
		else
		{
			selected.clear();
			selected.insert(edt.eid);
		}
	}
}


void Editor::DrawUI()
{
	if (ImGui::IsKeyDown(KEY_DELETE))
	{

		for (auto ent : selected)
		{
			scene.ecs->DestroyEntity(ent);

		}
		selected.clear();
	}
	std::shared_ptr<ECS> e = scene.ecs;
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	
	ImGui::NewFrame();
	ImGui::Begin("SceneGraph");

	
	
    if (ImGui::TreeNode("Scene"))
    {
     
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool align_label_with_current_x_position = false;
        static bool test_drag_and_drop = false;
       
        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());


        for (auto ent : scene.transformManager->entities)
        {
            Transform& t = e->GetComponent<Transform>(ent);
            EntityDescriptor& edt = e->GetComponent<EntityDescriptor>(ent);
            if (t.parent == EntityID(-1,-1))
            {
                DrawNode(t,edt,base_flags);
            }
        }

      
        ImGui::TreePop();
    }

	ImGui::End();

	ImGui::Begin("DirShadowMap");
	//get the mouse position
	ImVec2 pos = ImGui::GetCursorScreenPos();
	
	//pass the texture of the FBO
	//window.getRenderTexture() is the texture of the FBO
	//the next parameter is the upper left corner for the uvs to be applied at
	//the third parameter is the lower right corner
	//the last two parameters are the UVs
	//they have to be flipped (normally they would be (0,0);(1,1) 
	ImGui::GetWindowDrawList()->AddImage(
		(void*) scene.lightSys->dir_sMap.getMapBuff(),
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + scene.lightSys->dir_sMap.getWidth() / 3,
			ImGui::GetCursorScreenPos().y + scene.lightSys->dir_sMap.getHeight() / 3), ImVec2(0, 1), ImVec2(1, 0));
    
	ImGui::End();

	ImGui::Render();
	int display_w=0, display_h=0;
	std::shared_ptr<GLFWwindow> windowContext = context.lock();
	if (windowContext)
	{
		glfwGetFramebufferSize(windowContext.get(), &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
	}
	else
	{
		std::cout << "window context not initialized (or) is destroyed!\n";
		return;
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}


}

