//
// Created by leona on 05/07/2020.
//

#include <ctime>
#include <utility>
#include "CharClass.h"

CharClass::CharClass(std::string n, int l, int hP) : name(std::move(n)), level(l), hitPoints(hP){}

CharClass::CharClass(const std::shared_ptr<RuleClass>& c) {
name = c->getName();
level = 1;
hitPoints = 0;
bab.prog = c->getBab();
for(int i = 0; i < SAVE_THROWS; i++)
    saveBonuses[i].prog = (c->getSaveBonuses())[i];
update();
}

const std::string &CharClass::getName() const {
    return name;
}

void CharClass::setName(const std::string &n) {
    CharClass::name = n;
}

int CharClass::getLevel() const {
    return level;
}

int CharClass::getHitPoints() const {
    return hitPoints;
}

void CharClass::setHitPoints(int hP) {
    CharClass::hitPoints = hP;
}

const SaveBonus *CharClass::getSaveBonuses() const {
    return saveBonuses;
}

Bab CharClass::getBab() const {
    return bab;
}

void CharClass::setBab(Bab b) {
    CharClass::bab = b;
}

bool CharClass::operator==(const CharClass& c) {
    return name == c.getName();
}

void CharClass::update(){
    if(bab.prog == BabProgression::Low)
        bab.bonus = level/2;
    else if(bab.prog == BabProgression::Medium)
        bab.bonus = (level*3)/4;
    else
        bab.bonus = level;

    for(auto & saveBonus : saveBonuses){
        if(saveBonus.prog == SaveProgression::Slow)
            saveBonus.bonus = level/3;
        else
            saveBonus.bonus = (level/2)+2;
    }
}

bool CharClass::levelUp(int max) {
    if(level < max) {
        level++;
        return true;
    }
    else
        return false;
}

void CharClass::addHitPoints(int hitDie, int modifier){
    srand(time(nullptr));
    hitPoints += (rand() % hitDie +1) + modifier;
}

void CharClass::setSaveBonuses(SaveBonus *saves) {
    for(int i = 0; i < SAVE_THROWS; i++){
        saveBonuses[i] = saves[i];
    }
}
