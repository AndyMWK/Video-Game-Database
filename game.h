#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <vector>
#include <ncurses.h>

using namespace std;

class Game {
    
    private:

    string name;
    int copSold;
    int releaseYear;
    string dev;

    public:
    //constructor
    Game();

    Game(string n, int c, int r, string d);

    const string n();

    const string d();

    int rY();

    int cS();

    void setName(string n);

    void setCopSold(int n);

    void setReleaseYear(int n);

    void setDev(string n);

};

#endif