#include "Tables.h"

using namespace std;

namespace tbl
{
	map<string, tbl::level> levels;
	map<string, tbl::armor_upg> armors_upgrades;
	map<string, tbl::armor> armors;
	map<string, tbl::weapon_prop> melee_weapons_props;
	map<string, tbl::weapon_prop> ranged_weapons_props;
	map<string, tbl::weapon_upg> melee_weapons_upgrades;
	map<string, tbl::weapon_upg> ranged_weapons_upgrades;
	map<string, tbl::weapon> weapons;
	map<string, tbl::ammo_prop> ammos_props;
	map<string, tbl::ammo> ammos;
	map<string, tbl::explosive_prop> explosives_props;
	map<string, tbl::explosive> explosives_thrown;
	map<string, tbl::explosive> explosives_placed;
	map<string, tbl::food_drink_prop> foods_drinks_props;
	map<string, tbl::food_drink> foods_drinks;
	map<string, tbl::misc> gear;
	map<string, tbl::misc> junk; // no table for that yet
	map<string, tbl::misc> medicine;
	map<string, tbl::misc> chems;
	map<string, tbl::decay> decay_levels;
	map<string, tbl::rads> rads_levels;
	map<string, tbl::exhaustion> exhaustion_levels;
	map<string, tbl::thirst> thirst_levels;
	map<string, tbl::hunger> hunger_levels;
	map<string, tbl::limb_condition> limb_conditions;
}