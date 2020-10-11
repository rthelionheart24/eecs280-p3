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

class Simple_player : public Player
{
private:
    std::string name;
    std::vector<Card> hand;

public:
    Simple_player::Simple_player(const std::string &in_name) : name(in_name){};

    const std::string &get_name() const
    {
        return name;
    }

    void add_card(const Card &c)
    {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const
    {

        assert(round <= 2);
        std::string suit_considered;
        //During round 1
        if (round == 1)
        {
            suit_considered = upcard.get_suit();
            int num_trump = 0;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_trump(suit_considered))
                    num_trump++;
            }

            if (num_trump >= 2)
            {
                order_up_suit = suit_considered;
                return true;
            }
            return false;
        }
        //During round 2
        else if (round == 2)
        {
            //The new suit we are considering
            suit_considered = Suit_next(upcard.get_suit());

            //When the Player is the dealer, we force him to order up
            if (is_dealer == true)
            {
                screw_the_dealer(upcard, order_up_suit);
                return true;
            }

            //Count how many cards of the new suits we have
            int num_next = 0;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_trump(suit_considered))
                    num_next++;
            }

            if (num_next >= 1)
            {
                order_up_suit = suit_considered;
                return true;
            }
            return false;
        }
    }

    void add_and_discard(const Card &upcard)
    {
        assert(hand.size() != 0);
        add_card(upcard);
        hand.erase(hand.begin() + find_lowest(hand, upcard.get_suit()));
    }

    Card lead_card(const std::string &trump)
    {
        assert(hand.size() != 0);
        assert(check_suit(trump));
        std::sort(hand.begin(), hand.end());
        Card lead;
        //If there is no trump card, the last element after sort is the greatest
        if (!have_trump(trump))
        {
            lead = hand[hand.size() - 1];
            hand.pop_back();
            return lead;
        }
        //If there is trump card
        else
        {
            std::vector<Card> trumps;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_trump(trump))
                {
                    trumps.push_back(hand[i]);
                    hand.erase(hand.begin() + i);
                }
            }
            //If all are trump cards
            if (hand.empty())
            {
                hand = trumps;
                lead = hand[find_highest(hand, trump)];
                return lead;
            }
            //If some are not trump cards
            else
            {
                std::sort(hand.begin(), hand.end());
                lead = hand[hand.size() - 1];
                hand.pop_back();
                hand.insert(hand.end(), trumps.begin(), trumps.end());
                return lead;
            }
        }
    }

    Card play_card(const Card &led_card, const std::string &trump)
    {

        assert(hand.size() != 0);
        assert(check_suit(trump));
        std::string lead_suit = led_card.get_suit();
        Card play;
        if (!have_lead(lead_suit))
        {
            play = hand[find_lowest(hand, trump)];
            hand.erase(hand.begin() + find_lowest(hand, trump));
            return play;
        }
        else
        {
            //A seperate vector for cards with the leading suit
            std::vector<Card> lead;
            for (int i = 0; i < hand.size(); i++)
            {
                if (hand[i].get_suit() == led_card.get_suit())
                {
                    lead.push_back(hand[i]);
                    hand.erase(hand.begin() + i);
                }
            }

            play = lead[find_highest(lead, trump)];
            lead.erase(lead.begin() + find_highest(lead, trump));
            hand.insert(hand.end(), lead.begin(), lead.end());
            return play;
        }
    }

    int find_lowest(std::vector<Card> deck, const std::string &trump)
    {
        Card *lowest = &hand[0];
        int track = 0;
        for (int i = 0; i < hand.size(); i++)
        {
            if (Card_less(hand[i], *lowest, trump))
            {
                lowest = &hand[i];
                track = i;
            }
        }

        return track;
    }

    int find_highest(std::vector<Card> deck, const std::string &trump)
    {
        Card *highest = &hand[0];
        int track = 0;
        for (int i = 0; i < hand.size(); i++)
        {
            if (Card_less(*highest, hand[i], trump))
            {
                highest = &hand[i];
                track = i;
            }
        }

        return track;
    }
    //Additional functions belonging to Simple_player

    //REQUIRES No one to order up
    //EFFECT Forces the dealer to order up the suit
    void screw_the_dealer(const Card &upcard, std::string &order_up_suit) const
    {
        std::cout << "Screw the dealer!!!" << std::endl;
        order_up_suit = Suit_next(upcard.get_suit());
    }
    //EFFECT Check whether there are trump cards in hand
    bool have_trump(const std::string &trump)
    {
        for (int i = 0; i < hand.size(); i++)
        {
            if (hand[i].is_trump(trump))
                return true;
        }
        return false;
    }
    //EFFECT Check whether there are cards of the leading suit in hand
    bool have_lead(const std::string &lead)
    {
        for (int i = 0; i < hand.size(); i++)
        {
            if (hand[i].get_suit() == lead)
                return true;
        }
        return false;
    }
};

class Human_player : public Player
{
private:
    std::string name;
    std::vector<Card> hand;

public:
    Human_player::Human_player(const std::string &in_name) : name(in_name){};

    const std::string &get_name() const
    {
        return name;
    }

    void add_card(const Card &c)
    {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
    }
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, std::string &order_up_suit) const
    {
        assert(round <= 2);
        std::sort(hand.begin(), hand.end());

        for (int i = 0; i < hand.size(); i++)
            std::cout << hand[i] << std::endl;

        std::string choice;
        std::cout << "Order up or pass?" << std::endl
                  << "Spades\n"
                  << "Hearts\n"
                  << "Clubs\n"
                  << "Diamonds\n"
                  << "pass" << std::endl;
        std::cin >> choice;
        if (choice == "pass")
            return false;
        else
        {
            order_up_suit = choice;
            return true;
        }
    }

    void add_and_discard(const Card &upcard)
    {
        assert(hand.size() != 0);
        add_card(upcard);
        std::sort(hand.begin(), hand.end());

        for (int i = 0; i < hand.size(); i++)
            std::cout << i << ". " << hand[i] << std::endl;

        std::string choice;
        std::cout << "Which one do you want to discard?\n";
        std::cin >> choice;
        hand.erase(hand.begin() + stoi(choice));
    }
    Card lead_card(const std::string &trump)
    {
        assert(hand.size() != 0);
        assert(check_suit(trump));

        std::sort(hand.begin(), hand.end());

        for (int i = 0; i < hand.size(); i++)
            std::cout << i << ". " << hand[i] << std::endl;
        std::string choice;
        Card lead;
        std::cout << "Which one do you want lead?\n";
        std::cin >> choice;
        lead = hand[stoi(choice)];
        hand.erase(hand.begin() + stoi(choice));
        return lead;
    }

    Card play_card(const Card &led_card, const std::string &trump)
    {

        assert(hand.size() != 0);
        assert(check_suit(trump));

        std::sort(hand.begin(), hand.end());

        for (int i = 0; i < hand.size(); i++)
            std::cout << i << ". " << hand[i] << std::endl;
        std::string choice;
        Card play;
        std::cout << "Which one do you want play?\n";
        std::cin >> choice;
        play = hand[stoi(choice)];
        hand.erase(hand.begin() + stoi(choice));
        return play;
    }
};

Player *Player_factory(const std::string &name, const std::string &strategy)
{
    if (strategy == "Simple")

        return new Simple_player(name);

    if (strategy == "Human")
        return new Human_player(name);

    assert(false);
    return nullptr;
}

bool check_rank(const std::string &rank_in)
{
    if (find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, rank_in) !=
        (RANK_NAMES_BY_WEIGHT + NUM_RANKS))
        return true;
    return false;
}

bool check_suit(const std::string &suit_in)
{
    if (find(SUIT_NAMES_BY_WEIGHT, SUIT_NAMES_BY_WEIGHT + NUM_SUITS, suit_in) !=
        (SUIT_NAMES_BY_WEIGHT + NUM_SUITS))
        return true;
    return false;
}