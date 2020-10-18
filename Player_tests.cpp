// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name)
{ //simple
  Player *alice = Player_factory("Alice", "Simple");
  ASSERT_EQUAL("Alice", alice->get_name());

  delete alice;
}

TEST(test_add_card)
{ //simple
  Player *alice = Player_factory("Alice", "Simple");
  //用这种方法创建一张卡牌
  Card c(Card::RANK_TWO, Card::SUIT_DIAMONDS);
  Card e(Card::RANK_JACK, Card::SUIT_CLUBS);
  Card d(Card::RANK_JACK, Card::SUIT_SPADES);
  Card b(Card::RANK_TWO, Card::SUIT_HEARTS);
  Card a(Card::RANK_TWO, Card::SUIT_SPADES);
  //正确的给一个人加一张卡的函数
  alice->add_card(c);

  ASSERT_EQUAL(alice->play_card(e, Card::SUIT_DIAMONDS), c);
  delete alice;
}

TEST(test_make_trump)
{
  Player *bob = Player_factory("Bob", "Simple");
  Card c(Card::RANK_TWO, Card::SUIT_DIAMONDS);
  Card e(Card::RANK_JACK, Card::SUIT_CLUBS);
  Card d(Card::RANK_JACK, Card::SUIT_SPADES);
  Card b(Card::RANK_TWO, Card::SUIT_HEARTS);
  Card a(Card::RANK_TWO, Card::SUIT_SPADES);
  bob->add_card(c);
  bob->add_card(e);
  bob->add_card(d);
  bob->add_card(b);
  bob->add_card(a);

  Card nine_hearts(Card::RANK_NINE, Card::SUIT_HEARTS);
  string trump = Card::SUIT_HEARTS;
  bool orderup = bob->make_trump(
      nine_hearts, //upcard
      true,        //Bob is also the dealer
      1,           //first round
      trump        //suit ordered up (if any)
  );
  ASSERT_FALSE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_HEARTS);

  Card nine_spades(Card::RANK_NINE, Card::SUIT_SPADES);
  trump = Card::SUIT_SPADES;
  orderup = bob->make_trump(
      nine_spades, //upcard
      true,        //Bob is also the dealer
      2,           //first round
      trump        //suit ordered up (if any)
  );
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_CLUBS);

  trump = Card::SUIT_SPADES;
  orderup = bob->make_trump(
      nine_spades, //upcard
      false,       //Bob is also the dealer
      1,           //first round
      trump        //suit ordered up (if any)
  );
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_SPADES);

  trump = Card::SUIT_SPADES;
  orderup = bob->make_trump(
      nine_hearts, //upcard
      false,       //Bob is also the dealer
      2,           //first round
      trump        //suit ordered up (if any)
  );
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, Card::SUIT_DIAMONDS);

  delete bob;
}

TEST(add_and_discard)
{ //simple
  /*Card c("Two", "Diamonds");
    Card e("Jack", "Clubs");
    Card d("Jack", "Hearts");
    Card b("Two", "Hearts");
    Card a("Two", "Spades");*/
  Card c(Card::RANK_ACE, Card::SUIT_DIAMONDS);
  Card e(Card::RANK_JACK, Card::SUIT_CLUBS);
  Card d(Card::RANK_JACK, Card::SUIT_HEARTS);
  Card b(Card::RANK_ACE, Card::SUIT_HEARTS);
  Card a(Card::RANK_TWO, Card::SUIT_SPADES);
  Player *alice = Player_factory("Alice", "Simple");

  Card correct;

  alice->add_card(c);
  alice->add_card(e);
  alice->add_and_discard(d);
  correct = c;
  ASSERT_EQUAL(alice->lead_card(Card::SUIT_HEARTS), correct);

  //手里只剩下d了
  alice->add_card(a);
  alice->add_and_discard(b);
  correct = d;
  ASSERT_EQUAL(alice->lead_card(Card::SUIT_HEARTS), correct);

  delete alice;
}

TEST(test_lead_card)
{ //simple
  /*string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    ASSERT_EQUAL(alice->lead_card("Spades")->hand[0], b);
    ASSERT_EQUAL(alice->lead_card("Hearts")->hand[0], c);*/

  Player *bob = Player_factory("Bob", "Simple");

  Card c(Card::RANK_ACE, Card::SUIT_DIAMONDS);
  Card e(Card::RANK_JACK, Card::SUIT_CLUBS);
  Card d(Card::RANK_JACK, Card::SUIT_HEARTS);
  Card b(Card::RANK_ACE, Card::SUIT_HEARTS);
  Card a(Card::RANK_TWO, Card::SUIT_SPADES);

  bob->add_card(c);
  bob->add_card(e);
  bob->add_card(d);
  bob->add_card(b);
  bob->add_card(a);

  Card card_led = bob->lead_card(Card::SUIT_HEARTS);
  ASSERT_EQUAL(card_led, c); //check led card

  card_led = bob->lead_card(Card::SUIT_DIAMONDS);
  ASSERT_EQUAL(card_led, b); //check led card
  delete bob;
}

TEST(test_play_card)
{ //simple
  /*string c = "Spades";
    string b = "Spades";
    string a = "Hearts";
    Player *alice = Player_factory("Alice", "Simple");
    alice->add_card(c);
    alice->add_card(b);
    string x = "Hearts";
    ASSERT_EQUAL(alice->play_card(x, "Spades")->hand[0], b);
    ASSERT_EQUAL(alice->play_card(x, "Hearts")->hand[0], c);*/
  Card c(Card::RANK_ACE, Card::SUIT_DIAMONDS);
  Card e(Card::RANK_JACK, Card::SUIT_DIAMONDS);
  Card d(Card::RANK_JACK, Card::SUIT_HEARTS);
  Card b(Card::RANK_ACE, Card::SUIT_HEARTS);
  Card a(Card::RANK_TWO, Card::SUIT_SPADES);

  Player *bob = Player_factory("Bob", "Simple");

  bob->add_card(c);
  bob->add_card(e);
  bob->add_card(d);
  bob->add_card(b);
  bob->add_card(a);

  Card nine_hearts(Card::RANK_NINE, Card::SUIT_HEARTS);
  Card card_played = bob->play_card(nine_hearts, Card::SUIT_HEARTS);
  ASSERT_EQUAL(card_played, d);

  Card nine_clubs(Card::RANK_NINE, Card::SUIT_CLUBS);
  card_played = bob->play_card(nine_clubs, Card::SUIT_SPADES);
  ASSERT_EQUAL(card_played, d);
  delete bob;
}
// Add more tests here

TEST_MAIN()
