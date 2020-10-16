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
#include "Card.h"

bool player_check_rank(const std::string &rank_in);
bool player_check_suit(const std::string &suit_in);

class Simple_player : public Player
{

private:
    std::string name;
    std::vector<Card> hand;

public:
    Simple_player(const std::string &in_name) : name(in_name)
    {
    }

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
            for (unsigned int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_trump(suit_considered) && hand[i].is_face())
                    num_trump++;
            }

            if (num_trump >= 2)
            {
                order_up_suit = suit_considered;
                std::cout << this->get_name() << " orders up " << order_up_suit << std::endl
                          << std::endl;
                return true;
            }
            std::cout << this->get_name() << " passes" << std::endl;
            return false;
        }
        //During round 2
        else
        {
            //The new suit we are considering
            suit_considered = Suit_next(upcard.get_suit());

            //When the Player is the dealer, we force him to order up
            if (is_dealer == true)
            {
                screw_the_dealer(upcard, order_up_suit);
                std::cout << this->get_name() << " orders up " << order_up_suit << std::endl
                          << std::endl;
                return true;
            }

            //Count how many cards of the new suits we have
            int num_next = 0;
            for (unsigned int i = 0; i < hand.size(); i++)
            {
                if (hand[i].is_trump(suit_considered))
                    num_next++;
            }

            if (num_next >= 1)
            {
                order_up_suit = suit_considered;
                std::cout << this->get_name() << " orders up " << order_up_suit << std::endl
                          << std::endl;
                return true;
            }
            return false;
        }
    }
    void add_and_discard(const Card &upcard)
    {
        assert(hand.size() != 0);
        hand.push_back(upcard);
        int lowest = find_lowest(hand, upcard.get_suit());
        hand.erase(hand.begin() + lowest);
    }

    Card lead_card(const std::string &trump)
    {
        assert(hand.size() != 0);
        assert(player_check_suit(trump));
        std::sort(hand.begin(), hand.end());
        //If there is no trump card, the last element after sort is the greatest
        if (!have_trump(trump))
        {
            Card lead = hand[hand.size() - 1];
            hand.pop_back();
            std::cout << lead << " led by " << this->get_name() << std::endl;
            return lead;
        }
        //If there is trump card
        else
        {
            bool all_trump = true;
            for (unsigned int i = 0; i < hand.size(); i++)
            {
                if (!hand[i].is_trump(trump))
                    all_trump = false;
            }
            //If all are trump cards. the last card is the greatest
            if (all_trump == true)
            {
                Card lead = hand[find_highest(hand, trump)];
                hand.erase(find(hand.begin(), hand.end(), lead));
                std::cout << lead << " led by " << this->get_name() << std::endl;
                return lead;
            }
            //If some are not trump cards
            else
            {
                std::vector<Card> normal;
                for (unsigned int i = 0; i < hand.size(); i++)
                {
                    if (!hand[i].is_trump(trump))
                        normal.push_back(hand[i]);
                }

                Card lead = normal[find_highest(normal, trump)];
                hand.erase(find(hand.begin(), hand.end(), lead));
                std::cout << lead << " led by " << this->get_name() << std::endl;
                return lead;
            }
        }
    }

    Card play_card(const Card &led_card, const std::string &trump)
    {

        assert(hand.size() != 0);
        assert(player_check_suit(trump));
        std::string lead_suit = led_card.get_suit();
        //If can't follow suit
        if (!have_lead(lead_suit, trump))
        {
            Card play = hand[find_lowest(hand, trump)];
            hand.erase(hand.begin() + find_lowest(hand, trump));
            std::cout << play << " played by " << this->get_name() << std::endl;
            return play;
        }
        //If they can follow suit
        else
        {
            //A seperate vector for cards with the leading suit
            std::vector<Card> lead_suit_cards;
            for (unsigned int i = 0; i < hand.size(); i++)
            {
                if (hand[i].get_suit(trump) == led_card.get_suit())
                    lead_suit_cards.push_back(hand[i]);
            }

            Card play = lead_suit_cards[find_highest(lead_suit_cards, trump)];
            hand.erase(find(hand.begin(), hand.end(), play));
            std::cout << play << " played by " << this->get_name() << std::endl;
            return play;
        }
    }

    int find_lowest(std::vector<Card> deck, const std::string &trump)
    {
        Card *lowest = &deck[0];
        int track = 0;
        for (unsigned int i = 0; i < deck.size(); i++)
        {
            if (Card_less(deck[i], *lowest, trump))
            {
                lowest = &deck[i];
                track = i;
            }
        }

        return track;
    }

    int find_highest(std::vector<Card> deck, const std::string &trump)
    {
        Card *highest = &deck[0];
        int track = 0;
        for (unsigned int i = 0; i < deck.size(); i++)
        {
            if (Card_less(*highest, deck[i], trump))
            {
                highest = &deck[i];
                track = i;
            }
        }

        return track;
    }

    void copy_card(Card *c1, const Card c2)
    {
        c1 = new Card(c2.get_rank(), c2.get_suit());
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
        for (unsigned int i = 0; i < hand.size(); i++)
        {
            if (hand[i].is_trump(trump))
                return true;
        }
        return false;
    }
    //EFFECT Check whether there are cards of the leading suit in hand
    bool have_lead(const std::string &lead, const std::string &trump)
    {
        for (unsigned int i = 0; i < hand.size(); i++)
        {
            if (hand[i].get_suit(trump) == lead)
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
    Human_player(const std::string &in_name) : name(in_name)
    {
    }

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
        std::vector<Card> hand_for_show = hand;
        std::sort(hand_for_show.begin(), hand_for_show.end());

        for (unsigned int i = 0; i < hand.size(); i++)
            std::cout << hand_for_show[i] << std::endl;

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

        for (unsigned int i = 0; i < hand.size(); i++)
            std::cout << i << ". " << hand[i] << std::endl;

        std::string choice;
        std::cout << "Which one do you want to discard?\n";
        std::cin >> choice;
        hand.erase(hand.begin() + stoi(choice));
    }
    Card lead_card(const std::string &trump)
    {
        assert(hand.size() != 0);
        assert(player_check_suit(trump));

        std::sort(hand.begin(), hand.end());

        for (unsigned int i = 0; i < hand.size(); i++)
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
        assert(player_check_suit(trump));

        std::sort(hand.begin(), hand.end());

        for (unsigned int i = 0; i < hand.size(); i++)
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

std::ostream &operator<<(std::ostream &os, const Player &p)
{
    os << p.get_name();
    return os;
}

bool player_check_rank(const std::string &rank_in)
{
    if (find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, rank_in) !=
        (RANK_NAMES_BY_WEIGHT + NUM_RANKS))
        return true;
    return false;
}

bool player_check_suit(const std::string &suit_in)
{
    if (find(SUIT_NAMES_BY_WEIGHT, SUIT_NAMES_BY_WEIGHT + NUM_SUITS, suit_in) !=
        (SUIT_NAMES_BY_WEIGHT + NUM_SUITS))
        return true;
    return false;
}
