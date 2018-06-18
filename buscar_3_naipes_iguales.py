import io
import sys
import time
import pickle
from baraja import Naipe, Baraja

#sys.stdout.write('/033[31m' + 'some red text\\n')
#sys.stdout.flush()

b = Baraja()
n = 0
conjunto = set()
print("Inicializando Todo")

cycle_size = 1000
keep_looping = True
while keep_looping:
    try:
        n=n+1
        for i in range(cycle_size):
            b.revolver()
            if b.chequear_3_iguales():
                conjunto.add(b.sacar_mano(5))
        message_str = "Hallado: %s de %s \r"%(len(conjunto),n*cycle_size)
        sys.stdout.write(message_str)
        sys.stdout.flush()
        time.sleep(1)
    except KeyboardInterrupt:
        print('Salio Exito')
        keep_looping = False

print('Resultado Final: %s'%message_str)
