#include "App.h"

#include <fstream>
#include <sstream>

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

void App::Init()
{
    ExtractConditions();
}

void App::Run()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    BioWindow();
    MainParamsWindow();
    PassiveParamsWindow();
    SurvivalParamsWindow();
    SkillWindow();
    ConditionsWindow();
    PerkWindow();

	// Render All Windows
	Render();
}

// ---- RUN FUNCTIONS ----

void App::BioWindow()
{
    // Bio Window
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 160), ImGuiCond_Once);
    ImGui::Begin("Character Bio");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::Text("Name: ");
    ImGui::SameLine();
    ImGui::InputText("##Name", &m_character.name);
    ImGui::Text("Race: ");
    ImGui::SameLine();
    ImGui::InputText("##Race", &m_character.race);
    ImGui::Text("Background: ");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    ImGui::InputText("##Background", &m_character.background);
    ImGui::Text("Level: ");
    ImGui::SameLine();
    ImGui::InputInt("##level", &m_character.level, 0);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::MainParamsWindow()
{
    ImGui::SetNextWindowPos(ImVec2(320, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 400), ImGuiCond_Once);
    ImGui::Begin("S.P.E.C.I.A.L.");
    ImGui::SetWindowFontScale(4.0f);

    // SPECIAL letters
    {
        ImGui::SetCursorPos(ImVec2(50, 50));
        ImGui::Text("S");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(150, 50));
        ImGui::Text("P");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(250, 50));
        ImGui::Text("E");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(350, 50));
        ImGui::Text("C");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(450, 50));
        ImGui::Text("I");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(550, 50));
        ImGui::Text("A");
        ImGui::SameLine();
        ImGui::SetCursorPos(ImVec2(650, 50));
        ImGui::Text("L");
    }

    // ability scores
    {
        ImGui::SetWindowFontScale(3.0f);
        ImGui::SetCursorPos(ImVec2(40, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Strength", &m_character.special[str], 0))
        {
            m_character.special_mods[str] = m_character.special[str] - 5;
            m_character.carry_load = static_cast<float>(m_character.special[str] * 10);
            // melee
            m_character.skill[melee_weapons] = m_character.special_mods[str];
            // unarmed
            if(m_character.special_mods[str] > m_character.special_mods[agi])
                m_character.skill[unarmed] = m_character.special_mods[str];
            else
                m_character.skill[unarmed] = m_character.special_mods[agi];
            // intimidation
            if (m_character.special_mods[str] > m_character.special_mods[cha])
                m_character.skill[intimidation] = m_character.special_mods[str];
            else
                m_character.skill[intimidation] = m_character.special_mods[cha];
        }
        ImGui::SetCursorPos(ImVec2(140, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Perception", &m_character.special[per], 0))
        {
            m_character.special_mods[per] = m_character.special[per] - 5;
            m_character.passive_sense = 12 + m_character.special_mods[per];
            m_character.combat_seq = m_character.special_mods[per];
            // energy wpns
            m_character.skill[energy_weapons] = m_character.special_mods[per];
            // explosives
            m_character.skill[explosives] = m_character.special_mods[per];
            // medicine
            if (m_character.special_mods[per] > m_character.special_mods[inte])
                m_character.skill[medicine] = m_character.special_mods[per];
            else
                m_character.skill[medicine] = m_character.special_mods[inte];
            // lockpick
            m_character.skill[lockpick] = m_character.special_mods[per];
        }
        ImGui::SetCursorPos(ImVec2(240, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Endurance", &m_character.special[endu], 0))
        {
            m_character.special_mods[endu] = m_character.special[endu] - 5;
            m_character.rad_dc = 12 - m_character.special_mods[endu];
            if (m_character.race != "Robot" && m_character.race != "robot" && m_character.race != "Synth" && m_character.race != "synth")
                m_character.healing_rate = (m_character.level + m_character.special_mods[endu]) / 2;
            // survival
            if (m_character.special_mods[endu] > m_character.special_mods[inte])
                m_character.skill[survival] = m_character.special_mods[endu];
            else
                m_character.skill[survival] = m_character.special_mods[inte];
        }
        ImGui::SetCursorPos(ImVec2(340, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Charisma", &m_character.special[cha], 0))
        {
            m_character.special_mods[cha] = m_character.special[cha] - 5;
            // barter
            m_character.skill[barter] = m_character.special_mods[cha];
            // persuasion
            m_character.skill[persuasion] = m_character.special_mods[cha];
            // deception
            m_character.skill[deception] = m_character.special_mods[cha];
            // intimidation
            if (m_character.special_mods[cha] < m_character.special_mods[str])
                m_character.skill[intimidation] = m_character.special_mods[cha];
            else
                m_character.skill[intimidation] = m_character.special_mods[str];
        }
        ImGui::SetCursorPos(ImVec2(440, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Intelligence", &m_character.special[inte], 0))
        {
            m_character.special_mods[inte] = m_character.special[inte] - 5;
            if (m_character.race == "Robot" || m_character.race == "robot" || m_character.race == "Synth" || m_character.race == "synth")
                m_character.healing_rate = (m_character.level + m_character.special_mods[inte]) / 2;
            // medicine
            if (m_character.special_mods[inte] < m_character.special_mods[per])
                m_character.skill[medicine] = m_character.special_mods[inte];
            else
                m_character.skill[medicine] = m_character.special_mods[per];
            // hacking
            m_character.skill[hacking] = m_character.special_mods[inte];
            // crafting
            m_character.skill[crafting] = m_character.special_mods[inte];
            // science
            m_character.skill[science] = m_character.special_mods[inte];
            // survival
            if (m_character.special_mods[inte] < m_character.special_mods[endu])
                m_character.skill[survival] = m_character.special_mods[inte];
            else
                m_character.skill[survival] = m_character.special_mods[endu];
        }
        ImGui::SetCursorPos(ImVec2(540, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Agility", &m_character.special[agi], 0))
        {
            m_character.special_mods[agi] = m_character.special[agi] - 5;
            m_character.ap = 10 + m_character.special_mods[agi];
            // guns
            m_character.skill[guns] = m_character.special_mods[agi];
            // unarmed
            if (m_character.special_mods[str] < m_character.special_mods[agi])
                m_character.skill[unarmed] = m_character.special_mods[agi];
            else
                m_character.skill[unarmed] = m_character.special_mods[str];
        }
        ImGui::SetCursorPos(ImVec2(640, 110));
        ImGui::SetNextItemWidth(50);
        if (ImGui::InputInt("##Luck", &m_character.special[lck], 0))
        {
            m_character.special_mods[lck] = m_character.special[lck] - 5;
            int quarter_luck_mod = m_character.special_mods[lck] / 4;
            if (quarter_luck_mod < -1)
                quarter_luck_mod = -1;
            for (auto& skill : m_character.skill)
            {
                skill += quarter_luck_mod;
            }
        }
    }
    
    // modifiers
    {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetCursorPos(ImVec2(50, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##StrhMod", &m_character.special_mods[str], 0, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::SetCursorPos(ImVec2(150, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##PerMod", &m_character.special_mods[per], 0, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::SetCursorPos(ImVec2(250, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##EndMod", &m_character.special_mods[endu], 0, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::SetCursorPos(ImVec2(350, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##ChaMod", &m_character.special_mods[cha], 0, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::SetCursorPos(ImVec2(450, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##IntMod", &m_character.special_mods[inte], 0, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::SetCursorPos(ImVec2(550, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##AgiMod", &m_character.special_mods[agi], 0, 100, ImGuiInputTextFlags_ReadOnly);
        ImGui::SetCursorPos(ImVec2(650, 170));
        ImGui::SetNextItemWidth(30);
        ImGui::InputInt("##LckMod", &m_character.special_mods[lck], 0, 100, ImGuiInputTextFlags_ReadOnly);
}

    // sp and hp
    {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetCursorPos(ImVec2(20, 300));
        ImGui::Text("Stamina Points:");
        ImGui::SetNextItemWidth(40);
        ImGui::SameLine();
        //ImGui::SetCursorPos(ImVec2(180, 300));
        ImGui::InputInt("##SP", &m_character.sp, 0);
        ImGui::SetCursorPos(ImVec2(60, 350));
        ImGui::Text("Hit Points:");
        ImGui::SetNextItemWidth(40);
        //ImGui::SetCursorPos(ImVec2(180, 350));
        ImGui::SameLine();
        ImGui::InputInt("##HP", &m_character.hp, 0);
    }

    // ac and dt
    {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetCursorPos(ImVec2(280, 300));
        ImGui::Text("Armor Class:");
        ImGui::SetNextItemWidth(30);
        ImGui::SameLine();
        //ImGui::SetCursorPos(ImVec2(180, 300));
        ImGui::InputInt("##AC", &m_character.ac, 0);
        ImGui::SetCursorPos(ImVec2(280, 350));
        ImGui::Text("Damage Threshold:");
        ImGui::SetNextItemWidth(30);
        ImGui::SameLine();
        //ImGui::SetCursorPos(ImVec2(180, 350));
        ImGui::InputInt("##DT", &m_character.dt, 0);
    }

    // healing rate and combat sequence
    {
        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetCursorPos(ImVec2(550, 300));
        ImGui::Text("Healing rate:");
        ImGui::SetNextItemWidth(30);
        ImGui::SameLine();
        ImGui::InputInt("##HR", &m_character.healing_rate, 0);
        ImGui::SetCursorPos(ImVec2(550, 350));
        ImGui::Text("Combat Sequence:");
        ImGui::SetNextItemWidth(30);
        ImGui::SameLine();
        ImGui::InputInt("##CS", &m_character.combat_seq, 0);
    }

    // ap
    {
        ImGui::SetCursorPos(ImVec2(40, 250));
        ImGui::Text("Action Points:");
        ImGui::SetNextItemWidth(30);
        ImGui::SameLine();
        ImGui::InputInt("##AP", &m_character.ap, 0);
    }

    // reduce window header font size
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::PassiveParamsWindow()
{
    // Passives Window
    ImGui::SetNextWindowPos(ImVec2(10, 770), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Once);
    ImGui::Begin("Passives");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetCursorPos(ImVec2(10, 30));
    ImGui::Text("Passive Sense:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##PS", &m_character.passive_sense, 0);
    ImGui::SetCursorPos(ImVec2(10, 70));
    ImGui::Text("Party Nerve:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##PN", &m_character.party_nerve, 0);
    ImGui::SetCursorPos(ImVec2(10, 110));
    ImGui::Text("Party Luck:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##PL", &m_character.party_luck, 0);
    ImGui::SetCursorPos(ImVec2(10, 150));
    ImGui::Text("Party Sneak:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##PSN", &m_character.party_sneak, 0);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::SurvivalParamsWindow()
{
    // Survival Window
    ImGui::SetNextWindowPos(ImVec2(10, 440), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_Once);
    ImGui::Begin("Survival");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetCursorPos(ImVec2(10, 30));
    ImGui::Text("Rad DC:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##RDC", &m_character.rad_dc, 0);
    ImGui::SetCursorPos(ImVec2(10, 90));
    ImGui::Text("Rads:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##RAD", &m_character.rads, 0);
    ImGui::SetCursorPos(ImVec2(10, 130));
    ImGui::Text("Fatigue:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##FT", &m_character.fatigue, 0);
    ImGui::SetCursorPos(ImVec2(10, 170));
    ImGui::Text("Thirst:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##TS", &m_character.thirst, 0);
    ImGui::SetCursorPos(ImVec2(10, 210));
    ImGui::Text("Hunger:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##HG", &m_character.hunger, 0);
    ImGui::SetCursorPos(ImVec2(10, 250));
    ImGui::Text("Exhaustion:");
    ImGui::SetNextItemWidth(30);
    ImGui::SameLine();
    ImGui::InputInt("##EXH", &m_character.exhaustion, 0);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::SkillWindow()
{
    int y_offset = 35;
    int mul = 0;
    // Skill Window
    ImGui::SetNextWindowPos(ImVec2(1140, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(350, 600), ImGuiCond_Once);
    ImGui::Begin("Skills");
    ImGui::SetWindowFontScale(1.5f);
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##GUN", &m_character.skill[guns], 0);
    ImGui::SameLine();
    ImGui::Text("Guns (AGI)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##GUNC", &m_character.tags[guns]))
    {
        if (m_character.tags[guns]) { m_character.skill[guns] += 2; }
        else { m_character.skill[guns] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##ENW", &m_character.skill[energy_weapons], 0);
    ImGui::SameLine();
    ImGui::Text("Energy Weapons (PER)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##ENWC", &m_character.tags[energy_weapons]))
    {
        if (m_character.tags[energy_weapons]) { m_character.skill[energy_weapons] += 2; }
        else { m_character.skill[energy_weapons] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##MLW", &m_character.skill[melee_weapons], 0);
    ImGui::SameLine();
    ImGui::Text("Melee Weapons (STR)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##MLWC", &m_character.tags[melee_weapons]))
    {
        if (m_character.tags[melee_weapons]) { m_character.skill[melee_weapons] += 2; }
        else { m_character.skill[melee_weapons] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##UMD", &m_character.skill[unarmed], 0);
    ImGui::SameLine();
    ImGui::Text("Unarmed (STR or AGI)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##UMDC", &m_character.tags[unarmed]))
    {
        if (m_character.tags[unarmed]) { m_character.skill[unarmed] += 2; }
        else { m_character.skill[unarmed] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##EXP", &m_character.skill[explosives], 0);
    ImGui::SameLine();
    ImGui::Text("Explosives (PER)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##EXPC", &m_character.tags[explosives]))
    {
        if (m_character.tags[explosives]) { m_character.skill[explosives] += 2; }
        else { m_character.skill[explosives] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##MED", &m_character.skill[medicine], 0);
    ImGui::SameLine();
    ImGui::Text("Medicine (PER or INT)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##MEDC", &m_character.tags[medicine]))
    {
        if (m_character.tags[medicine]) { m_character.skill[medicine] += 2; }
        else { m_character.skill[medicine] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##LPK", &m_character.skill[lockpick], 0);
    ImGui::SameLine();
    ImGui::Text("Lockpick (PER)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##LPKC", &m_character.tags[lockpick]))
    {
        if (m_character.tags[lockpick]) { m_character.skill[lockpick] += 2; }
        else { m_character.skill[lockpick] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##HCK", &m_character.skill[hacking], 0);
    ImGui::SameLine();
    ImGui::Text("Hacking (INT)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##HCKC", &m_character.tags[hacking]))
    {
        if (m_character.tags[hacking]) { m_character.skill[hacking] += 2; }
        else { m_character.skill[hacking] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##CFT", &m_character.skill[crafting], 0);
    ImGui::SameLine();
    ImGui::Text("Crafting (INT)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##CFTC", &m_character.tags[crafting]))
    {
        if (m_character.tags[crafting]) { m_character.skill[crafting] += 2; }
        else { m_character.skill[crafting] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##SCI", &m_character.skill[science], 0);
    ImGui::SameLine();
    ImGui::Text("Science (INT)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##SCIC", &m_character.tags[science]))
    {
        if (m_character.tags[science]) { m_character.skill[science] += 2; }
        else { m_character.skill[science] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##SNK", &m_character.skill[sneak], 0);
    ImGui::SameLine();
    ImGui::Text("Sneak (AGI)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##SNKC", &m_character.tags[sneak]))
    {
        if (m_character.tags[sneak]) { m_character.skill[sneak] += 2; }
        else { m_character.skill[sneak] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##SUR", &m_character.skill[survival], 0);
    ImGui::SameLine();
    ImGui::Text("Survival (INT or END)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##SURC", &m_character.tags[survival]))
    {
        if (m_character.tags[survival]) { m_character.skill[survival] += 2; }
        else { m_character.skill[survival] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##BRT", &m_character.skill[barter], 0);
    ImGui::SameLine();
    ImGui::Text("Barter (CHA)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##BRTC", &m_character.tags[barter]))
    {
        if (m_character.tags[barter]) { m_character.skill[barter] += 2; }
        else { m_character.skill[barter] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##PRS", &m_character.skill[persuasion], 0);
    ImGui::SameLine();
    ImGui::Text("Persuasion (CHA)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##PRSC", &m_character.tags[persuasion]))
    {
        if (m_character.tags[persuasion]) { m_character.skill[persuasion] += 2; }
        else { m_character.skill[persuasion] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##DEC", &m_character.skill[deception], 0);
    ImGui::SameLine();
    ImGui::Text("Deception (CHA)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##DECC", &m_character.tags[deception]))
    {
        if (m_character.tags[deception]) { m_character.skill[deception] += 2; }
        else { m_character.skill[deception] -= 2; }
    }
    mul++;
    //
    ImGui::SetCursorPos(ImVec2(10, 30 + y_offset * mul));
    ImGui::SetNextItemWidth(30);
    ImGui::InputInt("##ITM", &m_character.skill[intimidation], 0);
    ImGui::SameLine();
    ImGui::Text("Intimidation (STR or CHA)");
    ImGui::SameLine();
    if (ImGui::Checkbox("##ITMC", &m_character.tags[intimidation]))
    {
        if (m_character.tags[intimidation]) { m_character.skill[intimidation] += 2; }
        else { m_character.skill[intimidation] -= 2; }
    }
    mul++;

    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::ConditionsWindow()
{
    // Conditions Window
    ImGui::SetNextWindowPos(ImVec2(1500, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);
    ImGui::Begin("Conditions");
    ImGui::SetCursorPos(ImVec2(140, 120));
    ImGui::Image((void*)(intptr_t)m_tex.image_texture, ImVec2(m_tex.width / 3, m_tex.height / 3));
    static string selected_cond = "";
    DisplayComboBox(ImVec2(240, 80), 70, "Head", m_character.head_conditions, selected_cond);
    DisplayComboBox(ImVec2(100, 100), 70, "Eyes", m_character.eye_conditions, selected_cond);
    DisplayComboBox(ImVec2(40, 160), 70, "Arms", m_character.arms_conditions, selected_cond);
    DisplayComboBox(ImVec2(80, 240), 70, "Torso", m_character.torso_conditions, selected_cond);
    DisplayComboBox(ImVec2(260, 240), 70, "Groin", m_character.groin_conditions, selected_cond);
    DisplayComboBox(ImVec2(200, 320), 70, "Legs", m_character.legs_conditions, selected_cond);
    map<string, tbl::limb_condition>* character_body_part = nullptr;
    if (DisplayComboBox(ImVec2(20, 360), 120, "Add Condition", tbl::limb_conditions, selected_cond))
    {
        m_character.FindLimbMap(selected_cond, character_body_part);
        if (character_body_part != nullptr)
        {
            auto it = tbl::limb_conditions.find(selected_cond);
            if (it != tbl::limb_conditions.end())
            {
                string key = it->first;
                tbl::limb_condition value = it->second;
                character_body_part->emplace(key, value);
                cout << selected_cond << " added." << endl;
                cout << "New size of body part map: " << character_body_part->size() << endl;
            }
            else
                cout << "da mu eba maikata" << endl;
        }
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(1500, 410), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_Once);
    ImGui::Begin("Condition Description");
    ImGui::TextWrapped(selected_cond.c_str());
    auto it = tbl::limb_conditions.find(selected_cond);
    if (it != tbl::limb_conditions.end())
    {
        string cost = "Cost: " + to_string(it->second.ap) + " AP";
        ImGui::TextWrapped(cost.c_str());
        string effect = "Effect: " + it->second.effect;
        ImGui::TextWrapped(effect.c_str());
        string condition = "Condition: " + it->second.condition;
        ImGui::TextWrapped(condition.c_str());
    }
    if (ImGui::Button("Remove"))
    {
        m_character.FindLimbMap(selected_cond, character_body_part);
        if (character_body_part != nullptr)
        {
            auto it_remove = character_body_part->find(selected_cond);
            if (it_remove != character_body_part->end())
            {
                character_body_part->erase(it_remove);
            }
            selected_cond = "";
        }
    }
    ImGui::End();
}

void App::PerkWindow()
{
    // Perk Window
    ImGui::SetNextWindowPos(ImVec2(1500, 630), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 340), ImGuiCond_Once);
    ImGui::Begin("Traits and Perks");
    if (ImGui::Button("Add", ImVec2(50, 25)))
    {
        pair<string, string> new_perk = { "My new perk", "My new perk's description" };
        m_character.traits_perks.push_back(new_perk);
    }
    ImGui::SameLine();
    ImGui::SetWindowFontScale(0.9f);
    ImGui::TextWrapped("Hit Enter to save a perk's title (description works fine). Tab or clicking sth else will reset.");
    ImGui::SetWindowFontScale(1.0f);
    // it's a bit annoying to fill many perks
    for (int i = 0; i < m_character.traits_perks.size(); i++)
    {
        auto& perk = m_character.traits_perks[i];
        string tagH = perk.first;
        if(ImGui::CollapsingHeader(tagH.c_str()))
        {
            string old_title = perk.first;
            string tagT = "##PerkTitle" + to_string(i);
            if (ImGui::InputText(tagT.c_str(), &old_title, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                perk.first = old_title;
            }
            ImGui::IsItemDeactivatedAfterEdit();
            string tagD = "##PerkDescription" + to_string(i);
            if(ImGui::InputTextMultiline(tagD.c_str(), &perk.second)) {}
        }
    }
    ImGui::End();
}

void App::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void App::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// ---- INIT FUNCTIONS ----

void App::ExtractConditions()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/limb_conditions.txt");
    if (!tsv_file.is_open())
    {
        cout << "Can't open file" << endl;
        return;
    }

    string line;
    bool skip_first = true;
    while (getline(tsv_file, line))
    {
        if (skip_first)
        {
            skip_first = false;
            continue;
        }
        istringstream iss(line);
        vector<string> row;
        string substr;
        while (getline(iss, substr, '\t'))
        {
            row.push_back(substr);
        }
        string key;
        tbl::limb_condition value;
        for (int i = 3; i < row.size(); i++)
        {
            if( i != row.size() - 1)
                key = row[0] + " " + to_string(i-2);
            else
                key = row[0] + " " + "Severe Injury";
            value.ap = stoi(row[1]);
            value.effect = row[2];
            value.condition = row[i];

            tbl::limb_conditions.emplace(key, value);
        }
    }
    // debug
    /*for (auto& pair : tbl::limb_conditions)
    {
        cout << pair.first << endl;
        cout << pair.second.ap << endl;
        cout << pair.second.effect << endl;
        cout << pair.second.condition << endl;
        cout << "-----------------------------------------------" << endl;
    }*/
}

// ---- HELPER FUNCTIONS ---

bool App::DisplayComboBox(
    const ImVec2& pos,
    const int width,
    const string& name,
    const map<string,
    tbl::limb_condition>& condition_list,
    string& selected_cond
)
{
    ImGui::SetCursorPos(pos);
    ImGui::SetNextItemWidth(width);
    bool new_select = false;
    string tag = "##" + name;
    //string combo_preview_value = m_character.arr_str[selected_cond];
    if (ImGui::BeginCombo(tag.c_str(), name.c_str()))
    {
        for (auto& cond : condition_list)
        {
            const bool is_selected = (selected_cond == cond.first);
            if (ImGui::Selectable(cond.first.c_str(), is_selected))
            {
                new_select = true;
                selected_cond = cond.first;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return new_select;
}