//
// Created by leona on 05/07/2020.
//

#include <algorithm>
#include "Character.h"

std::shared_ptr<Character> Character::instance = nullptr;

const BaseInfo &Character::getBaseInfo() const {
    return baseInfo;
}

int Character::getTotalLevel() const {
    return totalLevel;
}

void Character::setTotalLevel(int tL) {
    Character::totalLevel = tL;
}

const AbilityPair *Character::getAbilityScores() const {
    return abilityScores;
}

const std::shared_ptr<Race> &Character::getRace() const {
    if(race != nullptr)
        return race;
    throw Editor_Exception("Race not set", false);
}

bool Character::setRace(const std::shared_ptr<Race>& r, bool loading) {
    if(race == nullptr)
    {
        race = r;
        if(!loading)
            setAbilityModifiers(race->abilityModifiers);
        for(const auto& language: race->automaticLanguages)
            addLanguage(language);
        maxBonusLanguages = abilityScores[3].modifier;
        for(const auto& trait: race->racialTraits)
            addTrait(trait);
        return true;
    }
    return false;
}

Size Character::getSize() const {
    return race->baseSize;
}

int Character::getSpeed() const {
    return race->baseSpeed + searchBonus("Land speed");
}

const std::string &Character::getDeity() const {
    return deity;
}

void Character::setDeity(const std::string &d) {
    Character::deity = d;
}

const std::vector<Language> &Character::getLanguages() const {
    return languages;
}

bool Character::isLiterate() const {
    return literacy;
}

void Character::setLiteracy(bool l) {
    Character::literacy = l;
}

void Character::setBaseInfo(const BaseInfo &bI) {
    Character::baseInfo = bI;
}

Alignment Character::getAlignment() const {
    return alignment;
}

void Character::setAlignment(Alignment a) {
    Character::alignment = a;
}

const std::vector<Proficiency> &Character::getProficiencies() const {
    return *proficiencies.getProficiencies();
}

SpecialCapacities Character::getSpecialCapacities() const {
    return specialCapacities;
}

std::vector<CharSkill> Character::getSkills() const {
    return skills;
}

std::vector<std::shared_ptr<CharClass>> Character::getClasses() const {
    return classes;
}

bool Character::isLiteracy() const {
    return literacy;
}

int Character::getTotalHitPoints() const {
    int hP = 0;
    for(const auto& i: classes)
        hP += i->getHitPoints();
    return hP;
}

int Character::getTotalBab() const {
    int bab = 0;
    for(const auto& i: classes)
        bab += i->getBab().bonus;
    return bab;
}

int* Character::getTotalSaveBonuses(){
    return totalSaveBonuses;
}

void Character::update(){
    totalLevel = 0;
    for(const auto& i:classes)
        totalLevel += i->getLevel();
    for(int & totalSaveBonus : totalSaveBonuses)
        totalSaveBonus = 0;
    for(const auto& charClass: classes){
        for(int i = 0; i < SAVE_THROWS; i++)
            totalSaveBonuses[i] += charClass->getSaveBonuses()[i].bonus;
    }
}

const CharCoins *Character::getMoney() const {
    return money;
}

bool Character::addProficiency(const Proficiency& p) {
    return proficiencies.addProficiency(p);
}

bool Character::addTrait(const std::shared_ptr<Capacity>& t) const{
    if(!t->source)
        return false;
    else
    {
        auto a = specialCapacities.racialTraits;
        for (const auto &i: a)
            if (i == t)
                return false;
        a.push_back(t);
        return true;
    }
}

bool Character::addLanguage(const Language& l) {
    int count = BonusLanguagesCount();
    if(!l.bonusLanguage || count < maxBonusLanguages)
    {
        for (const auto& i: languages)
        {
            if (i == l)
                return false;
        }
        languages.push_back(l);
        return true;
    }
    return false;
}

void Character::addFeat(const std::shared_ptr<Feat>& f) {
    auto &a = specialCapacities.feats;
    for (auto &i : a)
    {
        if ((i.feat)->operator==(f)) {
            i.occurrencies++;
            break;
        }
    }
    a.emplace_back(f);
}

bool Character::addSkill(CharSkill s) {
    for(auto& i: skills)
    {
        if(i == s)
            return false;
    }
    auto keyAbility = s.skill->ability;
    for(auto& ability: abilityScores)
    {
        if(ability.ability == keyAbility)
        {
            s.abilityBonus = &ability.modifier;
            break;
        }
    }
    skills.push_back(s);
    return true;
}

void Character::setAbilityScores(const AbilityPair aM[ABILITIES]){
    for(int i = 0; i < ABILITIES; i++)
    {
        abilityScores[i].score += aM[i].score;
        abilityScores[i].update();
    }
}

void Character::setAbilityModifiers(const AbilityPair *modifiers){
    for(int i = 0; i < ABILITIES; i++)
    {
        abilityScores[i].score += modifiers[i].score;
        abilityScores[i].update();
    }
}

void Character::increaseAbilityScore(Ability a){
    for (auto& i: abilityScores)
    {
        if (i.ability == a)
        {
            i.score++;
            i.update();
            break;
        }
    }
}

void Character::addCharClass(const std::shared_ptr<CharClass>& c){
    classes.push_back(c);
    update();
}

bool Character::addCapacity(const std::shared_ptr<Feature>& feature, const std::string& className) {
    auto& a = specialCapacities.classFeatures;
    if(a.empty())
    {
        CharFeature newCharFeature(feature->id, className);
        newCharFeature.getCapacity(feature);
        a.push_back(newCharFeature);
        return true;
    }
    else
    {
        for(auto &charFeature: a)
        {
            if(charFeature.id == feature->id)
            {
                charFeature.rank++;
                charFeature.getCapacity(feature);
                return true;
            }
        }
        CharFeature newCharFeature(feature->id, className);
        newCharFeature.getCapacity(feature);
        a.push_back(newCharFeature);
        return true;
    }
}

void Character::addFeature(const std::shared_ptr<Feature>& f, int rank, std::string className) {
    CharFeature newFeature(f->id, std::move(className));
    newFeature.rank = rank;
    newFeature.getCapacity(f);
    specialCapacities.classFeatures.push_back(newFeature);
}

std::shared_ptr<Character> Character::getCharacter() {
    if(instance == nullptr)
    {
        instance = std::make_shared<Character>(Character());
    }
    return instance;
}

void Character::reset() {
    if(instance != nullptr)
    {
        baseInfo = BaseInfo();
        totalLevel = 0;
        for(auto & abilityScore : abilityScores) {
            abilityScore.score = 0;
            abilityScore.modifier = 0;
        }
        for(auto & save: totalSaveBonuses)
            save = 0;
        alignment = Alignment();
        proficiencies.container->clear();
        race = nullptr;
        specialCapacities.racialTraits.clear();
        specialCapacities.feats.clear();
        specialCapacities.classFeatures.clear();
        skills.clear();
        classes.clear();
        deity = std::string();
        languages.clear();
        maxBonusLanguages = 0;
        literacy = false;
        for(auto& m: money)
            m.quantity = 0;
    }
}

CharFeat::CharFeat(std::shared_ptr<Feat> f) {
    this->feat = std::move(f);
    occurrencies = 1;
}

void CharFeature::getCapacity(const std::shared_ptr<Feature>& feature){
    if(typeid(*feature) == typeid(RankFeature))
        capacity = dynamic_cast<RankFeature*>(feature.get())->getCapacity(rank);
}

bool Character::rankUpSkill(const std::string& skillName, double points){
    int maxRank = totalLevel+3;
    for(auto& skill: skills)
    {
        if(skill.skill->name == skillName)
        {
            if(!skill.classSkill)
                maxRank /= 2;

            if(skill.rank + points <= maxRank)
            {
               skill.rank += points;
               return true;
            }
            return false;
        }
    }
    return false;
}

int Character::searchBonus(const std::string& type) const{
    int bonus = -100;
    std::vector<Bonus> bonuses;
    for(const auto& i : specialCapacities.feats)
    {
        if(typeid(*i.feat) == typeid(BonusFeat))
            maxBonus(dynamic_cast<BonusFeat *>(i.feat.get())->bonuses, type, &bonus);
    }
    for(const auto& i : specialCapacities.classFeatures)
    {
        if(typeid(*i.capacity) == typeid(StandardCapacity))
            maxBonus(dynamic_cast<StandardCapacity*>(i.capacity.get())->bonuses, type, &bonus);
    }
    for(const auto& i: specialCapacities.racialTraits)
    {
        if(typeid(*i) == typeid(StandardCapacity))
            maxBonus(dynamic_cast<StandardCapacity*>(i.get())->bonuses, type, &bonus);
    }
    if(bonus != -100)
        return bonus;
    else
        return 0;
}

void Character::maxBonus(const std::vector<Bonus>& bonuses, const std::string& type, int *value) {
    for (auto &b : bonuses)
        if (b.type == type && b.value > *value)
        {
            *value = b.value;
            break;
        }
}

void Character::setClassSkill(const std::string& skillName) {
    for (auto &skill: skills)
        if (skill.skill->name == skillName) {
            skill.classSkill = true;
            break;
        }
}

int Character::getSkillTotalBonus(const std::string& skillName) {
    for(const auto& skill: skills)
        if(skill.skill->name == skillName) {
            if(skill.skill->training && skill.rank == 0)
                throw Editor_Exception{"The character is not trained in the given skill", false};
            return skill.rank + *skill.abilityBonus;
        }
    throw Editor_Exception{"The researched skill doesn't exist", false};
}

int Character::getTotalAC(bool touch, bool reaction) {
    int AC = 10  + searchBonus("Deflection") + searchBonus("AC Enhancement") + searchBonus("General AC");
    if(reaction)
        AC += abilityScores[1].modifier + searchBonus("Dodge");
    if(!touch)
        AC += searchBonus("Natural"); //TODO add armor and shield AC bonuses
    return AC;
}

int Character::getMeleeAttackBonus() {
    return getTotalBab() + abilityScores[0].modifier;
}

int Character::getRangedAttackBonus() {
    return getTotalBab() + abilityScores[1].modifier;
}

int Character::getInitiative() {
    return abilityScores[1].modifier + searchBonus("Initiative");
}


