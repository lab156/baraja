#include <iostream>
#include "baraja.h"
#include <gtest/gtest.h>

using namespace std;

TEST(TestRepr, NaipeTest) {
    Naipe nn(1);
    EXPECT_EQ(nn.palo(), 0);
    EXPECT_EQ(nn.numero(), 2);
    EXPECT_EQ(nn.repr(), "2S") <<"Should be 2S but gave "<< nn.repr()<<endl;

    Naipe n2(23);
    EXPECT_EQ(n2.repr(), "JH") <<"Should be JH but gave "<< n2.repr()<<endl;
    
    Naipe n3(35);
    EXPECT_EQ(n3.repr(), "10C") <<"Should be 10C but gave "<< n3.repr()<<endl;

};

TEST(TestEqual, NaipeTest) {
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
