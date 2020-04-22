#include <iostream>
#include "baraja.h"
#include <gtest/gtest.h>

using namespace std;

TEST(NaipeTests, TestStringRepr) {
    Naipe nn(1);
    EXPECT_EQ(nn.palo(), 0);
    EXPECT_EQ(nn.numero(), 2);
    EXPECT_EQ(nn.repr(), "2S") <<"Should be 2S but gave "<< nn.repr()<<endl;

    Naipe n2(23);
    EXPECT_EQ(n2.repr(), "JH") <<"Should be JH but gave "<< n2.repr()<<endl;
    
    Naipe n3(35);
    EXPECT_EQ(n3.repr(), "10C") <<"Should be 10C but gave "<< n3.repr()<<endl;

};

TEST(NaipeTests, TestEqualOperator) {
   Naipe n1(13);
   Naipe n2(13);
   EXPECT_EQ(n1, n2);

   EXPECT_EQ(n1, Naipe(n1.numero(), n1.palo()));
   EXPECT_EQ(Naipe(23), Naipe(23));
   EXPECT_EQ(Naipe(32), Naipe(32));

    Naipe n(51);
    EXPECT_EQ(n, Naipe(n.numero(), n.palo())) <<"Should be KD but gave "<< n.repr()<<endl;
    n = Naipe(14);
    EXPECT_EQ(n, Naipe(n.numero(), n.palo())) <<"Should be KD but gave "<< n.repr()<<endl;
    n = Naipe(0);
    EXPECT_EQ(n, Naipe(n.numero(), n.palo())) <<"Should be KD but gave "<< n.repr()<<endl;


    Naipe n7(13,3);
    EXPECT_EQ(n7.repr(), "KD") <<"Should be KD but gave "<< n7.repr()<<endl;
   EXPECT_NE(Naipe(23), Naipe(43));
};


TEST(NaipeTests, TestGetPaloFromString) {
    EXPECT_EQ(get_palo_from_str('S'), 0);
    EXPECT_EQ(get_palo_from_str('H'), 1);
    EXPECT_EQ(get_palo_from_str('C'), 2);
    EXPECT_EQ(get_palo_from_str('D'), 3);

    try {
        get_palo_from_str('U');
    }
    catch (int ee) {
        EXPECT_EQ(ee, 20);
    }
};

TEST(NaipeTests, TestCharContructor) {
    Naipe n("QC");
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));

    n = Naipe(24);
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));

    n = Naipe(51);
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));

    n = Naipe("3H");
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));
    n = Naipe("7D");
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));
    n = Naipe("2S");
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));
    n = Naipe("AH");
    EXPECT_EQ(n, Naipe(n.numero(), n.palo()));
};


TEST(Prizes, PrizeValues) {
    EXPECT_EQ(Loss, 0);
    EXPECT_EQ(JacksOrBetter, 1);
    EXPECT_EQ(TwoPair, 2);
    EXPECT_EQ(ThreeOfAKind, 3);
    EXPECT_EQ(Straight, 4);
    EXPECT_EQ(Flush, 6);
    EXPECT_EQ(FullHouse, 9);
    EXPECT_EQ(Poker, 25);
    EXPECT_EQ(StraightFlush, 50);
    EXPECT_EQ(RoyalFlush, 250);
};
