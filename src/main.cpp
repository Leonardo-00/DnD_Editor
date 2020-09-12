#include "SystemManager.h"

int main() {
    /*auto manager = SystemManager::getManager();
    manager->configure();
    auto Char = manager->getCharacter();


    manager->CharacterCreationSetup();
    *//* First step -- Race selection
     * The GUI will show all the possible races, with a button that will open a window that contains
     * all the information about the race, and a button to choose the race.                        *//*

    // for a demonstrative purpose we take a race arbitrarily from the container of the class
    auto example_race = 1;
    manager->setRace(example_race);
    std::cout << "Race of the character: "+Char->getRace()->name << std::endl;
    *//* Second step -- Insertion of the character base information
     * The GUI will show an interface with several fields to fill for the character name, name of the player, ecc.
     * The program will verify that the physical dimensions of the character meet the limits of the race, but it won't
     * be forbidden to exceed them, just a signal will be shown                                *//*
    double height = 200;
    double weight = 400;
    BaseInfo s{"Gimli", "Leonardo", gender::male, 262, height, weight,
               "Brown", "Red", "Pink"};
    manager->setBaseInfo(s);
    std::cout << "Character name: "+Char->getBaseInfo().characterName << std::endl << "Player name: "+Char->getBaseInfo().playerName << std::endl;
    manager->setAlignment(Alignment::CG);

    *//* Third step -- Ability scores determination
     * There are several ways to determinate the ability scores of a character, the client(player) will be asked
     * to choose the method he prefers.                                                             *//*
    AbilityPair scores[ABILITIES];
    {
        scores[0].score = 16;
        scores[0].update();
        scores[1].score = 12;
        scores[1].update();
        scores[2].score = 18;
        scores[2].update();
        scores[3].score = 10;
        scores[3].update();
        scores[4].score = 14;
        scores[4].update();
        scores[5].score = 10;
        scores[5].update();
    }
    manager->setAbilityScores(scores);
    auto q = Char->getAbilityScores();
    for(int i = 0; i < ABILITIES; i++)
        std::cout << "Ability: " << Head::ability_to_string(q[i].ability) << std::endl << "Score: " << q[i].score << std::endl << "Modifier: " << q[i].modifier << std::endl;
    //Now we add the first class to the character
    manager->addClass(1);
    manager->SaveCharacter();*/
    auto manager = SystemManager::getManager();
    manager->configure();
    auto c = SystemManager::getClasses()[0];
    auto Char = SystemManager::clearCharacter();
    Char->setAlignment(Alignment::LE);
    auto b = manager->PrerequisitesCheck(c->getPrerequisites());
    Char->setAlignment(Alignment::CE);
    b = manager->PrerequisitesCheck(c->getPrerequisites());
}