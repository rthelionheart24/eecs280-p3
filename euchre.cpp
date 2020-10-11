#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <algorithm>
#include "Player.h"
#include <Pack.h>
#include <Card.h>

using namespace std;

int main(int argc, char *argv[])
{

    assert(argc == 12);

    //Initialize the name of the file for the pack we use
    string filename(argv[1]);

    //Initialize shuffle or not
    if (!strcmp(argv[2], "unshuffle") && !strcmp(argv[2], "shuffle"))
    {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 2;
    }
    bool shuffle = true;
    if (strcmp(argv[2], "unshuffle") == 0)
        shuffle = false;

    //Initialize the points to win
    int points_to_win = stoi(argv[3]);
    if (points_to_win > 100 || points_to_win < 1)
    {

        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 3;
    }
    //Initialize all the players
    vector<Player *> players;
    for (int i = 4; i <= 10; i += 2)

    {
        string name(argv[i]), type(argv[i + 1]);

        if (type != "Simple" && type != "Human")
        {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                 << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                 << "NAME4 TYPE4" << endl;
            return 4;
        }
        players.push_back(Player_factory(name, type));
    }

   
    return 0;
}
