#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "game.h"
#include "database.h"
#include <string>
#include <sstream>
#include "menu.h"
#include <ncurses.h>

using namespace std;

//a response to invalid input
const string inputChoice = "Please enter a valid input!";

//converts all letters to lower case to make inputs case insensitive
void toLower(string &s) {
    for (int i = 0; i<s.size(); i++) {
        s.at(i) = tolower(s.at(i));
    }
}

//general input funciton that takes strings from user
string Menu::getString() {
    echo();
    noraw();
    string res;
    char c = getch();
    while(c != '\n') {
        res.push_back(c);
        c = getch();
    }
    
    toLower(res);
    return res;
}

//input function that allows user to choose options from menu
string Menu::getStringHighlight(int option, int y, int x) {
        echo();

        //allows function to take one character at a time
        cbreak();
        vector <string> menu;

        //different options  give different menu
        if (option == 1) {
            menu = choiceMain;
        }
        if (option == 2) {
            menu = choiceField;
        }

        string res;
        int highlight = 0; 
        y = y + 2;
        x = 2;
        char c;
        char pass;
        mvprintw(y+6, 4, "Enter: ");

        //a while loop that continuously prints highlighted element
        //until enter is pressed
        while (true) {
            for (int i =0; i<menu.size(); i++) {
                    if (highlight == i) {
                        attron(A_REVERSE);
                        mvprintw(y+i, x, menu.at(i).c_str());
                        attroff(A_REVERSE);
                        }
                    else {
                        mvprintw(y+i, x, menu.at(i).c_str());
                    }
            }
            move(y+6, 11);
            c = getch();
            if (c != '\n') {
                pass = c;
            }

            //all options for field and main menu
            if (c == 'a') {
                highlight = 0;
            }
            if (c == 'b') {
                
                highlight = 1;
            }
            if (c == 'c') {
                highlight = 2;
            }
            if (c == 'd') {
                highlight = 3;
            }
            if (c == 'f') {
                highlight = 4;
            }
            if (c == '\n') {
                break;
            }
        }

        //takes a dummy char pass to not take in an endline character as a response
        res.push_back(pass);
        toLower(res);
    return res;
}


Menu::Menu() {

    //initializes all menu prompts and variables needed
    userChoice = "";
    low = 0;
    high = 0;
    choiceMain.push_back("(a) to add a game");
    choiceMain.push_back("(b) to delete a game");
    choiceMain.push_back("(c) to find a game");
    choiceMain.push_back("(d) to list games");
    choiceMain.push_back("(f) to quit");

    choiceField.push_back("(a) for NAME of game");
    choiceField.push_back("(b) for RELEASE YEAR of game");
    choiceField.push_back("(c) for COPIES SOLD");
    choiceField.push_back("(d) for NAME OF DEVELOPERS");
    choiceField.push_back("(f) to go back to main menu");
}

//checks for user input
void Menu::checkInput() {
    int i = 0;
    int y = 0, x = 0;
    getyx(stdscr, y, x);
    while(userChoice != "a" && userChoice !="b") {
        i++;
        mvprintw(y+i, 4, inputChoice.c_str());
        i++;
        move(y+i , 4);
        userChoice = getString();
    }
}

//prints main menu and accepts user response
string Menu::mainMenu() {
    move(1, 2);
    printw("Welcome to the game database!");
    mvprintw(3, 3, "Press");

    //prints the menu options
    userChoice = getStringHighlight(1, 3, 4);
        int line = 1;

        //checks for valid menu options
        while ((userChoice < "a" || userChoice > "d") || (userChoice.size() > 1)) {
            if(userChoice == "f") {
                break;
            }
            mvprintw((11+line+1), 4, inputChoice.c_str());
            line++;
            userChoice = getStringHighlight(1, 3, 4);
            line++;
        }
       
        return userChoice;
}

//prints field menu options and takes in user input
string Menu::fieldMenu(int instructions) {
    clear();

    //prints different instructions depending on user option
    if (instructions == 0) {
         mvprintw(1, 2, "Press the correct letter to search by a field");
    }
    if (instructions == 1) {
        mvprintw(1, 2, "Press the correct letter to delete by a field");
    }
    if (instructions == 2) {
        mvprintw(1, 2, "Type in the correct letter to list by a field");
    }

    //prints field menu options
    userChoice = getStringHighlight(2, 1, 4);
    int line = 1;

    //checks for valid user input
         while (userChoice < "a" || userChoice > "d" || (userChoice.size() > 1)) {
            if (userChoice == "f") {
                break;
            } 
            line++;
            mvprintw((9+line), 4, inputChoice.c_str());
            line++;
            move((9+line), 4);
            userChoice = getStringHighlight(2, 1, 4);
            line++;

        }
        return userChoice;    
}

//Below are functions for searching: 


//asks for type of search
string Menu::searchChoiceString() {
    clear();
    mvprintw(1, 2, "Press");
    mvprintw(2, 4, "(g) for general search");
    mvprintw(3, 4, "(e) for exact search");
    mvprintw(5, 4, "Enter: ");

    //receives data from user
    userChoice = getString();
    int line = 0;

    //checks for valid user input
        while (userChoice != "g" && userChoice != "e") {
            mvprintw((6+line), 4, inputChoice.c_str());
            line++;
            move((6+line), 4);
            userChoice = getString();
            line++;
        }
        return userChoice;
}

//asks for type of search but for ints (same as above)
string Menu::searchChoiceInt() {
    clear();
    mvprintw(1, 2, "Press");
    mvprintw(2, 4, "(r) for ranged search");
    mvprintw(3, 4, "(e) for exact search");
    mvprintw(5, 4, "Enter: ");
    userChoice = getString();
    //i is the same as line
    int i = 0;
        while (userChoice != "r" && userChoice != "e") {
            mvprintw((6+i), 4, inputChoice.c_str());
            i++;
            move((6+i), 4);
            userChoice = getString();
            i++;
        }
        return userChoice;
}

//checks for valid input for integer values
bool Menu::validInputInt(string n) {
    for(int i=0; i<n.size(); i++) {
        if(n.at(i) < '0' || n.at(i) > '9') {
            return false;
        }
    }
    return true;
}

//takes the max and min of ranged searches
void Menu::rangeIntSearch() {
    clear();
    int line = 0;
    string lowS;
    bool repeat = true;

    //if min > max, must repeat
    while (repeat) {

        //asks for minimum value. 
        mvprintw(1+line, 2, 
        "Enter the earlierst release year or the minimum copies sold without alphabets: ");
        line++;

        //receives value and checks if valid
        lowS = getString();
        bool validLow = validInputInt(lowS);
        while (!validLow) {
            mvprintw((1+line), 4, inputChoice.c_str());
            line++;
            move((1+line), 4);
            lowS = getString();
            line++;
            validLow = validInputInt(lowS);
        }

        line++;
        string highS;

        //asks for maximum value
        mvprintw(1+line, 2, 
        "Enter the latest release year or maximum copies sold without alphabets: ");

        //receives user input and check if valid
        highS = getString();
        bool validHigh = validInputInt(highS);
        line++;
        while (!validHigh) {
            mvprintw((1+line), 4, inputChoice.c_str());
            line++;
            move((1+line), 4);
            highS = getString();
            line++;
            validHigh = validInputInt(highS);
        }
        line = line +2;

        //converts string input to integer
        stringstream lowSS (lowS);
        lowSS >> low;

        stringstream highSS (highS);
        highSS >> high;

        //check if minimum is greater than maximum
        if (low <= high) {
            repeat = false;
        }
        if (low > high) {
            //if minimum is greater than maximum, the program ask the user to repeat
            mvprintw(1+line, 6, "Invalid set of data. Press ENTER to re-type");
            line = line + 2;
            getch();
        }
    }
}

//gives the minimum value for ranged search
int Menu::giveLow() {
    return low;
}

//gives maximum value for ranged search
int Menu::giveHigh() {
    return high;
}
 
//converts string to number
int Menu::convertUserSearchInput(string s) {
    stringstream numSS (s);
    int num = 0;
    numSS >> num;
    return num;
}

//Below are functions for listing

//asks to list alphabeticaly or reverse
string Menu::listChoice() {
    clear();
    mvprintw(1, 2, "Press (a) to list alphbetically");
    mvprintw(2, 2, "Or press (b) to list reverse alphabetical order");
    mvprintw(4, 2, "Enter: ");
    userChoice = getString();
    checkInput();
    return userChoice;
}

//asks to list ascending or descending
string Menu::listChoiceInt() {
    clear();
    mvprintw(1, 2, "Press (a) to list in ascending order");
    mvprintw(2, 2, "Or press (b) to list in descending order");
    mvprintw(4, 2, "Enter: ");
    userChoice = getString();
    checkInput();
    return userChoice;
}


//Below are functions for adding


Game Menu::dataInput() {
    clear();
    Game g;
    string n;
    string d;
    string rYI;
    string cSI;
    int line = 0;
    const int baseVertical = 1;

    mvprintw((baseVertical + line), 2, "Enter NAME of game: ");
    n = getString();

    g.setName(n);
    line = line +2;

    mvprintw((baseVertical + line+1), 2, "Enter RELEASE YEAR of game: ");
    rYI = getString();

    //check for valid input of integer
    bool valid = validInputInt(rYI);
    int numRY = convertUserSearchInput(rYI);
    line = line +2;

    //check for valid input of year
    while (!valid || numRY > 2021 || numRY < 0) {
        mvprintw(baseVertical + line, 5, inputChoice.c_str());
        line++;
        move((baseVertical + line), 5);
        rYI = getString();
        valid = validInputInt(rYI);
        numRY = convertUserSearchInput(rYI);
        line++;
    }
    g.setReleaseYear(numRY);

    line = line +1;
    mvprintw((baseVertical + line+1), 2, "Enter COPIES SOLD of game: ");
    cSI = getString();

    //check for valid input of integer
    bool validB = validInputInt(cSI);
    int numCS = convertUserSearchInput(cSI);
    line = line +2;

    //check for valid input of copies sold
    while (!validB || numCS < 0) {
        mvprintw(baseVertical + line, 5, inputChoice.c_str());
        line++;
        move((baseVertical + line), 5);
        cSI = getString();
        validB = validInputInt(cSI);
        numCS = convertUserSearchInput(cSI);
        line++;
    }
    g.setCopSold(numCS);

    line = line +1;
    mvprintw((baseVertical + line+1), 2, "Enter the name of the DEVELOPERS of game: ");
    d = getString();
    g.setDev(d);

    return g;
}

//function for deleting
//asks the user if they want to delete item
string Menu::deleteConfirm(int y) {
    mvprintw(y+2, 2, "Are you sure you want to delete the items above?");
    mvprintw(y+3, 2, "Press (a) to delete or press (b) to cancel");
    mvprintw(y+5, 2, "Enter: ");
    userChoice = getString();
    checkInput();
    return userChoice;
}
