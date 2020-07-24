#include <iostream>
#include "baraja.h"
#include <gtest/gtest.h>
#include <set>
#include <time.h>
#include <cmath>
#include <vector>

using namespace std;

double zvalue( double phat, double p0, int n ) {
    return (phat - p0)/sqrt(p0*(1-p0)/n);
};

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


TEST(PrizeTests, PrizeValues) {
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

TEST(ManoTests, Constructors) {
    Mano m2 = {Naipe(2),Naipe(2),Naipe(3)};
    Mano m1 = {Naipe(2),Naipe(3)};
    EXPECT_EQ(m1, m2);
    vector<string> str_lst = {"2H", "QC", "AS", "3H", "KD"};
    m1 = Mano(str_lst);
    Mano m3 = {Naipe("2H"), Naipe("QC"), Naipe("AS"), Naipe("3H"), Naipe("KD")};
    EXPECT_EQ(m1, m3);
};

TEST(ManoTests, Representations) {
    Mano m1 = {Naipe(2),Naipe(3)};
    EXPECT_EQ(m1.repr(), "3S 4S ");
};

TEST(ManoTests, JacksOrBetter) {
    int carta;
    Mano m1 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("3H"), Naipe("KD")};
    EXPECT_EQ(m1.is_jacks_or_better(carta), true);
    EXPECT_EQ(carta, 12);
    
    Mano m2 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("KD")};
    EXPECT_EQ(m2.is_jacks_or_better(carta), false);

    Mano m3 = {Naipe("2H"), Naipe("QC"), Naipe("AS"), Naipe("3H"), Naipe("KD")};
    EXPECT_EQ(m3.is_jacks_or_better(carta), false);

    m1 = Mano({"2H", "2C", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_jacks_or_better(carta), false);

    m1 = Mano({"2H", "2C", "AS", "AH", "KD"}); // two pair
    EXPECT_EQ(m1.is_jacks_or_better(carta), false);

    m1 = Mano({"2H", "2C", "AS", "AH", "2D"}); // full house
    EXPECT_EQ(m1.is_jacks_or_better(carta), false);

    m1 = Mano({"AH", "AC", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_jacks_or_better(carta), false);

    m1 = Mano({"2H", "2C", "2S", "KH", "KD"});
    EXPECT_EQ(m1.is_jacks_or_better(carta), false);

    m1 = Mano({"AH", "2C", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_jacks_or_better(carta), true);
    EXPECT_EQ(carta, 1);
};

TEST(ManoTests, ThreeOfAKind) {
    int carta;
    Mano m1 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("3H"), Naipe("QD")};
    EXPECT_EQ(m1.is_three_of_a_kind(carta), true);
    EXPECT_EQ(carta, 12);
    
    Mano m2 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("KD")};
    EXPECT_EQ(m2.is_three_of_a_kind(carta), false);

    Mano m3 = {Naipe("2H"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("AD")};
    EXPECT_EQ(m3.is_three_of_a_kind(carta), true);
    EXPECT_EQ(carta, 1);

    m1 = Mano({"2H", "2C", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_three_of_a_kind(carta), false);

    m1 = Mano({"2H", "2C", "2S", "2D", "KD"}); // two pair
    EXPECT_EQ(m1.is_three_of_a_kind(carta), false);

    m1 = Mano({"2H", "3C", "AS", "AH", "2D"}); // full house
    EXPECT_EQ(m1.is_three_of_a_kind(carta), false);

    m1 = Mano({"AH", "AC", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_three_of_a_kind(carta), true);
    EXPECT_EQ(carta, 1);

    m1 = Mano({"2H", "2C", "4S", "KH", "KD"});
    EXPECT_EQ(m1.is_three_of_a_kind(carta), false);

    m1 = Mano({"AH", "2C", "AS", "3H", "AD"});
    EXPECT_EQ(m1.is_three_of_a_kind(carta), true);
    EXPECT_EQ(carta, 1);
};

TEST(ManoTests, Poker) {
    int carta;
    Mano m1 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("QS"), Naipe("QD")};
    EXPECT_EQ(m1.is_poker(carta), true);
    EXPECT_EQ(carta, 12);
    
    Mano m2 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("KD")};
    EXPECT_EQ(m2.is_poker(carta), false);

    Mano m3 = {Naipe("AC"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("AD")};
    EXPECT_EQ(m3.is_poker(carta), true);
    EXPECT_EQ(carta, 1);

    m1 = Mano({"2H", "2C", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_poker(carta), false);

    m1 = Mano({"2H", "2C", "2S", "2D", "KD"}); // two pair
    EXPECT_EQ(m1.is_poker(carta), true);
    EXPECT_EQ(carta, 2);

    m1 = Mano({"2H", "3C", "AS", "AH", "2D"}); // full house
    EXPECT_EQ(m1.is_poker(carta), false);

    m1 = Mano({"AH", "AC", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_poker(carta), false);

    m1 = Mano({"2H", "2C", "4S", "KH", "KD"});
    EXPECT_EQ(m1.is_poker(carta), false);

    m1 = Mano({"AH", "2C", "AS", "3H", "AD"});
    EXPECT_EQ(m1.is_poker(carta), false);
};

TEST(ManoTests, TwoPair) {
    int c1, c2;
    Mano m1 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("QS"), Naipe("QD")};
    EXPECT_EQ(m1.is_two_pair(c1, c2), false);
    
    Mano m2 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("KD")};
    EXPECT_EQ(m2.is_two_pair(c1, c2), true);
    set<int> exp_set = {c1, c2};
    set<int> act_set = {1, 12};
    EXPECT_EQ(exp_set, act_set);


    Mano m3 = {Naipe("AC"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("AD")};
    EXPECT_EQ(m3.is_two_pair(c1, c2), false);

    m1 = Mano({"2H", "2C", "2S", "3H", "KD"});
    EXPECT_EQ(m1.is_two_pair(c1, c2), false);

    m1 = Mano({"2H", "2C", "2S", "2D", "KD"}); 
    EXPECT_EQ(m1.is_two_pair(c1, c2), false);

    m1 = Mano({"3H", "3C", "6S", "6H", "6D"}); // full house
    EXPECT_EQ(m1.is_two_pair(c1, c2), false);

    m1 = Mano({"AH", "AC", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_two_pair(c1, c2), false);

    m1 = Mano({"2H", "2C", "4S", "KH", "KD"});
    EXPECT_EQ(m1.is_two_pair(c1, c2), true);
    exp_set = {c1, c2};
    act_set = {2, 13};
    EXPECT_EQ(exp_set, act_set);

    m1 = Mano({"AH", "2C", "AS", "3H", "AD"});
    EXPECT_EQ(m1.is_two_pair(c1, c2), false);
};

TEST(ManoTests, FullHouse) {
    int c1, c2;
    Mano m1 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("QS"), Naipe("QD")};
    EXPECT_EQ(m1.is_full_house(c1, c2), false);
    
    Mano m2 = {Naipe("QH"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("AD")};
    EXPECT_EQ(m2.is_full_house(c1, c2), true);
    EXPECT_EQ(c1, 1);
    EXPECT_EQ(c2, 12);


    Mano m3 = {Naipe("AC"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("AD")};
    EXPECT_EQ(m3.is_full_house(c1, c2), false);

    m1 = Mano({"2H", "2C", "2S", "3H", "3D"});
    EXPECT_EQ(m1.is_full_house(c1, c2), true);
    EXPECT_EQ(c1, 2);
    EXPECT_EQ(c2, 3);

    m1 = Mano({"2H", "2C", "2S", "2D", "KD"}); 
    EXPECT_EQ(m1.is_full_house(c1, c2), false);

    m1 = Mano({"3H", "3C", "6S", "6H", "6D"}); // full house
    EXPECT_EQ(m1.is_full_house(c1, c2), true);
    EXPECT_EQ(c1, 6);
    EXPECT_EQ(c2, 3);

    m1 = Mano({"AH", "AC", "AS", "3H", "KD"});
    EXPECT_EQ(m1.is_full_house(c1, c2), false);

    m1 = Mano({"2H", "2C", "4S", "KH", "KD"});
    EXPECT_EQ(m1.is_full_house(c1, c2), false);

    m1 = Mano({"AH", "2C", "AS", "3H", "AD"});
    EXPECT_EQ(m1.is_full_house(c1, c2), false);
};

TEST(ManoTests, Flush) {
    Mano m1({"2H", "2C", "2S", "3H", "3D"});
    EXPECT_EQ(m1.is_flush(), false);

    m1 = Mano({"AH", "2H", "5H", "3H", "KH"});
    EXPECT_EQ(m1.is_flush(), true);

    m1 = Mano({"AD", "2D", "5D", "3D", "KD"});
    EXPECT_EQ(m1.is_flush(), true);
};

TEST(ManoTests, Straight) {
    Mano m1({"2H", "2C", "2S", "3H", "3D"});
    EXPECT_EQ(m1.is_straight(), false);

    m1 = Mano({"AH", "2H", "5H", "3H", "KH"});
    EXPECT_EQ(m1.is_straight(), false);

    m1 = Mano({"AD", "2D", "5D", "3D", "KD"});
    EXPECT_EQ(m1.is_straight(), false);

    m1 = Mano({"AD", "2H", "5D", "3D", "4D"});
    EXPECT_EQ(m1.is_straight(), true);

    m1 = Mano({"JD", "KD", "QD", "10D", "9D"});
    EXPECT_EQ(m1.is_straight(), true);

    m1 = Mano({"JD", "KD", "KH", "10D", "9D"});
    EXPECT_EQ(m1.is_straight(), false);

    m1 = Mano({"JD", "KD", "AH", "10D", "9D"});
    EXPECT_EQ(m1.is_straight(), false);

    m1 = Mano({"JC", "KD", "AH", "10D", "JD"});
    EXPECT_EQ(m1.is_straight(), false);

    m1 = Mano({"QD", "KD", "AH", "10D", "JD"});
    EXPECT_EQ(m1.is_straight(), true);
};

TEST(ManoTests, StraightFlush) {
    Mano m1({"2H", "2C", "2S", "3H", "3D"});
    EXPECT_EQ(m1.is_straight_flush(), false);

    m1 = Mano({"JD", "KD", "QD", "10D", "9D"});
    EXPECT_EQ(m1.is_straight_flush(), true);

    m1 = Mano({"AH", "2H", "5H", "3H", "KH"});
    EXPECT_EQ(m1.is_straight_flush(), false);

    m1 = Mano({"AH", "JH", "10H", "QH", "KH"});
    EXPECT_EQ(m1.is_straight_flush(), true);

    m1 = Mano({"AH", "2H", "5H", "3H", "4H"});
    EXPECT_EQ(m1.is_straight_flush(), true);
};

TEST(ManoTests, RoyalFlush) {
    Mano m1({"2H", "2C", "2S", "3H", "3D"});
    EXPECT_EQ(m1.is_royal_flush(), false);

    m1 = Mano({"JD", "KD", "QD", "10D", "9D"});
    EXPECT_EQ(m1.is_royal_flush(), false);

    m1 = Mano({"AH", "2H", "5H", "3H", "KH"});
    EXPECT_EQ(m1.is_royal_flush(), false);

    m1 = Mano({"AH", "JH", "10H", "QH", "KH"});
    EXPECT_EQ(m1.is_royal_flush(), true);

    m1 = Mano({"AH", "2H", "5H", "3H", "4H"});
    EXPECT_EQ(m1.is_royal_flush(), false);
};

TEST(ManoTests, Prize) {
    Mano m1({"AH", "JH", "10H", "QH", "KH"});
    EXPECT_EQ(m1.prize(), RoyalFlush);

    m1 = Mano({"9H", "JH", "10H", "QH", "KH"});
    EXPECT_EQ(m1.prize(), StraightFlush);

    m1 = Mano({"AD", "2H", "5D", "3D", "4D"});
    EXPECT_EQ(m1.prize(), Straight);

    m1 = Mano({"AH", "2H", "5H", "3H", "KH"});
    EXPECT_EQ(m1.prize(), Flush);

    m1 = Mano({"3H", "3C", "6S", "6H", "6D"}); // full house
    EXPECT_EQ(m1.prize(), FullHouse);

    m1 = Mano({"2H", "2C", "4S", "KH", "KD"});
    EXPECT_EQ(m1.prize(), TwoPair);

    Mano m3 = {Naipe("AC"), Naipe("QC"), Naipe("AS"), Naipe("AH"), Naipe("AD")};
    EXPECT_EQ(m3.prize(), Poker);

    m1 = Mano({"AH", "AC", "AS", "3H", "KD"});
    EXPECT_EQ(m1.prize(), ThreeOfAKind);

    m1 = Mano({"AH", "2C", "AS", "3H", "KD"});
    EXPECT_EQ(m1.prize(), JacksOrBetter);

    m1 = Mano({"AH", "2D", "6S", "3H", "KD"});
    EXPECT_EQ(m1.prize(), Loss);
};

TEST(BarajaTests, Print) {
    Baraja B;
    B.print(15);
    B.shuffle();
    B.print(15);
};

TEST(BarajaTests, Index) {
    Baraja B;
    EXPECT_EQ(B.index(Naipe("AS")), 0);
    EXPECT_EQ(B.index(Naipe("KD")), 51);

    Naipe N("JH");
    int Ind = B.index(N);
    EXPECT_EQ(B[Ind], N);

    N = Naipe("3C");
    Ind = B.index(N);
    EXPECT_EQ(B[Ind], N);
};

TEST(BarajaTests, Swap) {
    Baraja B;
    Naipe N1("AS");
    Naipe N2("2S");
    B.swap(N1, N2);
    EXPECT_EQ(B.index(N1), 1);
    EXPECT_EQ(B.index(N2), 0);

    N1 = Naipe("3H");
    N2 = Naipe("JD");
    int orig1 = B.index(N1);
    int orig2 = B.index(N2);
    B.swap(N1,N2);
    EXPECT_EQ(B.index(N2), orig1);
    B.swap(N1,N2);
    EXPECT_EQ(orig1, B.index(N1));
    EXPECT_EQ(orig2, B.index(N2));
    //Test trivial swap
    orig1 = B.index(N1);
    B.swap(N1, N1);
    orig2 = B.index(N1);
    EXPECT_EQ(orig1, orig2);
};

TEST(BarajaTests, Shuffle) {
   Baraja B;
   int c1 = 0, c2 = 0, c3 = 0;
   int N = 10000;
   Naipe N1("2H");
   //unsigned int seed = time(NULL);
   B.shuffle();
   for (int i=0; i<N; i++) {
       B.shuffle();
       //B.print(15);
       if (B[0] == N1)
       {c1++;};
       if (B[23] == N1)
       { c2++;};
       if (B[51] == N1)
       {c3++;};
   }
   double p1 = (double)c1/(double)N;
   double p2 = (double)c2/(double)N;
   double p3 = (double)c3/(double)N;
   double p0 = 1.0/52.0;
   cout<<c1<<": "<<zvalue(p1, p0, N)<<"   "<<c2<<": "<<zvalue(p2, p0, N)<<"   "<<c3<<": "<<zvalue(p3, p0, N)<<"   "<<endl;
   EXPECT_LT(zvalue(p1, p0, N), 3);
   EXPECT_LT(-3, zvalue(p1, p0, N));
   EXPECT_LT(zvalue(p2, p0, N), 3);
   EXPECT_LT(-3, zvalue(p2, p0, N));
   EXPECT_LT(zvalue(p3, p0, N), 3);
   EXPECT_LT(-3, zvalue(p3, p0, N));
};

TEST(BarajaTests, StartWith) {
    Baraja B;
    vector<Naipe> card_lst{ Naipe("3H"), Naipe("KS"), Naipe("10C") };
    B.start_with(card_lst);
    EXPECT_EQ(B[0], card_lst[0]);
    EXPECT_EQ(B[1], card_lst[1]);
    EXPECT_EQ(B[2], card_lst[2]);
};

TEST(BarajaTests, Play) {
    Baraja B;
//    B.print(10);
    Prize p = B.play(2);
    EXPECT_EQ(p, 6);
    B.start_with({Naipe("6C"), Naipe("6H"), Naipe("6D")});
//    B.print(10);
    EXPECT_EQ(B.play(16), Poker);
    B.start_with({Naipe("7C"), Naipe("7H"), Naipe("KH"), Naipe("KS"), Naipe("7D")});
//    B.print(10);
    EXPECT_EQ(B.play(18), Poker);
};

TEST(BarajaTests, Play2) {
    Baraja B((unsigned) 13);
    B.shuffle();
    //B.print(10);
    EXPECT_EQ(B.play(25), ThreeOfAKind);
};

TEST(BarajaTests, Play3) {
    Baraja B((unsigned) 25);
    B.shuffle();
    //B.print(10);
    EXPECT_EQ(B.play(0), JacksOrBetter);
};

TEST(BarajaTests, Play4) {
    Baraja B((unsigned) 221);
    B.shuffle();
    //B.print(10);
    EXPECT_EQ(B.play(2), JacksOrBetter);
};

TEST(BarajaTests, Play5) {
    Baraja B(29);
    B.shuffle();
    B.print(10);
    EXPECT_EQ(B.play(31), Straight);
};

TEST(BarajaTests, PlayRandom) {
    Baraja B;
    B.shuffle();
    int prize = 0;
    for (int i=0; i<100; i++) 
        prize += B.play(23, true);
    EXPECT_LT(0, prize);
};

TEST(BarajaTests, Evaluate) {
    Baraja B(81);
    B.shuffle();
    B.print(5);
    float ex = B.evaluate(30, 1000);
    cout<<"Eval is: "<<ex<<endl;
    EXPECT_LT(0, ex);
};

TEST(ActionsTests, Basic) {
    std::vector<int> v1 = {0,};
    std::vector<int> v2 = {2,4};
    EXPECT_EQ(actions[1], v1);
    EXPECT_EQ(actions[1][0], 0);
    EXPECT_EQ(actions[14], v2);
    EXPECT_EQ(actions[14][1], 4);
};
