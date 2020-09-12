//
// Created by leona on 01/09/2020.
//

#include "Head.h"

bool Head::checkDice(int d) {
    for (int i = 4; i <= 12; i += 2) {
        if (i == d)
            return true;
    }
    return false;
}

std::string Head::ability_to_string(Ability a) {
    switch (int(a)) {
        case 1:
            return "Strength";
            break;
        case 2:
            return "Dexterity";
            break;
        case 3:
            return "Constitution";
            break;
        case 4:
            return "Intelligence";
            break;
        case 5:
            return "Wisdom";
            break;
        default:
            return "Charisma";
    }
}

std::string Head::size_to_string(Size s) {
    switch (int(s)) {
        case 1:
            return "Tiny";
            break;
        case 2:
            return "Small";
            break;
        case 3:
            return "Medium";
            break;
        case 4:
            return "Large";
            break;
        case 5:
            return "Huge";
            break;
        case 6:
            return "Gargantuan";
            break;
        default:
            return "Colossal";
    }
}

Size Head::string_to_size(std::string s) {
    if (s == "Tiny")
        return Size::Tiny;
    else if (s == "Small")
        return Size::Small;
    else if (s == "Medium")
        return Size::Medium;
    else if (s == "Large")
        return Size::Large;
    else if (s == "Huge")
        return Size::Huge;
    else if (s == "Gargantuan")
        return Size::Gargantuan;
    else
        return Size::Colossal;
}

Ability Head::string_to_ability(std::string s) {
    if (s == "Strength")
        return Ability::Strength;
    else if (s == "Dexterity")
        return Ability::Dexterity;
    else if (s == "Constitution")
        return Ability::Constitution;
    else if (s == "Intelligence")
        return Ability::Intelligence;
    else if (s == "Wisdom")
        return Ability::Wisdom;
    else
        return Ability::Charisma;
}

bool Head::string_to_bool(std::string s) {
    if (s == "true")
        return true;
    else
        return false;
}

std::string Head::bool_to_string(bool b) {
    if (b)
        return "true";
    else
        return "false";
}

std::string Head::alignment_to_string(Alignment a) {
    switch (int(a)) {
        case 1:
            return "Lawful Good";
            break;
        case 2:
            return "Lawful Neutral";
            break;
        case 3:
            return "Lawful Evil";
            break;
        case 4:
            return "Neutral Good";
            break;
        case 5:
            return "Pure Neutral";
            break;
        case 6:
            return "Neutral Evil";
            break;
        case 7:
            return "Chaotic Good";
            break;
        case 8:
            return "Chaotic Neutral";
            break;
        default:
            return "Chaotic Evil";
    }
}

Alignment Head::string_to_alignment(std::string s) {
    if (s == "Lawful Good")
        return Alignment::LG;
    else if (s == "Lawful Neutral")
        return Alignment::LN;
    else if (s == "Lawful Evil")
        return Alignment::LE;
    else if (s == "Neutral Good")
        return Alignment::NG;
    else if (s == "Pure Neutral")
        return Alignment::N;
    else if (s == "Neutral Evil")
        return Alignment::NE;
    else if (s == "Chaotic Good")
        return Alignment::CG;
    else if (s == "Chaotic Neutral")
        return Alignment::CN;
    else if (s == "Chaotic Evil")
        return Alignment::CE;
    //TODO exception : illegal value
    throw std::exception();
}

std::string Head::coin_to_string(Coin c) {
    switch (int(c)) {
        case 1:
            return "Copper";
        case 2:
            return "Silver";
        case 3:
            return "Gold";
        default:
            return "Platinum";
    }
}

Coin Head::string_to_coin(std::string s) {
    if (s == "Copper")
        return Coin::Copper;
    else if (s == "Silver")
        return Coin::Silver;
    else if (s == "Gold")
        return Coin::Gold;
    else
        return Coin::Platinum;
}

ArmorPenalty Head::string_to_penalty(std::string s) {
    if (s == "none")
        return ArmorPenalty::none;
    else if (s == "once")
        return ArmorPenalty::once;
    else
        return ArmorPenalty::twice;
}

std::string Head::penalty_to_string(ArmorPenalty a) {
    switch (int(a)) {
        case 1:
            return "none";
        case 2:
            return "once";
        default:
            return "twice";
    }
}

BabProgression Head::string_to_bab(std::string s) {
    if (s == "Low")
        return BabProgression::Low;
    else if (s == "Medium")
        return BabProgression::Medium;
    else
        return BabProgression::High;
}

std::string Head::bab_to_string(BabProgression b) {
    switch (int(b)) {
        case 1:
            return "Low";
        case 2:
            return "Medium";
        default:
            return "High";
    }
}

SaveProgression Head::string_to_save(std::string s) {
    if (s == "Slow")
        return SaveProgression::Slow;
    else
        return SaveProgression::Fast;
}

std::string Head::save_to_string(SaveProgression p) {
    switch (int(p)) {
        case 1:
            return "Slow";
        default:
            return "Fast";
    }
}

std::string Head::gender_to_string(gender g){
    switch(int(g)){
        case 1:
            return "Male";
        default:
            return "Female";
    }
}

gender Head::string_to_gender(const std::string& s){
    if(s == "Male")
        return gender::male;
    else
        return gender::female;
}

std::string Head::class_to_string(className c) {
    switch(int(c)){
        case 1:
            return "Barbarian";
            break;
        case 2:
            return "Bard";
            break;
        case 3:
            return "Cleric";
            break;
        case 4:
            return "Druid";
            break;
        case 5:
            return "Fighter";
            break;
        case 6:
            return "Paladin";
            break;
        case 7:
            return "Ranger";
            break;
        case 8:
            return "Rogue";
            break;
        case 9:
            return "Sorcerer";
            break;
        default:
            return "Wizard";
    }
}

className Head::string_to_class(std::string s) {
    if(s == "Barbarian")
        return className::Barbarian;
    else if(s == "Bard")
        return className::Bard;
    else if(s == "Cleric")
        return className::Cleric;
    else if(s == "Druid")
        return className::Druid;
    else if(s == "Fighter")
        return className::Fighter;
    else if(s == "Paladin")
        return className::Paladin;
    else if(s == "Ranger")
        return className::Ranger;
    else if(s == "Rogue")
        return className::Rogue;
    else if(s == "Sorcerer")
        return className::Sorcerer;
    else if(s == "Wizard")
        return className::Wizard;
    else if(s == "Any")
        return className::Any;
    else
        throw std::exception();
}

bool Head::tagsEquality(std::vector<std::string> t1, std::vector<std::string> t2){
    bool check = true;
    if(t1.size() != t2.size())
        check = false;
    else
    {
        for (int i = 0; i < t1.size(); i++)
            if (t1[i] != t2[i]){
                check = false;
                break;
            }
    }
    return check;
}