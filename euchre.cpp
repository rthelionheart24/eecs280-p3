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
#include "Pack.h"
#include "Card.h"

using namespace std;

class Game
{
private:
    bool shuffle;
    Player *players[4];
    Pack *deck;
    int points_to_win;
    Card middle[4];
    int points[4], tricks_won[4];

    string trump;
    Player *dealer, *leader, *defender1, *defender2, *current_player;
    Card up_card, lead_card;

public:
    //After checking that all arguments are valid
    Game(Player *in_players[], Pack *in_deck, bool in_shuffle, int in_points_to_win)
    {

        for (int i = 0; i < 4; i++)
        {
            players[i] = in_players[i];
            points[i] = 0;
            tricks_won[i] = 0;
        }
        deck = in_deck;
        shuffle = in_shuffle;
        points_to_win = in_points_to_win;
        dealer = players[0];
    }

    Player *get_dealer()
    {
        return dealer;
    }

    int seat_of(Player *p)
    {

        for (int i = 0; i < 4; i++)
            if (players[i] == p)
                return i;

        return -1;
    }
    //Returns pointer to the next player in sequence
    Player *next_player(Player *current)
    {
        int index = seat_of(current);
        if (index == 3)
            return players[0];
        return players[index + 1];
    }

    Player *teammate(Player *p)
    {
        return next_player(next_player(p));
    }

    Player *set_dealer()
    {
        return next_player(dealer);
    }

    void first_round_leader()
    {
        leader = next_player(dealer);
    }

    //Shuffle the deck based on the option
    void shuffle_deck()
    {
        deck->reset();
        if (shuffle)
            deck->shuffle();
    }

    //Deal cards to players
    void deal_card()
    {
        current_player = next_player(dealer);
        int deal_num = 3;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < deal_num; j++)
            {
                Card dealt_card = deck->deal_one();
                current_player->add_card(dealt_card);
            }

            current_player = next_player(current_player);

            if (deal_num == 3)
                deal_num--;
            else
                deal_num++;
        }
        deal_num = 2;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < deal_num; j++)
            {
                Card dealt_card = deck->deal_one();
                current_player->add_card(dealt_card);
            }
            current_player = next_player(current_player);

            if (deal_num == 3)
                deal_num--;
            else
                deal_num++;
        }
    }

    //Used for making trump
    //Returns the trump suit
    int order_up()
    {
        up_card = deck->deal_one();

        cout << up_card << " turned up" << endl;
        bool result;

        //Round 1
        current_player = next_player(dealer);
        for (int i = 0; i < 4; i++)
        {
            result = current_player->make_trump(up_card, false, 1, trump);
            if (result == true)
            {
                std::cout << current_player->get_name() << " orders up "
                          << trump << std::endl;
                dealer->add_and_discard(up_card);
                defender1 = next_player(current_player);
                defender2 = next_player(next_player(defender1));
                return 1;
            }
            std::cout << current_player->get_name() << " passes" << std::endl;
            current_player = next_player(current_player);
        }

        //Round 2
        current_player = next_player(dealer);
        for (int i = 0; i < 3; i++)
        {
            result = current_player->make_trump(up_card, false, 2, trump);
            if (result == true)
            {
                std::cout << current_player->get_name() << " orders up "
                          << trump << std::endl;
                defender1 = next_player(current_player);
                defender2 = next_player(next_player(defender1));
                return 2;
            }
            std::cout << current_player->get_name() << " passes" << std::endl;
            current_player = next_player(current_player);
        }

        current_player->make_trump(up_card, true, 2, trump);
        defender1 = next_player(dealer);
        defender2 = next_player(next_player(defender1));
        return 2;
    }

    void lead()
    {
        //Leader leads a card
        lead_card = leader->lead_card(trump);

        //Place the card in the same index as the player who plays it
        middle[seat_of(leader)] = lead_card;
    }
    void play()
    {

        //The rest follow
        current_player = next_player(leader);
        for (int i = 0; i < 3; i++)
        {
            middle[seat_of(current_player)] = current_player->play_card(lead_card, trump);
            current_player = next_player(current_player);
        }
    }

    //Check who wins a trick by finding the greatest valued card
    //and find who played that card
    void who_win_the_trick()
    {
        Card highest = middle[0];
        int track = 0;
        for (int i = 0; i < 4; i++)
        {
            if (Card_less(highest, middle[i], lead_card, trump))
            {
                highest = middle[i];
                track = i;
            }
        }
        //Set the new leader
        leader = players[track];
        dealer = next_player(dealer);
        cout << leader->get_name() << " takes the trick" << endl
             << endl;
        win_a_trick(leader);
    }

    int get_points(Player *p)
    {
        return points[seat_of(p)];
    }
    void win_points(Player *p, int new_point)
    {
        points[seat_of(p)] += new_point;
    }

    int get_tricks_won(Player *p)
    {
        return tricks_won[seat_of(p)];
    }
    void win_a_trick(Player *p)
    {
        tricks_won[seat_of(p)]++;
    }

    void update_points()
    {
        int tricks_won_t1 = get_tricks_won(players[0]) + get_tricks_won(players[2]);
        int tricks_won_t2 = get_tricks_won(players[1]) + get_tricks_won(players[3]);
        if (tricks_won_t1 >= 3)
        {
            cout << players[0]->get_name() << " and "
                 << players[2]->get_name() << " win the hand" << endl;
            //When Team one orders up
            if (defender1 != players[0] && defender1 != players[2])
            {
                if (tricks_won_t1 == 5)
                {
                    cout << "march!" << endl;
                    win_points(players[0], 2);
                    win_points(players[2], 2);
                }
                else
                {
                    win_points(players[0], 1);
                    win_points(players[2], 1);
                }
            }
            else
            {
                cout << "euchred!" << endl;
                win_points(players[0], 2);
                win_points(players[2], 2);
            }
        }

        else if (tricks_won_t2 >= 3)
        {
            cout << players[1]->get_name() << " and "
                 << players[3]->get_name() << " win the hand" << endl;
            //When Team one orders up
            if (defender1 != players[1] && defender1 != players[3])
            {
                if (tricks_won_t2 == 5)
                {
                    cout << "march!" << endl;
                    win_points(players[1], 2);
                    win_points(players[3], 2);
                }
                else
                {
                    win_points(players[1], 1);
                    win_points(players[3], 1);
                }
            }
            else
            {
                cout << "euchred!" << endl;
                win_points(players[1], 2);
                win_points(players[3], 2);
            }
        }

        tricks_won[0] = 0;
        tricks_won[1] = 0;
        tricks_won[2] = 0;
        tricks_won[3] = 0;
    }

    //Check whether there is a winner for the entire game,
    //if there is, return the index of the winner. Else returns -1
    int is_there_game_winner()
    {
        for (int i = 0; i < 4; i++)
        {
            if (points[i] >= points_to_win)
                return i;
        }
        return -1;
    }
};

int check_args(int argc, char *argv[]);
void print_error();
void game_summary(Game *euchre, Player *players[]);
void clean_up(Player *players[], Pack *deck, Game *euchre);

int main(int argc, char *argv[])
{

    //Used to set up the Game instance
    bool shuffle = true;
    Player *players[4];
    Pack *deck;
    int points_to_win, check;

    check = check_args(argc, argv);

    if (check != 0)
        return check;

    //Initialize the pack we use
    string filename = argv[1];
    ifstream in(filename);
    deck = new Pack(in);

    //Shuffle or not
    if (strcmp(argv[2], "noshuffle") == 0)
        shuffle = false;

    //Initialize the points to win
    points_to_win = stoi(argv[3]);

    //Initialize all the players
    for (int i = 4; i <= 10; i += 2)
    {
        players[i / 2 - 2] = Player_factory(argv[i], argv[i + 1]);
    }

    Game *euchre = new Game(players, deck, shuffle, points_to_win);
    //Initialization complete

    //Set the first dealer as player 0
    int hand = 0, winner_seat = 0;
    do
    {
        cout << "Hand " << hand << endl
             << *euchre->get_dealer() << " deals" << endl;

        if (hand > 0)
            euchre->set_dealer();

        euchre->shuffle_deck();
        euchre->deal_card();
        euchre->order_up();
        cout << endl;
        for (int trick = 0; trick < 5; trick++)
        {
            if (trick == 0)
                euchre->first_round_leader();

            euchre->lead();
            euchre->play();
            euchre->who_win_the_trick();
        }
        euchre->update_points();

        game_summary(euchre, players);
        hand++;
        winner_seat = euchre->is_there_game_winner();

    } while (winner_seat == -1);

    cout << *players[winner_seat] << " and "
         << *euchre->teammate(players[winner_seat]) << " win!" << endl;

    clean_up(players, deck, euchre);

    return 0;
}

int check_args(int argc, char *argv[])
{
    for (int i = 0; i < argc; i++)
    {
        cout << argv[i] << " ";
    }
    cout << endl;

    assert(argc == 12);

    //Initialize the pack we use
    string filename = argv[1];
    ifstream in;
    in.open(filename);
    if (!in.is_open())
    {
        cout << "Error opening " << filename << endl;
        return 1;
    }

    //Shuffle or not
    if (!strcmp(argv[2], "noshuffle") && !strcmp(argv[2], "shuffle"))
    {
        print_error();
        return 2;
    }

    //Initialize the points to win
    if (stoi(argv[3]) > 100 || stoi(argv[3]) < 1)
    {

        print_error();
        return 3;
    }

    //Initialize all the players
    for (int i = 4; i <= 10; i += 2)

    {
        string name(argv[i]), type(argv[i + 1]);

        if (type != "Simple" && type != "Human")
        {
            print_error();
            return 4;
        }
    }

    return 0;
}

void print_error()
{
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
         << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
         << "NAME4 TYPE4" << endl;
}

void game_summary(Game *euchre, Player *players[])
{
    cout << players[0]->get_name() << " and " << players[2]->get_name()
         << " have " << euchre->get_points(players[0])
         << " points" << endl
         << players[1]->get_name() << " and "
         << players[3]->get_name() << " have " << euchre->get_points(players[1])
         << " points" << endl
         << endl;
}

void clean_up(Player *players[], Pack *deck, Game *euchre)
{

    for (int i = 0; i < 4; i++)
        delete players[i];

    delete deck;
    delete euchre;
}