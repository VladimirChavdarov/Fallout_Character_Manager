#include "Character.h"

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