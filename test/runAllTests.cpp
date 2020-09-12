#include "gtest/gtest.h"

#include "../src/SystemManager.h"

TEST(RaceTests, Trait_Insertion)
{
    auto r = SystemManager::getRaces()[0];
    auto c = r->racialTraits[0];
    c->source = false;
    ASSERT_FALSE(r->addTrait(c));
    c->source = true;
    ASSERT_FALSE(r->addTrait(c));
    c = std::make_shared<StandardCapacity>();
    c->source = true;
    c->name = "name";
    ASSERT_TRUE(r->addTrait(c));
}

TEST(ClassTests, Constructors)
{
    RuleClass c;
    ASSERT_EQ(c.getName(), "");
    ASSERT_EQ(c.getMaxLevel(), 0);
    c = RuleClass(10);
    ASSERT_EQ(c.getMaxLevel(), 10);
}

TEST(ClassTests, Skill_Insertion)
{
    RuleClass c(10);
    ASSERT_TRUE(c.addSkill("Skill1"));
    ASSERT_FALSE(c.addSkill("Skill1"));
}

TEST(ClassTests, SaveProgression_Insertion)
{
    RuleClass c(10);
    {
        SaveProgression s[SAVE_THROWS]{SaveProgression::Fast, SaveProgression::Slow, SaveProgression::Slow};
        c.setSave(s);
    }
    ASSERT_EQ(c.getSaveBonuses()[0], SaveProgression::Fast);
}

TEST(ClassTests, Capacity_Insertion)
{
    RuleClass c(10);
    ASSERT_TRUE(c.addTableRow(1, "Capacity"));
    ASSERT_FALSE(c.addTableRow(1, "Capacity"));
}

TEST(ClassTests, Prerequisite_Check)
{
    auto manager = SystemManager::getManager();
    auto c = SystemManager::getClasses()[0];
    auto Char = SystemManager::clearCharacter();
    Char->setAlignment(Alignment::LE);
    ASSERT_FALSE(manager->PrerequisitesCheck(c->getPrerequisites()));
    Char->setAlignment(Alignment::CE);
    ASSERT_TRUE(manager->PrerequisitesCheck(c->getPrerequisites()));
}

TEST(ClassTests, Proficiency_Insertion)
{
    Proficiency p{"Shields", "Light"};
    RuleClass c(10);
    ASSERT_TRUE(c.addProficiency(p));
    ASSERT_EQ(c.getProficiencies().size(), 1);
    ASSERT_FALSE(c.addProficiency(p));
    p.specs = "ANY";
    ASSERT_TRUE(c.addProficiency(p));
    ASSERT_EQ(c.getProficiencies().size(), 1);
}

TEST(FeatTests, Prerequisites_Check)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    auto f = manager->getFeats()[1];
    Char->setAbilityScore(1, 10);
    ASSERT_FALSE(manager->PrerequisitesCheck(f->prerequisites));
    Char->setAbilityScore(1, 13);
    ASSERT_TRUE(manager->PrerequisitesCheck(f->prerequisites));
}

TEST(FeatTests, Tag_Insertion)
{
    auto f = std::make_shared<Feat>("Feat name", "Feat description");
    f->addTag("A tag");
    ASSERT_EQ(f->tags[0], "A tag");
    f->addTag("A second tag");
    ASSERT_EQ(f->tags[0], "A second tag");
    f->addTag("A third tag");
    ASSERT_EQ(f->tags[2], "A third tag");
}

TEST(CharacterTests, Singleton_Check)
{
    auto manager = SystemManager::getManager();
    auto Char1 = Character::getCharacter();
    auto Char2 = Character::getCharacter();
    ASSERT_EQ(Char1, Char2);
    Char1->setRace(manager->getRaces()[0]);
    ASSERT_EQ(Char1, Char2);
}

TEST(CharacterTests, Race_Getter)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    try{
        Char->getRace();
    }
    catch (Editor_Exception& e) {
        ASSERT_EQ(e.message, "Race not set");
    }
    Char->setRace(manager->getRaces()[0]);
    ASSERT_EQ(Char->getRace()->name, "Dwarf");
}

TEST(CharacterTests, Race_Modifiers_Application_Check)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    ASSERT_EQ(Char->getAbilityScores()[2].score, 10);
    Char->setRace(manager->getRaces()[0]);
    ASSERT_EQ(Char->getAbilityScores()[2].score, 12);
}

TEST(CharacterTests, Bonus_Search)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->clearCharacter();
    ASSERT_EQ(Char->searchBonus("Swim"), 0);
    ASSERT_EQ(Char->searchBonus("Jump skill check"), 0);
    Char->addFeat(manager->getFeats()[0]);
    ASSERT_EQ(Char->searchBonus("Jump skill check"), 22);
}

TEST(CharacterTests, Bonus_Language_Maximum_Check)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    auto r = manager->getRaces()[0];
    Char->setRace(r);
    ASSERT_EQ(Char->getAbilityScores()[3].score, 13);
    ASSERT_EQ(Char->getLanguages().size(), 2);
    ASSERT_EQ(Char->BonusLanguagesCount(), 0);
    ASSERT_TRUE(Char->addLanguage(r->bonusLanguages[0]));
    ASSERT_EQ(Char->getLanguages().size(), 3);
    ASSERT_EQ(Char->BonusLanguagesCount(), 1);
    ASSERT_FALSE(Char->addLanguage(r->bonusLanguages[1]));
    ASSERT_EQ(Char->getLanguages().size(), 3);
    ASSERT_EQ(Char->BonusLanguagesCount(), 1);
}

TEST(CharacterTests, Skill_Ranking)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    manager->addClass(1);
    ASSERT_EQ(Char->getSkillTotalBonus("Climb"), 0);
    manager->rankSkill("Climb", 4);
    ASSERT_EQ(Char->getSkillTotalBonus("Climb"), 4);
}

TEST(CharacterTests, Score_increment)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    manager->addClass(1);
    ASSERT_EQ(Char->getSkillTotalBonus("Search"), 1);
    Char->increaseAbilityScore(Ability::Intelligence);
    ASSERT_EQ(Char->getSkillTotalBonus("Search"), 2);
}

TEST(CharacterTests, Correct_BAB_n_Save_Scaling)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    for(int i = 0; i < 20; i++){
        manager->addClass(1, true);
        ASSERT_EQ(Char->getTotalBab(), i+1);
        ASSERT_EQ(Char->getTotalSaveBonuses()[0], ((i+1)/2) + 2);
    }
}

TEST(CharacterTests, AC_variants)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    ASSERT_EQ(Char->getTotalAC(), 10);
    Char->setAbilityScore(2, 12);
    ASSERT_EQ(Char->getTotalAC(), 11);
    ASSERT_EQ(Char->getTotalAC(false, false), 10);
}

TEST(CharacterTests, Skill_Training)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    ASSERT_THROW(Char->getSkillTotalBonus("Open lock"), Editor_Exception);
}

TEST(CharacterModelTests, Inexistent_Skill)
{
    auto manager = SystemManager::getManager();
    auto Char = manager->LoadTestCharacter();
    try{
        manager->rankSkill("A certain skill", 42);
    }
    catch(Editor_Exception& e)
    {
        ASSERT_EQ(e.message, "The given skill does not exist");
    }
}

TEST(CharacterModelTests, Missing_File_Check)
{
    auto manager = SystemManager::getManager();
    try{
        manager->LoadCharacter("Gandalf");
    }
    catch (File_Exception& e) {
        ASSERT_EQ(e.message, "The file is not present in the given directory.");
        ASSERT_EQ(e.file_name, "Gandalf.xml");
    }
}

int main(int argc, char **argv)
{
    auto manager = SystemManager::getManager();
    try {
        manager->configure_test();
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    } catch (File_Exception& e) {
        std::clog << e.file_name << std::endl << e.message;
    }
}
