#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <set>
#include <array>
#include <vector>
#include <time.h>
#include <iomanip>
#include <algorithm>
//#include <cstdlib>
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
// Definition of the Hold actions
std::vector<int> actions[32] = {{},
{0,},
{1,},
{2,},
{3,},
{4,},
{0, 1},
{0, 2},
{0, 3},
{0, 4},
{1, 2},
{1, 3},
{1, 4},
{2, 3},
{2, 4},
{3, 4},
{0, 1, 2},
{0, 1, 3},
{0, 1, 4},
{0, 2, 3},
{0, 2, 4},
{0, 3, 4},
{1, 2, 3},
{1, 2, 4},
{1, 3, 4},
{2, 3, 4},
{0, 1, 2, 3},
{0, 1, 2, 4},
{0, 1, 3, 4},
{0, 2, 3, 4},
{1, 2, 3, 4},
{0, 1, 2, 3, 4}};

class Naipe {
    private:
// Internally implemented as just an int value 
// 0 <= naipe <= 51 
// 0 = AS, 13 = AH, 26 = AC, 39 = AD, 51 = KD 
        unsigned short naipe;
    public:
        Naipe() { };
        ~Naipe() { };
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
        bool is_flush(); //conflates with royal flush and straight flush
        bool is_straight(); //conflates with straight-flush and royal-flush. 
        bool is_straight_flush() { return this->is_flush() && this->is_straight(); };//conflates with royal flush
        bool is_royal_flush() ;
        Prize prize();
};

class Baraja: public std::array<Naipe, 52> {
    public:
      Baraja(unsigned int seed_int);
      Baraja() : Baraja(time(NULL)) {};
      void print(int cuantas);
      int index(Naipe); //should always find the naipe
      void swap(Naipe, Naipe);
      void shuffle();
      void start_with(std::vector<Naipe> card_lst);
      Prize play(int action);
};

Baraja::Baraja(unsigned int seed_int) {
    srand(seed_int);

    for (int i=0; i<52; i++) {
        (*this)[i] = Naipe(i);
    }
};

int Baraja::index(Naipe N) {
    int senti = 0;
    for (auto c : *this) {
        if (c == N) break;
        senti++;
    };
    return senti;
};

Prize Baraja::play(int hold_action) {
    Naipe temp[5];
    std::vector<int> hold = actions[hold_action];
// Action is a vector with the indexes that are held
// Replace with the cards that are right after the hand
// Another option is to sample from the rest of the deck
    int i = 0;
    for (int j = 0 ; j < 5; j++) {
        if ( std::binary_search(hold.begin(), hold.end(), j)) {
            temp[j] = (*this)[j];
        }
        else {
            temp[j] = (*this)[i+5];
            i++;
        }
    }

    Mano M(temp, temp + 5);
    return M.prize();
};

void Baraja::start_with(std::vector<Naipe> card_lst) {
    int i = 0;
    for (auto c : card_lst) {
        this->swap((*this)[i], c);
        i++;
    }
};

void Baraja::shuffle(){
    int j;
    for (int i=(DECK_SIZE-1); i > 0; i--) {
        j = rand()%(i+1);
        this->swap((*this)[i], (*this)[j]);
    }
};

void Baraja::swap(Naipe n1, Naipe n2) {
    int i1 = this->index(n1);
    int i2 = this->index(n2);
    (*this)[i1] = n2;
    (*this)[i2] = n1;
};

void Baraja::print(int cuantas) {
    int senti = 1;
    for (auto c : *this) {
        std::cout<<std::setw(3)<<c.repr()<<" ";
        senti++;
        if (senti > cuantas) break;
    };
    std::cout<<std::endl;
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

Prize Mano::prize() {
        //Guaranteed to return a prize or loss
        Prize premio;
        int carta1, carta2;
        if (this->is_jacks_or_better(carta1))
            premio = JacksOrBetter;
        else if (this->is_full_house(carta1, carta2))
            //FULL HOUSE TIENE QUE IR ANTES DE 3 of a kind
            //PORQUE LOS DOS SE DISPARAN AL MISMO TIEMPO.
            premio = FullHouse;
        else if (this->is_three_of_a_kind(carta1))
            //DEBERIA DE DECIR DE QUE ES EL PAR
            premio = ThreeOfAKind;
        else if (this->is_poker(carta1))
            //DEBERIA DE DECIR DE QUE ES EL POQUER
            premio = Poker;
        else if (this->is_two_pair(carta1, carta2))
            premio = TwoPair;
        else if (this->is_royal_flush())
            premio = RoyalFlush;
        else if (this->is_straight_flush())
            premio = StraightFlush;
        else if (this->is_straight())
            premio = Straight;
        else if (this->is_flush())
            premio = Flush;
        else
            premio = Loss;

        return premio;
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

bool Mano::is_flush() {
    assert(this->size() == 5);
    int first_palo = (*this->begin()).palo();
    unsigned int same_palo_cnt = 0;
    for (const Naipe &n : *this) {
        if (n.palo() == first_palo) { same_palo_cnt++; };
    };
    return (same_palo_cnt == this->size()) ? true : false;
};

bool Mano::is_straight() {
    assert(this->size() == 5);
    std::set<int> num_set;
    for (const Naipe &n : *this) {
        num_set.insert(n.numero());
    };
    int low = *num_set.begin();
    int high = *num_set.rbegin();
    if (num_set.size() == 5) {
    if ((high - low) == 4 )
        return true;
    else { //check if it is a high straight
        if ((low == 1) & (high == 13) & (*next(num_set.begin()) == 10))
            return true;
    }
    } 
    return false;
};

// Straigh Flush is defined in the class definition

bool Mano::is_royal_flush() {
    int contains_A = 0;
    int contains_10 = 0;
    for (const Naipe &n : *this) {
        if (n.numero() == 1) contains_A++;
        if (n.numero() == 10) contains_10++;
    };

    return is_straight_flush() && contains_A > 0 && contains_10 > 0;
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

