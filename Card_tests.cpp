// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

TEST(test_card_ctor)
{
    Card c(Card::RANK_ACE, Card::SUIT_HEARTS);
    ASSERT_EQUAL(Card::RANK_ACE, c.get_rank());
    ASSERT_EQUAL(Card::SUIT_HEARTS, c.get_suit());
}

TEST(test_card_get_rank)
{
    Card c(Card::RANK_TWO, Card::SUIT_SPADES);
    ASSERT_EQUAL(c.get_rank(), "Two");
}
TEST(test_card_get_suit)
{
    Card c(Card::RANK_TWO, Card::SUIT_SPADES);
    ASSERT_EQUAL(c.get_suit(), "Spades");
}

//question
TEST(test_card_get_suit_trump)
{
    Card c(Card::RANK_TWO, Card::SUIT_SPADES);
    ASSERT_EQUAL(c.get_suit(), "Spades");

    Card d(Card::RANK_JACK, Card::SUIT_CLUBS);
    ASSERT_EQUAL(d.get_suit(Card::SUIT_SPADES), "Spades");
}

TEST(test_is_face)
{
    Card c("Two", "Spades");
    ASSERT_EQUAL(c.is_face(), false);
    Card b("Jack", "Spades");
    ASSERT_EQUAL(b.is_face(), true);
}

TEST(test_is_trump)
{
    Card e("Jack", "Clubs");
    Card d("Jack", "Hearts");
    Card c("Jack", "Spades");
    Card b("Two", "Hearts");
    Card a("Two", "Spades");
    string x = "Spades";
    ASSERT_EQUAL(e.is_trump(x), true);
    ASSERT_EQUAL(d.is_trump(x), false);
    ASSERT_EQUAL(c.is_trump(x), true);
    ASSERT_EQUAL(b.is_trump(x), false);
    ASSERT_EQUAL(a.is_trump(x), true);
}

TEST(test_is_right_bower)
{
    Card e("Jack", "Clubs");
    Card d("Jack", "Hearts");
    Card c("Jack", "Spades");
    Card b("Two", "Hearts");
    Card a("Two", "Spades");
    string x = "Spades";
    ASSERT_EQUAL(e.is_right_bower(x), false);
    ASSERT_EQUAL(d.is_right_bower(x), false);
    ASSERT_EQUAL(c.is_right_bower(x), true);
    ASSERT_EQUAL(b.is_right_bower(x), false);
    ASSERT_EQUAL(a.is_right_bower(x), false);
}

TEST(test_is_left_bower)
{
    Card e("Jack", "Clubs");
    Card d("Jack", "Hearts");
    Card c("Jack", "Spades");
    Card b("Two", "Hearts");
    Card a("Two", "Spades");
    string x = "Spades";
    ASSERT_EQUAL(e.is_left_bower(x), true);
    ASSERT_EQUAL(d.is_left_bower(x), false);
    ASSERT_EQUAL(c.is_left_bower(x), false);
    ASSERT_EQUAL(b.is_left_bower(x), false);
    ASSERT_EQUAL(a.is_left_bower(x), false);
}

TEST(test_operator1)
{
    Card c("Two", "Spades");
    Card b("Jack", "Spades");
    Card a("Two", "Hearts");
    ASSERT_EQUAL(a < b, true);
    ASSERT_EQUAL(a < c, false);
    ASSERT_EQUAL(c < b, true);
}

TEST(test_operator2)
{
    Card c("Two", "Spades");
    Card b("Jack", "Spades");
    Card a("Two", "Hearts");
    ASSERT_EQUAL(a > b, false);
    ASSERT_EQUAL(a > c, true);
    ASSERT_EQUAL(c > b, false);
}

TEST(test_operator3)
{
    Card c("Two", "Spades");
    Card b("Jack", "Spades");
    Card a("Two", "Hearts");
    ASSERT_EQUAL(a == b, false);
    ASSERT_EQUAL(a == c, false);
    ASSERT_EQUAL(c == b, false);
    ASSERT_EQUAL(a == a, true);
}
TEST(test_operator4)
{
    Card c("Two", "Spades");
    Card b("Jack", "Spades");
    Card a("Two", "Hearts");
    ASSERT_EQUAL(a != b, true);
    ASSERT_EQUAL(a != c, true);
    ASSERT_EQUAL(c != b, true);
    ASSERT_EQUAL(a != a, false);
}

TEST(test_suit_next)
{
    string c = "Diamonds";
    string b = "Spades";
    string a = "Hearts";
    ASSERT_EQUAL(Suit_next(a), "Diamonds");
    ASSERT_EQUAL(Suit_next(b), "Clubs");
    ASSERT_EQUAL(Suit_next(c), "Hearts");
}

TEST(test_card_less)
{
    Card c("Two", "Diamonds");
    Card b("Jack", "Spades");
    Card a("Jack", "Hearts");
    ASSERT_EQUAL(Card_less(a, b, "Hearts"), false);
    ASSERT_EQUAL(Card_less(c, b, "Hearts"), true);
    ASSERT_EQUAL(Card_less(a, c, "Hearts"), false);
}

TEST(test_card_less2)
{
    Card c("Two", "Spades");
    Card b("Jack", "Spades");
    Card a("Two", "Hearts");
    ASSERT_EQUAL(Card_less(a, b, c, "Hearts"), false);
    ASSERT_EQUAL(Card_less(c, b, a, "Hearts"), true);
    ASSERT_EQUAL(Card_less(a, c, b, "Hearts"), false);
}

// Add more test cases here

TEST_MAIN()
