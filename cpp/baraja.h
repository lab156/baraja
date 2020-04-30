#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <set>
#include <vector>
// Equivalent of baraja.py implementation of classes Naipe, Baraja
// shuffle algorithm: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
//
// For an example of the card deck implementation:
// checkout https://www.boost.org/doc/libs/1_72_0/libs/multi_index/example/rearrange.cpp
//
// At some point need to iterate through combinations check the following examples
// https://javol.wordpress.com/2011/06/09/next_combination-function/
// https://www.codeproject.com/Articles/4675/Combinations-in-C


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
        friend bool operator < (const Naipe &lhs, const Naipe &rhs) { return lhs.naipe < rhs.naipe; };

        int numero() const { return (naipe)%13 + 1; };
        int palo() const { return naipe/13; };

        std::string numero_char() const;
        std::string palo_char() const; 
        std::string repr() const { return (this->numero_char()).append(this->palo_char()); };
};

/* DEFINITION OF THE PRIZES 
 * Defined as an enum. Loss is defined to be zero
 */
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

/* #####################################
 *      DEFINITION OF MANO CLASS
 * ##################################### 
 * TODO: idea for better prizes classification
 * If two numbers are returned there is no way to track the prize
 * If a map where key is numero and value is frequency then we can track anything
 * |       | length | max freq |
   |-------|--------|----------|
   | JoB   | 4      | 1        |
   | 3oaK  | 3      | 3        | 
   | 2pair | 3      | 2        | 
   | FH    | 2      | 3        | 
   | Poker | 2      | 4        | */

class Mano: public std::set<Naipe> {
    public:
        using std::set<Naipe>::set;
        Mano(std::vector<std::string> lst);  
        std::string repr();
        void print();

        bool is_jacks_or_better(int &carta); 
        bool is_three_of_a_kind(int &carta); //conflates with full house
        bool is_poker(int &carta);
        bool is_two_pair(int &carta1, int &carta2);
        bool is_full_house(int &carta1, int &carta2); //first card is the triple
};

Mano::Mano(std::vector<std::string> lst) {
    for (const std::string &s : lst){
        this->insert(Naipe(s));
    }
};

std::string Mano::repr() {
        std::string result = "";
        auto it = this->cbegin();
        while ( it != this->cend() ) {
            result.append(it->repr()).append(" ");
            it++;
        } 
        return result;
    };

void Mano::print() { 
            for (auto it = this->cbegin(); it != this->cend(); ++it) 
                std::cout<<it->repr()<<" ";
            std::cout<<std::endl;
        };

bool Mano::is_jacks_or_better(int &carta) {
    assert(this->size() == 5);
    std::vector<int> el_par ;
    for (const Naipe &n : *this) {
        int cuantas_iguales = 0;
        for (const Naipe &m : *this) 
           if ( n.numero() == m.numero() ) { cuantas_iguales++; };
        if ( cuantas_iguales == 2 )  
            el_par.push_back(n.numero());
        else if (cuantas_iguales > 2 ) 
            return false;
            };
    if (el_par.size() == 2) {
        carta = el_par[0];
        if ( carta >= 11 || carta == 1 ) 
            return true ;
        else 
            return false;
    }
    else
        return false;
};

bool Mano::is_three_of_a_kind(int &carta) {
    assert(this->size() == 5);
    for (const Naipe &n : *this) {
        int cuantas_iguales = 0;
        for (const Naipe &m : *this) 
           if ( n.numero() == m.numero() ) { cuantas_iguales++; };
        if ( cuantas_iguales == 3 ) {
            carta = n.numero();
            return true;
        }
            };
    return false;
};

bool Mano::is_full_house(int &carta1, int &carta2) {
    assert(this->size() == 5);
    if (this->is_three_of_a_kind(carta1)) {
    for (const Naipe &n : *this) {
        int cuantas_iguales = 0;
        for (const Naipe &m : *this) 
           if ( n.numero() == m.numero() ) { cuantas_iguales++; };
        if ( cuantas_iguales == 2 ) {
            carta2 = n.numero();
            return true;
         } }
    } 
    return false;
};

bool Mano::is_poker(int &carta) {
    assert(this->size() == 5);
    for (const Naipe &n : *this) {
        int cuantas_iguales = 0;
        for (const Naipe &m : *this) 
           if ( n.numero() == m.numero() ) { cuantas_iguales++; };
        if ( cuantas_iguales == 4 ) {
            carta = n.numero();
            return true;
        }
            };
    return false;
};

bool Mano::is_two_pair(int &carta1, int &carta2) {
    assert(this->size() == 5);
    std::set<int> pares_set;
    for (const Naipe &n : *this) {
        int cuantas_iguales = 0;
        for (const Naipe &m : *this) 
           if ( n.numero() == m.numero() ) { cuantas_iguales++; };
        if ( cuantas_iguales == 2 ) {
            pares_set.insert(n.numero());
            };
    }
    if ( pares_set.size() == 2 ) {
        carta1 = *(pares_set.begin());
        carta2 = *(std::next(pares_set.begin()));
        return true;
    }
    else
        return false;
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

std::string Naipe::numero_char() const{
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

std::string Naipe::palo_char() const {
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

