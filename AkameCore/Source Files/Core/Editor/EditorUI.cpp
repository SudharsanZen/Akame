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
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/DeferredPipeline.h"
#include"Rendering/System/PSSMFBO.h"
float pssmx = 1.45f;
float pssmy = 1.45f;
void Editor::initImGui()
{
	fbo[0].updateTextureSize(512,512);
	fbo[1].updateTextureSize(512,512);
	fbo[2].updateTextureSize(512,512);
	fbo[3].updateTextureSize(512,512);
	fbo[4].updateTextureSize(512,512);
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
	std::string entName = edt.name;
	//tree nodes
	if (viewDebugInfo)
		entName += str.str();
    if (numOfChild>0)
    {
		
        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)edt.eid.index, node_flags, (entName).c_str(), edt.eid.index);
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
        ImGui::TreeNodeEx((void*)(intptr_t)edt.eid.index, node_flags, (entName).c_str(), edt.eid.index);
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

	if (ImGui::Button("ToggleDebugInfo", ImVec2(200.0f, 25.0f)))
		viewDebugInfo = !viewDebugInfo;
	
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
	
	ImGui::Begin("DirShadowMaps");
	//get the mouse position
	ImGui::Text("dir light");

	Lights &dir = scene.ecs->GetComponent<Lights>(scene.lightSys->lightsList[LIGHT::DIRECTIONAL][0]);
	ImGui::SliderFloat("X", &dir.lightDirection.x, -1.0f, 1.0f, "%.3f");
	ImGui::SliderFloat("Y", &dir.lightDirection.y, -1.0f, 1.0f, "%.3f");
	ImGui::SliderFloat("Z", &dir.lightDirection.z, -1.0f, 1.0f, "%.3f");
	ImGui::SliderFloat("multX", &pssmx, 1.0f, 2.0f, "%.3f");
	ImGui::SliderFloat("multY", &pssmy, 1.0f, 2.0f, "%.3f");
	static float maxz = 1000;
	static float minz = 100;
	ImGui::DragScalar("ShadowDist", ImGuiDataType_Float, &scene.lightSys->shadowDistance, 1,&minz,&maxz, "%f");
	bool clicked=ImGui::Button("Reload Shaders",ImVec2(200.0f,25.0f) );
	
	if (clicked)
	{

		ShaderManager::releaseAllShaders();
		ShaderManager::loadAllShaders();
		scene.renderSys->attachAllBuiltInSRP();
		
	}
	ImGui::SliderFloat("log-uniform lambda", &scene.lightSys->lambda, 0.0f, 1.0f, "ratio = %.3f");
	ImVec2 pos = ImGui::GetCursorScreenPos();
	
	//pass the texture of the FBO
	//window.getRenderTexture() is the texture of the FBO
	//the next parameter is the upper left corner for the uvs to be applied at
	//the third parameter is the lower right corner
	//the last two parameters are the UVs
	//they have to be flipped (normally they would be (0,0);(1,1) 
	
	ShaderManager::GetShader("PSSM")->useShaderProgram();
	std::vector<unsigned int> texList;
	for (int i = 0; i < scene.lightSys->dirLightSpace.size(); i++)
	{
		glDisable(GL_CULL_FACE);
		fbo[i].Bind();
		ShaderManager::GetShader("PSSM")->setUniformInteger("layer", i);
		scene.lightSys->dir_sMap.useTextureArray(0);
		fbo[i].quadMesh.renderMesh();
		fbo[i].unBind();
		glEnable(GL_CULL_FACE);
		texList.push_back(fbo[i].getColorBuffer());
	}
	std::shared_ptr<DeferredPipeline> dfrp = std::static_pointer_cast<DeferredPipeline>(ShaderManager::shaderRenderPipeline["DEFERRED"]);
	if (dfrp)
	{
		texList.push_back(dfrp->drfb.drfb.getAlbedoBuff());
		texList.push_back(dfrp->drfb.drfb.getPositionBuff());
		texList.push_back(dfrp->drfb.drfb.Normal);
		texList.push_back(dfrp->drfb.drfb.getDepthBuff());
	}
	
	int div = 2,res=512/3;
	for (int i = 0; i < texList.size(); i++)
	{
		
		float size = res;
		float gapx = 5.0f*(i%div);
		float gapy = 5.0f*((i/div));
		float dispY = (i / div) * (size);
		ImVec2 pos= ImGui::GetCursorScreenPos();
		float x = pos.x + (size)*((i)%div);
		float y = pos.y + dispY;
		unsigned int tex = texList[i];
		ImGui::GetWindowDrawList()->AddImage(
			(void*)tex,
			ImVec2(ImVec2(x+gapx,y+gapy)),
			ImVec2(x+size+gapx ,
				y+size+gapy), ImVec2(0, 1), ImVec2(1, 0));
	}
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

