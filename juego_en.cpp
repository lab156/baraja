#include <iostream>
#include "baraja.h"
// 
// compiles with: g++ juego_en.cpp

using namespace std;
int main() {
    for (int i =0; i<52; i++) {
    Naipe nn(i);
    cout<<"Hola, el naipe es: ("<<nn.numero()<<","<<nn.palo()<<")"<<" y numero es: "
        <<nn.numero_char()<<nn.palo_char()<<nn.repr()<<endl;
    }
}

