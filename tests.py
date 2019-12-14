from baraja import Naipe, Baraja, Mano, Prize
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
        self.assertEqual(Prize.JacksOrBetter, man.prize())

        man = Mano([(12,3),(3,1),(2,1),(12,1),(2,3)])
        self.assertIn('two pair', man.hay_premio().lower())
        self.assertEqual(Prize.TwoPair, man.prize())

        man = Mano([(1,3),(3,3),(2,3),(12,3),(5,3)])
        self.assertEqual('flush', man.hay_premio().lower())
        self.assertEqual(Prize.Flush, man.prize())

        man = Mano([(1,3),(13,1),(11,2),(12,3),(2,3)])
        self.assertEqual(None, man.hay_premio())
        self.assertEqual(Prize.Loss, man.prize())

        man = Mano([(13,3),(13,1),(13,2),(9,3),(9,2)])
        self.assertIn('full house', man.hay_premio().lower())
        self.assertEqual(Prize.FullHouse, man.prize())

        man = Mano([(1,3),(13,3),(11,3),(12,3),(10,3)])
        self.assertIn('royal', man.hay_premio().lower())
        self.assertEqual(Prize.RoyalFlush, man.prize())

        man = Mano([(6,3),(8,3),(7,3),(5,3),(4,3)])
        self.assertIn('straight', man.hay_premio().lower())
        self.assertIn('flush', man.hay_premio().lower())
        self.assertEqual(Prize.StraightFlush, man.prize())


    def test_premios2(self):
        with self.assertRaises(AssertionError):
            man = Mano([(12,3),])
            man.is_jacks_or_better()

            man = Mano([(1,1),(6,3),(8,3),(7,3),(5,3),(4,3)])
            man.is_poker()

    def test_premios3(self):
        '''Revisa si la fucion hay_premio asigna los premios correctamente'''
        man = Mano(['KC', 'AC', 'QS', '3C', '6C'])
        self.assertEqual(None, man.hay_premio())
        self.assertEqual(Prize.Loss, man.prize())

        man = Mano(['AC', 'AS', 'QC', '3C', '3H'])
        self.assertEqual('two pair', man.hay_premio().lower())
        self.assertEqual(Prize.TwoPair, man.prize())

        man = Mano(['2S', '2D', '2H', '2C', '3D'])
        self.assertEqual('poker', man.hay_premio().lower())
        self.assertEqual(Prize.Poker, man.prize())

        man = Mano(['2S', '3S', '4S', '5C', '6D'])
        self.assertEqual('straight', man.hay_premio().lower())
        self.assertEqual(Prize.Straight, man.prize())

        man = Mano(['2C', '3C', '4C', '5C', '6C'])
        self.assertEqual('straight flush', man.hay_premio().lower())
        self.assertEqual(Prize.StraightFlush, man.prize())

        man = Mano(['KC', 'AC', 'QC', '3C', '6C'])
        self.assertEqual('flush', man.hay_premio().lower())
        self.assertEqual(Prize.Flush, man.prize())

        man = Mano(['KC', 'AC', 'QC', 'JC', '10C'])
        self.assertEqual('royal flush', man.hay_premio().lower())
        self.assertEqual(Prize.RoyalFlush, man.prize())

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

    def test_naipe_init_str(self):
        with self.assertRaisesRegex(ValueError, "The string \w has incorrect format"):
            Naipe('A')
            Naipe('ACC')
            Naipe('AAC')
            Naipe('AZ')
            Naipe('ACC')
            Naipe('11C')
            Naipe('99C')
            Naipe('0C')
            Naipe('1C')

    def test_naipe_init_str2(self):
        str_lst = ['10C', '7D', 'AH', 'KH', '2H']
        for s in str_lst:
            self.assertEqual(s, Naipe(s).repr_image_name())

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
        print("Probando a ver si: %s < %s < %s"%(lim_inf,lam,lim_sup))
        self.assertTrue(lim_inf < lam < lim_sup)

    def test_Mano_new(self):
        m1 = Mano.new([0,1,2,3])
        m2 = Mano.new([(1,0),(2,0),(3,0),(4,0)])
        m3 = Mano.new(list(map(Naipe, m2)))
        self.assertEqual(m1 ,  m2)
        self.assertEqual(m1 ,  m3)

    def test_repr_image_name(self):
        B = Baraja().sacar_lista_naipes(52)
        name_str = ''' 10C.png  2H.png  4C.png  5H.png  7C.png  8H.png  AC.png    JD.png  KS.png
10D.png  2S.png  4D.png  5S.png  7D.png  8S.png  AD.png    JH.png  QC.png
10H.png  3C.png  4H.png  6C.png  7H.png  9C.png  AH.png    JS.png  QD.png
10S.png  3D.png  4S.png  6D.png  7S.png  9D.png  AS.png    KC.png  QH.png
2C.png   3H.png  5C.png  6H.png  8C.png  9H.png  back.png  KD.png  QS.png
2D.png   3S.png  5D.png  6S.png  8D.png  9S.png  JC.png    KH.png'''

        for n in B:
            self.assertTrue(n.repr_image_name() in name_str)

    def test_sacar_lista_naipes(self):
        B = Baraja()
        self.assertEqual(len(B.sacar_lista_naipes(26)) , 26)
        self.assertEqual(len(B.sacar_lista_naipes(26)) , 26)
        self.assertEqual(len(B.sacar_lista_naipes(26)) , 0)

    def test_preview_length(self):
        B = Baraja()
        B.preview()
        self.assertEqual(len(B.preview()), 10)

    def test_certificate_stays_the_same(self):
        B = Baraja()
        certif1 = B.certificate
        B.revolver
        certif2 = B.certificate
        self.assertEqual(certif1, certif2)

    def test_baraja_index1(self):
        B = Baraja()
        ind = B.index((2,0))
        self.assertEqual(ind, 4)

    def test_baraja_index2(self):
        B = Baraja()
        carta = Naipe((2,0))
        ind = B.index(carta)
        self.assertEqual(ind, 4)

    def test_swap_2_cards(self):
        B = Baraja()
        B.swap_2_cards((1,0), Naipe((13,3)))
        self.assertEqual(B.index((13,3)), 0)
        self.assertEqual(B.index((1,0)), 51)
        # Swap same card
        B.swap_2_cards((1,0), Naipe((1,0)))
        self.assertEqual(B.index((1,0)), 51)


    def test_start_with(self):
        B = Baraja()
        B.revolver()
        start_lst = [Naipe((1,2)), Naipe((3,0)), Naipe((6,1))]
        B.start_with(start_lst)
        self.assertEqual(B.sacar_lista_naipes(3), start_lst)

        start_lst = [Naipe((1,2)),]
        B.start_with(start_lst)
        self.assertEqual(B.sacar_lista_naipes(1, start_at_0=True), start_lst)

    def test_premios4(self):
        B = Baraja()
        B.revolver()
        start_lst = [Naipe('AH'), Naipe('QH'), Naipe('JH'), Naipe('10H'), Naipe('KH')]
        B.start_with(start_lst)
        man = B.sacar_mano(5)
        self.assertEqual('royal flush', man.hay_premio().lower())
        self.assertEqual('royal flush', man.hay_premio().lower())

    def test_randNaipe_alot(self):
        B = Baraja()
        for _ in range(1000):
            B.randNaipe(15)

    def test_randSample(self):
        B = Baraja()
        s = B.randSample(1,after=51)[0]
        self.assertEqual(Naipe('KD'), Naipe(s))

    def test_play(self):
        B = Baraja()
        B.start_with(list(map(Naipe, ['2S','2D','3S','2H','2C'])))
        self.assertEqual(Prize.Poker, B.play(28))
        B.start_with(list(map(Naipe, ['2S','2D','3S','3H','2C'])))
        self.assertEqual(Prize.FullHouse, B.play(31))

    def test_naipe_is_idempotent(self):
        self.assertIsInstance(Naipe('KD'), Naipe)
        self.assertIsInstance(Naipe(Naipe('KD')), Naipe)
