// Equivalent of baraja.py implementation of classes Naipe, Baraja
// shuffle algorithm: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle


const int DECK_SIZE = 52;

class Naipe {
    private:
// Internally implemented as just an int value 
// 1 <= naipe <= 52 
        int naipe;
    public:
        Naipe(int N) { naipe = N; };
        int numero() { return naipe%13; };
        int palo() { return naipe/13; };
};


