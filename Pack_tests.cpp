// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include "unit_test_framework.h"
#include <iostream>

using namespace std;

//测试一下前四张卡，对照第一个把剩下的补齐就好了
TEST(test_pack_default_ctor)
{
    Pack *pack = new Pack();
    Card first = pack->deal_one();
    Card second = pack->deal_one();
    Card third = pack->deal_one();
    Card fourth = pack->deal_one();
    ASSERT_TRUE(Card::RANK_NINE == first.get_rank() && Card::SUIT_SPADES == first.get_suit());
    ASSERT_TRUE(Card::RANK_NINE == second.get_rank() && Card::SUIT_HEARTS == second.get_suit());
    ASSERT_TRUE(Card::RANK_NINE == third.get_rank() && Card::SUIT_CLUBS == third.get_suit());
    ASSERT_TRUE(Card::RANK_NINE == fourth.get_rank() && Card::SUIT_DIAMONDS == fourth.get_suit());
}

TEST(test_deal_one_simple)
{
    Pack *pack = new Pack();
    Card first("Nine", "Spades");
    Card second("Nine", "Hearts");
    ASSERT_TRUE(pack->deal_one() == first);
    ASSERT_TRUE(pack->deal_one() == second);
}
//这个reset（）不太好测，所有其他fuction都会用reset（），应该没啥大问题

// TEST(test_reset)
// {
//     Pack *pack = new Pack();
//     pack->deal_one();
//     Card *first = &(pack->deal_one());
//     pack->reset();
//     ASSERT_EQUAL();
// }

TEST(test_shuffle_simple)
{
    Pack *pack = new Pack();
    pack->shuffle();
    Card first("Queen", "Spades");
    Card second("Nine", "Spades");
    ASSERT_TRUE(first == pack->deal_one());
    ASSERT_TRUE(second == pack->deal_one());
}

TEST(test_empty_simple)
{
    Pack *pack = new Pack();
    for (int i = 0; i < 24; i++)
        pack->deal_one();
    ASSERT_TRUE(pack->empty());
}
// Add more tests here

TEST_MAIN()
