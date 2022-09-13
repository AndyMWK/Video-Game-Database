#ifndef DATABASE_H
#define DATABASE_H

#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "game.h"
#include <fstream>
#include <ncurses.h>

class Database {
    private:
    
    vector <Game> list;

    //conversion to lower case
    void toLower(string &res);

    public:

    //starting

    vector <string> readStringFromFile(string n);

    Game convertToGame(string n);


    Database();

    //searching

    vector <Game> search_general(string n, bool name);

    vector <Game> search_exact(string n, bool name);

    vector <Game> search_int_range(bool copSold, int high, int low);

    vector <Game> search_int_exact(int n, bool copSold);

    //sorting

    vector <Game> giveList();
    vector <Game> returnSorted(const int &userChoice, bool reverse);

    //adding

    bool addGame(Game g);

    //deleting

    void deleteGame(const vector <Game> &v);

    //quitting
    void quittingPrompts();
};



#endif