#pragma once

#include <iostream>
#include <cmath>
#include "Character.h"
#include "exception"

struct LevelUpFlags {
    bool abilityIncrease = false;
    bool featChoice = false;
    std::vector<std::string> currentLevelSkills;
    int skillPoints = 0;
};

class SystemManager{
public:

    static std::shared_ptr<SystemManager> getManager();

    void configure();

    void configure_test();

    void CharacterCreationSetup();

    bool SaveCharacter(const std::string& path = "../../Characters/");

    bool LoadCharacter(const std::string& char_name, const std::string& path = "../../Characters/");

    std::shared_ptr<Character> LoadTestCharacter(){
        if(LoadCharacter("test", "../../Characters/"))
            return character;
        else
            return nullptr;
    }

    void setRace(int index);

    bool addClass(int index, bool f = false);

    bool canLevelUp();

    static std::vector<std::shared_ptr<RuleClass>> getClasses() {
        return classes;
    }

    static std::vector<std::shared_ptr<Feat>> getFeats() {
        return feats;
    }

    static std::vector<std::shared_ptr<Race>> getRaces(){
        return races;
    }

    static std::vector<std::shared_ptr<Skill>> getSkills(){
        return skills;
    }

    static std::shared_ptr<Character> clearCharacter(){
        levelUpFlags = LevelUpFlags();
        character->reset();
        return character;
    }

    bool addFeat(int index, bool passAnyway);

    bool rankSkill(const std::string& skillName, int points);

    bool PrerequisitesCheck(const std::vector<std::shared_ptr<Prerequisite>>& prerequisites);

    bool setBaseInfo(BaseInfo);

    void setAlignment(Alignment a);

    void setAbilityScores(AbilityPair pPair[6]);

    static std::shared_ptr<Character> getCharacter() {
        return character;
    }

private:

    SystemManager();

    static std::shared_ptr<SystemManager> instance;

    static std::shared_ptr<Character> character;

    static LevelUpFlags levelUpFlags;

    static std::vector<std::shared_ptr<Race>> races;

    static std::vector<std::shared_ptr<RuleClass>> classes;

    static std::vector<std::shared_ptr<Skill>> skills;

    static std::vector<std::shared_ptr<Feat>> feats;

    bool LoadRaces(const std::string& file_name = "../data_files/Races.xml");

    bool LoadSkills(const std::string& file_name = "../data_files/Skills.xml");

    bool LoadFeats(const std::string& file_name = "../data_files/Feats.xml");

    bool LoadClasses(const std::string& file_name = "../data_files/Classes.xml");

    void levelUpClass(const std::shared_ptr<RuleClass>& c);

    void newClass(const std::shared_ptr<RuleClass>& c);

    std::shared_ptr<Feat> searchFeat(const std::string& featName);

    void addCurrentLevelSkills(const std::vector<std::string>& s);
};