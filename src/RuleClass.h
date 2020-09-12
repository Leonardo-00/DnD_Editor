//
// Created by leona on 20/06/2020.
//
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "Head.h"

struct Feature {
    std::string id;
    virtual bool operator==(std::shared_ptr<Feature> f) const = 0;
};

struct RankFeature : Feature {
    std::vector<std::shared_ptr<Capacity>> levels;

    bool operator==(const std::shared_ptr<Feature> f) const override{
        if (typeid(*f) == typeid(RankFeature)) {
            auto a = dynamic_cast<RankFeature*>(f.get());
            if (levels.size() != a->levels.size())
                return false;
            else {
                for (int i = 0; i < levels.size(); i++)
                    if (levels[i] != a->levels[i])
                        return false;
                return id == f->id;
            }
        }
        else
            return false;
    }
    std::shared_ptr<Capacity> getCapacity(int rank){
        return levels[rank-1];
    }
};
class Character;
class Prerequisite;
class RuleClass {
public:

    RuleClass() = default;

    RuleClass(int maxLevel){
        this->maxLevel = maxLevel;
        initTable();
    }

    RuleClass(std::string n, int maxL, int hD, BabProgression babType, const SaveProgression saveB[SAVE_THROWS], int sP);

    void initTable();

    bool addFeature(const std::shared_ptr<Feature>& f);

    bool addSkill(const std::string& s);

    bool addPrerequisite(const std::shared_ptr<Prerequisite>& p);

    bool addProficiency(const Proficiency& p);

    std::vector<Proficiency> getProficiencies();

    const std::string &getName() const;

    void setName(const std::string &name);

    int getMaxLevel() const;

    void setMaxLevel(int maxLevel);

    int getHitDie() const;

    void setHitDie(int hitDie);

    BabProgression getBab() const;

    void setBab(BabProgression bab);

    const SaveProgression *getSaveBonuses() const;

    void setSave(SaveProgression* s);

    int getSkillPoints() const;

    const std::vector<std::string> &getClassSkills() const;

    const std::vector<std::string> &getTableRow(int level) const;

    const std::vector<std::shared_ptr<Prerequisite>> &getPrerequisites() const;

    bool operator<(const RuleClass& c){
        return name < c.name;
    }

    void setSkillPoints(int skillPoints);

    bool isLiteracy() const;

    void setLiteracy(bool literacy);

    std::vector<std::shared_ptr<Feature>> getFeatures() const;

    bool addTableRow(int level, const std::string& capacity);

private:
    std::string name{""};
    int maxLevel{0};
    int hitDie{0};
    bool literacy{false};
    BabProgression bab{};
    SaveProgression saveBonuses[SAVE_THROWS]{SaveProgression{}, SaveProgression{}, SaveProgression{}};
    int skillPoints{0}; // per level
    std::vector<std::string> classSkills;
    std::map<const int, std::vector<std::string>> specialTable;
    std::vector<std::shared_ptr<Feature>> classFeatures;
    std::vector<std::shared_ptr<Prerequisite>> prerequisites;
    ProficiencyContainer proficiencies;
};