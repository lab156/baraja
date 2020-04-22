#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <set>
// Equivalent of baraja.py implementation of classes Naipe, Baraja
// shuffle algorithm: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
// compiles with: g++ juego_en.cpp
// checkout https://www.boost.org/doc/libs/1_72_0/libs/multi_index/example/rearrange.cpp


const int DECK_SIZE = 52;

int get_palo_from_str(char c);

class Naipe {
    private:
// Internally implemented as just an int value 
// 0 <= naipe <= 51 
// 0 = AS, 13 = AH, 26 = AC, 39 = AD, 51 = KD 
        unsigned short naipe;
    public:
        Naipe(int N) { assert(N < DECK_SIZE); naipe = N; }; 
        Naipe(int numero, int palo) : Naipe( numero - 1 + palo*13 ) {};
        Naipe(std::string str);

        friend bool operator == (const Naipe &lhs, const Naipe &rhs) { return lhs.naipe == rhs.naipe; };
        friend bool operator != (const Naipe &lhs, const Naipe &rhs) { return !(lhs == rhs); };

        int numero() { return (naipe)%13 + 1; };
        int palo() { return naipe/13; };

        std::string numero_char();
        std::string palo_char(); 
        std::string repr() { return (this->numero_char()).append(this->palo_char()); };
};

enum Prize {
    Loss = 0,
    JacksOrBetter = 1,
    TwoPair = 2,
    ThreeOfAKind = 3,
    Straight = 4,
    Flush = 6,
    FullHouse = 9,
    Poker = 25,
    StraightFlush = 50,
    RoyalFlush = 250
};

class Mano: public std::set<Naipe> {
    public:
        std::string repr() { return "hola"; };
};

Naipe::Naipe(std::string str) {
    int numero, palo;
    switch (str[0]) {
        case '1': // has to be folowed by 0
            assert(str[1] == '0');
            numero = 10;
            palo = get_palo_from_str(str[2]);
            break;
        case 'A':
            numero = 1;
            palo = get_palo_from_str(str[1]);
            break;
        case 'J':
            numero = 11;
            palo = get_palo_from_str(str[1]);
            break;
        case 'Q':
            numero = 12;
            palo = get_palo_from_str(str[1]);
            break;
        case 'K':
            numero = 13;
            palo = get_palo_from_str(str[1]);
            break;
        default:
            assert(isdigit(str[0]));
            numero = str[0] - 48; // ASCII trick
            palo = get_palo_from_str(str[1]);
            break;
    };
    *this = Naipe(numero, palo);
};

int get_palo_from_str(char c) {
    switch (c) {
        case 'S':
            return 0;
        case 'H':
            return 1;
        case 'C':
            return 2;
        case 'D':
            return 3;
        default:
            throw 20; // This case should raise an error
    }};

std::string Naipe::numero_char() {
    switch (int val = this->numero()) {
        case 1:
            return "A";
        case 11:
            return "J";
        case 12:
            return "Q";
        case 13:
            return "K";
        default: 
            return std::to_string(val);
    }};

std::string Naipe::palo_char() {
    switch (int val = this->palo()) {
        case 0:
            return "S"; // Spades
        case 1:
            return "H"; // Hearts
        case 2:
            return "C"; // Clovers
        case 3:
            return "D"; // Diamonds
        default: 
            return std::to_string(val); // this should not be reached TODO: raise error
    }};

