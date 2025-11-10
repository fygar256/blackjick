#!/usr/bin/env python3
import random

class Card:
    def __init__(self, mark, number):
        self.mark = mark  # 0:no card 1:heart 2:dia 3:club 4:spade 5:joker
        self.number = number  # 1 ~ 13 0:joker

trump = 53
drawmax = 12
cards = []
cardidx = 0
comp_cards = [Card(0, 0) for _ in range(drawmax)]
player_cards = [Card(0, 0) for _ in range(drawmax)]

def id2card(c):
    mark = 5 if c == 0 else (c - 1) // 13 + 1
    number = 0 if c == 0 else (c - 1) % 13 + 1
    return Card(mark, number)

def shuffle():
    global cards, cardidx
    r = [0] * trump
    ss = []
    random.seed()
    for _ in range(trump):
        c = random.randint(0, trump-1)
        while r[c]:
            c = random.randint(0, trump-1)
        r[c] = 1
        ss.append(c)
    cards = [id2card(ss[i]) for i in range(trump)]

def draw_card(cd):
    global cardidx
    i = 0
    while cd[i].mark:
        i += 1
    if cardidx < trump and i < drawmax:
        cd[i] = cards[cardidx]
        cardidx += 1

def init_players():
    global comp_cards, player_cards
    for i in range(drawmax):
        comp_cards[i].mark = player_cards[i].mark = 0
    draw_card(comp_cards)
    draw_card(comp_cards)
    draw_card(player_cards)
    draw_card(player_cards)

def number_of_cards(cd):
    idx = 0
    while cd[idx].mark:
        idx += 1
    return idx

def isjoker(cd):
    idx = 0
    while cd[idx].mark:
        if cd[idx].mark == 5:
            return 1
        idx += 1
    return 0

def printcard(cd):
    marks = ["", "♥", "♦", "♣", "♠", "Joker"]
    numbers = ["", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"]
    print("[{} {}] ".format(marks[cd.mark], numbers[cd.number]), end='')

def count_cards(cd):
    i = 0
    total_value = 0
    while cd[i].mark:
        n = cd[i].number
        total_value += n if n <= 10 else 10
        i += 1
    return total_value

def eval_cards(cd):
    total_value = count_cards(cd)
    joker_value = 0
    if isjoker(cd):
        joker_value=21-total_value
        if joker_value>10:
            joker_value=10
    return total_value + joker_value

def print_players_line(cd):
    i = 0
    while cd[i].mark:
        printcard(cd[i])
        i += 1

    print(f"{i} cards ",end='')
    print("total={:3d} ".format(eval_cards(cd)), end='')
    if eval_cards(cd) == 21:
        print("*BlackJick*",end='')
    print()

def print_comp():
    idx = 0
    while comp_cards[idx].mark:
        print("[###] ", end='')
        idx += 1
    print("{} cards.".format(idx))

def game():
    cv = eval_cards(comp_cards)
    pv = eval_cards(player_cards)
    if (cv == 21 and pv == 21) or cv == pv:
        if isjoker(comp_cards):
            return 2
        elif isjoker(player_cards):
            return 1
        else:
            return 0
    elif cv >= 22 and pv >= 22:
        return 0
    elif cv >= 22 and pv <= 21:
        return 1
    elif cv <= 21 and pv >= 22:
        return 2
    elif cv < pv:
        return 1
    else:
        return 2

def expected_value_of_next_card(self):
    noc = trump - number_of_cards(self)
    n = count_cards(self)
    j = 0 if isjoker(self) else 0 if n >= 21 else 10 if n < 11 else 21 - n
    return (340 + j - n) / noc

def todrawp(self):
    return 0 if eval_cards(self) >= 21 else 1 if count_cards(self) + expected_value_of_next_card(self) <= 21.0 else 0

def computer_turn():
    global comp_cards
    if todrawp(comp_cards):
        print("Computer drew a card.")
        draw_card(comp_cards)
        return 1
    return 0

def play():
    d = 0
    while d != 2:
        print("---------------------------------------------------")
        print(f"Pile : {trump - cardidx} cards")
        print("Computer's cards : ", end='')
        print_comp()
        print("Player's cards : ", end='')
        print_players_line(player_cards)
        e = expected_value_of_next_card(player_cards)
        print(f"Expected value of next 1 card is {e:.8f}.")
        print("Have to draw ? {}.".format("Yes" if todrawp(player_cards) else "No"))
        d = int(input("1. Draw a card.\n2. Game.\n: "))
        if d == 1:
            draw_card(player_cards)
        computer_turn()
    while computer_turn():
        pass

def result(g):
    print("====================Result====================")
    print("Computer : ", end='')
    print_players_line(comp_cards)
    print("Player : ", end='')
    print_players_line(player_cards)
    print(["Draw game", "Player win", "Computer win"][g])

if __name__ == "__main__":
    print("**********BlackJick ver 1.0**********")
    shuffle()
    init_players()
    play()
    result(game())
