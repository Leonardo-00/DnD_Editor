//
// Created by leona on 05/07/2020.
//
#pragma once

#include <iostream>
#include "RuleClass.h"
#include "Head.h"
struct Bab{
    int bonus;
    BabProgression prog;

    Bab& operator=(Bab b){
        bonus = b.bonus;
        prog = b.prog;
        return *this;
    }
};
class RuleClass;

class CharClass {
public:

    CharClass(std::string name, int level, int hitPoints);

    explicit CharClass(const std::shared_ptr<RuleClass>& c);

    bool operator==(const CharClass& c);

    const std::string &getName() const;

    void setName(const std::string &name);

    int getLevel() const;

    int getHitPoints() const;

    void setHitPoints(int hitPoints);

    const SaveBonus *getSaveBonuses() const;

    void setSaveBonuses(SaveBonus* saves);

    Bab getBab() const;

    void setBab(Bab bab);

    void update();

    bool levelUp(int max);

    void addHitPoints(int hitDie, int modifier);

private:
    std::string name;
    int level;
    int hitPoints;
    SaveBonus saveBonuses[SAVE_THROWS];
    Bab bab;
};

