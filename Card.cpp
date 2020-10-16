// Project UID 1d9f47bfc76643019cfbf037641defe1

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
#include "Card.h"
// add any necessary #include or using directives here

// rank and suit names -- do not remove these
constexpr const char *const Card::RANK_TWO;
constexpr const char *const Card::RANK_THREE;
constexpr const char *const Card::RANK_FOUR;
constexpr const char *const Card::RANK_FIVE;
constexpr const char *const Card::RANK_SIX;
constexpr const char *const Card::RANK_SEVEN;
constexpr const char *const Card::RANK_EIGHT;
constexpr const char *const Card::RANK_NINE;
constexpr const char *const Card::RANK_TEN;
constexpr const char *const Card::RANK_JACK;
constexpr const char *const Card::RANK_QUEEN;
constexpr const char *const Card::RANK_KING;
constexpr const char *const Card::RANK_ACE;

constexpr const char *const Card::SUIT_SPADES;
constexpr const char *const Card::SUIT_HEARTS;
constexpr const char *const Card::SUIT_CLUBS;
constexpr const char *const Card::SUIT_DIAMONDS;

// add your code below
bool check_rank(const std::string &rank_in);
bool check_suit(const std::string &suit_in);
//Public functions
Card::Card() : rank(RANK_TWO), suit(SUIT_SPADES)
{
}

Card::Card(const std::string &rank_in, const std::string &suit_in) : rank(rank_in), suit(suit_in)
{
    assert(check_suit(suit_in));
    assert(check_rank(rank_in));
    this->rank = rank_in;
    this->suit = suit_in;
}

std::string Card::get_rank() const
{
    return this->rank;
}
std::string Card::get_suit() const
{
    return this->suit;
}
std::string Card::get_suit(const std::string &trump) const
{
    assert(check_suit(trump));
    if (this->get_suit() == trump ||
        (this->get_rank() == RANK_JACK && this->get_suit() == Suit_next(trump)))
        return trump;
    return this->get_suit();
}

bool Card::is_face() const
{
    if (this->get_rank() == RANK_JACK ||
        this->get_rank() == RANK_QUEEN ||
        this->get_rank() == RANK_KING ||
        this->get_rank() == RANK_ACE)
        return true;
    return false;
}

bool Card::is_right_bower(const std::string &trump) const
{
    assert(check_suit(trump));
    if (this->get_suit() == trump && this->get_rank() == RANK_JACK)
        return true;
    return false;
}
bool Card::is_left_bower(const std::string &trump) const
{
    assert(check_suit(trump));
    if (this->get_suit() == Suit_next(trump) && this->get_rank() == RANK_JACK)
        return true;
    return false;
}

bool Card::is_trump(const std::string &trump) const
{
    if (this->get_suit(trump) == trump)
        return true;
    return false;
}

//non-class functions
bool operator<(const Card &lhs, const Card &rhs)
{

    if (find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, lhs.get_rank()) <
        find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, rhs.get_rank()))
        return true;
    else if (find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, lhs.get_rank()) >
             find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, rhs.get_rank()))
        return false;
    else
    {
        if (find(SUIT_NAMES_BY_WEIGHT, SUIT_NAMES_BY_WEIGHT + NUM_SUITS, lhs.get_suit()) <
            find(SUIT_NAMES_BY_WEIGHT, SUIT_NAMES_BY_WEIGHT + NUM_SUITS, rhs.get_suit()))
            return true;
        return false;
    }
}

bool operator>(const Card &lhs, const Card &rhs)
{

    if (find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, lhs.get_rank()) >
        find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, rhs.get_rank()))
        return true;
    else if (find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, lhs.get_rank()) <
             find(RANK_NAMES_BY_WEIGHT, RANK_NAMES_BY_WEIGHT + NUM_RANKS, rhs.get_rank()))
        return false;
    else
    {

        if (find(SUIT_NAMES_BY_WEIGHT, SUIT_NAMES_BY_WEIGHT + NUM_SUITS, lhs.get_suit()) <
            find(SUIT_NAMES_BY_WEIGHT, SUIT_NAMES_BY_WEIGHT + NUM_SUITS, rhs.get_suit()))
            return true;
        return false;
    }
}

bool operator==(const Card &lhs, const Card &rhs)
{
    if (!(lhs > rhs) && !(lhs < rhs))
        return true;
    return false;
}
bool operator!=(const Card &lhs, const Card &rhs)
{
    if (!(lhs == rhs))
        return true;
    return false;
}
std::string Suit_next(const std::string &suit)
{
    assert(check_suit(suit));

    if (suit == "Spades")
        return "Clubs";
    else if (suit == "Clubs")
        return "Spades";
    else if (suit == "Hearts")
        return "Diamonds";
    else
        return "Hearts";
}
std::ostream &operator<<(std::ostream &os, const Card &card)
{
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

bool Card_less(const Card &a, const Card &b, const std::string &trump)
{
    assert(check_suit(trump));
    if (a.is_trump(trump) && !b.is_trump(trump))
        return false;
    if (!a.is_trump(trump) && b.is_trump(trump))
        return true;
    if (a.is_trump(trump) && b.is_trump(trump))
    {
        if (b.is_right_bower(trump))
            return true;
        if (a.is_right_bower(trump))
            return false;
        if (b.is_left_bower(trump))
            return true;
        if (a.is_left_bower(trump))
            return false;
        return a < b;
    }
    else
        return a < b;
}
bool Card_less(const Card &a, const Card &b, const Card &led_card,
               const std::string &trump)
{
    assert(check_suit(trump));
    std::string led_suit = led_card.get_suit();
    if (a.is_trump(trump) && !b.is_trump(trump))
        return false;
    if (!a.is_trump(trump) && b.is_trump(trump))
        return true;
    if (a.is_trump(trump) && b.is_trump(trump))
    {
        if (b.is_right_bower(trump))
            return true;
        if (a.is_right_bower(trump))
            return false;
        if (b.is_left_bower(trump))
            return true;
        if (a.is_left_bower(trump))
            return false;
        return a < b;
    }
    else
    {
        if (a.get_suit() == led_suit && b.get_suit() != led_suit)
            return false;
        if (a.get_suit() != led_suit && b.get_suit() == led_suit)
            return true;
        //when both are led or both aren't
        return a < b;
    }
}

//helper functions to check whether the suits and ranks are valid

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