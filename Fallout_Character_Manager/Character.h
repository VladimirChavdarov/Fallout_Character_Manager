#pragma once
#include "Tables.h"
#include "Utils.h"

enum special
{
	str,
	per,
	endu,
	cha,
	inte,
	agi,
	lck
};

enum skills
{
	guns,
	energy_weapons,
	melee_weapons,
	unarmed,
	explosives,
	medicine,
	lockpick,
	hacking,
	crafting,
	science,
	sneak,
	survival,
	barter,
	persuasion,
	deception,
	intimidation
};

class Character
{
public:
	Character() = default;
	~Character() = default;

	void FindLimbMap(const string& selected_cond, map<string, tbl::limb_condition>*& character_body_part);

	// set character filename in filename.txt
	void SetCharacterFilename(const string& new_filename);
	// get character filename from filename.txt
	void GetCharacterFilename();
	// update the character dir to full dir.
	void SetCharacterDir();

	// resets the member functions for selected weapon/armor/items.
	void ResetSelected();

	void CalculateHPSP();

	// also calculates carry capacity just in case
	void CalculateCarryLoad();

	// delete armor/weapon/item marked for delete in this frame.
	void DeleteItems();

	void DeletePerks();

public:
	string m_filename = ""; // dir + filename to get the full path to the character folder
	string m_dir = "../Fallout_Character_Manager/characters/"; // dir + filename to get the full path to the character folder
	bool autosave = true;
	//bool autoload = true;
	bool default_character = false;

	// bio
	string name = "none";
	int level = 0;
	string race = "...";

	string background = "...";

	// main stats
	int special[7] = { 0 };
	int special_mods[7] = { 0 };
	int hp = 10; // health points
	int hp_modifier = 0; // manual modifier
	int sp = 10; // stamina points
	int sp_modifier = 0; // manual modifier
	int ac = 10; // armor class
	int ac_modifier = 0; // manual modifier
	int dt = 0; // damage threshold
	int dt_modifier = 0; // manual modifier
	int healing_rate = 0;
	int healing_rate_modifier = 0; // manual modifier
	int ap = 10; // action points
	int ap_modifier = 0; // manual modifier

	// secondary stats
	int rad_dc = 0; // radiation difficulty class
	int rad_dc_modifier = 0; // manual modifier
	float carry_load = 0;
	float carry_capacity = 0;
	float carry_load_modifier = 0; // manual modifier
	int combat_seq = 0; // combat sequence bonus
	int combat_seq_modifier = 0; // manual modifier

	// passives
	int passive_sense = 0;
	int passive_sense_modifier = 0; // manual modifier
	int party_nerve = 0;
	int party_luck = 0;
	int party_sneak = 0;

	// skills
	int skill[16] = { 0 };
	int skill_modifiers[16] = { 0 };
	bool tags[16] = { false };

	// survival stats
	int hunger = 0;
	int thirst = 0;
	int exhaustion = 0;
	int rads = 0;
	int fatigue = 0;

	// resources
	int caps = 0;
	vector<pair<string, tbl::armor>> armors; //not a map cuz you may have more of the same armor (Askata s 40 kopiq be like)
	string selected_armor = "";
	int armor_index = -1;
	int armor_index_marked_for_delete = -1;
	vector<pair<string, tbl::weapon>> weapons; //not a map cuz you may have more of the same weapon (Askata s 40 kopiq be like)
	string selected_weapon = "";
	int weapon_index = -1;
	int weapon_index_marked_for_delete = -1;
	vector<pair<string, tbl::ammo>> ammos;
	vector<pair<string, tbl::explosive>> explosives;
	vector<pair<string, tbl::food_drink>> foods_drinks;
	vector<pair<string, tbl::misc>> gear;
	vector<pair<string, tbl::misc>> medicine;
	vector<pair<string, tbl::misc>> chems;
	vector<pair<string, tbl::misc>> junk;
	string selected_item = "";
	int item_index = -1;
	int item_category = 0;
	int item_index_marked_for_delete = -1;

	// gameplay
	map<string, tbl::limb_condition> head_conditions;
	map<string, tbl::limb_condition> eye_conditions;
	map<string, tbl::limb_condition> arms_conditions;
	map<string, tbl::limb_condition> torso_conditions;
	map<string, tbl::limb_condition> groin_conditions;
	map<string, tbl::limb_condition> legs_conditions;
	vector<pair<string, string>> traits_perks;
	int perk_index_marked_for_delete = -1;

	// stuff for testing

};

