from baraja import Naipe, Baraja, Mano
import unittest as ut
from math import sqrt
from scipy.stats import chi2

class BasicResults(ut.TestCase):
    def setUp(self):
        self.baraja = Baraja()

    def test_manos_in_sets(self):
        '''Revisa si puedo meter Manos en sets...
        Algo con lo que pase bastante apuro fue 
        haciendo que se pudiera hacer conjuntos con
        elementos de la clase Mano'''
        conjunto = set()
        conjunto.add(self.baraja.revolver().sacar_mano(5))
        conjunto.add(self.baraja.sacar_mano(4))
        conjunto.add(self.baraja.sacar_mano(14))
        self.assertEqual(len(conjunto), 3)

    def test_naipe_has_inverse(self):
        '''
        Revisa que la operacion de Naipe se pueda invertir
        '''
        prueba = Naipe((2,3)).get_as_tuple() == (2,3)
        self.assertTrue(prueba)

    def test_comparison_of_mano_sets(self):
        '''Revisa si puedo comparar manos y si puedo sacar una mano completa'''
        res = self.baraja.sacar_mano(24) <= self.baraja.revolver().sacar_mano(52)
        self.assertTrue(res)

    def test_naipe_init_cases(self):
        '''Revisa si el init de la clase Naipe funciona como fue disenado
        Naipe tiene que funcionar tanto con un par ordenado como con otro 
        objeto Naipe'''
        nn = Naipe((5,2))
        nnnn = Naipe(nn)
        self.assertTrue(nn.numero == nnnn.numero and nn.palo == nnnn.palo)

    def test_premios(self):
        '''Revisa si la fucion hay_premio asigna los premios correctamente'''
        man = Mano([(12,3),(3,1),(2,1),(12,1),(5,1)])
        self.assertIn('jacks', man.hay_premio().lower())

        man = Mano([(12,3),(3,1),(2,1),(12,1),(2,3)])
        self.assertIn('two pair', man.hay_premio().lower())

        man = Mano([(1,3),(3,3),(2,3),(12,3),(5,3)])
        self.assertEqual('flush', man.hay_premio().lower())
        
        man = Mano([(1,3),(13,1),(11,2),(12,3),(2,3)])
        self.assertEqual(None, man.hay_premio())
        
        man = Mano([(13,3),(13,1),(13,2),(9,3),(9,2)])
        self.assertIn('full house', man.hay_premio().lower())

        man = Mano([(1,3),(13,3),(11,3),(12,3),(10,3)])
        self.assertIn('royal', man.hay_premio().lower())

        man = Mano([(6,3),(8,3),(7,3),(5,3),(4,3)])
        self.assertIn('straight', man.hay_premio().lower())
        self.assertIn('flush', man.hay_premio().lower())

        
    def test_premios2(self):
        with self.assertRaises(AssertionError):
            man = Mano([(12,3),])
            man.is_jacks_or_better()

            man = Mano([(1,1),(6,3),(8,3),(7,3),(5,3),(4,3)])
            man.is_poker()

    def test_int_naipe_conversion(self):
        n = Naipe((2,2))
        self.assertTrue(Naipe.from_int( n.get_as_int() ) == n)
        self.assertTrue(Naipe.from_int(43).get_as_int() == 43)
        self.assertTrue(n.get_as_tuple() == (2,2))
        self.assertTrue(Naipe.from_int( n.get_as_int() ).get_as_tuple() == (2,2))
        n = Naipe((1,2))
        self.assertTrue(n.get_as_tuple() == (1,2))
        with self.assertRaises(AssertionError):
            n = Naipe((0,2))
        with self.assertRaises(AssertionError):
            Naipe((14,1))
        with self.assertRaises(AssertionError):
            Naipe((2,-1))
        with self.assertRaises(AssertionError):
            Naipe((2,4))
            


    def test_repr_naipes(self):
        #Revisa si la representasion esta funcionando para la guaya
        self.assertTrue(Naipe((1,0)).repr_naipe() == "\N{PLAYING CARD ACE OF SPADES}")
        self.assertTrue(Naipe((5,3)).repr_naipe() == "\N{PLAYING CARD FIVE OF DIAMONDS}")
        self.assertTrue(Naipe((11,2)).repr_naipe() == "\N{PLAYING CARD JACK OF CLUBS}")
        self.assertTrue(Naipe((13,1)).repr_naipe() == "\N{PLAYING CARD KING OF HEARTS}")

    def test_repr_naipes2(self):
        #Revisa si la representasion esta funcionando para la guaya
        self.assertTrue(Naipe.from_int(0).repr_naipe() == "\N{PLAYING CARD ACE OF SPADES}")
        self.assertTrue(Naipe.from_int(51).repr_naipe() == "\N{PLAYING CARD KING OF DIAMONDS}")
        self.assertTrue(Naipe.from_int(30).repr_naipe() == "\N{PLAYING CARD FIVE OF CLUBS}")
        self.assertTrue(Naipe.from_int(20).repr_naipe() == "\N{PLAYING CARD EIGHT OF HEARTS}")

    def test_random_1(self):
        '''
        Probamos la distribucion uniforme de la funcion revolver.
        '''
        n = 10e3
        p = 1/52.0
        # z_{\alpha/2} donde \alpha = 95%
        z = 1.95996398454
        stdev = sqrt(p*(1 - p)/n)

        B = Baraja()
        X = 0.0
        for i in range(int(n)):
            B.revolver()
            if Naipe((1,0)) == B.sacar_lista_naipes(1)[0]:
                X += 1.0
        print('lo que se esta probando %s < %s < %s'%(p - z*stdev, X/n, p + z*stdev))
        self.assertTrue(p - z*stdev < X/n < p + z*stdev)

    def test_randomness_2(self):
        '''
        Usando la aproximacion de la distribucion binomial a la Poisson
        y usando como exito el evento Full House, creamos un intervalo 
        de confianza alfa y probamos las probabilidades de la 
        funcion revolver.
        Es claro que esta prueba va a fallar de vez en cuando.
        '''
        n = 10e3
        lam = n*0.00144057623049
        K = 0
        B = Baraja()
        xi2 = lambda a,k: chi2.ppf(a,k)
        alfa = 0.05
        for i in range(int(n)):
            B.revolver()
            if B.sacar_mano(5).is_full_house():
                K += 1
        lim_inf = 0.5*xi2(alfa/2, 2*K)
        lim_sup = 0.5*xi2(1 - alfa/2, 2*K + 2)
        print("Probando haber si: %s < %s < %s"%(lim_inf,lam,lim_sup))
        self.assertTrue(lim_inf < lam < lim_sup)
