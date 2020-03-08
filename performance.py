from baraja import Naipe, Mano, Baraja, Prize
import actions as act
import logging
from math import ceil

def play_game(sample_size):
    bb = Baraja()
    bb.revolver()
    A = bb.approx_best_move(sample_size=sample_size)
    prize = bb.play(A[0]).value
    logging.debug('%s -- %s -- %s'%(bb.preview(),
        act.actions[A[0]], prize))
    return prize


if __name__ == '__main__':
    import sys
    import argparse
    parser = argparse.ArgumentParser(description='simulate games to assess the performance of a policy')
    parser.add_argument("sample_size", type=int, nargs='+',
            help='The sample size to check in each game')
    parser.add_argument("games_num", type=int,
            help='the number of games to play')
    parser.add_argument("--log", type=str, default=None,
            help='log level, options are: debug, info, warning, error, critical')
    args = parser.parse_args(sys.argv[1:])

    if args.log:
        logging.basicConfig(level=getattr(logging, args.log.upper()))
    logging.debug('sample size: %s   number of games: %s'%(args.sample_size, args.games_num))
    size_dict = {}
    for S in args.sample_size:
        credit = 0
        for k in range(args.games_num):
            credit += play_game(S)
            if k%ceil(args.games_num/10) == 0: # print about ten messages
                logging.info('playing game %s'%k)
        size_dict[S] = credit/args.games_num - 1
    print(size_dict)
