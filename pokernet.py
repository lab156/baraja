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

def Batch(size):
    xbat = []
    ybat = []
    b = Baraja()
    for _ in range(size):
        b.revolver()
        xbat.append(b.one_hot())
        #ybat.append(one_hot(b.approx_best_move(sample_size=100)[0]))
        ybat.append(b.approx_best_move(sample_size=150)[0])
    return np.reshape(np.array(xbat).astype(np.float32),[size, 52]), np.reshape(np.array(ybat).astype(np.float32),[size,1])

for _ in range(5):
    print('started batching')
    x_train, y_train = Batch(10)
    model.fit(x_train, y_train, epochs = 5)
