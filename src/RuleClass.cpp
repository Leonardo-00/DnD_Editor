//
// Created by leona on 22/06/2020.
//

#include "RuleClass.h"
#include <algorithm>
#include <utility>

RuleClass::RuleClass(std::string n, int maxL, int hD,
                     BabProgression babType, const SaveProgression saveB[SAVE_THROWS], int sP) :
             name(std::move(n)), maxLevel(maxL),
             hitDie(hD), bab(babType), skillPoints(sP){
    for (int i = 0; i < SAVE_THROWS; i++)
        saveBonuses[i] = saveB[i];
    initTable();
}

void RuleClass::initTable() {
    auto r = std::vector<std::string>();
    for (int i = 1; i <= maxLevel; i++) {
        specialTable.emplace(i, r);
    }
}

bool RuleClass::addFeature(const std::shared_ptr<Feature>& f) {
    for(auto &i: classFeatures)
        if(i == f)
            return false;
    classFeatures.push_back(f);
    return true;
}

bool RuleClass::addSkill(const std::string& s) {
    for(const auto& i: classSkills){
        if(i == s)
            return false;
    }
    classSkills.push_back(s);
    return true;
}

bool RuleClass::addPrerequisite(const std::shared_ptr<Prerequisite>& p) {
    for(const auto& i: prerequisites){
        if(i->operator==(p))
            return false;
    }
    prerequisites.push_back(std::shared_ptr<Prerequisite>(p));
    return true;
}

bool RuleClass::addProficiency(const Proficiency& p) {
    return proficiencies.addProficiency(p);
}

std::vector<Proficiency> RuleClass::getProficiencies(){
    return *proficiencies.getProficiencies();
}

const std::string &RuleClass::getName() const {
    return name;
}

void RuleClass::setName(const std::string &n) {
    RuleClass::name = n;
}

int RuleClass::getMaxLevel() const {
    return maxLevel;
}

void RuleClass::setMaxLevel(int mL) {
    RuleClass::maxLevel = mL;
}

int RuleClass::getHitDie() const {
    return hitDie;
}

void RuleClass::setHitDie(int hD) {
    RuleClass::hitDie = hD;
}

BabProgression RuleClass::getBab() const {
    return bab;
}

void RuleClass::setBab(BabProgression b) {
    RuleClass::bab = b;
}

const SaveProgression *RuleClass::getSaveBonuses() const {
    return saveBonuses;
}

int RuleClass::getSkillPoints() const {
    return skillPoints;
}

const std::vector<std::string> &RuleClass::getClassSkills() const {
    return classSkills;
}

const  std::vector<std::string> &RuleClass::getTableRow(int l) const{
    return specialTable.at(l);
}

const std::vector<std::shared_ptr<Prerequisite>> &RuleClass::getPrerequisites() const {
    return prerequisites;
}

void RuleClass::setSave(SaveProgression *s) {
    for(int i = 0; i < SAVE_THROWS; i++)
        saveBonuses[i] = s[i];
}

void RuleClass::setSkillPoints(int sP) {
    RuleClass::skillPoints = sP;
}

bool RuleClass::isLiteracy() const {
    return literacy;
}

void RuleClass::setLiteracy(bool l) {
    RuleClass::literacy = l;
}

std::vector<std::shared_ptr<Feature>> RuleClass::getFeatures() const{
    return classFeatures;
}

bool RuleClass::addTableRow(int level, const std::string& c) {
    auto& row = specialTable.at(level);
    for(auto &capacity: row){
        if(capacity == c)
            return false;
    }
    row.push_back(c);
    return true;
}

