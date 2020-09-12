//
// Created by leona on 21/08/2020.
//

#include "SystemManager.h"
#include "../pugixml-1.10/src/pugixml.hpp"

std::vector<std::shared_ptr<Race>> SystemManager::races = std::vector<std::shared_ptr<Race>>();
std::vector<std::shared_ptr<RuleClass>> SystemManager::classes = std::vector<std::shared_ptr<RuleClass>>();
std::vector<std::shared_ptr<Skill>> SystemManager::skills = std::vector<std::shared_ptr<Skill>>();
std::vector<std::shared_ptr<Feat>> SystemManager::feats = std::vector<std::shared_ptr<Feat>>();
std::shared_ptr<SystemManager> SystemManager::instance = nullptr;
std::shared_ptr<Character> SystemManager::character = Character::getCharacter();
LevelUpFlags SystemManager::levelUpFlags = LevelUpFlags();

void SystemManager::configure() {
    try{
        LoadRaces();
        LoadSkills();
        LoadClasses();
        LoadFeats();
        std::sort(races.begin(), races.end(), Head::sortByName<std::shared_ptr<Race>>);
        std::sort(classes.begin(), classes.end(), Head::sortByName<std::shared_ptr<RuleClass>>);
        std::sort(feats.begin(), feats.end(), Head::sortByName<std::shared_ptr<Feat>>);
    }
    catch(File_Exception& e)
    {
        std::clog << e.message << std::endl << e.file_name;
        if(e.fatal)
            exit(EXIT_FAILURE);
    }
    catch (Node_Exception& e) {
        std::clog << e.message << std::endl << e.node_name;
        if(e.fatal)
            exit(EXIT_FAILURE);
    }
}

void SystemManager::CharacterCreationSetup() {
    clearCharacter();
    for(auto& skill: skills){
        CharSkill cS(skill);
        character->addSkill(cS);
    }
}

bool SystemManager::LoadRaces(const std::string& file_name){
    pugi::xml_document doc;

    auto result = doc.load_file(file_name.c_str());
    if(result.status == pugi::status_ok) {
        pugi::xml_node root = (doc.child("races"));
        for (auto &iter : root) {
            auto a = iter.child("physicalLimits");
            physLimits p{std::stod(a.child_value("minHeight")), std::stod(a.child_value("maxHeight")),
                         std::stod(a.child_value("minWeight")), std::stod(a.child_value("maxWeight"))};
            auto* m = new AbilityPair[6];
            a = iter.child("abilityModifiers");
            int i = 0;
            for (auto &it : a) {
                m[i].ability = Head::string_to_ability(it.child("name").child_value());
                m[i].score = std::stoi(it.child("value").child_value());
                i++;
            }
            auto r = std::make_shared<Race>(iter.attribute("name").value(), iter.child_value("description"),
                                                             p, Head::string_to_size(iter.child_value("size")),
                                                             std::stoi(iter.child_value("speed")),
                                                             iter.child_value("favoredClass"), m);
            a = iter.child("autoLang");
            for (auto &it : a) {
                r->automaticLanguages.emplace_back(it.child_value("name"), it.child_value("alphabet"), false);
            }
            a = iter.child("bonusLang");
            for (auto &it : a) {
                r->bonusLanguages.emplace_back(it.child_value("name"), it.child_value("alphabet"), true);
            }
            a = iter.child("traits");
            for (auto &it : a) {
                std::string type = it.attribute("type").value();
                std::shared_ptr<Capacity> trait;
                if (type == "standard") {
                    trait = std::make_shared<StandardCapacity>();
                    auto c = dynamic_cast<StandardCapacity *>(trait.get());
                    auto d = it.child("bonuses");
                    for (auto &it2 : d) {
                        Bonus b{it2.child_value("type"), std::stoi(it2.child_value("value"))};
                        c->bonuses.push_back(b);
                    }
                }

                {
                    trait->name = it.child("name").child_value();
                    trait->description = it.child_value("description");
                    trait->source = true;
                }
                r->racialTraits.push_back(trait);
            }
            races.push_back(r);
        }
        return true;
    }
    else if(result.status == pugi::status_file_not_found)
        throw File_Exception{"The file containing the races to be loaded was not found.", "../../data_files/Races.xml", true};
    return false;
}

bool SystemManager::LoadSkills(const std::string& file_name){
    pugi::xml_document doc;
    auto result = doc.load_file(file_name.c_str());
    if(result.status == pugi::status_ok) {
        pugi::xml_node root = doc.child("skills");
        for (auto &iter : root) {
            Skill s{iter.child_value("name"), Head::string_to_ability(iter.child_value("keyAbility")),
                    Head::string_to_penalty(iter.child_value("penalty")), iter.child_value("description"),
                    Head::string_to_bool(iter.child_value("training"))};
            skills.emplace_back(std::make_shared<Skill>(s));
        }
        return true;
    }
    else if(result.status == pugi::status_file_not_found)
        throw File_Exception{"The file containing the skills to be loaded was not found.", "../../data_files/Skills.xml", true};
    return false;
}

bool SystemManager::LoadFeats(const std::string& file_name){
    pugi::xml_document doc;
    auto result = doc.load_file(file_name.c_str());
    if(result.status == pugi::status_ok) {
        pugi::xml_node root = doc.child("feats");
        for (auto &iter : root) {
            std::string s = iter.child_value("type");
            std::shared_ptr<Feat> f;
            if (s == "feat") {
                f = std::make_shared<Feat>(Feat(iter.child_value("name"),
                                                iter.child_value("description")));
            } else if (s == "bonus") {
                f = std::make_shared<BonusFeat>(BonusFeat(iter.child_value("name"),
                                                          iter.child_value("description"),
                                                          Head::string_to_bool(iter.child_value("stacks")),
                                                          Head::string_to_bool(iter.child_value("choice"))));
                auto bonuses = iter.child("bonuses");
                for (auto &bonusIter : bonuses) {
                    auto bonusFeat = dynamic_cast<BonusFeat *>(f.get());
                    bonusFeat->addBonus(
                            Bonus{bonusIter.child_value("type"), std::stoi(bonusIter.child_value("value"))});
                }
            } else
                throw Node_Exception{"Illegal value in the type attribute of a feat", iter.child_value("name"),true};
            auto a = iter.child("tags");
            std::string tag;
            for (auto &it: a) {
                tag = it.child_value();
                f->addTag(tag);
            }
            a = iter.child("prerequisites");
            for (auto &it : a) {
                s = it.child_value("type");
                if (s == "ability") {
                    std::shared_ptr<Prerequisite> p = std::make_shared<AbilityPrerequisite>(
                            AbilityPrerequisite(std::stoi(it.child_value("minValue")),
                                                Head::string_to_ability(it.child_value("ability")), ""));
                    f->addPrerequisite(p);
                }

            }
            feats.push_back(f);

        }
        return true;
    }
    else if(result.status == pugi::status_file_not_found)
        throw File_Exception{"The file containing the feats to be loaded was not found.", "../../data_files/Feats.xml", true};
    return false;
}

bool SystemManager::LoadClasses(const std::string& file_name){
    pugi::xml_document doc;

    auto result = doc.load_file(file_name.c_str());
    if(result.status == pugi::status_ok) {
        pugi::xml_node root = doc.child("classes");
        for (auto &iter : root) {
            std::shared_ptr<RuleClass> c = std::make_shared<RuleClass>();
            c->setName(iter.child("name").child_value());
            c->setMaxLevel(std::stoi(iter.child("maxLevel").child_value()));
            c->setHitDie(std::stoi(iter.child("hitDie").child_value()));
            c->setLiteracy(Head::string_to_bool(iter.child("literacy").child_value()));
            c->setBab(Head::string_to_bab(iter.child("babProgression").child_value()));
            SaveProgression save[SAVE_THROWS];
            int i = 0;
            auto a = iter.child("saveBonuses");
            for (auto &it : a) {
                save[i] = Head::string_to_save(it.child_value());
                i++;
            }
            c->setSave(save);
            c->setSkillPoints(std::stoi(iter.child("skillPoints").child_value()));
            a = iter.child("proficiencies");
            for (auto &it : a) {
                Proficiency p{it.child("type").child_value(), it.child("specs").child_value()};
                c->addProficiency(p);
            }
            {
                a = iter.child("classSkills");
                std::string skillName;
                for (auto &it : a) {
                    skillName = it.child_value();
                    if (Head::getByName(skills, skillName))
                        c->addSkill(skillName);
                }
            }
            a = iter.child("classFeatures");
            for (auto &it : a) {
                std::shared_ptr<Feature> feature = std::make_shared<RankFeature>();
                feature->id = it.child_value("id");
                auto ranks = it.child("ranks");
                for (auto &it2 : ranks) {
                    std::shared_ptr<Capacity> capacity = std::make_shared<StandardCapacity>();
                    capacity->name = it2.child_value("name");
                    capacity->description = it2.child_value("description");
                    capacity->source = false;
                    auto bonuses = it2.child("bonuses");
                    for (auto &bonusIt : bonuses) {
                        dynamic_cast<StandardCapacity *>(capacity.get())->bonuses.push_back(
                                Bonus{bonusIt.child_value("type"), std::stoi(bonusIt.child_value("value"))});
                    }
                    dynamic_cast<RankFeature *>(feature.get())->levels.push_back(capacity);
                }
                c->addFeature(feature);
            }
            c->initTable();
            a = iter.child("specialTable");
            for (auto &it : a) {
                int level = std::stoi(it.attribute("level").value());
                for (auto &it2 : it) {
                    c->addTableRow(level, it2.child_value());
                }
            }
            a = iter.child("prerequisites");
            for (auto &it: a) {
                std::string type = it.attribute("type").value();
                if (type == "alignment") {
                    std::shared_ptr<Prerequisite> prerequisite = std::make_shared<AlignmentPrerequisite>("");
                    auto b = dynamic_cast<AlignmentPrerequisite *>(prerequisite.get());
                    for (auto &alignment: it) {
                        Alignment permittedAlignment = Head::string_to_alignment(alignment.child_value());
                        b->addAlignment(permittedAlignment);
                    }
                    c->addPrerequisite(prerequisite);
                }
            }
            classes.push_back(c);
        }
        return true;
    }
    else if(result.status == pugi::status_file_not_found)
        throw File_Exception{"The file containing the classes to be loaded was not found.", "../../data_files/Classes.xml", true};
    return false;
}

bool SystemManager::SaveCharacter(const std::string& path) {
        const BaseInfo &s = character->getBaseInfo();
        //Construction of the xml file
        pugi::xml_document doc;
        auto root = doc.append_child("Character");
        pugi::xml_node nodeChild;
        //Meta info insertion
        {
            auto b = root.append_child("metaInfo");
            b.append_child("ability").append_child(pugi::node_pcdata).set_value(std::to_string(levelUpFlags.abilityIncrease).c_str());
            b.append_child("feat").append_child(pugi::node_pcdata).set_value(std::to_string(levelUpFlags.featChoice).c_str());
            b.append_child("skillPoints").append_child(pugi::node_pcdata).set_value(std::to_string(levelUpFlags.skillPoints).c_str());
            auto skillNode = b.append_child("levelSkills");
            for(auto& i: levelUpFlags.currentLevelSkills)
            {
                skillNode.append_child("skill").append_child(pugi::node_pcdata).set_value(i.c_str());
            }
        }
        //Base info insertion
        {
            auto b = root.append_child("baseInfo");
            b.append_child("characterName").append_child(pugi::node_pcdata).set_value(s.characterName.c_str());
            b.append_child("playerName").append_child(pugi::node_pcdata).set_value(s.playerName.c_str());
            b.append_child("gender").append_child(pugi::node_pcdata).set_value(Head::gender_to_string(s.gender).c_str());
            b.append_child("age").append_child(pugi::node_pcdata).set_value(std::to_string(s.age).c_str());
            b.append_child("height").append_child(pugi::node_pcdata).set_value(std::to_string(s.height).c_str());
            b.append_child("weight").append_child(pugi::node_pcdata).set_value(std::to_string(s.weight).c_str());
            b.append_child("eyes").append_child(pugi::node_pcdata).set_value(s.eyesColor.c_str());
            b.append_child("hair").append_child(pugi::node_pcdata).set_value(s.hairColor.c_str());
            b.append_child("skin").append_child(pugi::node_pcdata).set_value(s.skinColor.c_str());
        }
        //Total level insertion
        {
            nodeChild = root.append_child("totalLevel");
            nodeChild.append_child(pugi::node_pcdata).set_value(std::to_string(character->getTotalLevel()).c_str());
        }
        //Ability scores insertion
        {
            nodeChild = root.append_child("abilityScores");
            auto scores = character->getAbilityScores();
            pugi::xml_node abilityNode;
            for (int i = 0; i < ABILITIES; i++) {
                abilityNode = nodeChild.append_child("ability");
                abilityNode.append_child("name").append_child(pugi::node_pcdata).set_value(
                        Head::ability_to_string(scores[i].ability).c_str());
                abilityNode.append_child("value").append_child(pugi::node_pcdata).set_value(
                        std::to_string(scores[i].score).c_str());
            }
        }
        //Race insertion
        {
            nodeChild = root.append_child("race");
            nodeChild.append_child(pugi::node_pcdata).set_value(character->getRace()->name.c_str());
        }
        //Alignment insertion
        {
            nodeChild = root.append_child("alignment");
            nodeChild.append_child(pugi::node_pcdata).set_value(Head::alignment_to_string(character->getAlignment()).c_str());
        }
        //Size and speed insertion
        {
            nodeChild = root.append_child("size");
            nodeChild.append_child(pugi::node_pcdata).set_value(Head::size_to_string(character->getSize()).c_str());
            nodeChild = root.append_child("speed");
            nodeChild.append_child(pugi::node_pcdata).set_value(std::to_string(character->getSpeed()).c_str());
        }
        //Proficiencies insertion
        {
            nodeChild = root.append_child("proficiencies");
            pugi::xml_node proficiency;
            for (const auto& charProficiency: character->getProficiencies()) {
                proficiency = nodeChild.append_child("proficiency");
                proficiency.append_child("type").append_child(pugi::node_pcdata).set_value(
                        charProficiency.type.c_str());
                proficiency.append_child("specs").append_child(pugi::node_pcdata).set_value(
                        charProficiency.specs.c_str());
            }
        }
        //specialCapacities insertion
        {
            auto specialCapacitiesNode = root.append_child("specialCapacities");
            //Feat insertion
            {
                auto featsNode = specialCapacitiesNode.append_child("feats");
                pugi::xml_node feat;
                auto charFeats = character->getSpecialCapacities().feats;
                for (const auto &charFeat: charFeats) {
                    feat = featsNode.append_child("feat");
                    feat.append_attribute("occurrencies").set_value(std::to_string(charFeat.occurrencies).c_str());
                    feat.append_child("name").append_child(pugi::node_pcdata).set_value(
                            charFeat.feat->name.c_str());
                    if (typeid(BonusFeat) == typeid(*charFeat.feat)) {
                        feat.append_attribute("type").set_value("bonus");
                    } else
                        feat.append_attribute("type").set_value("feat");
                }
            }
            //RuleClass features insertion
            {
                auto featuresNode = specialCapacitiesNode.append_child("classFeatures");
                pugi::xml_node feature;
                auto charFeatures = character->getSpecialCapacities().classFeatures;
                for (const auto &charFeature: charFeatures) {
                    feature = featuresNode.append_child("feature");
                    feature.append_child("id").append_child(pugi::node_pcdata).set_value(charFeature.id.c_str());
                    feature.append_child("class").append_child(pugi::node_pcdata).set_value(charFeature.Class.c_str());
                    feature.append_child("rank").append_child(pugi::node_pcdata).set_value(
                            std::to_string(charFeature.rank).c_str());
                }
            }
        }
        //Skills insertion
        {
            nodeChild = root.append_child("skills");
            const auto &charSkills = character->getSkills();
            pugi::xml_node skill;
            for (const auto &charSkill: character->getSkills()) {
                skill = nodeChild.append_child("skill");
                skill.append_child("name").append_child(pugi::node_pcdata).set_value(charSkill.skill->name.c_str());
                skill.append_child("rank").append_child(pugi::node_pcdata).set_value(
                        std::to_string(charSkill.rank).c_str());
                skill.append_child("classSkill").append_child(pugi::node_pcdata).set_value(
                        Head::bool_to_string(charSkill.classSkill).c_str());
            }

        }
        //Classes insertion
        {
            nodeChild = root.append_child("classes");
            pugi::xml_node Class;
            pugi::xml_node charSaves;
            for (const auto &charClass: character->getClasses()) {
                Class = nodeChild.append_child("class");
                Class.append_child("name").append_child(pugi::node_pcdata).set_value(charClass->getName().c_str());
                Class.append_child("level").append_child(pugi::node_pcdata).set_value(
                        std::to_string(charClass->getLevel()).c_str());
                Class.append_child("hitPoints").append_child(pugi::node_pcdata).set_value(
                        std::to_string(charClass->getHitPoints()).c_str());
                charSaves = Class.append_child("saveThrows");
                pugi::xml_node charSave;
                auto saveThrows = charClass->getSaveBonuses();
                for (int i = 0; i < SAVE_THROWS; i++) {
                    charSave = charSaves.append_child("saveThrow");
                    charSave.append_child("bonus").append_child(pugi::node_pcdata).set_value(
                            std::to_string(saveThrows[i].bonus).c_str());
                    charSave.append_child("progression").append_child(pugi::node_pcdata).set_value(
                            Head::save_to_string(saveThrows[i].prog).c_str());
                }
                pugi::xml_node bab = Class.append_child("bab");
                bab.append_child("bonus").append_child(pugi::node_pcdata).set_value(
                        std::to_string(charClass->getBab().bonus).c_str());
                bab.append_child("prog").append_child(pugi::node_pcdata).set_value(
                        Head::bab_to_string(charClass->getBab().prog).c_str());
            }
        }
        //Languages insertion
        {
            nodeChild = root.append_child("languages");
            auto charLanguages = character->getLanguages();
            pugi::xml_node language;
            for (const auto &charLanguage: charLanguages) {
                language = nodeChild.append_child("language");
                language.append_child("name").append_child(pugi::node_pcdata).set_value(charLanguage.language.c_str());
                language.append_child("alphabet").append_child(pugi::node_pcdata).set_value(
                        charLanguage.alphabet.c_str());
                language.append_child("bonusLanguage").append_child(pugi::node_pcdata).set_value(
                        std::to_string(charLanguage.bonusLanguage).c_str());
            }
        }
        //Literacy insertion
        {
            nodeChild = root.append_child("literacy");
            nodeChild.append_child(pugi::node_pcdata).set_value(std::to_string(character->isLiteracy()).c_str());
        }
        //Money insertion
        {
            auto money = root.append_child("money");
            auto charMoney = character->getMoney();
            pugi::xml_node coin;
            for (int i = 0; i < 4; i++) {
                coin = money.append_child("coin");
                coin.append_child("coinType").append_child(pugi::node_pcdata).set_value(
                        Head::coin_to_string(charMoney[i].coinType).c_str());
                coin.append_child("quantity").append_child(pugi::node_pcdata).set_value(
                        std::to_string(charMoney[i].quantity).c_str());
            }
        }
        //Save command

        doc.save_file((path + character->getBaseInfo().characterName + ".xml").c_str());
        return true;
}

bool SystemManager::LoadCharacter(const std::string& char_name, const std::string& path) {
    pugi::xml_document doc;
    auto result = doc.load_file((path+char_name+".xml").c_str());
    if(result.status == pugi::status_ok) {
        clearCharacter();
        auto Char = character;
        auto root = doc.child("Character");

        //Meta info loading
        {
            auto meta = root.child("metaInfo");
            levelUpFlags.abilityIncrease = Head::string_to_bool(meta.child_value("ability"));
            levelUpFlags.featChoice = Head::string_to_bool(meta.child_value("feat"));
            levelUpFlags.skillPoints = std::stoi(meta.child_value("skillPoints"));
            meta = meta.child("levelSkills");
            levelUpFlags.currentLevelSkills.clear();
            for(auto & it : meta) {
                levelUpFlags.currentLevelSkills.emplace_back(it.child_value());
            }
        }
        //Base info loading
        {
            auto info = root.child("baseInfo");
            BaseInfo s{info.child_value("characterName"), info.child_value("playerName"),
                       Head::string_to_gender(info.child_value("gender")),
                       std::stoi(info.child_value("age")),
                       std::stod(info.child_value("height")), std::stod(info.child_value("weight")),
                       info.child_value("eyes"),
                       info.child_value("hair"), info.child_value("skin")};
            character->setBaseInfo(s);
        }
        //Total level loading
        {
            character->setTotalLevel(std::stoi(root.child_value("totalLevel")));
        }
        //Scores loading
        {
            auto scores = root.child("abilityScores");
            AbilityPair charScores[ABILITIES];
            int i = 0;
            for (auto &score : scores) {
                charScores[i].ability = Head::string_to_ability(score.child_value("name"));
                charScores[i].score = std::stoi(score.child_value("value"));
                i++;
            }
            character->setAbilityScores(charScores);
        }
        //Race loading
        {
            std::string charRace = root.child_value("race");
            if(!charRace.empty()) {
                bool validRace = false;
                for (const auto &race: races) {
                    if (race->name == charRace) {
                        character->setRace(race, true);
                        validRace = true;
                    }
                }
                if (!validRace) {
                    throw Editor_Exception{"The race "+ charRace + " does not exist.", true};
                }
            }
        }
        //Alignment loading
        {
            character->setAlignment(Head::string_to_alignment(root.child_value("alignment")));
        }
        //Proficiencies loading
        {
            auto proficiencies = root.child("proficiencies");
            for (auto &proficiency: proficiencies) {
                character->addProficiency(Proficiency{proficiency.child_value("type"),
                                                         proficiency.child_value("specs")});
            }
        }
        //Special capacities loading
        {
            auto specialCapacities = root.child("specialCapacities");
            bool loadResult;
            //Feats loading
            {
                auto charFeats = specialCapacities.child("feats");
                loadResult = false;
                for (auto &charFeat: charFeats) {
                    loadResult = false;
                    for (const auto &feat: feats) {
                        if (feat->name == charFeat.child_value("name")) {
                            character->addFeat(feat);
                            loadResult = true;
                        }
                    }
                    if (!loadResult)
                        throw std::exception();
                }
            }
            //RuleClass features loading
            {
                auto charFeatures = specialCapacities.child("classFeatures");
                for (auto &charFeature: charFeatures) {
                    loadResult = false;
                    std::string className = charFeature.child_value("class");
                    std::string feature = charFeature.child_value("id");
                    int rank = std::stoi(charFeature.child_value("rank"));
                    for (const auto &Class: classes) {
                        if (Class->getName() == className) {
                            for (const auto &classFeature: Class->getFeatures()) {
                                if (classFeature->id == feature) {
                                    character->addFeature(classFeature, rank, className);
                                    loadResult = true;
                                }
                            }
                        }
                    }
                    if(!loadResult)
                        throw std::exception();
                }
            }
        }
        //Languages loading
        {
            auto languages = root.child("languages");
            for (auto &language: languages) {
                Language charLanguage(language.child_value("name"), language.child_value("alphabet"),
                                      Head::string_to_bool(language.child_value("bonusLanguage")));
                character->addLanguage(charLanguage);
            }
        }
        //Skill loading
        {
            auto charSkills = root.child("skills");
            for (auto &charSkill: charSkills) {
                for (const auto &skill: skills) {
                    if (charSkill.child_value("name") == skill->name) {
                        CharSkill newCharSkill(skill);
                        newCharSkill.rank = std::stod(charSkill.child_value("rank"));
                        newCharSkill.classSkill = Head::string_to_bool(charSkill.child_value("classSkill"));
                        character->addSkill(newCharSkill);
                        break;
                    }
                }
            }
        }
        //Classes loading
        {
            auto charClasses = root.child("classes");
            for (auto &charClass: charClasses) {
                std::shared_ptr<CharClass> newCharClass = std::make_shared<CharClass>(charClass.child_value("name"),
                                                                                      std::stoi(charClass.child_value("level")),
                                                                                      std::stoi(charClass.child_value("hitPoints")));
                auto charSaves = charClass.child("saveThrows");
                SaveBonus newSaves[SAVE_THROWS];
                int i = 0;
                for (auto &charSave: charSaves) {
                    newSaves[i].bonus = std::stoi(charSave.child_value("bonus"));
                    newSaves[i].prog = Head::string_to_save(charSave.child_value("progression"));
                    i++;
                }
                newCharClass->setSaveBonuses(newSaves);
                auto charBab = charClass.child("bab");
                Bab newBab{};
                newBab.bonus = std::stoi(charBab.child_value("bonus"));
                newBab.prog = Head::string_to_bab(charBab.child_value("prog"));
                newCharClass->setBab(newBab);
                character->addCharClass(newCharClass);
            }
        }
        //Literacy loading
        {
            character->setLiteracy(root.child_value("literacy"));
        }
        return true;
    }
    throw File_Exception{char_name+".xml", "The file is not present in the given directory.", false};
}

bool SystemManager::addClass(int index, bool f){
    auto c = classes[index - 1];
    if((PrerequisitesCheck(c->getPrerequisites()) && canLevelUp()) || f){
        if(!character->getClasses().empty())
            levelUpClass(c);
        else
            newClass(c);
        character->update();
        addCurrentLevelSkills(c->getClassSkills());
        int totalLevel = character->getTotalLevel();
        if(totalLevel % 4 == 0)
            levelUpFlags.abilityIncrease = true;
        if(totalLevel % 3 == 0 || totalLevel == 1)
            levelUpFlags.featChoice = true;
        return true;
    }
    return false;
}

bool SystemManager::PrerequisitesCheck(const std::vector<std::shared_ptr<Prerequisite>>& prerequisites){
    for(const auto& p: prerequisites)
    {
        if(typeid(*p) == typeid(AbilityPrerequisite)) {
            auto P = dynamic_cast<AbilityPrerequisite*>(p.get());
            auto scores = character->getAbilityScores();
            for(int i = 0; i < ABILITIES; i++)
                if(scores[i].ability == P->ability && scores[i].score < P->minValue)
                    return false;
        }
        else if(typeid(*p) == typeid(AlignmentPrerequisite)) {
            auto P = dynamic_cast<AlignmentPrerequisite*>(p.get());
            if(!Head::contains(P->permittedAlignments, character->getAlignment()))
                return false;
        }
    }
    return true;
}

bool SystemManager::addFeat(int index, bool pass = false){
    if(pass || PrerequisitesCheck(feats[index - 1]->prerequisites)) {
        character->addFeat(feats[index - 1]);
        return true;
    }
    return false;
}

void SystemManager::setRace(int index) {
    character->setRace(races[index-1]);
}

SystemManager::SystemManager() {
    character = Character::getCharacter();
}

void SystemManager::levelUpClass(const std::shared_ptr<RuleClass>& c) {
    auto Char = character;
    for (const auto& i: character->getClasses()) {
        if (i->getName() == c->getName()) {
            if(i->levelUp(c->getMaxLevel())) {
                auto newFeatures = c->getTableRow(i->getLevel());
                bool result;
                auto charFeatures = character->getSpecialCapacities().classFeatures;
                for (const auto &newFeature: newFeatures) {
                    result = false;
                    for (auto &charFeature: charFeatures) {
                        if (charFeature.id == newFeature) {
                            charFeature.rank++;
                            result = true;
                        }
                    }
                    if (!result) {
                        CharFeature newCharFeature(newFeature, c->getName());
                        charFeatures.push_back(newCharFeature);
                    }
                }
                for (auto &charFeature: charFeatures) {
                    for (const auto &classFeature: c->getFeatures()) {
                        if (charFeature.id == classFeature->id) {
                            charFeature.getCapacity(classFeature);
                        }
                    }
                }
                i->update();
                auto abilityScores = character->getAbilityScores();
                i->addHitPoints(c->getHitDie(), abilityScores[2].modifier);
                levelUpFlags.skillPoints += c->getSkillPoints() + abilityScores[3].modifier;
            }
        }
    }
}

void SystemManager::newClass(const std::shared_ptr<RuleClass> &c) {
    std::shared_ptr<CharClass> newClass = std::make_shared<CharClass>(c);
    {
    auto abilityScores = character->getAbilityScores();
    if(character->getTotalLevel() == 0) {
        newClass->setHitPoints(c->getHitDie() + abilityScores[2].modifier);
        levelUpFlags.skillPoints += 4 * (c->getSkillPoints() + abilityScores[3].modifier);
    }
    else {
        newClass->addHitPoints(c->getHitDie(), abilityScores[2].modifier);
        levelUpFlags.skillPoints += c->getSkillPoints() + abilityScores[3].modifier;
    }
    }
    character->addCharClass(newClass);
    {
        auto newProficiencies = c->getProficiencies();
        for (const auto &proficiency: newProficiencies)
            character->addProficiency(proficiency);
    }
    if(!character->isLiterate()) {
        character->setLiteracy(c->isLiteracy());
    }
    {
        auto newFeatures = c->getTableRow(1);
        for (auto &capacity: newFeatures) {
            for (auto &feature: c->getFeatures()) {
                if (feature->id == capacity)
                    character->addCapacity(feature, c->getName());
            }
        }
    }
}

std::shared_ptr<Feat> SystemManager::searchFeat(const std::string& featName) {
    for(const auto& i: feats)
        if(i->name == featName)
            return i;
    return nullptr;
}

bool SystemManager::canLevelUp(){
    return character->getTotalLevel() <= 19 && !levelUpFlags.featChoice && !levelUpFlags.abilityIncrease && levelUpFlags.skillPoints == 0;
}

bool SystemManager::rankSkill(const std::string& skillName, int points) {
    if(Head::getByName(skills, skillName)) {
        if (points <= levelUpFlags.skillPoints) {
            double rankPerPoint = .5;
            if (Head::contains(levelUpFlags.currentLevelSkills, skillName))
                rankPerPoint = 1;
            if(character->rankUpSkill(skillName, points * rankPerPoint)) {
                levelUpFlags.skillPoints -= points;
                return true;
            }
            else
                return false;
        }
        return false;
    }
    throw Editor_Exception{"The given skill does not exist", false};
}

void SystemManager::addCurrentLevelSkills(const std::vector<std::string> &s) {
    levelUpFlags.currentLevelSkills.clear();
    for(auto& i: s)
    {
        character->setClassSkill(i);
        levelUpFlags.currentLevelSkills.push_back(i);
        for (auto &charSkill: character->getSkills())
        {
            if (charSkill.skill->name == i)
            {
                charSkill.classSkill = true;
                break;
            }
        }
    }
}

void SystemManager::configure_test() {
    LoadRaces("../../data_files/Races.xml");
    LoadFeats("../../data_files/Feats.xml");
    LoadSkills("../../data_files/Skills.xml");
    LoadClasses("../../data_files/Classes.xml");
    LoadTestCharacter();
    std::sort(races.begin(), races.end());
    std::sort(classes.begin(), classes.end());
    std::sort(feats.begin(), feats.end(), Head::sortByName<std::shared_ptr<Feat>>);
}

bool SystemManager::setBaseInfo(BaseInfo b) {
    if(character->getBaseInfo().characterName.empty()) {
        character->setBaseInfo(b);
        return true;
    }
    return false;

}

void SystemManager::setAlignment(Alignment a) {
    character->setAlignment(a);
}

void SystemManager::setAbilityScores(AbilityPair *pPair) {
    character->setAbilityScores(pPair);
}

std::shared_ptr<SystemManager> SystemManager::getManager() {
    if(instance == nullptr)
        instance = std::make_shared<SystemManager>(SystemManager());
    return instance;
}
