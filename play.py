import argparse
import random
import sys
from subprocess import Popen, PIPE

def get_move(player, board):
	p = Popen((player, board), stdout=PIPE, stderr=PIPE)
	win = False
	for line in p.stderr.readlines():
		if 'win' in line.decode('utf-8'):
			win = True
		sys.stdout.write(line.decode('utf-8'))
	return p.stdout.read(), win

parser = argparse.ArgumentParser()
parser.add_argument('player1')
parser.add_argument('player2')
parser.add_argument('-n', '--num-games', type=int, default=1)
parser.add_argument('-o', '--keep-order', action='store_false')
args = parser.parse_args()

def play_game(player1, player2):
	win = False
	position = '11111111111111110000000000000000000000000000000022222222222222221'
	turns = [player1, player2]
	if not args.keep_order:
		random.shuffle(turns)
	while not win:
		player = turns.pop(0)
		position, win = get_move(player, position)
		turns.append(player)
	return player

score = {}
for _ in range(args.num_games):
	winner = play_game(args.player1, args.player2)
	score[winner] = score.get(winner, 0) + 1
print(score)
