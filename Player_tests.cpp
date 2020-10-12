// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name)
{
    Player *alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_add_card)
{
    Player *alice = Player_factory("Alice", "Simple");
    Card c("Two", "Diamonds");
    ASSERT_EQUAL(alice->add_card(c)->hand[0], c);
    delete alice;
}

TEST(test_make_trump)
{
}

TEST(add_and_discard)
{
    string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    ASSERT_EQUAL(alice->add_and_discard(a)->hand[0], a);
    ASSERT_EQUAL(alice->add_and_discard(b)->hand[0], b);
}

TEST(test_lead_card)
{
    string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    ASSERT_EQUAL(alice->lead_card("Spades")->hand[0], b);
    ASSERT_EQUAL(alice->lead_card("Hearts")->hand[0], c);
}

TEST(test_play_card)
{
    string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    string x = "Hearts";
    ASSERT_EQUAL(alice->play_card(x, "Spades")->hand[0], b);
    ASSERT_EQUAL(alice->play_card(x, "Hearts")->hand[0], c);
}
// Add more tests here

TEST_MAIN()
