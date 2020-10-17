// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name)
{   //simple
    Player *alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_add_card)
{   //simple
    Player *alice = Player_factory("Alice", "Simple");
    Card c("Two", "Diamonds");
    Card e("Jack", "Clubs");
    Card d("Jack", "Hearts");
    Card b("Two", "Hearts");
    Card a("Two", "Spades");
    ASSERT_EQUAL(alice->add_card(c)->hand[0], c);
    delete alice;
}

TEST(test_make_trump)
{
    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);

    Card nine_hearts(Card::RANK_NINE, Card::SUIT_HEARTS);
    string trump = Card::SUIT_HEARTS;
    bool orderup = bob->make_trump(
      nine_hearts,    //upcard
      true,           //Bob is also the dealer
      1,              //first round
      trump           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup);
    ASSERT_EQUAL(trump, Card::SUIT_HEARTS);

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);

    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    string trump = Card::SUIT_SPADES;
    bool orderup = bob->make_trump(
      nine_spades,    //upcard
      true,           //Bob is also the dealer
      2,              //first round
      trump           //suit ordered up (if any)
    );
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, Card::SUIT_SPADES);

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);

    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    string trump = Card::SUIT_SPADES;
    bool orderup = bob->make_trump(
      nine_spades,    //upcard
      false,           //Bob is also the dealer
      1,              //first round
      trump           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup);
    ASSERT_EQUAL(trump, Card::SUIT_SPADES);

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);

    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    string trump = Card::SUIT_SPADES;
    bool orderup = bob->make_trump(
      nine_spades,    //upcard
      false,           //Bob is also the dealer
      2,              //first round
      trump           //suit ordered up (if any)
    );
    ASSERT_FALSE(orderup);
    ASSERT_EQUAL(trump, Card::SUIT_SPADES);

    delete bob;

}

TEST(add_and_discard)
{   //simple
    /*Card c("Two", "Diamonds");
    Card e("Jack", "Clubs");
    Card d("Jack", "Hearts");
    Card b("Two", "Hearts");
    Card a("Two", "Spades");*/
    string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    string trump="Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    alice->add_card(a);
    ASSERT_EQUAL(alice->add_and_discard(a)->hand[0], a);
    ASSERT_EQUAL(alice->add_and_discard(trump)->hand[0], a);
}

TEST(test_lead_card)
{   //simple
    /*string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    ASSERT_EQUAL(alice->lead_card("Spades")->hand[0], b);
    ASSERT_EQUAL(alice->lead_card("Hearts")->hand[0], c);*/

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);
    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    bob->add_and_discard(nine_spades);
    Card card_led = bob->lead_card(Card::SUIT_HEARTS);
    ASSERT_EQUAL(card_led, nine_spades); //check led card

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);
    Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
    bob->add_and_discard(nine_spades);
    Card card_led = bob->lead_card(Card::SUIT_DIAMONDS);
    ASSERT_EQUAL(card_led, nine_spades); //check led card
    delete bob;
}

TEST(test_play_card)
{   //simple
    /*string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    string x = "Hearts";
    ASSERT_EQUAL(alice->play_card(x, "Spades")->hand[0], b);
    ASSERT_EQUAL(alice->play_card(x, "Hearts")->hand[0], c);*/

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);
    Card nine_hearts(Card::RANK_NINE, Card::SUIT_HEARTS);
    string trump = Card::SUIT_HEARTS;
    Card card_played = bob->play_card(nine_hearts, trump); 
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    ASSERT_EQUAL(card_played, two_spades);
    

    Player * bob = Player_factory("Bob", "Simple");
    add_cards(bob);
    Card nine_clubs(Card::RANK_NINE, Card::SUIT_CLUBS);
    string trump = Card::SUIT_HEARTS;
    Card card_played = bob->play_card(nine_clubs, trump); 
    Card two_spades = Card(Card::RANK_TWO, Card::SUIT_SPADES);
    ASSERT_EQUAL(card_played, two_spades);
    delete bob;
}
// Add more tests here

TEST_MAIN()
