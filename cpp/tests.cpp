#include <iostream>
#include "baraja.h"
#include <gtest/gtest.h>

using namespace std;

TEST(NaipeTest, TestRepr) {
    Naipe nn(1);
    EXPECT_EQ(nn.palo(), 0);
    EXPECT_EQ(nn.numero(), 2);
    EXPECT_EQ(nn.repr(), "2S") <<"Should be 2S but gave "<< nn.repr()<<endl;

    Naipe n2(23);
    EXPECT_EQ(n2.repr(), "JH") <<"Should be JH but gave "<< n2.repr()<<endl;
    
    Naipe n3(35);
    EXPECT_EQ(n3.repr(), "10C") <<"Should be 10C but gave "<< n3.repr()<<endl;

    Naipe n4(51);
    EXPECT_EQ(n4.repr(), "KD") <<"Should be KD but gave "<< n4.repr()<<endl;
};

