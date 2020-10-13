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

string filename, trump;
bool shuffle;
Player *dealer, *leader, *denfender1, *defender2, *current_player;
vector<Player *> players;
vector<int> points = {}, tricks_won = {};
Pack *deck;
Card *up_card;

int main(int argc, char *argv[])
{

    assert(argc == 12);

    //Initialize the name of the file for the pack we use
    string filename(argv[1]);

    //Shuffle or not
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
    cout << "Shuffle option: " << shuffle << endl;

    //Initialize the points to win
    int points_to_win = stoi(argv[3]);
    if (points_to_win > 100 || points_to_win < 1)
    {

        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
             << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
             << "NAME4 TYPE4" << endl;
        return 3;
    }
    cout << "Points to win have been set to " << points_to_win << endl;

    //Initialize all the players
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
    cout << "Team 1: " << players[0]->get_name() << ", " << players[2]->get_name() << endl
         << "Team 2: " << players[1]->get_name() << ", " << players[3]->get_name() << endl;

    //Set the first dealer as player 0
    dealer = players[0];

    do
    {
        //Initialize, shuffle, and deal the deck
        deck = new Pack();
        shuffle_deck(deck, shuffle);
        deal_card(players, deck, dealer);

        //Time to make trump
        up_card = &deck->deal_one();
        cout << "The upcard is" << up_card << endl;

    } while (check_winner(points, points_to_win) == -1);

    return 0;
}

//Shuffle the deck based on the option
void shuffle_deck(Pack *deck, bool shuffle)
{
    if (shuffle)
        deck->shuffle();
}

//Deal cards to players
void deal_card(vector<Player *> players, Pack *deck, Player *dealer)
{
    Player *current = players[1];
    int deal_num = 3;
    for (unsigned int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < deal_num; j++)
            current->add_card(deck->deal_one());

        current = next_player(players, current);

        if (deal_num == 3)
            deal_num--;
        else
            deal_num++;
    }
    for (unsigned int i = 0; i < players.size(); i++)
    {
        for (int j = 0; j < deal_num; j++)
            current->add_card(deck->deal_one());

        current = next_player(players, current);

        if (deal_num == 3)
            deal_num--;
        else
            deal_num++;
    }
}
//Point current_player to the next player in sequence
Player *next_player(vector<Player *> players, Player *current_player)
{
    if (find(players.begin(), players.end(), current_player + 1) >= players.end())
        return players[0];
    else
        return current_player + 1;
}

//Used for making trump
void order_up(vector<Player *> players, Player *current_player, Player *dealer,
              Card *upcard, string &trump)
{
    bool result;
    Player *current = players[1];
    //Two rounds of making trump

    do
    {
        result = current->make_trump(*upcard, false, 1, trump);
        current = next_player(players, current);
        if (result == true)
        {
            break;
        }
    } while (current != players[1]);
    result
}

//Check whether there is a winner, if there is, return the index of the winner. Else returns -1
int check_winner(vector<int> points, int points_to_win)
{
    for (unsigned int i = 0; i < points.size(); i++)
    {
        if (points[i] >= points_to_win)
            return i;
    }
    return -1;
}