#pragma once
#include <vector>
#include <iterator>
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
	// Note: The key is a string for consistency with everything else. If you want to use it as an int, convert it to int on the spot.
	extern map<string, tbl::level> levels;

	struct armor_upg
	{
		int cost = 0;
		string rank_description[3] = { "" };
	};
	extern map<string, tbl::armor_upg> armors_upgrades;

	struct armor
	{
		int cost = 0;
		int ac = 0;
		int dt = 0;
		int upg_slots = 0;
		float load = 0.0f;
		int str_req = 0;

		int decay_level = 0;
		bool equipped = false;
		map<string, tbl::armor_upg> upgrades;
	};
	extern map<string, tbl::armor> armors;


	struct weapon_prop
	{
		string description;
	};
	extern map<string, tbl::weapon_prop> melee_weapons_props;
	extern map<string, tbl::weapon_prop> ranged_weapons_props;

	struct weapon_upg
	{
		string description;
		string req; //which weapons can have this mod
	};
	extern map<string, tbl::weapon_upg> melee_weapons_upgrades;
	extern map<string, tbl::weapon_upg> ranged_weapons_upgrades;

	struct weapon
	{
		int cost = 0;
		int ap = 0;
		string dmg = "";
		string range = "";
		string crit = "";
		string ammo = "";
		int clip_size = 0;
		map<string, weapon_prop> props;
		//vector<string> props_keywords;
		//vector<weapon_props> props; //TODO
		float load = 0.0f;
		int str_req = 0;

		int decay_level = 0;
		bool equipped = false;
		map<string, weapon_upg> upgrades;
	};
	extern map<string, tbl::weapon> weapons;

	struct ammo_prop
	{
		string description;
		string req;
	};
	extern map<string, tbl::ammo_prop> ammos_props;

	struct ammo
	{
		int amount = 1; // used for character inventory

		int cost = 0; // per bullet
		int pack_size = 0;
		int quantity = 0; // I honestly forgot what this was for.
		float load = 1.0f; // load of a pack if pack size isn't 1
	};
	extern map<string, tbl::ammo> ammos;

	struct explosive_prop
	{
		string description;
	};
	extern map<string, tbl::explosive_prop> explosives_props;

	struct explosive
	{
		int amount = 1; // used for character inventory

		int cost;
		int ap;
		string dmg;
		string range_or_arm_dc;
		string aoe;
		map<string, explosive_prop> props;
		float load;
	};
	extern map<string, tbl::explosive> explosives_thrown;
	extern map<string, tbl::explosive> explosives_placed;

	struct food_drink_prop
	{
		string description;
	};
	extern map<string, tbl::food_drink_prop> foods_drinks_props;

	struct food_drink
	{
		int amount = 1; // used for character inventory

		int cost;
		map<string, food_drink_prop> props;
		float load;
	};
	extern map<string, tbl::food_drink> foods_drinks;

	struct misc // gear, medicine, chems, junk
	{
		int amount = 1; // used for character inventory

		int cost;
		string description;
		float load;
	};
	extern map<string, tbl::misc> gear;
	extern map<string, tbl::misc> junk; // no table for that yet
	extern map<string, tbl::misc> medicine;
	extern map<string, tbl::misc> chems;

	struct decay
	{
		int level;
		string armor_effect;
		string weapon_effect;
		int cost_reduction_percentage;
	};
	extern map<string, tbl::decay> decay_levels;

	struct rads
	{
		int level;
		string event;
	};
	extern map<string, tbl::rads> rads_levels;

	struct exhaustion
	{
		int level;
		string event;
	};
	extern map<string, tbl::exhaustion> exhaustion_levels;

	struct thirst
	{
		int level;
		string event;
	};
	extern map<string, tbl::thirst> thirst_levels;

	struct hunger
	{
		int level;
		string event;
	};
	extern map<string, tbl::hunger> hunger_levels;

	struct limb_condition
	{
		int ap = 0;
		string effect = "";
		string condition = "";
	};
	extern map<string, tbl::limb_condition> limb_conditions;
}