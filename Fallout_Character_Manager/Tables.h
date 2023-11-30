#pragma once
#include <vector>
#include <string>
#include <map>

using namespace std;

namespace tbl
{
	struct level
	{
		int flat_hp;
		int end_mul;
		int flat_sp;
		int agi_mul;
	};

	struct armor_upg
	{
		int cost;
		string ranks[3] = { "" };
	};

	struct armor
	{
		int cost = 0;
		int ac = 0;
		int dt = 0;
		int upg_slots = 0;
		int load = 0;
		int str_req = 0;

		int decay_level = 0;
		bool equipped = false;
		vector<armor_upg> upgrades;
	};
	static map<string, tbl::armor> armors;


	struct weapon_props
	{
		string description;
	};

	struct weapon_upg
	{
		string description;
		string req; //which weapons can have this mod
	};

	struct weapon
	{
		int cost;
		int ap;
		string dmg;
		string range;
		string crit;
		string ammo;
		int clip_size;
		vector<string> props_keywords;
		vector<weapon_props> props; //TODO
		float load;
		int str_req;

		int decay_level = 0;
		bool equipped = false;
		vector<weapon_upg> upgrades;
	};
	static map<string, tbl::weapon> weapons;


	struct explosive_props
	{
		string description;
	};

	struct explosive
	{
		int cost;
		int ap;
		string dmg;
		string range_or_arm_dc;
		string aoe;
		//string props;
		vector<explosive_props> props;
		float load;
	};

	struct misc // gear, food/drinks, medicine, chems
	{
		int cost;
		string props;
		float load;
	};

	struct decay
	{
		string level;
		string armor_effect;
		string weapon_effect;
		int cost_reduction_percentage;
	};

	struct rads
	{
		int level;
		string event;
	};

	struct exhaustion
	{
		int level;
		string event;
	};

	struct thirst
	{
		int level;
		string event;
	};

	struct hunger
	{
		int level;
		string event;
	};

	struct limb_condition
	{
		int ap = 0;
		string effect = "";
		string condition = "";
	};
	static map<string, tbl::limb_condition> limb_conditions;
}