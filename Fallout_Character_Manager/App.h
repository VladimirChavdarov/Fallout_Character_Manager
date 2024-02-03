#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"
#include "Character.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>

#ifdef _WIN32
#include <direct.h> // for _mkdir
#elif __linux__ || __APPLE__
#include <sys/stat.h> // for mkdir
#include <unistd.h>   // for access
#endif

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
	void ExtractLeveling();
	void ExtractArmors();
	void ExtractArmorsUpgrades();
	void ExtractWeaponsProperties();
	void ExtractWeapons();
	void ExtractWeaponsUpgrades();
	void ExtractAmmoProperties();
	void ExtractAmmo();
	void ExtractExplosivesProperties();
	void ExtractExplosives();
	void ExtractGear();
	void ExtractFoodsDrinksProps();
	void ExtractFoodsDrinks();
	void ExtractMedicine();
	void ExtractChems();
	void ExtractRads();
	void ExtractExhaustion();
	void ExtractThirst();
	void ExtractHunger();
	void ExtractDecay();
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
	void CatalogueWindow();
	void InfoWindow();
	void InventoryWindow();
	
	// Save/Load
	void LoadFromTSV();
	void SaveToTSV();


	// helper functions

	bool CreateDirectory(const string& path);

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

	template <typename Container>
	bool DisplayList(
		const ImVec2& pos,
		const ImVec2& size,
		const string& name,
		const Container& list,
		string& selected_item);
	template <typename Container, typename ItemAddTo>
	bool DisplayList(
		const ImVec2& pos,
		const ImVec2& size,
		const string& name,
		const Container& list,
		string& selected_item,
		ItemAddTo& add_to);
	template <typename Container, typename ItemAddTo>
	bool DisplayList(
		const ImVec2& pos,
		const ImVec2& size,
		const string& name,
		const Container& list,
		string& selected_item,
		vector<pair<string, ItemAddTo>>& add_to);

	template <typename Container>
	bool DisplayListInventory(
		const ImVec2& pos,
		const ImVec2& size,
		const string& name,
		const Container& list,
		string& selected_item);


	void ReloadCharacterFromBaseStats();



private:
	Character m_character;
	texture m_tex;

	// catalogue stuff
	enum Categories
	{
		armor = 1, // armor
		armor_upgrades, // armor_upg
		melee_weapons_props, // weapon_props
		ranged_weapons_props, // weapon_props
		weapons, // weapons
		melee_weapons_upgrades, // weapon_upg
		ranged_weapons_upgrades, // weapon_upg
		ammo_props,
		ammo,
		explosives_props,
		explosives_thrown,
		explosives_placed,
		gear,
		food_drinks_props,
		food_drinks,
		medicine_items,
		chems,
		decay,
		rads,
		exhaustion,
		thirst,
		hunger,
		junk // this is used only for player inventory. TODO: implement junk table
	};
	int m_selected_list = 1;
	string m_selected_name = "";
	int m_list_id_of_selected_name = 0;
	// False for catalogue. True for inventory.
	bool catalogue_or_inventory = false;

	bool show_manual_modifiers = false;
};

