//
// Created by leona on 04/07/2020.
//
#pragma once
#include "memory"
#include "iostream"
#include <exception>
#include <utility>
#include <algorithm>
#include <vector>
#include <cmath>

static const int ABILITIES = 6;
static const int SAVE_THROWS = 3;

enum class Ability {
    Strength = 1, Dexterity, Constitution, Intelligence, Wisdom, Charisma
};

enum class Size {
    Tiny = 1, Small, Medium, Large, Huge, Gargantuan, Colossal
};

enum class className {
    Barbarian = 1, Bard, Cleric, Druid, Fighter, Monk, Paladin, Ranger, Rogue, Sorcerer, Wizard, Any
};

enum class Alignment {
    LG = 1, LN, LE, NG, N, NE, CG, CN, CE
};

enum class Coin {
    Copper = 1, Silver, Gold, Platinum
};

enum class ArmorPenalty {
    none = 1, once, twice
};

enum class BabProgression {
    Low = 1, Medium, High
};

enum class SaveProgression {
    Slow = 1, Fast
};

enum class gender {
    male = 1, female
};

struct Editor_Exception {
    std::string message;
    bool fatal;
    Editor_Exception(std::string m, bool f): message(std::move(m)), fatal(f){}
};

struct File_Exception : public Editor_Exception {
    std::string file_name;
    File_Exception(std::string n, std::string m, bool f): file_name(std::move(n)), Editor_Exception(std::move(m), f){}
};

struct Node_Exception : public Editor_Exception {
    std::string node_name;
    Node_Exception(std::string n, std::string m, bool f): node_name(std::move(n)), Editor_Exception(std::move(m), f){}
};

struct Enum_Exception: public Editor_Exception {
    std::string enumName;
    Enum_Exception(std::string n, std::string m, bool f): enumName(std::move(n)), Editor_Exception(std::move(m), f){}
};

struct AbilityPair {
    Ability ability;
    int score = 0;
    int modifier = 0;

    AbilityPair() = default;

    AbilityPair(Ability a, int s, int m): ability(a), score(s), modifier(m){}

    void update() {
        modifier = floor((double(score) - 10)/2);
    }
};

struct Language {
    Language(std::string l, std::string a, bool b) {
        language = std::move(l);
        alphabet = std::move(a);
        bonusLanguage = b;
    }
    std::string language;
    std::string alphabet;
    bool bonusLanguage;
    bool operator==(const Language& l) const {
        return language == l.language && alphabet == l.alphabet && bonusLanguage == l.bonusLanguage;
    }
};

struct SaveBonus {
    int bonus;
    SaveProgression prog;
};

struct Skill {
    std::string name;
    Ability ability;
    ArmorPenalty penalty;
    std::string description;
    bool training;

    bool operator==(Skill& s) const {
        return name == s.name
        /*&& ability == s.ability && penalty == s.penalty
               && description == s.description && training == s.training*/;
    }

    bool operator<(const std::shared_ptr<Skill>& s) const {
        return name < s->name;
    }

    std::string getName() const {
        return name;
    }
};

struct Bonus {
    std::string type;
    int value;

    bool operator==(const Bonus& b) const {
        return type == b.type && value == b.value;
    }

    bool operator!=(const Bonus& b) const {
        return !(*this == b);
    }
};

struct Proficiency {
    std::string type;
    std::string specs;

    bool operator==(const Proficiency& p) const {
        return type == p.type && specs == p.specs;
    }

    bool operator!=(const Proficiency& p) const {
        return !this->operator==(p);
    }
};

struct ProficiencyContainer {
    std::vector<Proficiency>* container{new std::vector<Proficiency>()};

    bool addProficiency(const Proficiency& p) const {
        for(const auto& i: *container) {
            if(i.operator==(p))
                return false;
        }
        container->push_back(p);
        if(p.specs == "ANY") {
            for (int i = 0; i < container->size(); i++) {
                if ((*container)[i].type == p.type && (*container)[i].specs != "ANY") {
                    container->erase(container->begin() + i);
                    i--;
                }
            }
        }
        return true;
    }

    std::vector<Proficiency>* getProficiencies() const {
        return container;
    }
};

struct BaseInfo {
    std::string characterName;
    std::string playerName;
    gender gender;
    int age;
    double height;
    double weight;
    std::string eyesColor;
    std::string hairColor;
    std::string skinColor;
};

struct physLimits {
    double minHeight;
    double maxHeight;
    double minWeight;
    double maxWeight;
};
namespace Head{
    template<typename T>
    bool contains(std::vector<T> container, T t) {
        for (auto &i: container) {
            if (i == t)
                return true;
        }
        return false;
    }

    template<typename T>
    void sort(std::vector<T> &container) {
        for (int i = 0; i < container.size(); i++) {
            for (int j = 0; j < container.size(); j++) {
                if (container[j] < container[i]) {
                    std::swap(container[i], container[j]);
                    j--;
                }
            }
        }
    }

    bool tagsEquality(std::vector<std::string> t1, std::vector<std::string> t2);

    std::string ability_to_string(Ability a);

    std::string size_to_string(Size s);

    Size string_to_size(std::string s);

    Ability string_to_ability(std::string s);

    bool string_to_bool(std::string s);

    std::string bool_to_string(bool b);

    std::string alignment_to_string(Alignment a);

    Alignment string_to_alignment(std::string s);

    std::string coin_to_string(Coin c);

    Coin string_to_coin(std::string s);

    ArmorPenalty string_to_penalty(std::string s);

    std::string penalty_to_string(ArmorPenalty a);

    BabProgression string_to_bab(std::string s);

    std::string bab_to_string(BabProgression b);

    SaveProgression string_to_save(std::string s);

    std::string save_to_string(SaveProgression p);

    std::string gender_to_string(gender g);

    gender string_to_gender(const std::string& s);

    std::string class_to_string(className n);

    className string_to_class(std::string s);

    bool checkDice(int d);

    template<typename T>
    bool sortByName(const T &d1, const T &d2) {
        return d1->getName() < d2->getName();
    }

    template<typename T>
    bool vectorEquality(const std::vector<T> &v1, const std::vector<T> &v2) {
        if (v1.size() == v2.size()) {
            for (int i = 0; i < v1.size(); i++) {
                if (v1[i].operator!=(v2[i]))
                    return false;
            }
            return true;
        }
        else
            return false;
    }

    template<typename T>
    bool getByName(const std::vector<std::shared_ptr<T>>& container, const std::string& name) {
        for(const auto& i: container)
            if(i->getName() == name)
                return true;
        return false;
    }
};

struct Capacity {
    std::string name;
    std::string description;
    bool source{}; // false for class, true for race

    virtual bool operator==(const std::shared_ptr<Capacity>& c) const {
        return name == c->name && description == c->description && source == c->source;
    }

    virtual bool operator!=(const std::shared_ptr<Capacity>& c) const = 0;
};

struct StandardCapacity: Capacity {
    std::vector<Bonus> bonuses;

    bool operator==(const std::shared_ptr<Capacity>& c) const override {
        if(typeid(*c) == typeid(StandardCapacity))
            return this->Capacity::operator==(c) && Head::vectorEquality(bonuses, dynamic_cast<StandardCapacity*>(c.get())->bonuses);
        else return false;
    }
    bool operator!=(const std::shared_ptr<Capacity>& c) const override {
        return !this->operator==(c);
    }
};

struct FeatCapacity: Capacity {
    std::string featName;

    bool operator==(const std::shared_ptr<Capacity>& c) const override {
        if(typeid(*c) == typeid(FeatCapacity))
            return this->Capacity::operator==(c) && featName == dynamic_cast<FeatCapacity*>(c.get())->featName;
        else
            return false;
    }
    bool operator!=(const std::shared_ptr<Capacity>& c) const override {
        return !this->operator==(c);
    }
};

struct FeatChoiceCapacity: Capacity {
    std::string choiceTag;

    bool operator==(const std::shared_ptr<Capacity>& c) const override {
        if(typeid(*c) == typeid(FeatChoiceCapacity))
            return this->Capacity::operator==(c) && choiceTag == dynamic_cast<FeatChoiceCapacity*>(c.get())->choiceTag;
        else
            return false;
    }
    bool operator!=(const std::shared_ptr<Capacity>& c) const override {
        return !this->operator==(c);
    }
};

struct ProficiencyCapacity: Capacity {
    std::vector<Proficiency> extraProficiencies;

    bool operator==(const std::shared_ptr<Capacity>& c) const override {
        if(typeid(*c) == typeid(ProficiencyCapacity))
            return this->Capacity::operator==(c) &&
                   Head::vectorEquality(extraProficiencies, dynamic_cast<ProficiencyCapacity*>(c.get())->extraProficiencies);
        else
            return false;
    }
    bool operator!=(const std::shared_ptr<Capacity>& c) const override {
        return !this->operator==(c);
    }
};

struct Race {
    std::string name;
    std::string description;  //FIXME maybe add a description class(?)
    physLimits physicalLimits{};
    Size baseSize;
    int baseSpeed{};            //in meters
    className favoredClass;
    AbilityPair abilityModifiers[ABILITIES]{AbilityPair{Ability::Strength, 0, 0},
                                            AbilityPair{Ability::Dexterity, 0, 0},
                                            AbilityPair{Ability::Constitution, 0, 0},
                                            AbilityPair{Ability::Intelligence, 0, 0},
                                            AbilityPair{Ability::Wisdom, 0, 0},
                                            AbilityPair{Ability::Charisma, 0, 0}};
    std::vector<Language> automaticLanguages;
    std::vector<Language> bonusLanguages;
    std::vector<std::shared_ptr<Capacity>> racialTraits;

    Race() = default;

    Race(std::string n, std::string d, physLimits m, Size s, int bS,
         std::string fC, AbilityPair aM[ABILITIES]) : name(std::move(n)), physicalLimits(m), description(std::move(d)),
         baseSize(s), baseSpeed(bS), favoredClass(Head::string_to_class(std::move(fC))){
        for(int i = 0; i < ABILITIES; i++)
            abilityModifiers[i].score = aM[i].score;
    }

    bool operator<(const Race& r) const{
        return name < r.name;
    }

    bool addTrait(const std::shared_ptr<Capacity>& trait){
        if(trait->source)
        {
            for(const auto& i: racialTraits)
                if(i == trait)
                    return false;
            racialTraits.push_back(trait);
            return true;
        }
        return false;
    }

    std::string getName() const {
        return name;
    }
};

struct Prerequisite {
    std::string description;

    explicit Prerequisite(std::string d): description(std::move(d)){}

    virtual bool operator==(std::shared_ptr<Prerequisite> p) = 0;

    virtual bool operator!=(std::shared_ptr<Prerequisite> p) = 0;

};

struct AbilityPrerequisite : Prerequisite {
    int minValue{};
    Ability ability;

    AbilityPrerequisite(int mV, Ability a, std::string d) : Prerequisite(std::move(d)), minValue(mV), ability(a){}

    bool operator==(std::shared_ptr<Prerequisite> p) override{
        if(typeid(*p) == typeid(AbilityPrerequisite))
        {
            auto aP = dynamic_cast<AbilityPrerequisite*>(p.get());
            return minValue == aP->minValue && ability == aP->ability && description == aP->description;
        }
        return false;
    }

    bool operator!=(std::shared_ptr<Prerequisite> p) override {
        return !this->operator==(p);
    }

};

struct AlignmentPrerequisite : Prerequisite {
    std::vector<Alignment> permittedAlignments;

    explicit AlignmentPrerequisite(std:: string d): Prerequisite(std::move(d)) {}

    bool addAlignment(Alignment a) {
        for(auto& alignment: permittedAlignments){
            if(alignment == a)
                return false;
        }
        permittedAlignments.push_back(a);
        return true;
    }

    bool operator==(std::shared_ptr<Prerequisite> p) override{
        if(typeid(*p).name() == typeid(AlignmentPrerequisite).name()){
            auto a = dynamic_cast<AlignmentPrerequisite*>(p.get());
            if(permittedAlignments.size() != a->permittedAlignments.size())
                return false;
            else
            {
                for(int i = 0; i < permittedAlignments.size(); i++){
                    if(permittedAlignments[i] != a->permittedAlignments[i])
                        return false;
                }
                return true;
            }
        }
        else
            return false;
    }

    bool operator!=(std::shared_ptr<Prerequisite> p) override{
        return !this->operator==(p);
    }

};

struct Feat {
    std::string name;
    std::string description;
    std::vector<std::string> tags;
    std::vector<std::shared_ptr<Prerequisite>> prerequisites;

    Feat(std::string n, std::string d): name(std::move(n)), description(std::move(d)){}

    bool isEqual(const Feat& f) const {
        auto a = this->prerequisites;
        auto b = f.prerequisites;
        if(a.size() == b.size()) {
            for(int i = 0; i < a.size(); i++){
                if(dynamic_cast<Prerequisite*>(a[i].get())->operator!=(b[i]))
                    return false;
            }
            return name == f.name && description == f.description && Head::tagsEquality(tags, f.tags);
        }
        return false;
    }

    bool addPrerequisite(const std::shared_ptr<Prerequisite>& p) {
        for(const auto& i: prerequisites){
            if(i->operator==(p))
                return false;
        }
        prerequisites.push_back(p);
        return true;
    }

    bool addTag(const std::string& t) {
        for(auto& featTag: tags)
            if(featTag == t)
                return false;
        tags.push_back(t);
        std::sort(tags.begin(), tags.end());
        return true;
    }

    virtual bool operator==(std::shared_ptr<Feat> f) {
        if (typeid(*f) != typeid(Feat)) {
            return false;
        } else {
            return this->isEqual(*f);
        }
    }

    bool operator!=(std::shared_ptr<Feat> f){
        return !this->operator==(f);
    }

    std::string getName() const {
        return name;
    }

};

struct BonusFeat: Feat {
    std::vector<Bonus> bonuses;
    bool choice;
    bool stack;

    BonusFeat(std::string n, std::string d, bool c, bool s): Feat(n, d), choice(c), stack(s){}

    bool addBonus(Bonus b){
        for(auto i: bonuses){
            if(i.operator==(b))
                return false;
        }
        bonuses.push_back(b);
        return true;
    }
};