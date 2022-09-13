#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "game.h"
#include "database.h"
#include <string>
#include <sstream>
#include <vector>
#include <ncurses.h>

using namespace std;

void Database::toLower(string &res) {
    for (int i =0; i<res.size(); i++) {
        res.at(i) = tolower(res.at(i));
    }
}

//read data from file as astring and converToGame will
//process and convert string to game values

vector <string> Database::readStringFromFile(string n) {
    ifstream file (n);
    vector <string> row;
    while(true) {
        string game;
        getline(file, game);
        row.push_back(game);
        if(file.fail()) {
            break;
        }
    }
    file.close();
    return row;
}

Game Database::convertToGame(string n) {
    Game g;
    //i is for iterator 1
    //j is for iterator 2
    int i = 0;
    int j = 0;
    while(i < n.size() && j < 4) {
        string nameOfGame;
        string releaseYear;
        string copSold;
        string devs;
        while (j == 0) {
            //reads in string until comma
            nameOfGame.push_back(n.at(i));
            i++;
            if (n.at(i) == ',') {
                g.setName(nameOfGame);
                j++;
            }
        }
        i++;
        while (j == 1) {
            //reads in release year until coma
            if (n.at(i) == ' ') {
                i++;
            }

            if (n.at(i) == ',') {
                //converts string to integer
                stringstream releaseYearSS (releaseYear);
                int num = 0;
                releaseYearSS >> num;
                g.setReleaseYear(num);
                j++;
                break;
            }
            if (n.at(i) != ',' && n.at(i) != ' ') {
                releaseYear.push_back(n.at(i));
                i++;
            }
            
        }
        i++;
        while (j == 2) {
            //same as while loop above. string -> int
            if (n.at(i) == ' ') {
                i++;
            } 
            
            if (n.at(i) == ',') {
                stringstream copSoldSS (copSold);
                int num = 0;
                copSoldSS >> num;
                g.setCopSold(num);
                j++;
                break;
            }
            if (n.at(i) != ',' && n.at(i) != ' ') {
                copSold.push_back(n.at(i));
                i++;
            } 
            
        }
        i++;
        i++;
        while (j == 3) {
            //reads string and set it as developer
            if (i == (n.size()-1)) {
                devs.push_back(n.at(i));
                g.setDev(devs);
                j++;
            }
            if (i < n.size()) {
                devs.push_back(n.at(i));
                i++;
            }
            
        }
    }
    return g;
}

Database::Database() {
    //reads data from database.txt
    vector <string> listS = readStringFromFile("database.txt");
    listS.pop_back();
    for (string s: listS) {
        Game g = convertToGame(s);
        list.push_back(g);
    }
}

//Below are functions for searching

vector <Game> Database::search_general(string n, bool name) {
     vector <Game> res;
        for (Game g : list) {
            //if name is true, searching for name of game
            //if name is false, searching for developer of game
            if (name) {
                string nameOfGame = g.n();
                toLower(nameOfGame);

                //uses .find() to find position of substring
                int pos = nameOfGame.find(n);

                //checks for valid position of substring
                if (pos < nameOfGame.size() && pos >= 0) {
                    res.push_back(g);
                }
            }
            if (!name) {
                string nameOfGame = g.d();
                toLower(nameOfGame);
                int pos = nameOfGame.find(n);
                if (pos < nameOfGame.size() && pos >= 0) {
                    res.push_back(g);
                }
            }
            
        }
        return res;
}

//same as function above, just searches for exact string instead of substring
vector <Game> Database::search_exact(string n, bool name) {
     vector <Game> res;
        for (Game g : list) {
            if (name) {
                string nameOfGame = g.n();
                toLower(nameOfGame);
                if (n == nameOfGame) {
                    res.push_back(g);
                }
            }
            if (!name) {
                string nameOfDev = g.d();
                toLower(nameOfDev);
                if (n == nameOfDev) {
                    res.push_back(g);
                }
            }
        }

        return res;
}

vector <Game> Database::search_int_range(bool copSold, int high, int low) {
    vector <Game> res;
    for (Game g: list) {
        //if copSold is true, searching for copies sold
        //if copSold is false, searching for release year
        if (copSold) {
            int num = g.cS();
            //check of given number is within the max and min range
            if (num >= low && num <= high) {
                res.push_back(g);
            }
        }
        if (!copSold) {
            int num = g.rY();
            if (num >= low && num <= high) {
                res.push_back(g);
            }
        }
    }
    return res;
}

//same as function above, just searching for the exact number 
vector <Game> Database::search_int_exact(int n, bool copSold) {
    vector <Game> res;
    for (Game g: list) {
        if (copSold) {
            int num = g.cS();
            if (num == n) {
                res.push_back(g);
            }
        }
        if (!copSold) {
            int num = g.rY();
            if (num == n) {
                res.push_back(g);
            }
        }
    }
    return res;
}

//gives a copy of the entire list of games
vector <Game> Database::giveList() {
    return list;
}

//frees duplicates, but for strings
vector <string> dupeFreeS(const vector <string> &v) {
    vector <string> res;
    for(string s : v) {
        bool found = binary_search(res.begin(), res.end(), s);
        if (!found) {
            res.push_back(s);
        }
    }

    return res;
}

//frees duplicates, but for integers
vector <int> dupeFreeI(const vector <int> &v) {
    vector <int> res;
    for(int s : v) {
        bool dupe = binary_search(res.begin(), res.end(), s);
        if (!dupe) {
            res.push_back(s);
        }
    }

    return res;
}

//below are functions for sorting/listing

//functions to compare for different fields.
//from professor's lecture
bool byGameName(Game &j, Game &i) {
    return j.n() <= i.n();
}

bool byGameDev(Game &j, Game&i) {
    return j.d() <= i.d();
}

bool byReleaseYear(Game &j, Game &i) {
    return j.rY() <= i.rY();
}

bool byCopiesSold(Game &j, Game &i) {
    return j.cS() <= i.cS();
}

vector <Game> Database::returnSorted(const int &userChoice, bool rev) {
    vector <Game> newList = list;
    if (userChoice == 1) {
        //use sort to compare different fields
        sort(begin(newList), end(newList), byGameName);
        if (rev) {
            reverse(begin(newList), end(newList));
        }
    }
    else if (userChoice == 2) {
        sort(begin(newList), end(newList), byReleaseYear);
        if (rev) {
            reverse(begin(newList), end(newList));
        }
    }
    else if (userChoice == 3) {
        sort(begin(newList), end(newList), byCopiesSold);
        if (rev) {
            reverse(begin(newList), end(newList));
        }
    }
    else if (userChoice == 4) {
        sort(begin(newList), end(newList), byGameDev);
        if (rev) {
            reverse(begin(newList), end(newList));
        }
    }
    return newList;
}

//Below are functions for adding

bool Database::addGame(Game g) {
    //search for the name of the game. 
    //if match found, then check the rest of the paramteres
    //if all parameters match, then don't add, and print a message. 

    bool duplicate = false;
    for (Game n : list) {
        string name = n.n();
        toLower(name);
        string dev = n.d();
        toLower(dev);
        if (g.n() == name) {
            if (g.rY() == n.rY()) {
                if (g.cS() == n.cS()) {
                    if (g.d() == dev) {
                        duplicate = true;
                        break;
                    }
                }
            }
        }
    }
    if (duplicate) {
        return false;
    }
    list.push_back(g);
    return true;
   
}

//below are functions for deleting

void Database::deleteGame(const vector <Game> &v) {
    vector <Game> newList;
    for (Game g: list) {
        int i = 0;
        for (Game l : v) {
            if (g.n() == l.n()) {
                if (g.rY() == l.rY()) {
                    if (g.cS() == l.cS()) {
                        if (g.d() == l.d()) {
                            break;
                        }
                    }
                }
            }
            i++;
            if (i == (v.size())) {
                newList.push_back(g);
            }
        }
    }
    list = newList;
    cout << "successfully deleted!\n";
}

//function for quitting
void Database::quittingPrompts() {
    //opens databse.txt and saves all data changed in list
    ofstream fileSave("database.txt");
    for(Game g : list) {
        fileSave << g.n() << ", " << g.rY() << ", " << g.cS() << ", " << g.d() << endl;
    }
    fileSave.close();
}