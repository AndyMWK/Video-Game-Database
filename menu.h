#ifndef MENU_H
#define MENU_H

#include "game.h"
#include "database.h"
#include <ncurses.h>
#include <iostream>
#include <ncurses.h>

using namespace std;

class Menu {
    private:
    //user input
    string userChoice;

    //max and min for ranged searches
    int low;
    
    int high;
    
    //checking input
    void checkInput();

    //options for menu
    vector <string> choiceMain;

    vector <string> choiceField;
    

    public:

    //check for valid input of integers
    bool validInputInt(string n);
    
    Menu();

    //for getting response from user

    string getString();

    string getStringHighlight(int option, int y, int x);

    //for searching
    string mainMenu();

    string fieldMenu(int instructions);

    string searchChoiceString();

    string searchChoiceInt();

    void rangeIntSearch();

    int giveLow();

    int giveHigh();

    int convertUserSearchInput(string s);

    //for listing
    string listChoice();

    string listChoiceInt();

    //for adding
    Game dataInput();

    //for deleting
    string deleteConfirm(int y);
};

#endif