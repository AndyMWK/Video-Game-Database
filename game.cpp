#include <string>
#include <iostream>
#include <vector>
#include "game.h"
#include <ncurses.h>

using namespace std;

Game::Game() {}

Game::Game(string n, int c, int r, string d)  : 
name(n), copSold(c), releaseYear(r), dev(d) {}

//gives the name of game
const string Game::n() {
    return name;
}

//gives the name of the developers
const string Game::d() {
    return dev;
}

//gives the release year of games
int Game::rY() {
    return releaseYear;
}

//gives the copies sold of games
int Game::cS() {
    return copSold;
}

//sets the name of game
void Game::setName(string n) {
    name = n;
}

//sets the copies sold of game
void Game::setCopSold(int n) {
    copSold = n;
}

//sets the release year of game
void Game::setReleaseYear(int n) {
    releaseYear = n;
}

//sets the name of the developers of game
void Game::setDev(string n) {
    dev = n;
}
