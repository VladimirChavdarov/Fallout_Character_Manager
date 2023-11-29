#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"
#include "Character.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

struct texture
{
	GLuint image_texture;
	int width, height;
};

class App
{
public:
	App() = default;
	~App() = default;

	void Init();
	void Run();
	void Shutdown();

	void SetTexture(GLuint* img_tex, int* w, int* h)
	{
		m_tex.image_texture = *img_tex;
		m_tex.width = *w;
		m_tex.height = *h;
	}
	

private:
	// Init functions
	void ExtractConditions();

	// Run functions
	void BioWindow();
	void MainParamsWindow();
	void PassiveParamsWindow();
	void SurvivalParamsWindow();
	void SkillWindow();
	void ConditionsWindow();

	void Render();

	// helper functions
	bool DisplayComboBox(
		const ImVec2& pos,
		const int width,
		const string& name,
		const map<string, tbl::limb_condition>& condition_list, 
		string& selected_cond
	);

private:
	Character m_character;
	texture m_tex;
};

