import random as r

class Naipe():
    def __init__(self, naipe):
        if isinstance(naipe,Naipe):
            self.numero = naipe.numero
            self.palo = naipe.palo
        else:
            try:
                self.numero = naipe[0]
                self.palo = naipe[1]
            except:
                raise ValueError('Naipe recibio una cosa rara')
            assert(self.numero <= 13 and self.numero >= 1 and \
                        self.palo <= 3 and self.palo >= 0), "Values out of valid ranges for Naipe"
        
    def __str__(self):
        '''Prints the representation of the card 
        taken from http://en.wikipedia.org/wiki/Playing_cards_in_Unicode
        '''
        return '(%s,%s)'%(self.repr_numero(), self.repr_palo_unicode())
    
    def __repr__(self):
        return self.repr_naipe()

    def __eq__(self,other):
        return (self.numero == other.numero) and (self.palo == other.palo) 

    @classmethod
    def from_int(cls, i):
        assert((i <= 51 and i >= 0)), "int = %s is not between 0 and 51"%i
        return cls([i%13 + 1, int(i/13)])

    def get_as_int(self):
        '''
        returns self as an integer from 0 to 51
        '''
        return self.numero + 13*self.palo - 1
        
    def get_as_tuple(self):
       '''
       Returns a tuple with the information of the Naipe
       consider this an inverse of the operation Naipe((2,3))
       '''
       return self.numero,self.palo
    def repr_numero(self):
        '''cambia el numero de un naipe'''
        if self.numero == 1:
            return 'A'
        elif self.numero == 11:
            return 'J'
        elif self.numero == 12:
            return 'Q'
        elif self.numero == 13:
            return 'K'
        else:
            return str(self.numero)

    def is_spades(self):
        return self.palo == 0
    def is_hearts(self):
        return self.palo == 1
    def is_clovers(self):
        return self.palo == 2
    def is_diamonds(self):
        return self.palo == 3

    def repr_image_name(self):
        '''
        This function returns the name of the image which has this format
        10C.png  2H.png  4C.png  5H.png  7C.png  8H.png  AC.png    JD.png  KS.png
10D.png  2S.png  4D.png  5S.png  7D.png  8S.png  AD.png    JH.png  QC.png
10H.png  3C.png  4H.png  6C.png  7H.png  9C.png  AH.png    JS.png  QD.png
10S.png  3D.png  4S.png  6D.png  7S.png  9D.png  AS.png    KC.png  QH.png
2C.png   3H.png  5C.png  6H.png  8C.png  9H.png  back.png  KD.png  QS.png
2D.png   3S.png  5D.png  6S.png  8D.png  9S.png  JC.png    KH.png
        '''
        if self.is_spades():
            palo = 'S'
        if self.is_hearts():
            palo = 'H'
        if self.is_clovers():
            palo = 'C'
        if self.is_diamonds():
            palo = 'D'
        return self.repr_numero() + palo

    def get_successor(self):
        '''
        Retorna el sucesor de una carta si es A retorna 2
        si es K retorna A.
        '''
        if self.numero == 13:
            return Naipe((1,self.palo))
        else:
            return Naipe((self.numero + 1, self.palo))
        
    def repr_palo_unicode(self):
        ''' usamos los codigos que aparecen en 
        http://en.wikipedia.org/wiki/Playing_cards_in_Unicode
        de manera rapida podemos decir:
        Espadas: 0
        Corazones: 1
        Treboles: 2
        Diamantes: 3
        '''
        if self.is_spades():
            return '\u2660'
        elif self.is_hearts():
            return '\u2665'
        elif self.is_clovers():
            return '\u2663'
        elif self.is_diamonds():
            return '\u2666'
        else:
            raise ValueError()
        
    def _repr_naipe(self):
        '''
        _repr_naipe(self) -> int

        Retorna el codigo del simbolo unicode de un Naipe
        '''
        rep = 0x1f0A0
        if self.numero <= 11:
            rep += self.numero
        else:
            rep += self.numero + 1
        if self.is_spades():
            rep += 0x0
        elif self.is_hearts():
            rep += 0x10
        elif self.is_clovers():
            rep += 0x30
        elif self.is_diamonds():
            rep += 0x20
        else:
            raise ValueError('valor no identificado en repr_naipe')
        return rep

    def repr_naipe(self):
        return chr(self._repr_naipe())
    def repr_html(self):
        return hex(self._repr_naipe())[1:] 


def check_has_5_cards(f):
    def wrapper(self):
        assert(len(self) == 5), "Poker hands should have 5 cards"
        return f(self)
    return wrapper

class Mano(frozenset):
    '''Una mano de naipes, deberia de incluir las rutinas que tipicamente
    no involucran toda la baraja y en donde el orden no es muy importante.

    La implementacion es una lista de tuples por cuestiones para que el contenido sea hashable.

    USO: mano deberia de ser un iterable de Naipes.

    Ejemplo Prototipico: Una mano de Poquer de cinco cartas. 
    * No importa el orden en que estan las cartas.
    * Tiene rutina eficiente para revisar cosas como si hay tres iguales, si es un flush o un royal flush.
    '''
#    def __new__(self, mano):
#        if hasattr(mano, '__iter__'):
#            return frozenset.__new__(self, [Naipe(naipe) for naipe in mano])
#        else:
#            raise ValueError('Mano was fed a non iterable object: %s'%mano)

    def __str__(self):
        return ','.join([Naipe(naipe).__str__() for naipe in self])

    def __repr__(self):
        return ','.join([Naipe(naipe).__str__() for naipe in self])

    @classmethod
    def new(self, lst):
        '''
        A smart contructor of Mano
        is able to determine anything that Naipe understands
        '''
        if all(isinstance(n, int) for n in lst):
            mano = Mano([p.get_as_tuple() for p in map(Naipe.from_int, lst )])
        elif all(isinstance(n, tuple) for n in lst):
            mano = Mano(lst)
        elif all(isinstance(n, Naipe) for n in lst):
            mano = Mano([p.get_as_tuple() for p in lst])
        else:
            raise(ValueError('Los valores de la lista estan mal'))
        return mano


    @check_has_5_cards
    def is_jacks_or_better(self):
        '''
        Retorna si hay un par de cartas mayores que J
        '''
        el_par = []
        for i in self:
            cuantas_iguales = sum([1 for j in self if i[0] == j[0]])
            if cuantas_iguales == 2:
                el_par.append(i)
            elif cuantas_iguales > 2:
                return False
        if len(el_par) == 2:
            valor = el_par[0][0]
            salida = True if (valor >= 11 or valor == 1)  else False
            return salida
        else:
            return False

    @check_has_5_cards
    def is_three_of_a_kind(self):
        for i in self:
            cuantas_iguales = sum([1 for j in self if i[0] == j[0]])
            if cuantas_iguales == 3:
                return True
        else:
            return False
    
    def is_poker(self):
        for i in self:
            cuantas_iguales = sum([1 for j in self if i[0] == j[0]])
            if cuantas_iguales == 4:
                return True
        else:
            return False

    def is_two_pair(self):
        '''Verdadero si la mano tiene dos pares sin contar los pocares; en donde
        hay dos pares pero de la misma carta'''
        num_pares = 0
        for i in self:
            cuantas_iguales = sum([1 for j in self if i[0] == j[0]])
            if cuantas_iguales == 2:
                num_pares += 1
        salida = True if num_pares == 4 else False
        return salida

    @check_has_5_cards
    def is_full_house(self):
        '''Full House es three of a kind y un par'''
        if self.is_three_of_a_kind():
            for i in self:
                cuantas_iguales = sum([1 for j in self if i[0] == j[0]])
                if cuantas_iguales == 2:
                    return True
            else:
                return False
        else:
            return False

    @check_has_5_cards
    def is_flush(self):
        palo = next(iter(self))[1]
        return len(self) == sum([1 for j in self if j[1] == palo])

    @check_has_5_cards
    def is_straight_flush(self):
        return self.is_straight() and self.is_flush()

    @check_has_5_cards
    def is_royal_flush(self):
        return self.dentro(10) and self.dentro(1) and\
                self.is_straight_flush()

    def dentro(self, item):
        if isinstance(item,tuple):
            return item in self
        elif isinstance(item,Naipe):
            return (item.numero,item.palo) in self
        elif isinstance(item,int):
            return ((item,0) in self or (item,1) in self or (item,2) in self or (item,3) in self)
        else:
            raise TypeError('Dentro fue llamado  con un argumento que no entiendo')

#    def _sort(self):
#        lst = sorted(list(self), key = lambda x: x[0])
#        if lst[0][0] == 1 and lst[-1][0] == 13:
#            K = lst.pop()
#            lst.insert(0,K)
#        return lst 

    @check_has_5_cards
    def is_straight(self):
        '''
        Retorna verdadero si la mano es un straight 
        ''' 
        suce = min(self, key = lambda x: x[0])
        if suce[0] == 1 and self.dentro(10):
            #EN EL CASO QUE LA MANO CONTENGA A y J HAY QUE REVISAR EL OTRO EXTREMO
            #INTERCAMBIAMOS EL AS CON LA SOTA
            suce = list(filter(lambda x: x[0] == 10, self))[0]
        assert(len(self) < 13)
        cnt = 0
        suce = Naipe(suce)
        while self.dentro(suce.numero):
            suce = suce.get_successor()
            cnt += 1
        return cnt == len(self)
    
    def hay_premio(self):
        premio = None
        if self.is_jacks_or_better():
            premio = 'Jacks or Better' 
        elif self.is_full_house():
            #FULL HOUSE TIENE QUE IR ANTES DE 3OAK
            #PORQUE LOS DOS SE DISPARAN AL MISMO TIEMPO.
            premio = 'Full House'
        elif self.is_three_of_a_kind():
            #DEBERIA DE DECIR DE QUE ES EL PAR
            premio = 'Three of a Kind' 
        elif self.is_poker():
            #DEBERIA DE DECIR DE QUE ES EL POQUER
            premio = 'Poker' 
        elif self.is_two_pair():
            premio = 'Two Pair'
        elif self.is_royal_flush():
            premio = 'Royal Flush'
        elif self.is_straight_flush():
            premio = 'Straight Flush'
        elif self.is_straight():
            premio = 'Straight'
        elif self.is_flush():
            premio = 'Flush'

        return premio
        


class Baraja():
    '''
    Implementacion de una baraja de naipe la 
    intencion es que haga todos los trabajos y 
    algoritmos que usan toda una baraja Nota: Baraja 
    no convierte los elementos a naipes esto podria 
    ser un problema
    '''
    def __init__(self):
        self._cartas_ = [(nmro,palo) for nmro in range(1,14) for palo in range(4)]
        self._cnt_ = 0
        
    def __str__(self):
        return ','.join([Naipe(naipe).__str__()\
                for naipe in self._cartas_])

        
    def revolver(self):
        '''
        Revuelve la baraja completa y vuelve a cero el contador
        '''
        r.shuffle(self._cartas_)
        self._cnt_ = 0
        return self
    
    def chequear_3_iguales(self, n=5):
        '''Chequea si hay tres cartas de una misma numero en 
        una mano de n (5 es default) naipes'''
        mano = self._cartas_[0:n]
        for i in range(1,14):
            cuantas_ies = sum([1 for j in mano if j[0] == i])
            if cuantas_ies == 3:
                return True
        else:
            return False
        
    def sacar_mano(self,n):
        '''Retorna una mano (tuple) de tamano n del 
        principio de la baraja'''
        N = self._cnt_
        self._cnt_ += n
        return Mano(self._cartas_[N:(n + N)])

    def sacar_lista_naipes(self,n, start_at_0=False):
        '''Retorna una lista de Naipes. A diferencia de 
        sacar_mano que retorna un elemento tipo mano 
        (con elementos tuples), esta funcion retorna una
        lista con elementos tipo Naipe'''
        if start_at_0:
            N = 0
        else:
            N = self._cnt_
            self._cnt_ += n
        return [Naipe(carta) for carta in self._cartas_[N:(n + N)]]

    def preview(self, num=10):
        """
        For debugging purposes: returns a console friendly list of the firs num naipes in the baraja without updating _cnt_
        """
        return [n.repr_image_name() for n in self._cartas_[:num]]
