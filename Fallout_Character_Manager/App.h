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
	void Render();

	void SetTexture(GLuint* img_tex, int* w, int* h)
	{
		m_tex.image_texture = *img_tex;
		m_tex.width = *w;
		m_tex.height = *h;
	}
	

private:
	// Init functions
	void ExtractArmors();
	void ExtractWeapons();
	void ExtractConditions();

	// Run functions
	void MenuBar();
	void BioWindow();
	void MainParamsWindow();
	void PassiveParamsWindow();
	void SurvivalParamsWindow();
	void SkillWindow();
	void ConditionsWindow();
	void PerkWindow();
	void EquippedWindow();
	
	// Save/Load
	void LoadFromTSV();
	void SaveToTSV();


	// helper functions
	
	// chatGPT sa izkora sa taq templatenata funkciq
	//template <template <typename, typename...> class Container, typename T>
	template <typename Container>
	bool DisplayComboBox(
		const ImVec2& pos,
		const int width,
		const string& name,
		const Container& list, 
		string& selected_item
	);

	void ReloadCharacterFromBaseStats();

private:
	Character m_character;
	texture m_tex;
};

