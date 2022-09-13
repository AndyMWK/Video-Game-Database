#include "menu.h"
#include "database.h"
#include "game.h"
#include <algorithm> 
#include <ncurses.h>

// a5.cpp

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Min Woo (Andy) Kim>
// St.# : <301448177>
// Email: <andy_kim_10@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//

using namespace std;

//basic function for listing items
void listV(const vector <Game> &v, int& y, int x) {
    y = y + 2;
    x = 2;
    mvprintw(y, 2, "Results are: ");
    y = y + 2;
    //prints no results if no results have been found
    if (v.size() == 0) {
        y++;
        mvprintw(y, x, "No results");
    } 
    for (Game g : v) {
        //main part for printing all parts of game. 
        y++;
        string output = g.n() + ", " + to_string(g.rY()) + ", " + to_string(g.cS()) + ", " +g.d();
        mvprintw(y, x+2, output.c_str());
    }
}

void enterPrompt() {
    //this function waits until the user pressed enter

    //this function is needed as the program recognizes the char but won't skip
    //until enter is pressed. 

    noraw();
    char c = getch();
    while (c != '\n') {
        c = getch();
    }
}

//shows different prompts after listing items has been finished. 
void showPrompts(int& y, const int &choice) {
    y = y + 3;
    if (choice == 1) {
        mvprintw(y, 2, "Search finished. Press ENTER to exit");
        noecho();
        enterPrompt();
    }
    else if (choice == 2) {
        mvprintw(y, 2, "Search finished. Press ENTER to proceed");
        noecho();
        enterPrompt();
    }
    else if (choice == 3) {
        mvprintw(y, 2, "Listing finished. Press ENTER to proceed");
        noecho();
       enterPrompt();
    }
}

//calls functions from menu to show user prompts for deleting
void deleteModule(const vector <Game> &res, Menu &gameMenu, 
Database &gameBase, string &userChoice, int& y) {
    
    if (res.size() == 0) {
        userChoice = "";
    }
    else if (res.size() > 0) {
        string userInput = gameMenu.deleteConfirm(y);
        if (userInput == "a") {
            gameBase.deleteGame(res);
            clear();
            noecho();
            mvprintw(1, 2, "Deletion successful!. Press ENTER to return");
            enterPrompt();
        }
        if (userInput == "b") {
            userChoice = "";
        }
    }
}

//this is a summary function that takes a list of games and prints the correct prompt
void completeList(const vector <Game> &res) {
    int x = 0, y = 0;
    getyx(curscr, y, x);
    x = 2;
    listV(res, y, x);
    showPrompts(y, 3);
}

//this function checks input for int values that are not checked in menu.cpp
void checkInputI(Menu gameMenu, string &s) {
    bool valid = gameMenu.validInputInt(s);
    int y = 0, x = 0;
    getyx(stdscr, y, x);
    x= 2;
    int i = 0;
    while (!valid) {
        i++;
        mvprintw((y+i), x, "Please enter a valid value");
        i++;
        move(y+i, x);
        s = gameMenu.getString();
        valid = gameMenu.validInputInt(s);
    }
}

int main() {
    //database and menu are initialized
    Database gameBase;
    Menu gameMenu;

    //main variable that decides if the program quits or not
    bool quit = false;
     
    while (!quit) {

       //ncurses initializer functions
       initscr();
       clear();
       keypad(stdscr, true);
       noraw();
       echo();

       bool deleteStuff = false;

       //calls basic menu screen
       string userChoice = gameMenu.mainMenu();
        
        //when the user wants to add a record
        while (userChoice == "a") {
            clear();
            echo();
            mvprintw(1, 2, "Type in the correct data to add a record");

            //takes in input and processes it
            //also decides whether the record exists
            Game userInput = gameMenu.dataInput();
            bool added = gameBase.addGame(userInput);

            if(added) {
                clear();
                mvprintw(1, 2, "Record successfully added!");
                mvprintw(2, 2, "Press ENTER to exit to main menu");
                noecho();
                enterPrompt();
                
            }
            //this is when the record does not exist
            if (!added) {
                clear();
                mvprintw(1, 2, "Record already exists! Do not add.");
                mvprintw(2, 2, "Press ENTER to exit to main menu");
                noecho();
               enterPrompt();
            }
            userChoice = "";
        }
        
        //when the user wants to delete an item
        while(userChoice == "b") {
            //since delete is basically searching until deletion, this just calls 
            //the searching prompt

            deleteStuff = true;
            userChoice = "c";
        }

        //when the user wants to search for data
        while (userChoice == "c") {
            clear();
            echo();

            //providing field menu is universal for deleting, searching and listing
            //so need separate instructions for the user, which is decided here

            int instruction = 0;
            if (deleteStuff) {
                instruction = 1;
            }
            string userFieldChoice = gameMenu.fieldMenu(instruction);

            //if the user wants to search by name
            if (userFieldChoice == "a") {
                string userChoicePrecision = gameMenu.searchChoiceString();
                clear();
                mvprintw(1, 2, "Enter your search: ");
                string userInput = gameMenu.getString();

                //g is for generalized search
                if(userChoicePrecision == "g") {
                    vector <Game> res = gameBase.search_general(userInput, true);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;

                    //if the user wants to delete items, then the delete prompts are called
                    //so that the user can delete. 
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }

                    //if not delete, then just list items
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }

                //e is for exact search. 
                //this section is very similar to generalized search, just a different 
                //function for searching is called

                if (userChoicePrecision == "e") {
                    vector <Game> res = gameBase.search_exact(userInput, true);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;

                    //same as general search, delete option is present
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }

                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }
            }

            //if the user wants to search by the name of developers. 
            //same  as name section (section above)

            if (userFieldChoice == "d") {
                string userChoicePrecision = gameMenu.searchChoiceString();
                clear();
                mvprintw(1, 2, "Enter your search: ");
                string userInput = gameMenu.getString();
                if(userChoicePrecision == "g") {
                    vector <Game> res = gameBase.search_general(userInput, false);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }
                if (userChoicePrecision == "e") {
                    vector <Game> res = gameBase.search_exact(userInput, false);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }
            }

            //if the user wants to search by release year of game
            if (userFieldChoice == "b") {

                //prompt asking for integer is called
                string userChoicePrecision = gameMenu.searchChoiceInt();
                clear();

                //r means ranged search
                //eg. searching games released between 2015 and 2020. 
                if (userChoicePrecision == "r") {

                    //prompt for asking the latest year and earliest year of release
                    gameMenu.rangeIntSearch();

                    //processes given data
                    int max = gameMenu.giveHigh();
                    int min = gameMenu.giveLow();
                    vector <Game> res = gameBase.search_int_range(false, max, min);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;

                    //if the user wishes to delete these items, delete prompt is called
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }

                //e means user wants exact year
                if (userChoicePrecision == "e") {

                    //prompt for entering search year
                    mvprintw(1, 2, "Enter your search: ");
                    string userInput = gameMenu.getString();

                    //checks for validity of input data
                    checkInputI(gameMenu, userInput);

                    //processing of data such as conversion and searching
                    int userInputI = gameMenu.convertUserSearchInput(userInput);
                    vector <Game> res = gameBase.search_int_exact(userInputI, false);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;

                    //same as ranged, this calls delete prompts
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }
            }
             
             //if the user wants to search by copies sold
             if (userFieldChoice == "c") {
                 //shows prompt for entering choice
                string userChoicePrecision = gameMenu.searchChoiceInt();
                clear();
                
                //the rest if the same as previous if statement brach with release year of game. 
                //refer to that if needed

                if (userChoicePrecision == "r") {
                    gameMenu.rangeIntSearch();
                    int max = gameMenu.giveHigh();
                    int min = gameMenu.giveLow();
                    vector <Game> res = gameBase.search_int_range(true, max, min);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }
                if (userChoicePrecision == "e") {
                    mvprintw(1, 2, "Enter your search: ");
                    string userInput = gameMenu.getString();
                    checkInputI(gameMenu, userInput);
                    int userInputI = gameMenu.convertUserSearchInput(userInput);
                    vector <Game> res = gameBase.search_int_exact(userInputI, true);
                    int x = 0, y = 0;
                    getyx(curscr, y, x);
                    x = 2;
                    if (deleteStuff) {
                        listV(res, y, x);
                        showPrompts(y, 2);
                        deleteModule(res, gameMenu, gameBase, userChoice, y);
                    }
                    else {
                        listV(res, y, x);
                        showPrompts(y, 1);
                    }
                }
            }

            //this is an option to return to main menu
            if (userFieldChoice == "f") {
                userChoice = "";
            }
            
        }

        //option for listing itmes
        while (userChoice == "d") {
            //provides user prompt to enter data
            clear();
            string userFieldChoice = gameMenu.fieldMenu(2);

            //if the user wants to list by name
            if (userFieldChoice == "a") {
                string userChoiceList = gameMenu.listChoice();

                //a means alphabetical and b means reverse
                if (userChoiceList == "a") {
                    //processing and listing
                    vector <Game> res = gameBase.returnSorted(1, false);
                    completeList(res);
                }
                if (userChoiceList == "b") {
                    vector <Game> res = gameBase.returnSorted(1, true);
                    completeList(res);
                }
            }

            //if the user wants to list by name of developers
            if (userFieldChoice == "d") {

                //this if statement is the same as the statement above
                //just searching for different fields

                string userChoiceList = gameMenu.listChoice();
                if (userChoiceList == "a") {
                    vector <Game> res = gameBase.returnSorted(4, false);
                    completeList(res);
                }
                if (userChoiceList == "b") {
                    vector <Game> res = gameBase.returnSorted(4, true);
                   completeList(res);
                }
            }

            //if the user wants to list by release year
            if (userFieldChoice == "b") {
                string userChoiceList = gameMenu.listChoiceInt();

                //a means ascending order and b means descending order
                if (userChoiceList == "a") {
                    vector <Game> res = gameBase.returnSorted(2, false);
                   completeList(res);
                }
                if (userChoiceList == "b") {
                    vector <Game> res = gameBase.returnSorted(2, true);
                    completeList(res);
                }
            }

            //if the user wants to list by copies sold
            if (userFieldChoice == "c") {
                string userChoiceList = gameMenu.listChoiceInt();

                //this if statement is the same as the statement above
                //just searching for different fields

                if (userChoiceList == "a") {
                    vector <Game> res = gameBase.returnSorted(3, false);
                    completeList(res);
                }
                if (userChoiceList == "b") {
                    vector <Game> res = gameBase.returnSorted(3, true);
                    completeList(res);
                }
            }
            
            //option to return to main menu
            if (userFieldChoice == "f") {
                userChoice = "";
            }

        }

        //option to quit
        if (userChoice == "f") {
            clear();
            printw("Quitting...\nPress ENTER to complete quitting\n");
            getch();
            quit = true;
        }
    }

    //saves all data and ends program
    gameBase.quittingPrompts();
    endwin();
}