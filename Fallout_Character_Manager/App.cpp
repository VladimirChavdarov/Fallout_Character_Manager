#include "App.h"

#include <fstream>
#include <sstream>

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!

void App::Init()
{
    ExtractLeveling();
    ExtractArmorsUpgrades();
    ExtractArmors();
    ExtractWeaponsProperties();
    ExtractWeaponsUpgrades();
    ExtractWeapons();
    ExtractAmmoProperties();
    ExtractAmmo();
    ExtractExplosivesProperties();
    ExtractExplosives();
    ExtractGear();
    ExtractFoodsDrinksProps();
    ExtractFoodsDrinks();
    ExtractMedicine();
    ExtractChems();
    ExtractRads();
    ExtractExhaustion();
    ExtractThirst();
    ExtractHunger();
    ExtractDecay();
    ExtractConditions();

    if (m_character.autoload)
        LoadFromTSV();
}

void App::Run()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    MenuBar();
    BioWindow();
    MainParamsWindow();
    PassiveParamsWindow();
    SurvivalParamsWindow();
    SkillWindow();
    ConditionsWindow();
    PerkWindow();
    EquippedWindow();
    CatalogueWindow();
    InfoWindow();

	// Render All Windows
	//Render();
}

void App::Shutdown()
{
    if (m_character.autosave)
        SaveToTSV();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


// ---- RUN FUNCTIONS ----

void App::MenuBar()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::Button("Save", ImVec2(40, 20)))
    {
        SaveToTSV();
        cout << "File saved!" << endl;
    }
    if (ImGui::Button("Load", ImVec2(40, 20)))
    {
        LoadFromTSV();
        cout << "File loaded" << endl;
    }
    if (ImGui::Checkbox("Autosave", &m_character.autosave))
    {
        
    }
    if (ImGui::Checkbox("Autoload", &m_character.autoload))
    {

    }
    float width = ImGui::CalcTextSize(m_character.filename.c_str()).x;
    ImGui::SetNextItemWidth(width + 30.0f);
    ImGui::InputText("Path", &m_character.filename);
    ImGui::EndMainMenuBar();
}

void App::BioWindow()
{
    // Bio Window
    ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Once);
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
    ImGui::SetNextWindowPos(ImVec2(330, 30), ImGuiCond_Once);
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
            // sneak
            m_character.skill[sneak] = m_character.special_mods[agi];
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
    ImGui::SetNextWindowPos(ImVec2(10, 500), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 190), ImGuiCond_Once);
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
    ImGui::SetNextWindowPos(ImVec2(10, 200), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 290), ImGuiCond_Once);
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
    ImGui::SetNextWindowPos(ImVec2(1140, 30), ImGuiCond_Once);
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
    ImGui::SetNextWindowPos(ImVec2(1500, 30), ImGuiCond_Once);
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

    ImGui::SetNextWindowPos(ImVec2(1500, 430), ImGuiCond_Once);
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
    ImGui::SetNextWindowPos(ImVec2(1500, 640), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(400, 330), ImGuiCond_Once);
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
            if(ImGui::InputText(tagD.c_str(), &perk.second)) {}
            ImGui::TextWrapped(perk.second.c_str());
        }
    }
    ImGui::End();
}

void App::EquippedWindow()
{
    // Invetory for equippable items
    ImGui::SetNextWindowPos(ImVec2(330, 440), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 350), ImGuiCond_Once);
    ImGui::Begin("Equippable Items Inventory");
    ImGui::SetWindowFontScale(1.5f);
    // armor
    ImGui::SeparatorText("Armor");
    static int armor_index = -1; // index of the selected armor
    if (DisplayComboBox(ImVec2(10, 60), 300, m_character.selected_armor, m_character.armors, m_character.selected_armor))
    {
        for (int i = 0; i < m_character.armors.size(); i++)
            if (m_character.selected_armor == m_character.armors[i].first)
                armor_index = i;
    }
    if (armor_index != -1)
    {
        // name
        ImGui::SetCursorPos(ImVec2(10, 100));
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(250);
        ImGui::InputText("##ArmorName", &m_character.armors[armor_index].first);
        // upgrades
        if (DisplayComboBox(ImVec2(600, 60), 180, "Armor Upgrades", m_character.armors[armor_index].second.upgrades, m_character.selected_item))
        {
            m_list_id_of_selected_name = armor_upgrades;
            m_selected_name = m_character.selected_item;
        }
        // equipped
        ImGui::SetCursorPos(ImVec2(10, 130));
        ImGui::Checkbox("Equipped##A", &m_character.armors[armor_index].second.equipped);
        // ac, dt
        m_character.ac = m_character.armors[armor_index].second.ac;
        m_character.dt = m_character.armors[armor_index].second.dt;
        // cost
        ImGui::SetCursorPos(ImVec2(450, 100));
        ImGui::Text("Cost:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(40);
        ImGui::InputInt("##ArmorCost", &m_character.armors[armor_index].second.cost, 0, 100, ImGuiInputTextFlags_ReadOnly);
        // decay
        ImGui::SetCursorPos(ImVec2(450, 135));
        ImGui::Text("Decay:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(90);
        ImGui::InputInt("##ArmorDecay", &m_character.armors[armor_index].second.decay_level);
    }

    // weapons
    ImGui::SeparatorText("Weapons");
    static int weapon_index = -1; // index of the selected armor
    if (DisplayComboBox(ImVec2(10, 200), 300, m_character.selected_weapon, m_character.weapons, m_character.selected_weapon))
    {
        for (int i = 0; i < m_character.weapons.size(); i++)
            if (m_character.selected_weapon == m_character.weapons[i].first)
                weapon_index = i;
    }
    if (weapon_index != -1)
    {
        // name
        ImGui::SetCursorPos(ImVec2(10, 240));
        ImGui::Text("Name:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(250);
        ImGui::InputText("##WeaponName", &m_character.weapons[weapon_index].first);
        // equipped
        ImGui::SetCursorPos(ImVec2(10, 270));
        ImGui::Checkbox("Equipped##W", &m_character.weapons[weapon_index].second.equipped);
        // cost
        ImGui::SetCursorPos(ImVec2(350, 240));
        ImGui::Text("Cost:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(40);
        ImGui::InputInt("##WeaponCost", &m_character.weapons[weapon_index].second.cost, 0, 100, ImGuiInputTextFlags_ReadOnly);
        // decay
        ImGui::SetCursorPos(ImVec2(350, 275));
        ImGui::Text("Decay:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(90);
        ImGui::InputInt("##WeaponDecay", &m_character.weapons[weapon_index].second.decay_level);
        // dmg
        ImGui::SetCursorPos(ImVec2(10, 310));
        ImGui::Text("Damage:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##WeaponDamage", &m_character.weapons[weapon_index].second.dmg);
        // dmg
        ImGui::SetCursorPos(ImVec2(350, 310));
        ImGui::Text("Crit:");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("##WeaponCrit", &m_character.weapons[weapon_index].second.crit);
        // upgrades
        if (DisplayComboBox(ImVec2(600, 200), 180, "Weapon Upgrades", m_character.weapons[weapon_index].second.upgrades, m_character.selected_item))
        {
            for (auto& weapon_upg : tbl::melee_weapons_upgrades)
            {
                if (weapon_upg.first == m_character.selected_item);
                {
                    m_list_id_of_selected_name = melee_weapons_upgrades;
                    break;
                }
            }
            for (auto& weapon_upg : tbl::ranged_weapons_upgrades)
            {
                if (weapon_upg.first == m_character.selected_item);
                {
                    m_list_id_of_selected_name = melee_weapons_upgrades;
                    break;
                }
            }
            m_selected_name = m_character.selected_item;
        }
        // properties
        if (DisplayComboBox(ImVec2(360, 200), 200, "Properties", m_character.weapons[weapon_index].second.props, m_character.selected_item))
        {
            for (auto& weapon_upg : tbl::melee_weapons_props)
            {
                if (weapon_upg.first == m_character.selected_item);
                {
                    m_list_id_of_selected_name = melee_weapons_props;
                    break;
                }
            }
            for (auto& weapon_upg : tbl::ranged_weapons_props)
            {
                if (weapon_upg.first == m_character.selected_item);
                {
                    m_list_id_of_selected_name = melee_weapons_props;
                    break;
                }
            }
            m_selected_name = m_character.selected_item;
        }
    }
    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::CatalogueWindow()
{
    // Catalogue Window
    ImGui::SetNextWindowPos(ImVec2(10, 700), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 270), ImGuiCond_Once);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.0f, 0.1f, 0.9f));
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.3f, 0.0f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.6f, 0.0f, 0.6f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.6f, 0.0f, 0.6f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.0f, 0.5f, 1.0f));
    bool b = false;
    ImGui::Begin("Catalogue", &b, ImGuiWindowFlags_MenuBar);
    //ImGui::Begin("Catalogue");

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Category"))
        {
            if (ImGui::MenuItem("Armor", NULL, !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            if (ImGui::MenuItem("Armor Upgrades", NULL, !(m_selected_list - armor_upgrades)))
            {
                m_selected_list = armor_upgrades;
            }
            if (ImGui::MenuItem("Melee Weapons Properties", NULL, !(m_selected_list - melee_weapons_props)))
            {
                m_selected_list = melee_weapons_props;
            }
            if (ImGui::MenuItem("Ranged Weapons Properties", NULL, !(m_selected_list - ranged_weapons_props)))
            {
                m_selected_list = ranged_weapons_props;
            }
            if (ImGui::MenuItem("Weapons", NULL, !(m_selected_list - weapons)))
            {
                m_selected_list = weapons;
            }
            if (ImGui::MenuItem("Melee Weapons Upgrades", NULL, !(m_selected_list - melee_weapons_upgrades)))
            {
                m_selected_list = melee_weapons_upgrades;
            }
            if (ImGui::MenuItem("Ranged Weapons Upgrades", NULL, !(m_selected_list - ranged_weapons_upgrades)))
            {
                m_selected_list = ranged_weapons_upgrades;
            }
            if (ImGui::MenuItem("Ammo", NULL, !(m_selected_list - ammo)))
            {
                m_selected_list = ammo;
            }
            if (ImGui::MenuItem("Ammo Props", NULL, !(m_selected_list - ammo_props)))
            {
                m_selected_list = ammo_props;
            }
            if (ImGui::MenuItem("Explosives Properties", NULL, !(m_selected_list - explosives_props)))
            {
                m_selected_list = explosives_props;
            }
            if (ImGui::MenuItem("Explosives, Thrown", NULL, !(m_selected_list - explosives_thrown)))
            {
                m_selected_list = explosives_thrown;
            }
            if (ImGui::MenuItem("Explosives, Placed", NULL, !(m_selected_list - explosives_placed)))
            {
                m_selected_list = explosives_placed;
            }
            if (ImGui::MenuItem("Gear", NULL, !(m_selected_list - gear)))
            {
                m_selected_list = gear;
            }
            if (ImGui::MenuItem("Foods and Drinks Properties", NULL, !(m_selected_list - food_drinks_props)))
            {
                m_selected_list = food_drinks_props;
            }
            if (ImGui::MenuItem("Foods and Drinks", NULL, !(m_selected_list - food_drinks)))
            {
                m_selected_list = food_drinks;
            }
            if (ImGui::MenuItem("Medicine", NULL, !(m_selected_list - medicine_items)))
            {
                m_selected_list = medicine_items;
            }
            if (ImGui::MenuItem("Chems", NULL, !(m_selected_list - chems)))
            {
                m_selected_list = chems;
            }
            if (ImGui::MenuItem("Decay Levels", NULL, !(m_selected_list - decay)))
            {
                m_selected_list = decay;
            }
            if (ImGui::MenuItem("Rads Levels", NULL, !(m_selected_list - rads)))
            {
                m_selected_list = rads;
            }
            if (ImGui::MenuItem("Exhaustion Levels", NULL, !(m_selected_list - exhaustion)))
            {
                m_selected_list = exhaustion;
            }
            if (ImGui::MenuItem("Thirst Levels", NULL, !(m_selected_list - thirst)))
            {
                m_selected_list = thirst;
            }
            if (ImGui::MenuItem("Hunger Levels", NULL, !(m_selected_list - hunger)))
            {
                m_selected_list = hunger;
            }
            /*if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Armor", !(m_selected_list - armor)))
            {
                m_selected_list = armor;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Weapons", !(m_selected_list - weapons)))
            {
                m_selected_list = weapons;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Misc", !(m_selected_list - misc)))
            {
                m_selected_list = misc;
            }*/
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::SetWindowFontScale(0.9f);
    ImGui::Text("Note: Everything is sorted alphabetically.");
    ImGui::SetWindowFontScale(1.0f);
    switch (m_selected_list)
    {
    case armor_upgrades:
    {
        ImGui::Text("Armor Upgrades:");
        pair<string, tbl::armor>* equipped_armor = nullptr;
        for (auto& armor : m_character.armors)
        {
            if (armor.first == m_character.selected_armor)
                equipped_armor = &armor;
        }
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Armor Upgrades", tbl::armors_upgrades, m_selected_name, equipped_armor))
        {
            m_list_id_of_selected_name = armor_upgrades;
        }
        break;
    }
    case armor:
    {
        ImGui::Text("Armor:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Armor", tbl::armors, m_selected_name, m_character.armors))
        {
            m_list_id_of_selected_name = armor;
        }
        break;
    }
    case melee_weapons_props:
    {
        ImGui::Text("Melee Weapons Properties:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Melee Weapons Properties", tbl::melee_weapons_props, m_selected_name))
        {
            m_list_id_of_selected_name = melee_weapons_props;
        }
        break;
    }
    case ranged_weapons_props:
    {
        ImGui::Text("Ranged Weapons Properties:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Ranged Weapons Properties", tbl::ranged_weapons_props, m_selected_name))
        {
            m_list_id_of_selected_name = ranged_weapons_props;
        }
        break;
    }
    case weapons:
    {
        ImGui::Text("Weapons:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Weapons", tbl::weapons, m_selected_name, m_character.weapons))
        {
            m_list_id_of_selected_name = weapons;
        }
        break;
    }
    case melee_weapons_upgrades:
    {
        ImGui::Text("Melee Weapons Upgrades:");
        pair<string, tbl::weapon>* equipped_weapon = nullptr;
        for (auto& weapon : m_character.weapons)
        {
            if (weapon.first == m_character.selected_weapon)
                equipped_weapon = &weapon;
        }
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Melee Weapons Upgrades", tbl::melee_weapons_upgrades, m_selected_name, equipped_weapon))
        {
            m_list_id_of_selected_name = melee_weapons_upgrades;
        }
        break;
    }
    case ranged_weapons_upgrades:
    {
        ImGui::Text("Ranged Weapons Upgrades:");
        pair<string, tbl::weapon>* equipped_weapon = nullptr;
        for (auto& weapon : m_character.weapons)
        {
            if (weapon.first == m_character.selected_weapon)
                equipped_weapon = &weapon;
        }
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Ranged Weapons Upgrades", tbl::ranged_weapons_upgrades, m_selected_name, equipped_weapon))
        {
            m_list_id_of_selected_name = ranged_weapons_upgrades;
        }
        break;
    }
    case ammo_props:
    {
        ImGui::Text("Ammo Properties:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Ammo Properties", tbl::ammos_props, m_selected_name))
        {
            m_list_id_of_selected_name = ammo_props;
        }
        break;
    }
    case ammo:
    {
        ImGui::Text("Ammo:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Ammo", tbl::ammos, m_selected_name, m_character.ammos))
        {
            m_list_id_of_selected_name = ammo;
        }
        break;
    }
    case explosives_props:
    {
        ImGui::Text("Explosives Properties:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Explosives Properties", tbl::explosives_props, m_selected_name))
        {
            m_list_id_of_selected_name = explosives_props;
        }
        break;
    }
    case explosives_thrown:
    {
        ImGui::Text("Explosives, Thrown:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Explosives, Thrown", tbl::explosives_thrown, m_selected_name, m_character.explosives))
        {
            m_list_id_of_selected_name = explosives_thrown;
        }
        break;
    }
    case explosives_placed:
    {
        ImGui::Text("Explosives, Placed:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Explosives, Placed", tbl::explosives_placed, m_selected_name, m_character.explosives))
        {
            m_list_id_of_selected_name = explosives_placed;
        }
        break;
    }
    case gear:
    {
        ImGui::Text("Gear:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Gear", tbl::gear, m_selected_name, m_character.misc))
        {
            m_list_id_of_selected_name = gear;
        }
        break;
    }
    case food_drinks_props:
    {
        ImGui::Text("Foods and Drinks Properties:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Foods and Drinks Properties", tbl::foods_drinks_props, m_selected_name))
        {
            m_list_id_of_selected_name = food_drinks_props;
        }
        break;
    }
    case food_drinks:
    {
        ImGui::Text("Foods and Drinks:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Foods and Drinks", tbl::foods_drinks, m_selected_name, m_character.foods_drinks))
        {
            m_list_id_of_selected_name = food_drinks;
        }
        break;
    }
    case medicine_items:
    {
        ImGui::Text("Medicine:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Medicine", tbl::medicine, m_selected_name, m_character.misc))
        {
            m_list_id_of_selected_name = medicine_items;
        }
        break;
    }
    case chems:
    {
        ImGui::Text("Chems:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Chems", tbl::chems, m_selected_name, m_character.misc))
        {
            m_list_id_of_selected_name = chems;
        }
        break;
    }
    case decay:
    {
        ImGui::Text("Decay Levels:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Decay Levels", tbl::decay_levels, m_selected_name))
        {
            m_list_id_of_selected_name = decay;
        }
        break;
    }
    case rads:
    {
        ImGui::Text("Rads Levels:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Rads Levels", tbl::rads_levels, m_selected_name))
        {
            m_list_id_of_selected_name = rads;
        }
        break;
    }
    case exhaustion:
    {
        ImGui::Text("Exhaustion Levels:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Exhaustion Levels", tbl::exhaustion_levels, m_selected_name))
        {
            m_list_id_of_selected_name = exhaustion;
        }
        break;
    }
    case thirst:
    {
        ImGui::Text("Thirst Levels:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Thirst Levels", tbl::thirst_levels, m_selected_name))
        {
            m_list_id_of_selected_name = thirst;
        }
        break;
    }
    case hunger:
    {
        ImGui::Text("Hunger Levels:");
        if (DisplayList(ImVec2(0, 80), ImVec2(300, 150), "Hunger Levels", tbl::hunger_levels, m_selected_name))
        {
            m_list_id_of_selected_name = hunger;
        }
        break;
    }
    default:
        break;
    }

    if (ImGui::Button("Add Custom"))
    {
        ImGui::OpenPopup("Add a custom Miscellaneous Item");
    }
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Add a custom Miscellaneous Item", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        //tbl::misc new_item_props;
        //new_item_props.amount = 0;
        //new_item_props.cost = 0;
        //new_item_props.description = "no description";
        //new_item_props.load = 0.0f;
        static pair<string, tbl::misc> item = { "[new item]", {0, 0, "no description", 0.0f}};

        //ImGui::Text("This mod can't be applied to the equipped item.\nPlease equip another item and try again.");
        ImGui::InputText("Name", &item.first);
        ImGui::InputText("Description", &item.second.description);
        ImGui::InputInt("Cost", &item.second.cost);
        ImGui::InputFloat("Load", &item.second.load);
        ImGui::Separator();
        ImGui::InputInt("Amount", &item.second.amount, 1.0f, 1, 1000);
        ImGui::Separator();
        if (ImGui::Button("Confirm", ImVec2(120, 0)))
        {
            m_character.misc.push_back(item);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void App::InfoWindow()
{
    // Bio Window
    ImGui::SetNextWindowPos(ImVec2(330, 800), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(800, 170), ImGuiCond_Once);
    ImGui::Begin("Info Window");
    ImGui::SetWindowFontScale(1.2f);
    
    switch (m_list_id_of_selected_name)
    {
    case armor_upgrades:
    {
        for (auto& armor_upgrade : tbl::armors_upgrades)
        {
            if (armor_upgrade.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(armor_upgrade.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(armor_upgrade.second.cost);
                ImGui::Text(cost.c_str());
                for (int i = 0; i < 3; i++)
                {
                    string s = "Rank " + to_string(i + 1) + ": " + armor_upgrade.second.rank_description[i];
                    ImGui::Text(s.c_str());
                }
            }
        }
        break;
    }
    case armor:
    {
        for (auto& armor : tbl::armors)
        {
            if (armor.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(armor.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(armor.second.cost);
                ImGui::Text(cost.c_str());
                string ac = "Armor Class: " + to_string(armor.second.ac);
                ImGui::Text(ac.c_str());
                string dt = "Damage Threshold: " + to_string(armor.second.dt);
                ImGui::Text(dt.c_str());
                string upg_slots = "Upgrade Slots: " + to_string(armor.second.upg_slots);
                ImGui::Text(upg_slots.c_str());
                string load = "Load: " + to_string(armor.second.load);
                ImGui::Text(load.c_str());
                string str_req = "Strength Requirement: " + to_string(armor.second.str_req);
                ImGui::Text(str_req.c_str());
            }
        }
        break;
    }
    case melee_weapons_props:
    {
        for (auto& weapon_prop : tbl::melee_weapons_props)
        {
            if (weapon_prop.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(weapon_prop.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + weapon_prop.second.description;
                ImGui::Text(description.c_str());
            }
        }
        break;
    }
    case ranged_weapons_props:
    {
        for (auto& weapon_prop : tbl::ranged_weapons_props)
        {
            if (weapon_prop.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(weapon_prop.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + weapon_prop.second.description;
                ImGui::Text(description.c_str());
            }
        }
        break;
    }
    case weapons:
    {
        for (auto& weapon : tbl::weapons)
        {
            if (weapon.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(weapon.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(weapon.second.cost);
                ImGui::Text(cost.c_str());
                string ap = "Action Points: " + to_string(weapon.second.ap);
                ImGui::Text(ap.c_str());
                string dmg = "Damage: " + weapon.second.dmg;
                ImGui::Text(dmg.c_str());
                string range = "Range: " + weapon.second.range;
                ImGui::Text(range.c_str());
                string crit = "Critical: " + weapon.second.crit;
                ImGui::Text(crit.c_str());
                string ammo = "Ammo: " + weapon.second.ammo;
                ImGui::Text(ammo.c_str());
                string clip_size = "Clip Size: " + to_string(weapon.second.clip_size);
                ImGui::Text(clip_size.c_str());
                ImGui::Text("--- Props ---");
                for (auto& prop : weapon.second.props)
                {
                    string s = prop.first + ": " + prop.second.description;
                    ImGui::Text(s.c_str());
                }
                ImGui::Text("-------------");
                string load = "Load: " + to_string(weapon.second.load);
                ImGui::Text(load.c_str());
                string str_req = "Strength Requirement: " + to_string(weapon.second.str_req);
                ImGui::Text(str_req.c_str());
            }
        }
        break;
    }
    case melee_weapons_upgrades:
    {
        for (auto& weapon_upg : tbl::melee_weapons_upgrades)
        {
            if (weapon_upg.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(weapon_upg.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + weapon_upg.second.description;
                ImGui::Text(description.c_str());
                string req = "Requirement: " + weapon_upg.second.req;
                ImGui::Text(req.c_str());
            }
        }
        break;
    }
    case ranged_weapons_upgrades:
    {
        for (auto& weapon_upg : tbl::ranged_weapons_upgrades)
        {
            if (weapon_upg.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(weapon_upg.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + weapon_upg.second.description;
                ImGui::Text(description.c_str());
                string req = "Requirement: " + weapon_upg.second.req;
                ImGui::Text(req.c_str());
            }
        }
        break;
    }
    case ammo_props:
    {
        for (auto& ammo_prop : tbl::ammos_props)
        {
            if (ammo_prop.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(ammo_prop.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + ammo_prop.second.description;
                ImGui::Text(description.c_str());
                string req = "Requirement: " + ammo_prop.second.req;
                ImGui::Text(req.c_str());
            }
        }
        break;
    }
    case ammo:
    {
        for (auto& ammo : tbl::ammos)
        {
            if (ammo.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(ammo.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost per bullet: " + to_string(ammo.second.cost);
                ImGui::Text(cost.c_str());
                string pack_size = "Pack Size: " + to_string(ammo.second.pack_size);
                ImGui::Text(pack_size.c_str());
                string load = "Load: " + to_string(ammo.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case explosives_props:
    {
        for (auto& explosive_prop : tbl::explosives_props)
        {
            if (explosive_prop.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(explosive_prop.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + explosive_prop.second.description;
                ImGui::Text(description.c_str());
            }
        }
        break;
    }
    case explosives_thrown:
    {
        for (auto& explosive : tbl::explosives_thrown)
        {
            if (explosive.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(explosive.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(explosive.second.cost);
                ImGui::Text(cost.c_str());
                string ap = "Action Points: " + to_string(explosive.second.ap);
                ImGui::Text(ap.c_str());
                string dmg = "Damage: " + explosive.second.dmg;
                ImGui::Text(dmg.c_str());
                string range_or_arm_dc = "Range or Arm DC: " + explosive.second.range_or_arm_dc;
                ImGui::Text(range_or_arm_dc.c_str());
                string aoe = "Area of Effect: " + explosive.second.aoe;
                ImGui::Text(aoe.c_str());
                ImGui::Text("--- Props ---");
                for (auto& prop : explosive.second.props)
                {
                    string s = prop.first + ": " + prop.second.description;
                    ImGui::Text(s.c_str());
                }
                ImGui::Text("-------------");
                string load = "Load: " + to_string(explosive.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case explosives_placed:
    {
        for (auto& explosive : tbl::explosives_placed)
        {
            if (explosive.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(explosive.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(explosive.second.cost);
                ImGui::Text(cost.c_str());
                string ap = "Action Points: " + to_string(explosive.second.ap);
                ImGui::Text(ap.c_str());
                string dmg = "Damage: " + explosive.second.dmg;
                ImGui::Text(dmg.c_str());
                string range_or_arm_dc = "Range or Arm DC: " + explosive.second.range_or_arm_dc;
                ImGui::Text(range_or_arm_dc.c_str());
                string aoe = "Area of Effect: " + explosive.second.aoe;
                ImGui::Text(aoe.c_str());
                ImGui::Text("--- Props ---");
                for (auto& prop : explosive.second.props)
                {
                    string s = prop.first + ": " + prop.second.description;
                    ImGui::Text(s.c_str());
                }
                ImGui::Text("-------------");
                string load = "Load: " + to_string(explosive.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case gear:
    {
        for (auto& gear : tbl::gear)
        {
            if (gear.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(gear.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(gear.second.cost);
                ImGui::Text(cost.c_str());
                string description = "Description: " + gear.second.description;
                ImGui::Text(description.c_str());
                string load = "Load: " + to_string(gear.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case food_drinks_props:
    {
        for (auto& food_drink_prop : tbl::foods_drinks_props)
        {
            if (food_drink_prop.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(food_drink_prop.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string description = "Description: " + food_drink_prop.second.description;
                ImGui::Text(description.c_str());
            }
        }
        break;
    }
    case food_drinks:
    {
        for (auto& food_drink : tbl::foods_drinks)
        {
            if (food_drink.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(food_drink.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(food_drink.second.cost);
                ImGui::Text(cost.c_str());
                ImGui::Text("--- Props ---");
                for (auto& prop : food_drink.second.props)
                {
                    string s = prop.first + ": " + prop.second.description;
                    ImGui::Text(s.c_str());
                }
                ImGui::Text("-------------");
                string load = "Load: " + to_string(food_drink.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case medicine_items:
    {
        for (auto& gear : tbl::medicine)
        {
            if (gear.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(gear.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(gear.second.cost);
                ImGui::Text(cost.c_str());
                string description = "Description: " + gear.second.description;
                ImGui::Text(description.c_str());
                string load = "Load: " + to_string(gear.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case chems:
    {
        for (auto& gear : tbl::chems)
        {
            if (gear.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(gear.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string cost = "Cost: " + to_string(gear.second.cost);
                ImGui::Text(cost.c_str());
                string description = "Description: " + gear.second.description;
                ImGui::Text(description.c_str());
                string load = "Load: " + to_string(gear.second.load);
                ImGui::Text(load.c_str());
            }
        }
        break;
    }
    case decay:
    {
        for (auto& decay : tbl::decay_levels)
        {
            if (decay.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(decay.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string level = "Level: " + to_string(decay.second.level);
                ImGui::Text(level.c_str());
                string armor_effect = "Armor Effect: " + decay.second.armor_effect;
                ImGui::Text(armor_effect.c_str());
                string weapon_effect = "Weapon Effect: " + decay.second.weapon_effect;
                ImGui::Text(weapon_effect.c_str());
                string cost_reduction_percentage = "Cost Reduction: " + to_string(decay.second.cost_reduction_percentage) + "%";
                ImGui::Text(cost_reduction_percentage.c_str());
            }
        }
        break;
    }
    case rads:
    {
        for (auto& rads : tbl::rads_levels)
        {
            if (rads.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(rads.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string level = "Level: " + to_string(rads.second.level);
                ImGui::Text(level.c_str());
                string event = "Event: " + rads.second.event;
                ImGui::Text(event.c_str());
            }
        }
        break;
    }
    case exhaustion:
    {
        for (auto& rads : tbl::exhaustion_levels)
        {
            if (rads.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(rads.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string level = "Level: " + to_string(rads.second.level);
                ImGui::Text(level.c_str());
                string event = "Event: " + rads.second.event;
                ImGui::Text(event.c_str());
            }
        }
        break;
    }
    case thirst:
    {
        for (auto& rads : tbl::thirst_levels)
        {
            if (rads.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(rads.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string level = "Level: " + to_string(rads.second.level);
                ImGui::Text(level.c_str());
                string event = "Event: " + rads.second.event;
                ImGui::Text(event.c_str());
            }
        }
        break;
    }
    case hunger:
    {
        for (auto& rads : tbl::hunger_levels)
        {
            if (rads.first == m_selected_name)
            {
                ImGui::PushTextWrapPos(780.0f);
                ImGui::SetWindowFontScale(1.8f);
                ImGui::Text(rads.first.c_str());
                ImGui::SetWindowFontScale(1.2f);
                string level = "Level: " + to_string(rads.second.level);
                ImGui::Text(level.c_str());
                string event = "Event: " + rads.second.event;
                ImGui::Text(event.c_str());
            }
        }
        break;
    }
    default:
        break;
    }

    ImGui::SetWindowFontScale(1.0f);
    ImGui::End();
}

void App::InventoryWindow()
{
    
}

void App::WeaponsInventoryWindow()
{

}

void App::ArmorInventoryWindow()
{

}

void App::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// ---- INIT FUNCTIONS ----

void App::ExtractLeveling()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/leveling.txt");
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
        tbl::level value;
        key = row[0];
        value.flat_sp = stoi(util::GetSubstringBetween(row[1], "", " SP"));
        value.agi_mul = stoi(util::GetSubstringBetween(row[1], "AGI mod x", ", "));
        value.flat_hp = stoi(util::GetSubstringBetween(row[1], ", ", " HP"));
        value.end_mul = stoi(util::GetSubstringBetween(row[1], "END mod x", ""));

        tbl::levels.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractArmors()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/armor.txt");
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
        tbl::armor value;
        key = row[0];
        row[1].pop_back();
        value.cost = stoi(row[1]);
        value.ac = stoi(row[2]);
        value.dt = stoi(row[3]);
        value.upg_slots = stoi(row[4]);
        size_t load_start = row[5].find("Load: ");
        size_t load_end = row[5].find(",");
        string load_str = row[5].substr(load_start + 6, load_end - (load_start + 6));
        value.load = stoi(load_str);
        size_t strreq_start = row[5].find("STR req: ");
        string strreq_str = row[5].substr(strreq_start + 9, (row[5].size() - 1) - (strreq_start + 9));
        value.str_req = stoi(strreq_str);

        tbl::armors.emplace(key, value);
    }
}

void App::ExtractArmorsUpgrades()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/armor_upgrades.txt");
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
        tbl::armor_upg value;
        key = row[0];
        value.cost = stoi(util::GetSubstringBetween(row[1], "", "c"));
        value.rank_description[0] = row[2];
        value.rank_description[1] = row[3];
        value.rank_description[2] = row[4];

        tbl::armors_upgrades.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractWeaponsProperties()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/melee_weapons_properties.txt");
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
        tbl::weapon_prop value;
        key = row[0];
        value.description = row[1];

        tbl::melee_weapons_props.emplace(key, value);
    }
    tsv_file.close();

    ifstream tsv_file2("../Fallout_character_Manager/spreadsheets/ranged_weapons_properties.txt");
    if (!tsv_file2.is_open())
    {
        cout << "Can't open file" << endl;
        return;
    }
    //string line;
    skip_first = true;
    while (getline(tsv_file2, line))
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
        tbl::weapon_prop value;
        key = row[0];
        value.description = row[1];

        tbl::ranged_weapons_props.emplace(key, value);
    }
    tsv_file2.close();
}

void App::ExtractWeapons()
{
    string files[2] = {
        "../Fallout_character_Manager/spreadsheets/ranged_weapons.txt",
        "../Fallout_character_Manager/spreadsheets/melee_weapons.txt" };

    for (auto& path : files)
    {
        ifstream tsv_file(path);
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
            tbl::weapon value;
            if (path.find("ranged_weapons") != string::npos)
            {
                key = row[0];
                row[1].pop_back();
                value.cost = stoi(row[1]);
                value.ap = row[2].at(0) - '0';
                value.dmg = row[3];
                value.range = row[4];
                value.crit = row[5];
                size_t ammo_start = 1;
                size_t ammo_end = row[6].find(",");
                string ammo_str = row[6].substr(ammo_start, ammo_end - (ammo_start - 1));
                value.ammo = ammo_str;
                size_t clip_start = row[6].find(", ") + 2;
                size_t clip_end = row[6].find(" round");
                string clip_str = row[6].substr(clip_start, clip_end - clip_start);
                value.clip_size = stoi(clip_str);
                istringstream iss_prop(row[7]);
                string prop;
                while (getline(iss_prop, prop, '.'))
                {
                    size_t start = prop.find_first_not_of(" \t\n\r");
                    size_t end = prop.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                    {
                        string name = prop.substr(start, end - (start - 1));
                        tbl::weapon_prop prop = { "" };
                        auto it = tbl::ranged_weapons_props.find(name);
                        if (it != tbl::ranged_weapons_props.end())
                            prop = it->second;
                        value.props.emplace(name, prop);
                    }
                }
                size_t load_start = row[8].find("Load: ");
                size_t load_end = row[8].find(",");
                string load_str = row[8].substr(load_start + 6, load_end - (load_start + 6));
                value.load = stoi(load_str);
                size_t strreq_start = row[8].find("STR req: ");
                string strreq_str = row[8].substr(strreq_start + 9, (row[8].size() - 1) - (strreq_start + 9));
                value.str_req = stoi(strreq_str);
            }
            else if (path.find("melee_weapons") != string::npos)
            {
                key = row[0];
                row[1].pop_back();
                value.cost = stoi(row[1]);
                value.ap = row[2].at(0) - '0';
                value.dmg = row[3];
                value.range = "melee";
                value.crit = row[4];
                value.ammo = "NONE";
                value.clip_size = 0;
                istringstream iss_prop(row[5]);
                string prop;
                while (getline(iss_prop, prop, '.'))
                {
                    size_t start = prop.find_first_not_of(" \t\n\r");
                    size_t end = prop.find_last_not_of(" \t\n\r");
                    if (start != string::npos && end != string::npos)
                    {
                        string name = prop.substr(start, end - (start - 1));
                        tbl::weapon_prop prop = { "" };
                        auto it = tbl::melee_weapons_props.find(name);
                        if (it != tbl::melee_weapons_props.end())
                            prop = it->second;
                        value.props.emplace(name, prop);
                    }
                }
                size_t load_start = row[6].find("Load: ");
                size_t load_end = row[6].find(",");
                string load_str = row[6].substr(load_start + 6, load_end - (load_start + 6));
                value.load = stoi(load_str);
                size_t strreq_start = row[6].find("STR req: ");
                string strreq_str = row[6].substr(strreq_start + 9, (row[6].size() - 1) - (strreq_start + 9));
                value.str_req = stoi(strreq_str);
            }

            tbl::weapons.emplace(key, value);
        }
    }
    /*for (auto weapon : tbl::weapons)
    {
        cout << weapon.first << endl;
        cout << weapon.second.ammo << endl;
        cout << weapon.second.clip_size << endl;
        for (auto& prop : weapon.second.props_keywords)
            cout << prop << endl;
        cout << weapon.second.load << endl;
        cout << weapon.second.str_req << endl;
        cout << "-------------------------" << endl;
    }*/
}

void App::ExtractWeaponsUpgrades()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/melee_weapons_upgrades.txt");
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
        tbl::weapon_upg value;
        key = row[0];
        value.description = row[1];

        tbl::melee_weapons_upgrades.emplace(key, value);
    }
    tsv_file.close();

    ifstream tsv_file2("../Fallout_character_Manager/spreadsheets/ranged_weapons_upgrades.txt");
    if (!tsv_file2.is_open())
    {
        cout << "Can't open file" << endl;
        return;
    }
    //string line;
    skip_first = true;
    while (getline(tsv_file2, line))
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
        tbl::weapon_upg value;
        key = row[0];
        value.description = row[1];
        value.req = row[2];

        tbl::ranged_weapons_upgrades.emplace(key, value);
    }
    tsv_file2.close();
}

void App::ExtractAmmoProperties()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/ammo_special.txt");
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
        tbl::ammo_prop value;
        key = row[0];
        value.description = row[1];
        value.req = row[2];

        tbl::ammos_props.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractAmmo()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/ammo.txt");
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
        tbl::ammo value;
        key = row[0];
        value.cost = stoi(util::GetSubstringBetween(row[1], "(", "c)"));
        //cout << row[2] << endl;
        string pack_size = util::GetSubstringBetween(row[2], "Pack of ", ".");
        if (pack_size != "")
        {
            value.pack_size = stoi(pack_size);
            //value.load = static_cast<float>(value.quantity) / static_cast<float>(value.pack_size);
        }
        else
        {
            pack_size = util::GetSubstringBetween(row[2], "", "sth.");
            value.pack_size = 1;
            value.load = stoi(pack_size);
        }

        tbl::ammos.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractExplosivesProperties()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/explosives_properties.txt");
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
        tbl::explosive_prop value;
        key = row[0];
        value.description = row[1];

        tbl::explosives_props.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractExplosives()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/explosives_thrown.txt");
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
        tbl::explosive value;
        key = row[0];
        value.cost = stoi(util::GetSubstringBetween(row[1], "", "c"));
        value.ap = stoi(util::GetSubstringBetween(row[2], "", " AP"));
        value.dmg = row[3];
        value.range_or_arm_dc = row[4];
        value.aoe = row[5];
        istringstream iss_prop(row[6]);
        string prop;
        while (getline(iss_prop, prop, '.'))
        {
            size_t start = prop.find_first_not_of(" \t\n\r");
            size_t end = prop.find_last_not_of(" \t\n\r");
            if (start != string::npos && end != string::npos)
            {
                string name = prop.substr(start, end - (start - 1));
                tbl::explosive_prop prop = { "" };
                auto it = tbl::explosives_props.find(name);
                if (it != tbl::explosives_props.end())
                    prop = it->second;
                value.props.emplace(name, prop);
            }
        }
        value.load = stoi(util::GetSubstringBetween(row[7], "Load: ", "."));

        tbl::explosives_thrown.emplace(key, value);
    }
    tsv_file.close();

    ifstream tsv_file2("../Fallout_character_Manager/spreadsheets/explosives_placed.txt");
    if (!tsv_file2.is_open())
    {
        cout << "Can't open file" << endl;
        return;
    }
    //string line;
    skip_first = true;
    while (getline(tsv_file2, line))
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
        tbl::explosive value;
        key = row[0];
        value.cost = stoi(util::GetSubstringBetween(row[1], "", "c"));
        value.ap = stoi(util::GetSubstringBetween(row[2], "", " AP"));
        value.dmg = row[3];
        value.range_or_arm_dc = row[4];
        value.aoe = row[5];
        istringstream iss_prop(row[6]);
        string prop;
        while (getline(iss_prop, prop, '.'))
        {
            size_t start = prop.find_first_not_of(" \t\n\r");
            size_t end = prop.find_last_not_of(" \t\n\r");
            if (start != string::npos && end != string::npos)
            {
                string name = prop.substr(start, end - (start - 1));
                tbl::explosive_prop prop = { "" };
                auto it = tbl::explosives_props.find(name);
                if (it != tbl::explosives_props.end())
                    prop = it->second;
                value.props.emplace(name, prop);
            }
        }
        value.load = stoi(util::GetSubstringBetween(row[7], "Load: ", "."));

        tbl::explosives_placed.emplace(key, value);
    }
    tsv_file2.close();
}

void App::ExtractGear()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/gear.txt");
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
        tbl::misc value;
        key = row[0];
        value.description = row[1];
        value.cost = stoi(util::GetSubstringBetween(row[2], "", "c"));
        value.load = stoi(row[3]);

        tbl::gear.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractFoodsDrinksProps()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/food_drink_properties.txt");
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
        tbl::food_drink_prop value;
        key = row[0];
        value.description = row[1];

        tbl::foods_drinks_props.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractFoodsDrinks()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/food_drink.txt");
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
        tbl::food_drink value;
        key = row[0];
        value.cost = stoi(util::GetSubstringBetween(row[1], "", "c"));
        istringstream iss_prop(row[2]);
        string prop;
        while (getline(iss_prop, prop, '.'))
        {
            size_t start = prop.find_first_not_of(" \t\n\r");
            size_t end = prop.find_last_not_of(" \t\n\r");
            if (start != string::npos && end != string::npos)
            {
                string name = prop.substr(start, end - (start - 1));
                tbl::food_drink_prop prop = { "" };
                auto it = tbl::foods_drinks_props.find(name);
                if (it != tbl::foods_drinks_props.end())
                    prop = it->second;
                value.props.emplace(name, prop);
            }
        }
        value.load = stoi(row[3]);

        tbl::foods_drinks.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractMedicine()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/medicine.txt");
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
        tbl::misc value;
        key = row[0];
        value.description = row[1];
        value.cost = stoi(util::GetSubstringBetween(row[2], "", "c"));
        value.load = stoi(row[3]);

        tbl::medicine.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractChems()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/chems.txt");
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
        tbl::misc value;
        key = row[0];
        value.cost = stoi(util::GetSubstringBetween(row[1], "", "c"));
        value.description = row[2];
        value.load = stoi(row[3]);

        tbl::chems.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractRads()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/rads.txt");
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
        tbl::rads value;
        key = row[1];
        value.level = stoi(util::GetSubstringBetween(row[0], "Level ", ""));
        value.event = row[2];

        tbl::rads_levels.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractExhaustion()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/exhaustion.txt");
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
        tbl::exhaustion value;
        key = row[1];
        value.level = stoi(row[0]);
        value.event = row[2];

        tbl::exhaustion_levels.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractThirst()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/thirst.txt");
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
        tbl::thirst value;
        key = row[1];
        value.level = stoi(row[0]);
        value.event = row[2];

        tbl::thirst_levels.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractHunger()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/hunger.txt");
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
        tbl::hunger value;
        key = row[1];
        value.level = stoi(row[0]);
        value.event = row[2];

        tbl::hunger_levels.emplace(key, value);
    }
    tsv_file.close();
}

void App::ExtractDecay()
{
    ifstream tsv_file("../Fallout_character_Manager/spreadsheets/decay.txt");
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
        tbl::decay value;
        key = row[1];
        value.level = stoi(row[0]);
        value.weapon_effect = row[2];
        value.armor_effect = row[3];
        value.cost_reduction_percentage = stoi(util::GetSubstringBetween(row[4], "", "%"));

        tbl::decay_levels.emplace(key, value);
    }
    tsv_file.close();
}

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


// ---- SAVE/LOAD FUNCTIONS ----

void App::SaveToTSV()
{
    ofstream f_base(m_character.filename + "base.txt");
    if (!f_base.is_open())
    {
        cout << "Can't open file" << endl;
    }
    string output = "";
    // auto save/load
    output += "autosave: " + to_string(m_character.autosave) + "\n";
    output += "autoload: " + to_string(m_character.autoload) + "\n";
    // bio
    output += "name: " + m_character.name + "\n";
    output += "race: " + m_character.race + "\n";
    output += "background: " + m_character.background + "\n";
    output += "level: " + to_string(m_character.level) + "\n";
    // base stats
    output += "hp: " + to_string(m_character.hp) + "\n";
    output += "sp: " + to_string(m_character.sp) + "\n";
    output += "ac: " + to_string(m_character.ac) + "\n";
    output += "dt: " + to_string(m_character.dt) + "\n";
    output += "ap: " + to_string(m_character.ap) + "\n";
    output += "healing_rate: " + to_string(m_character.healing_rate) + "\n";
    output += "combat_seq: " + to_string(m_character.combat_seq) + "\n";
    // special
    output += "SPECIAL: ";
    for (int i = 0; i < 7; i++)
    {
        output += to_string(m_character.special[i]) + "\t";
    }
    output += "\n";
    // skill tags
    output += "tags: ";
    for (int i = 0; i < 16; i++)
    {
        output += to_string(m_character.tags[i]) + '\t';
    }
    output += "\n";
    // skills
    output += "skills: ";
    for (int i = 0; i < 16; i++)
    {
        output += to_string(m_character.skill[i]) + '\t';
    }
    output += "\n";
    // passives
    output += "passive_sense: " + to_string(m_character.passive_sense) + "\n";
    output += "party_nerve: " + to_string(m_character.party_nerve) + "\n";
    output += "party_luck: " + to_string(m_character.party_luck) + "\n";
    output += "party_sneak: " + to_string(m_character.party_sneak) + "\n";
    // survival
    output += "hunger: " + to_string(m_character.hunger) + "\n";
    output += "thirst: " + to_string(m_character.thirst) + "\n";
    output += "exhaustion: " + to_string(m_character.exhaustion) + "\n";
    output += "fatigue: " + to_string(m_character.fatigue) + "\n";
    output += "rads: " + to_string(m_character.rads) + "\n";
    f_base.write(output.c_str(), output.size());
    f_base.close();

    ofstream f_perks(m_character.filename + "perks.txt");
    if (!f_perks.is_open())
    {
        cout << "Can't open file" << endl;
    }
    output = "";
    // perks
    for (int i = 0; i < m_character.traits_perks.size(); i++)
    {
        output += m_character.traits_perks[i].first + "\t" + m_character.traits_perks[i].second + "\n";
    }
    f_perks.write(output.c_str(), output.size());
    f_perks.close();

    ofstream f_conditions(m_character.filename + "conditions.txt");
    if (!f_conditions.is_open())
    {
        cout << "Can't open file" << endl;
    }
    output = "";
    // conditions
    for (auto& c : m_character.head_conditions) { output += c.first + "\n"; }
    for (auto& c : m_character.eye_conditions) { output += c.first + "\n"; }
    for (auto& c : m_character.arms_conditions) { output += c.first + "\n"; }
    for (auto& c : m_character.torso_conditions) { output += c.first + "\n"; }
    for (auto& c : m_character.groin_conditions) { output += c.first + "\n"; }
    for (auto& c : m_character.legs_conditions) { output += c.first + "\n"; }
    f_conditions.write(output.c_str(), output.size());
    f_conditions.close();

    ofstream f_armors(m_character.filename + "armors.txt");
    if (!f_armors.is_open())
    {
        cout << "Can't open file" << endl;
    }
    output = "";
    // armors
    for (auto& a : m_character.armors)
    {
        output += "name: " + a.first + "\t";
        output += "cost: " + to_string(a.second.cost) + "\t";
        output += "ac: " + to_string(a.second.ac) + "\t";
        output += "dt: " + to_string(a.second.dt) + "\t";
        output += "upg_slots: " + to_string(a.second.upg_slots) + "\t";
        output += "load: " + to_string(a.second.load) + "\t";
        output += "str_req: " + to_string(a.second.str_req) +"\t";
        output += "equipped: " + to_string(a.second.equipped) + "\t";
        output += "decay_level: " + to_string(a.second.decay_level) + "\t";
        output += "current_upg: " + to_string(a.second.upgrades.size()) + "\t";
        for (auto& a_u : a.second.upgrades)
        {
            output += a_u.first + "|";
        }
        output += "\t\n";
    }
    f_armors.write(output.c_str(), output.size());
    f_armors.close();

    ofstream f_weapons(m_character.filename + "weapons.txt");
    if (!f_weapons.is_open())
    {
        cout << "Can't open file" << endl;
    }
    output = "";
    // weapons
    for (auto& w : m_character.weapons)
    {
        output += "name: " + w.first + "\t";
        output += "cost: " + to_string(w.second.cost) + "\t";
        output += "ap: " + to_string(w.second.ap) + "\t";
        output += "dmg: " + w.second.dmg + "\t";
        output += "range: " + w.second.range + "\t";
        output += "crit: " + w.second.crit + "\t";
        output += "ammo: " + w.second.ammo + "\t";
        output += "clip_size: " + to_string(w.second.clip_size) + "\t";
        output += "props: " + to_string(w.second.props.size()) + "\t";
        for (auto& w_p : w.second.props)
        {
            output += w_p.first + "|";
        }
        output += "\t";
        output += "load: " + to_string(w.second.load) + "\t";
        output += "str_req: " + to_string(w.second.str_req) + "\t";
        output += "decay_level: " + to_string(w.second.decay_level) + "\t";
        output += "equipped: " + to_string(w.second.equipped) + "\t";
        output += "current_upg: " + to_string(w.second.upgrades.size()) + "\t";
        for (auto& w_u : w.second.upgrades)
        {
            output += w_u.first + "|";
        }
        output += "\t\n";
    }
    f_weapons.write(output.c_str(), output.size());
    f_weapons.close();
}

void App::LoadFromTSV()
{
    string line = "";
    int line_count = 0;

    // base
        enum base
        {
            autosave,
            autoload,
            name,
            race,
            background,
            level,
            hp,
            sp,
            ac,
            dt,
            ap,
            healing_rate,
            combat_seq,
            special,
            tags,
            skills,
            passive_sense,
            party_nerve,
            party_luck,
            party_sneak,
            hunger,
            thirst,
            exhaustion,
            fatigue,
            rads
        };
    ifstream f_base(m_character.filename + "base.txt");
    if (!f_base.is_open())
    {
        cout << "Can't open file" << endl;
    }
    line = "";
    line_count = 0;
    while (getline(f_base, line))
    {
        switch (line_count)
        {
        case autosave:
        {
            string autosave = util::GetSubstringBetween(line, "autosave: ", "");
            if (autosave != "")
                m_character.autosave = stoi(autosave);
            break;
        }
        case autoload:
        {
            string autoload = util::GetSubstringBetween(line, "autoload: ", "");
            if (autoload != "")
                m_character.autoload = stoi(autoload);
            break;
        }
        case name:
        {
            string name = util::GetSubstringBetween(line, "name: ", "");
            if (name != "")
                m_character.name = name;
            break;
        }
        case race:
        {
            string race = util::GetSubstringBetween(line, "race: ", "");
            if (race != "")
                m_character.race = race;
            break;
        }
        case background:
        {
            string background = util::GetSubstringBetween(line, "background: ", "");
            if (background != "")
                m_character.background = background;
            break;
        }
        case level:
        {
            string level = util::GetSubstringBetween(line, "level: ", "");
            if (level != "")               
                m_character.level = stoi(level);
            break;
        }
        case hp:
        {
            string hp = util::GetSubstringBetween(line, "hp: ", "");
            if (hp != "")
                m_character.hp = stoi(hp);
            break;
        }
        case sp:
        {
            string sp = util::GetSubstringBetween(line, "sp: ", "");
            if (sp != "")
                m_character.sp = stoi(sp);
            break;
        }
        case ac:
        {
            string ac = util::GetSubstringBetween(line, "ac: ", "");
            if (ac != "")
                m_character.ac = stoi(ac);
            break;
        }
        case dt:
        {
            string dt = util::GetSubstringBetween(line, "dt: ", "");
            if (dt != "")
                m_character.dt = stoi(dt);
            break;
        }
        case ap:
        {
            string ap = util::GetSubstringBetween(line, "ap: ", "");
            if (ap != "")
                m_character.ap = stoi(ap);
            break;
        }
        case healing_rate:
        {
            string healing_rate = util::GetSubstringBetween(line, "healing_rate: ", "");
            if (healing_rate != "")
                m_character.healing_rate = stoi(healing_rate);
            break;
        }
        case combat_seq:
        {
            string combat_seq = util::GetSubstringBetween(line, "combat_seq: ", "");
            if (combat_seq != "")
                m_character.combat_seq = stoi(combat_seq);
            break;
        }
        case special:
        {
            string special = util::GetSubstringBetween(line, "SPECIAL: ", "");
            vector<string> specials = util::SplitString(special, '\t');
            for (int i = 0; i < 7; i++)
            {
                m_character.special[i] = stoi(specials[i]);
                m_character.special_mods[i] = m_character.special[i] - 5;
            }
            break;
        }
        case tags:
        {
            string tag = util::GetSubstringBetween(line, "tags: ", "");
            vector<string> tags = util::SplitString(tag, '\t');
            for (int i = 0; i < 16; i++)
            {
                m_character.tags[i] = stoi(tags[i]);
            }
            break;
        }
        case skills:
        {
            string skill = util::GetSubstringBetween(line, "skills: ", "");
            vector<string> skills = util::SplitString(skill, '\t');
            for (int i = 0; i < 16; i++)
            {
                m_character.skill[i] = 0;
                m_character.skill[i] += stoi(skills[i]);
            }
            break;
        }
        case passive_sense:
        {
            string passive_sense = util::GetSubstringBetween(line, "passive_sense: ", "");
            if (passive_sense != "")
                m_character.passive_sense = stoi(passive_sense);
            break;
        }
        case party_nerve:
        {
            string party_nerve = util::GetSubstringBetween(line, "party_nerve: ", "");
            if (party_nerve != "")
                m_character.party_nerve = stoi(party_nerve);
            break;
        }
        case party_luck:
        {
            string party_luck = util::GetSubstringBetween(line, "party_luck: ", "");
            if (party_luck != "")
                m_character.party_luck = stoi(party_luck);
            break;
        }
        case party_sneak:
        {
            string party_sneak = util::GetSubstringBetween(line, "party_sneak: ", "");
            if (party_sneak != "")
                m_character.party_sneak = stoi(party_sneak);
            break;
        }
        case hunger:
        {
            string hunger = util::GetSubstringBetween(line, "hunger: ", "");
            if (hunger != "")
                m_character.hunger = stoi(hunger);
            break;
        }
        case thirst:
        {
            string thirst = util::GetSubstringBetween(line, "thirst: ", "");
            if (thirst != "")
                m_character.thirst = stoi(thirst);
            break;
        }
        case exhaustion:
        {
            string exhaustion = util::GetSubstringBetween(line, "exhaustion: ", "");
            if (exhaustion != "")
                m_character.exhaustion = stoi(exhaustion);
            break;
        }
        case fatigue:
        {
            string fatigue = util::GetSubstringBetween(line, "fatigue: ", "");
            if (fatigue != "")
                m_character.fatigue = stoi(fatigue);
            break;
        }
        case rads:
        {
            string rads = util::GetSubstringBetween(line, "rads: ", "");
            if (rads != "")
                m_character.rads = stoi(rads);
            break;
        }
        default:
            break;
        }
        line_count++;
    }
    f_base.close();

    // perks
    ifstream f_perks(m_character.filename + "perks.txt");
    if (!f_perks.is_open())
    {
        cout << "Can't open file" << endl;
    }
    line = "";
    line_count = 0;
    m_character.traits_perks.clear();
    while (getline(f_perks, line))
    {
        vector<string> perks = util::SplitString(line, '\t');
        if (perks.size() > 2)
            cout << "corrupted perk!" << endl;
        else
        {
            m_character.traits_perks.push_back({ perks[0], perks[1] });
        }
    }
    f_perks.close();

    // conditions
    ifstream f_conditions(m_character.filename + "conditions.txt");
    if (!f_conditions.is_open())
    {
        cout << "Can't open file" << endl;
    }
    line = "";
    line_count = 0;
    map<string, tbl::limb_condition>* character_body_part = nullptr;
    while (getline(f_conditions, line))
    {
        cout << line << endl;
        string selected_cond = line;
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
    f_conditions.close();

    // armors
    ifstream f_armors(m_character.filename + "armors.txt");
    if (!f_armors.is_open())
    {
        cout << "Can't open file" << endl;
    }
    line = "";
    line_count = 0;
    m_character.armors.clear();
    while (getline(f_armors, line))
    {
        vector<string> armor_string = util::SplitString(line, '\t');
        string name = util::GetSubstringBetween(armor_string[0], "name: ", "");
        tbl::armor armor;
        armor.cost = stoi(util::GetSubstringBetween(armor_string[1], "cost: ", ""));
        armor.ac = stoi(util::GetSubstringBetween(armor_string[2], "ac: ", ""));
        armor.dt = stoi(util::GetSubstringBetween(armor_string[3], "dt: ", ""));
        armor.upg_slots = stoi(util::GetSubstringBetween(armor_string[4], "upg_slots: ", ""));
        armor.load = stof(util::GetSubstringBetween(armor_string[5], "load: ", ""));
        armor.str_req = stoi(util::GetSubstringBetween(armor_string[6], "str_req: ", ""));
        armor.equipped = stoi(util::GetSubstringBetween(armor_string[7], "equipped: ", ""));
        armor.decay_level = stoi(util::GetSubstringBetween(armor_string[8], "decay_level: ", ""));
        int upg_num = stoi(util::GetSubstringBetween(armor_string[9], "current_upg: ", ""));
        cout << upg_num << endl;
        if (upg_num != 0)
        {
            vector<string> upgrades = util::SplitString(armor_string[10], '|');
            for (auto& name : upgrades)
            {
                tbl::armor_upg upgrade = { 0, "" };
                armor.upgrades.emplace(name, upgrade);
            }
        }
        m_character.armors.push_back({ name, armor });
    }
    f_armors.close();

    // weapons
    ifstream f_weapons(m_character.filename + "weapons.txt");
    if (!f_weapons.is_open())
    {
        cout << "Can't open file" << endl;
    }
    line = "";
    line_count = 0;
    m_character.weapons.clear();
    while (getline(f_weapons, line))
    {
        vector<string> weapon_string = util::SplitString(line, '\t');
        string name = util::GetSubstringBetween(weapon_string[0], "name: ", "");
        tbl::weapon weapon;
        weapon.cost = stoi(util::GetSubstringBetween(weapon_string[1], "cost: ", ""));
        weapon.ap = stoi(util::GetSubstringBetween(weapon_string[2], "ap: ", ""));
        weapon.dmg = util::GetSubstringBetween(weapon_string[3], "dmg: ", "");
        weapon.range = util::GetSubstringBetween(weapon_string[4], "range: ", "");
        weapon.crit = util::GetSubstringBetween(weapon_string[5], "crit: ", "");
        weapon.ammo = util::GetSubstringBetween(weapon_string[6], "ammo: ", "");
        weapon.clip_size = stoi(util::GetSubstringBetween(weapon_string[7], "clip_size: ", ""));
        int props_num = stoi(util::GetSubstringBetween(weapon_string[8], "props: ", ""));
        if (props_num != 0)
        {
            vector<string> props = util::SplitString(weapon_string[9], '|');
            for (auto& name : props)
            {
                tbl::weapon_prop prop = { "" };
                weapon.props.emplace(name, prop);
            }
        }
        weapon.load = stof(util::GetSubstringBetween(weapon_string[10], "load: ", ""));
        weapon.str_req = stoi(util::GetSubstringBetween(weapon_string[11], "str_req: ", ""));
        weapon.decay_level = stoi(util::GetSubstringBetween(weapon_string[12], "decay_level: ", ""));
        weapon.equipped = stoi(util::GetSubstringBetween(weapon_string[13], "equipped: ", ""));
        int upg_num = stoi(util::GetSubstringBetween(weapon_string[14], "current_upg: ", ""));
        if (upg_num != 0)
        {
            vector<string> upgrades = util::SplitString(weapon_string[15], '|');
            for (auto& name : upgrades)
            {
                tbl::weapon_upg upgrade = { "", "" };
                weapon.upgrades.emplace(name, upgrade);
            }
        }
        m_character.weapons.push_back({ name, weapon });
    }
    f_weapons.close();

}


// ---- HELPER FUNCTIONS ---

//template <template <typename, typename...> class Container, typename T>
template <typename Container>
bool App::DisplayComboBox(
    const ImVec2& pos,
    const int width,
    const string& name,
    const Container& list,
    string& selected_item
)
{
    ImGui::SetCursorPos(pos);
    ImGui::SetNextItemWidth(width);
    bool new_select = false;
    string tag = "##" + name;
    //string combo_preview_value = m_character.arr_str[selected_item];
    if (ImGui::BeginCombo(tag.c_str(), name.c_str()))
    {
        for (auto& item : list)
        {
            const bool is_selected = (selected_item == item.first);
            if (ImGui::Selectable(item.first.c_str(), is_selected))
            {
                new_select = true;
                selected_item = item.first;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return new_select;
}

template <typename Container>
bool App::DisplayList(
    const ImVec2& pos,
    const ImVec2& size,
    const string& name,
    const Container& list,
    string& selected_item)
{
    ImGui::SetCursorPos(pos);
    bool new_select = false;
    string tag = "##" + name;
    if (ImGui::BeginListBox(tag.c_str(), size))
    {
        // Display the items in the ListBox
        for (auto& item : list)
        {
            const bool is_selected = (selected_item == item.first);
            if (ImGui::Selectable(item.first.c_str(), is_selected))
            {
                new_select = true;
                selected_item = item.first;
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }
    
    return new_select;
}

// For adding upgrades to weapons/armor. Item must be of type tbl::weapon or tbl::armor
template<typename Container, typename ItemAddTo>
bool App::DisplayList(const ImVec2& pos, const ImVec2& size, const string& name, const Container& list, string& selected_item, ItemAddTo& add_to)
{
    ImGui::SetCursorPos(pos);
    bool new_select = false;
    string tag = "##" + name;
    if (ImGui::BeginListBox(tag.c_str(), size))
    {
        // Display the items in the ListBox
        for (auto& item : list)
        {
            const bool is_selected = (selected_item == item.first);
            if (ImGui::Selectable(item.first.c_str(), is_selected))
            {
                new_select = true;
                selected_item = item.first;
            }
            if (ImGui::BeginPopupContextItem())
            {
                new_select = true;
                selected_item = item.first;
                if (ImGui::Button("Add"))
                {
                    if (add_to->second.equipped)
                    {
                        add_to->second.upgrades.emplace(item.first, item.second);
                        ImGui::OpenPopup("Mod added.");
                    }
                    else
                    {
                        ImGui::OpenPopup("Mod can't be applied.");
                    }
                }

                // bad
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                if (ImGui::BeginPopupModal("Mod can't be applied.", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Please select your equipped item to apply the mod.");
                    ImGui::Separator();

                    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::EndPopup();
                }

                // good
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                if (ImGui::BeginPopupModal("Mod added.", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Mod added successfully.");
                    ImGui::SetWindowFontScale(1.8f);
                    ImGui::Text(item.first.c_str());
                    ImGui::SetWindowFontScale(1.0f);
                    ImGui::Separator();

                    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::EndPopup();
                }

                ImGui::EndPopup();
            }
            ImGui::SetItemTooltip("Right-click to open popup");

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    return new_select;
}

// For adding items to inventory
template<typename Container, typename ItemAddTo>
bool App::DisplayList(const ImVec2& pos, const ImVec2& size, const string& name, const Container& list, string& selected_item, vector<pair<string, ItemAddTo>>& add_to)
{
    ImGui::SetCursorPos(pos);
    bool new_select = false;
    string tag = "##" + name;
    if (ImGui::BeginListBox(tag.c_str(), size))
    {
        // Display the items in the ListBox
        for (auto& item : list)
        {
            const bool is_selected = (selected_item == item.first);
            if (ImGui::Selectable(item.first.c_str(), is_selected))
            {
                new_select = true;
                selected_item = item.first;
            }
            if (ImGui::BeginPopupContextItem())
            {
                new_select = true;
                selected_item = item.first;
                if (ImGui::Button("Add"))
                {
                    string key = item.first;
                    ItemAddTo value = item.second;
                    add_to.push_back({ key, value });
                    ImGui::OpenPopup("Item added.");
                }

                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
                if (ImGui::BeginPopupModal("Item added.", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Item added successfully.");
                    ImGui::SetWindowFontScale(1.8f);
                    ImGui::Text(item.first.c_str());
                    ImGui::SetWindowFontScale(1.0f);
                    ImGui::Separator();

                    if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
                    ImGui::SetItemDefaultFocus();
                    ImGui::EndPopup();
                }

                ImGui::EndPopup();
            }
            ImGui::SetItemTooltip("Right-click to open popup");

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    return new_select;
}

void App::ReloadCharacterFromBaseStats()
{

}