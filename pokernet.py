import tensorflow as tf
from itertools import combinations
import numpy as np
from baraja import Naipe, Mano, Baraja, Prize, one_hot_inv
import actions as act
import matplotlib.pyplot as plt
import random

# Tensorflow2 implementation
action_num = 32 # The number of possible action the player can take
naipes_num = 52 # Self-explanatory
class PokerNet(tf.keras.Model):
    def __init__(self):
        super(PokerNet, self).__init__()
        self.dense_final = tf.keras.layers.Dense(action_num, activation=tf.nn.softmax)
        self.dense2 = tf.keras.layers.Dense(naipes_num*naipes_num)
        self.dense1 = tf.keras.layers.Dense(naipes_num*naipes_num, 
                                            input_shape=(naipes_num,),activation=tf.nn.relu)
    def call(self, x):
        x = self.dense1(x)
        x = self.dense2(x)
        x = self.dense_final(x)
        return x

model = PokerNet()
model.compile(optimizer='sgd',
             loss='sparse_categorical_crossentropy',
             metrics=['accuracy'])

def batching(size):
    print('running batching')
    xbat = []
    ybat = []
    b = Baraja()
    for _ in range(size):
        b.revolver()
        xbat.append(b.one_hot())
        ybat.append(b.approx_best_move(sample_size=150)[0])
    return xbat, ybat

pool = mp.Pool(processes=4)
for _ in range(5):
    print('started batching ')
    ret = pool.map(batching, 4*[10])
    lx = reduce(lambda a,b: a+b, [r[0] for r in ret])
    ly = reduce(lambda a,b: a+b, [r[1] for r in ret])
    x_train = np.reshape(np.array(lx).astype(np.float32),[len(lx), 52]) 
    y_train = np.reshape(np.array(ly).astype(np.float32),[len(ly),1])
    model.fit(x_train, y_train, epochs = 5)


bar = Baraja()
num_games = 10
credit = 0
for _ in range(num_games):
    bar.revolver()
    oh = np.array(bar.one_hot(), ndmin=2)
    acc = np.argmax(model.call(oh))
    #acc = random.randint(0,31)
    credit += bar.play(acc).value
print("The expected return is: ", credit/num_games - 1)
model_filepath = 'poker.mdl'
print('saving model at: ', model_filepath)
model.save(model_filepath)
