#include <iostream>
#include "baraja.h"
// 
// compiles with: g++ juego_en.cpp

using namespace std;
int main() {
    Naipe nn(6);
    cout<<"Hola, el naipe es: ("<<nn.numero()<<","<<nn.palo()<<")"<<endl;
}

