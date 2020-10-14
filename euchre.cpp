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

static string filename, trump;
bool shuffle;
Player *dealer, *leader, *defender1, *defender2, *current_player, *winner_of_the_trick;
static Player *players[4];
static Card *middle[4];
static int points[4] = {0}, tricks_won[4] = {0};
static Pack *deck;
Card *up_card, *lead_card;

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
        players[i / 2 - 2] = Player_factory(name, type);
    }
    cout << "Team 1: " << players[0]->get_name() << ", " << players[2]->get_name() << endl
         << "Team 2: " << players[1]->get_name() << ", " << players[3]->get_name() << endl;

    //Set the first dealer as player 0
    dealer = players[0];
    int hand = 0;
    do
    {
        cout << "Hand " << hand << endl
             << dealer << endl;
        //Initialize, shuffle, and deal the deck
        deck = new Pack();
        shuffle_deck(deck, shuffle);
        deal_card(players, deck, dealer);

        //Time to make trump and see who is the defender
        up_card = &(deck->deal_one());
        cout << *up_card << " turned up" << endl;
        trump = order_up(players, dealer, defender1, defender2, up_card, trump);

        //Five tricks starts here
        leader = players[1];

        for (int trick = 0; trick < 5; trick++)
        {
            //Leader leads a card
            lead_card = &(leader->lead_card(trump));
            //Place the card in the same index as the player who plays it
            middle[&leader - players] = lead_card;
            //The rest follow
            current_player = next_player(players, leader);
            for (int i = 0; i < 3; i++)
            {
                if (i = &leader - players)
                    continue;

                middle[&current_player - players] = &(current_player->play_card(*lead_card, trump));
                current_player = next_player(players, current_player);
            }
            //Determine who wins the trick
            winner_of_the_trick = who_win_the_trick(players, middle, leader, lead_card, trump);
            win_the_trick(players, winner_of_the_trick);
            cout << winner_of_the_trick->get_name() << " takes the trick" << endl;

            leader = winner_of_the_trick;
        }
        update_points(players, points, tricks_won, dealer, defender1, defender2);

        cout << players[0]->get_name() << " and " << players[2]
             << " have " << get_points(players, players[0])
             << " points " << endl
             << players[1]->get_name() << " and "
             << players[3] << " have " << get_points(players, players[1]) << endl;

        dealer = next_player(players, dealer);

    } while (is_there_game_winner(points, points_to_win) == -1);

    cout << players[is_there_game_winner(points, points_to_win)] << " and "
         << players[is_there_game_winner(points, points_to_win) + 2] << " win!" << endl;

    for (int i = 0; i < 4; i++)
    {
        delete players[i];
    }

    return 0;
}

//Shuffle the deck based on the option
void shuffle_deck(Pack *deck, bool shuffle)
{
    if (shuffle)
        deck->shuffle();
}

//Deal cards to players
void deal_card(Player *players[], Pack *deck, Player *dealer)
{
    Player *current = next_player(players, dealer);
    int deal_num = 3;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < deal_num; j++)
            current->add_card(deck->deal_one());

        current = next_player(players, current);

        if (deal_num == 3)
            deal_num--;
        else
            deal_num++;
    }
    for (int i = 0; i < 4; i++)
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
Player *next_player(Player *players[], Player *current_player)
{
    if (&current_player - players == 4)
        return players[0];
    else
        return *(&current_player + 1);
}

//Used for making trump
string order_up(Player *players[], Player *dealer, Player *defender1,
                Player *defender2, Card *upcard, string trump)
{
    bool result;

    //Round 1
    Player *current = next_player(players, dealer);
    do
    {
        result = current->make_trump(*upcard, false, 1, trump);
        if (result == true)
        {
            dealer->add_and_discard(*upcard);
            defender1 = next_player(players, current);
            defender2 = next_player(players, next_player(players, defender1));
            return trump;
        }
        current = next_player(players, current);

    } while (current != players[1]);

    //Round 2
    current = next_player(players, dealer);
    do
    {
        result = current->make_trump(*upcard, false, 2, trump);
        if (result == true)
        {
            defender1 = next_player(players, current);
            defender2 = next_player(players, next_player(players, defender1));

            return trump;
        }
        current = next_player(players, current);

    } while (current != players[1]);

    defender1 = next_player(players, dealer);
    defender2 = next_player(players, next_player(players, defender1));
    return trump;
}
//Check who wins a trick by finding the greatest valued card and find who played that card
Player *who_win_the_trick(Player *players[], Card *middle[], Player *leader, Card *lead_card, string trump)
{
    Card *highest = middle[0];
    int track = 0;
    for (int i = 0; i < 4; i++)
    {
        if (Card_less(*highest, *middle[i], *lead_card, trump))
        {
            highest = middle[i];
            track = i;
        }
    }

    return players[track];
}

int get_points(Player *players[], Player *p)
{
    return points[&p - players];
}
void win_points(Player *players[], Player *p, int new_point)
{
    points[&p - players] += new_point;
}

int get_tricks_won(Player *players[], Player *p)
{
    return tricks_won[&p - players];
}
void win_a_trick(Player *players[], Player *p)
{
    tricks_won[&p - players]++;
}

//Give points to team who win the trick
void win_the_trick(Player *players[], Player *w1)
{
    win_a_trick(players, w1);
}

void update_points(Player *players[], int points[], int tricks_won[], Player *dealer, Player *defender1, Player *defender2)
{
    int tricks_won_t1 = get_tricks_won(players, players[0]) + get_tricks_won(players, players[2]);
    int tricks_won_t2 = get_tricks_won(players, players[1]) + get_tricks_won(players, players[3]);
    if (tricks_won_t1 >= 3)
    {
        cout << players[0]->get_name() << " and " << players[2]->get_name() << " wins the hand" << endl;
        //When Team one orders up
        if (&defender1 != &players[0] && &defender2 != &players[2])
        {
            if (tricks_won_t1 == 5)
            {
                cout << "A March!!!" << endl;
                win_points(players, players[0], 2);
                win_points(players, players[2], 2);
            }
            else
            {
                win_points(players, players[0], 1);
                win_points(players, players[2], 1);
            }
        }
        else
        {
            cout << "Euchred!!!" << endl;
            win_points(players, players[0], 3);
            win_points(players, players[2], 3);
        }
    }

    else
    {
        cout << players[1]->get_name() << " and " << players[3]->get_name() << " wins the hand" << endl;
        //When Team one orders up
        if (&defender1 != &players[1] && &defender2 != &players[3])
        {
            if (tricks_won_t1 == 5)
            {
                cout << "A March!!!" << endl;
                win_points(players, players[1], 2);
                win_points(players, players[3], 2);
            }
            else
            {
                win_points(players, players[1], 1);
                win_points(players, players[3], 1);
            }
        }
        else
        {
            cout << "Euchred!!!" << endl;
            win_points(players, players[1], 3);
            win_points(players, players[3], 3);
        }
    }
}

//Check whether there is a winner for the entire game, if there is, return the index of the winner. Else returns -1
int is_there_game_winner(int points[], int points_to_win)
{
    for (int i = 0; i < 4; i++)
    {
        if (points[i] >= points_to_win)
            return i;
    }
    return -1;
}
