#pragma once
#include"Core/Scene.h"

class ProjectGenerator
{
private:
	Scene& m_scene;
	std::string akame_root;
	std::string solution_root;
	std::string solution_name;
	bool m_show_window;

public:
	AKAME_API void show_window();
	AKAME_API void hide_window();
	AKAME_API ProjectGenerator(Scene &scene);
	AKAME_API void Draw();

};