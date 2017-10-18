import os
from pathlib import Path

import numpy as np

directions = [[1, 0], [-1, 0], [0, 1], [0, -1]]


def in_bounds(point):
    if (0 <= point[0] <= 1 or 5 <= point[0] <= 6) and (0 <= point[1] <= 1 or 5 <= point[1] <= 6):
        return False
    if point[0] < 0 or point[1] < 0 or point[0] > 6 or point[1] > 6:
        return False
    return True


def legal_move(peg, peg_list):
    pegs = []
    d = directions[np.random.random_integers(0, 3)]
    x1 = peg[0] + d[0]
    y1 = peg[1] + d[1]
    peg1 = [x1, y1]
    x2 = peg[0] + (d[0] * 2)
    y2 = peg[1] + (d[1] * 2)
    peg2 = [x2, y2]
    if in_bounds(peg1) and in_bounds(peg2):
        if [x1, y1] not in peg_list and [x2, y2] not in peg_list:
            pegs.append([x1, y1])
            pegs.append([x2, y2])
    return pegs


def gen_state(moves=10):
    peg_list = [[3, 3]]
    it = 0
    while it < moves:
        it += 1
        index = np.random.randint(0, len(peg_list))
        peg = peg_list[index]
        new_pegs = legal_move(peg, peg_list)
        if len(new_pegs) != 0:
            peg_list.pop(index)
            peg_list.extend(new_pegs)
    return peg_list


def make_board(pegs_state):
    board = [['.', '.', '+', '+', '+', '.', '.'], ['.', '.', '+', '+', '+', '.', '.'],
             ['+', '+', '+', '+', '+', '+', '+'],
             ['+', '+', '+', '+', '+', '+', '+'], ['+', '+', '+', '+', '+', '+', '+'],
             ['.', '.', '+', '+', '+', '.', '.'],
             ['.', '.', '+', '+', '+', '.', '.']]
    for peg in pegs_state:
        board[peg[1]][peg[0]] = 'o'
    return board[-1:-len(board) - 1:-1]


def make_states(moves, number=10):
    with open("moves:" + str(moves), 'a') as f:
        for i in range(1, number + 1):
            pegs = gen_state(moves)
            board = make_board(pegs)
            for i in range(0, len(board)):
                f.write("".join([str(b) for b in board[i]]))
            f.write("\n")


def humanize(file):
    with open(file, 'r') as f:
        states = f.readlines()
    with open('human_' + str(file), 'w') as h:
        for i in states:
            h.write(i[0:7] + '\n')
            h.write(i[7:14] + '\n')
            h.write(i[14:21] + '\n')
            h.write(i[21:28] + '\n')
            h.write(i[28:35] + '\n')
            h.write(i[35:42] + '\n')
            h.write(i[42:49] + '\n')
            h.write("\n\n")


for i in range(25, 110, 10):
    make_states(i, 100)
    humanize('moves:' + str(i))
