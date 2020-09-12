//
// Created by leona on 05/07/2020.
//

#ifndef DND_EDITOR_CHARACTER_H
#define DND_EDITOR_CHARACTER_H

#include <utility>
#include "memory"
#include "CharClass.h"
#include "Head.h"
#include "RuleClass.h"
class RuleClass;
class CharClass;

struct CharFeat {
    std::shared_ptr<Feat> feat;
    int occurrencies;
    explicit CharFeat(std::shared_ptr<Feat> f);
};

struct CharSkill {
    explicit CharSkill(std::shared_ptr<Skill> skill): skill(std::move(skill)){}
    std::shared_ptr<Skill> skill;
    double rank{0};
    int* abilityBonus = nullptr;
    bool classSkill{false};

    bool operator==(const CharSkill& s) const{
        return skill == s.skill;
    }

    bool operator < (const CharSkill& str) const
    {
        return skill < str.skill;
    }

    CharSkill& operator=(const CharSkill& Skill){
        this->skill = Skill.skill;
        this->rank = Skill.rank;
        *(this->abilityBonus) = *(Skill.abilityBonus);
        return *this;
    }
};
struct Feature;
struct CharFeature{
    explicit CharFeature(std::string id, std::string Class){
        this->id = std::move(id);
        this->Class = std::move(Class);
        rank = 1;
    }
    std::string id;
    std::string Class;
    int rank;
    std::shared_ptr<Capacity> capacity;
    void getCapacity(const std::shared_ptr<Feature>& feature);
};

struct SpecialCapacities{
    std::vector<CharFeat> feats;
    std::vector<CharFeature> classFeatures;
    std::vector<std::shared_ptr<Capacity>> racialTraits;
};

struct CharCoins {
    Coin coinType;
    int quantity = 0;

    CharCoins(Coin c): coinType(c){}

    CharCoins operator=(CharCoins coin){
        coinType = coin.coinType;
        quantity = coin.quantity;
        return coin;
    }
};
class Character{
public:

    static std::shared_ptr<Character> getCharacter();

    const BaseInfo &getBaseInfo() const;

    int getTotalLevel() const;

    void setTotalLevel(int totalLevel);

    const AbilityPair *getAbilityScores() const;

    void setAbilityModifiers(const AbilityPair* modifiers);

    void setAbilityScores(const AbilityPair* AbilityScores);

    void setAbilityScore(int index, int value){
        abilityScores[index - 1].score = value;
        abilityScores[index - 1].update();
    }

    const std::shared_ptr<Race> &getRace() const;

    bool setRace(const std::shared_ptr<Race>& r, bool loading = false);

    Size getSize() const;

    int getSpeed() const;

    const std::string &getDeity() const;

    void setDeity(const std::string &deity);

    const std::vector<Language> &getLanguages() const;

    bool isLiterate() const;

    void setLiteracy(bool literacy);

    const CharCoins *getMoney() const;

    bool addProficiency(const Proficiency& p);

    bool addTrait(const std::shared_ptr<Capacity>& trait) const;

    void addFeat(const std::shared_ptr<Feat>& f);

    bool addLanguage(const Language& language);

    void increaseAbilityScore(Ability a);

    void setBaseInfo(const BaseInfo &baseInfo);

    Alignment getAlignment() const;

    void setAlignment(Alignment alignment);

    const std::vector<Proficiency> &getProficiencies() const;

    SpecialCapacities getSpecialCapacities() const;

    std::vector<CharSkill> getSkills() const;

    std::vector<std::shared_ptr<CharClass>> getClasses() const;

    bool isLiteracy() const;

    int getTotalHitPoints() const;

    int getTotalBab() const;

    int* getTotalSaveBonuses();

    bool addSkill(CharSkill skill);

    void update();

    bool addCapacity(const std::shared_ptr<Feature>& feature, const std::string& className);

    void addFeature(const std::shared_ptr<Feature>& f, int rank, std::string className);

    void addCharClass(const std::shared_ptr<CharClass>& c);

    bool rankUpSkill(const std::string& skillName, double points = 1);

    void reset();

    int searchBonus(const std::string& type) const;

    int BonusLanguagesCount() {
        int count = 0;
        for (const auto& i: languages)
        {
            if(i.bonusLanguage)
                count++;
        }
        return count;
    }

    void setClassSkill(const std::string& skillName);

    int getSkillTotalBonus(const std::string& skillName);

    int getTotalAC(bool touch = false, bool reaction = true);

    int getMeleeAttackBonus();

    int getRangedAttackBonus();

    int getInitiative();

private:

    static std::shared_ptr<Character> instance;

    BaseInfo baseInfo;
    int totalLevel{0};
    AbilityPair abilityScores[ABILITIES]{ AbilityPair{Ability::Strength, 0, 0},
                                          AbilityPair{Ability::Dexterity, 0, 0},
                                          AbilityPair{Ability::Constitution, 0, 0},
                                          AbilityPair{Ability::Intelligence, 0, 0},
                                          AbilityPair{Ability::Wisdom, 0, 0},
                                          AbilityPair{Ability::Charisma, 0, 0}};
    int totalSaveBonuses[SAVE_THROWS]{0, 0, 0};
    std::shared_ptr<Race> race = nullptr;
    Alignment alignment{};
    ProficiencyContainer proficiencies;
    //TODO add inventory system
    SpecialCapacities specialCapacities;
    std::vector<CharSkill> skills;
    std::vector<std::shared_ptr<CharClass>> classes;
    std::string deity; //TODO adjust deity system
    std::vector<Language> languages;
    int maxBonusLanguages;
    bool literacy{false};
    CharCoins money[4]{CharCoins{Coin::Copper},
                       CharCoins{Coin::Silver},
                       CharCoins{Coin::Gold},
                       CharCoins{Coin::Platinum}};

    Character() = default;

    static void maxBonus(const std::vector<Bonus>& bonuses, const std::string& type, int* value);
};

#endif //DND_EDITOR_CHARACTER_H
