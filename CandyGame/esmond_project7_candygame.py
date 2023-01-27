
"""
File Names: esmond_project7_candygame.py
Author: Truman Esmond
Date: Oct 2022
Course: COMP 1351
Assignment: Project 7- Candy Game
Collaborators: N/A
Internet Source: None
"""
#Prints game instructions
print(f"You start out with 6 boxes of candy, and each box contains 7 pieces of candy. \n At each players turn, the player has to remove some candy from one of the boxes. \n The player can choose the box and the number of pieces of candy to remove from that box. \n The player who takes the last piece of candy wins the game.")

def display(boxes: list)->None:
    """
    displays the amount of candy in each box to the user
    
    boxes: takes a list of the candy values for each box
    """
    for i in range(len(boxes)):
        print(boxes[i], " ", end="")
    print()
def game_over(boxes: list)->bool:
    """
    determines whether there is a value greater than 0 in each element of the list, if not, game over.

    boxes: takes a list of candy values for each box

    return: boolean, false if there are still nonzero values in the list, true otherwise
    """
    for i in range(len(boxes)):
        if boxes[i] > 0:
            return False
    return True
boxes = []
for i in range(6):
    #populates the list with 6 'boxes' with 7 'candies' per element
    boxes.append(7)
player1 = input("Player 1, what is your name? ") #gets names for player 1 and 2
player2 = input("Player 2, what is your name? ")
turn = 1 #turn value, used to switch turns for the user
print(f"Hello {player1} and {player2}!")
while game_over(boxes) == False: #loops as long as game_over(boxes) returns False.
    display(boxes)
    if turn == 1:
        #if turn is 1, it is player 1's turn
        boxvalue = int(input(f"{player1}, pick the box you would like to take from: "))
    else:
        #if turn is -1, it is player 2's turn
        boxvalue = int(input(f"{player2}, pick the box you would like to take from: "))
    while boxvalue < 1 or boxvalue > 6:
        #loops infinitely while the user inputs a box number that is not between 1 and 6
        boxvalue = int(input("Please enter a value between 1 and 6 for the box: "))
    while boxes[boxvalue - 1] < 1:
        #loops infinitely as long as the user selects a box with a value of 0
        boxvalue = int(input("Please choose a box with candy in it: "))
    candyvalue = int(input("How many candies would you like to take? "))
    while candyvalue < 1 or candyvalue > boxes[boxvalue - 1]:
        #loops infinitely while the user wants to take an amount of candy that is either < 1 or > than the amount in the given box
        candyvalue = int(input("Please pick a valid number of candies to remove from the box: "))
    boxes.insert(boxvalue-1, boxes[boxvalue - 1] - candyvalue) #adds new value to list after user takes candy
    boxes.pop(boxvalue) #removes old value from list
    turn *= -1 #switches turn
turn *= -1 #switches turn again to player that took the last candy
if turn == 1: #depending on who took the last candy, program outputs the player's name.
    print(f"Congratulations {player1}, you win! Thanks for playing!")
else:
    print(f"Congratulations {player2}, you win! Thanks for playing!")