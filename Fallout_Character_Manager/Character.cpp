#include "Character.h"
#include <iostream>
#include <fstream>

using namespace std;

template <typename Container>
float GetItemLoadFromCategory(const Container& category);

void Character::FindLimbMap(const string& selected_cond, map<string, tbl::limb_condition>*& character_body_part)
{
    size_t found = selected_cond.find("Head");
    if (found != string::npos)
        character_body_part = &head_conditions;
    //character_body_part = make_shared< map<string, tbl::limb_condition>>(m_character.head_conditions);
    found = selected_cond.find("Eyes");
    if (found != string::npos)
        character_body_part = &eye_conditions;
    found = selected_cond.find("Arms");
    if (found != string::npos)
        character_body_part = &arms_conditions;
    found = selected_cond.find("Torso");
    if (found != string::npos)
        character_body_part = &torso_conditions;
    found = selected_cond.find("Groin");
    if (found != string::npos)
        character_body_part = &groin_conditions;
    found = selected_cond.find("Legs");
    if (found != string::npos)
        character_body_part = &legs_conditions;
}

// set character filename in filename.txt
void Character::SetCharacterFilename(const string& new_filename)
{
    m_filename = new_filename;
    ofstream f_filename("../Fallout_Character_Manager/characters/filename.txt");
    if (!f_filename.is_open())
    {
        cout << "Can't open file: ../Fallout_Character_Manager/characters/filename.txt" << endl;
    }
    f_filename.write(m_filename.c_str(), m_filename.size());
}

// get character filename from filename.txt
void Character::GetCharacterFilename()
{
    ifstream f_filename("../Fallout_Character_Manager/characters/filename.txt");
    if (!f_filename.is_open())
    {
        cout << "Can't open file: ../Fallout_Character_Manager/characters/filename.txt" << endl;
    }
    string line = "";
    while (getline(f_filename, line));
    {
        m_filename = line;
        cout << "New filename set to: " << m_filename << endl;
    }
}

void Character::SetCharacterDir()
{
    m_dir = "../Fallout_Character_Manager/characters/";
    m_dir += m_filename + "/";
}

void Character::ResetSelected()
{
    selected_armor = "";
    armor_index = -1;
    selected_weapon = "";
    weapon_index = -1;
    selected_item = "";
    item_index = -1;
    item_category = 0;
}

void Character::CalculateHPSP()
{
    string str_level = to_string(level);
    auto it = tbl::levels.find(str_level);
    if (it != tbl::levels.end())
    {
        auto lvl = it->second;
        hp = lvl.flat_hp + special_mods[endu] * lvl.end_mul + hp_modifier;
        sp = lvl.flat_sp + special_mods[agi] * lvl.agi_mul + sp_modifier;
    }
}

void Character::CalculateCarryLoad()
{
    carry_capacity = static_cast<float>(special[str] * 10.0f);
    carry_load = 0.0f;

    // armor
    for (auto& a : armors)
    {
        carry_load += a.second.load;
    }
    // weapons
    for (auto& w : weapons)
    {
        carry_load += w.second.load;
    }
    // other items
    carry_load += GetItemLoadFromCategory(ammos);
    carry_load += GetItemLoadFromCategory(explosives);
    carry_load += GetItemLoadFromCategory(foods_drinks);
    carry_load += GetItemLoadFromCategory(gear);
    carry_load += GetItemLoadFromCategory(medicine);
    carry_load += GetItemLoadFromCategory(chems);
    carry_load += GetItemLoadFromCategory(junk);
}

template <typename Container>
float GetItemLoadFromCategory(const Container& category)
{
    float load = 0;
    for (auto& item : category)
    {
        load += item.second.load * item.second.amount;
    }
    return load;
}