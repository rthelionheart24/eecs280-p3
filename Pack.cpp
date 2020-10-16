#include "Pack.h"
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

Pack::Pack()
{
    reset();
    for (int i = 0; i < NUM_SUITS; i++)
    {
        for (int j = 7; j < NUM_RANKS; j++)
        {

            cards[next++] = Card(RANK_NAMES_BY_WEIGHT[j], SUIT_NAMES_BY_WEIGHT[i]);
        }
    }
    reset();
}

Pack::Pack(std::istream &pack_input)
{
    reset();
    std::string rank_in, suit_in, junk;
    while (pack_input >> rank_in)
    {
        pack_input >> junk;
        pack_input >> suit_in;
        cards[next++] = Card(rank_in, suit_in);
    }
    reset();
}

Card Pack::deal_one()
{
    assert(next <= 23);
    return cards[next++];
}

void Pack::reset()
{
    next = 0;
}

void Pack::shuffle()
{
    reset();
    const int length = 24;

    Card p1[length / 2] = {};
    Card p2[length / 2] = {};
    for (int i = 0; i < 7; i++)
    {

        for (int j = 0; j < length; j++)
        {
            if (j < length / 2)
                p1[j] = deal_one();
            else
                p2[j - 12] = deal_one();
        }
        this->reset();
        int track1 = 0, track2 = 0;
        for (int k = 0; k < length; k++)
        {
            if (k % 2 == 0)
                cards[next++] = p2[track2++];
            else
                cards[next++] = p1[track1++];
        }
        reset();
    }

    reset();
}

bool Pack::empty() const
{
    if (next == 24)
        return true;
    return false;
}