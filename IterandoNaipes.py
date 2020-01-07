# ---
# jupyter:
#   jupytext:
#     formats: ipynb,py:light
#     text_representation:
#       extension: .py
#       format_name: light
#       format_version: '1.4'
#       jupytext_version: 1.2.4
#   kernelspec:
#     display_name: Python 3
#     language: python
#     name: python3
# ---

import random as r
import itertools as itt
import sys
from scipy.stats import chi2
# %load_ext autoreload
# %autoreload 2
import actions as act
from baraja import Naipe, Mano, Baraja, Prize
# #%run ~/code/EfedEquis/Efedequis/poquer/baraja/baraja

# %%time
b = Baraja()
b.start_with(map(Naipe,['3S', 'QS', 'JD', '7S', '6S']))
res = []
for i in range(32):
    res.append((act.actions[i], b.evaluate_eff(i, sample_size=10)))
{k: v for k, v in sorted(res, key=lambda item: -item[1])}
best = b.approx_best_move(sample_size=100)
act.actions[best[0]],best[1]

# %%time
b = Baraja()
b.start_with(map(Naipe,['AS', 'QS', 'QD', 'JS', '10S']))
res = []
for i in range(32):
    res.append((act.actions[i], b.evaluate(i, sample_size=100)))
{k: v for k, v in sorted(res, key=lambda item: -item[1])}

[i for i in range(5) if i not in act.actions[29] ], act.actions[29] 

for n in iter(m):
    print(Naipe(n).repr_naipe())

m = b.revolver().sacar_mano(5)
print(m)
m.hay_premio()

'jacks' in m.hay_premio().lower()

list(filter(lambda x: x[0] == 1, mac))[0]

n=0
#conjunto = set()
b.revolver()
while True and n<10000:
    b.revolver()
    ma = b.sacar_mano(5)
    if ma.is_jacks_or_better():
        print(b.sacar_mano(5))
        break
    n += 1

ma.is_full_house()

#

n = Naipe((3,2))
for k in range(26):
    print(n)
    n = n.get_successor()

isinstance(Naipe((1,0)),Naipe)

lst = [1,3,4,5]
lst.insert(0,'d')
print(lst)

conjunto

import itertools as itert
for i in itert.combinations(b.sacar_mano(10),5):
    print(i)

len(conjunto)    

print(b.revolver())
b.chequear_3_iguales()

con = set([1,2])
con.add(20)
print(con)

raise ValueError()

# +
import io
import sys
import time


#sys.stdout.write('/033[31m' + 'some red text\n')
#sys.stdout.flush()

t = 0
while t<400:
    for i in ["/","-","|","\\","|"]:
        sys.stdout.write("%s\r" %(i + "\033[6Cmejorando"))
        sys.stdout.flush()
        time.sleep(0.1)
        t=t+1
# -

lis = [1,3]
lis.

suma = 0
for i in itt.combinations(b._cartas_,5):
    if Mano(i).is_poker():
        suma += 1
print(suma)

print(Mano(i))

int(10e5)

Naipe((1,0)) == Naipe((1,0))


b.sacar_mano(5) == b.sacar_mano(5)

# %%time
X = 0
for i in range(int(10e5)):
    b.revolver()
    if Naipe((1,0)) == b.sacar_lista_naipes(1)[0]:
        X += 1

X

b.sacar_lista_naipes(1)[0]

# %timeit b.revolver()

# %timeit Naipe((1,0)) == b.sacar_lista_naipes(1)[0]

82.9e-9*10e5

# %timeit Naipe((1,0))

import re
r = re.match('^([2-9]|10|[AJQK])([SDCH])$', '10S')
r.group(1),r.group(2)

isinstance('A', int)


# +
class OO(frozenset):
    

OO({1,2,3})


# -

class Mano(frozenset):
    def __new__(cls,X):
        return frozenset.__new__(cls,X)
    
    def culi(self):
        print('culi es culito')


M = Mano([1,2,3])

M.culi()

man = Mano(['AS', '3S', '4S', '5C', '6D'])
print(man)

man.hay_premio()

list(map(Naipe,['KS']))[0]

a = act.actions[28]
print(a)
[(b in a) for b in range(5)]
