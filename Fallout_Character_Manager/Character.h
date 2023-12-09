#pragma once
#include "Tables.h"

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

public:
	string filename = "";

	// bio
	string name = "...";
	int level = 0;
	string race = "...";
	string background = "...";

	// main stats
	int special[7] = { 0 };
	int special_mods[7] = { 0 };
	int hp = 10; // health points
	int sp = 10; // stamina points
	int ac = 10; // armor class
	int dt = 0; // damage threshold
	int healing_rate = 0;
	int ap = 10; // action points

	// secondary stats
	int rad_dc = 0; // radiation difficulty class
	float carry_load = 0;
	int combat_seq = 0; // combat sequence bonus

	// passives
	int passive_sense = 0; // passive sense (perception)
	int party_nerve = 0;
	int party_luck = 0;
	int party_sneak = 0;

	// skills
	int skill[16] = { 0 };
	bool tags[16] = { false };

	// survival stats
	int hunger = 0;
	int thirst = 0;
	int exhaustion = 0;
	int rads = 0;
	int fatigue = 0;

	// resources
	int caps = 0;
	vector<pair<string, tbl::armor>> armors; //not a map cuz you may have more of the same weapon (Askata s 40 kopiq be like)
	string selected_armor = "";
	vector<pair<string, tbl::weapon>> weapons; //not a map cuz you may have more of the same weapon (Askata s 40 kopiq be like)
	string selected_weapon = "";

	// gameplay
	map<string, tbl::limb_condition> head_conditions;
	map<string, tbl::limb_condition> eye_conditions;
	map<string, tbl::limb_condition> arms_conditions;
	map<string, tbl::limb_condition> torso_conditions;
	map<string, tbl::limb_condition> groin_conditions;
	map<string, tbl::limb_condition> legs_conditions;
	vector<pair<string, string>> traits_perks;

	// stuff for testing
	
};

