"""
    Sand drop game
    Filename: Esmond_Project3_SandGame.py
    Author: Truman Esmond
    Date: Jan 2023
    Course: 1352
    Assignment: Project 3 - Sand game
    Collaborators: none
    Internet Source: none
"""
import dudraw
from random import randint
dudraw.set_canvas_size(500,500)
dudraw.set_x_scale(0,100)
dudraw.set_y_scale(0,100)
dudraw.set_pen_color_rgb(247,221,158) #sets pen color to tan
sandList = []
for i in range(100):
    new = []
    for j in range(100):      #populates a 100x100 matrix of zeroes called 'sandList'
        new.append(0)
    sandList.append(new)

key = ''

while key != 'q': #loops infinitely until 'q'
    dudraw.clear() #clears the canvas
    mouseX = 0
    mouseY = 0
    if dudraw.mouse_is_pressed(): #checks if mouse is pressed
        mouseX = int(dudraw.mouse_x()) + randint(-2,2) #gets the x coordinate of mouse, with an ineger variation of +- 2 (to randomize sand placement)
        mouseY = int(dudraw.mouse_y()) #gets the y coordinate of mouse
        if mouseX < 100 and mouseX > 0 and mouseY > 0 and mouseY < 100: #make sure the mouse stays within the bound of the list and canvas
            sandList[mouseY][mouseX] = 1 #changes the corresponding matrix item to 1
    for r in range(len(sandList)):
        for c in range(len(sandList[r])): #loops for every item in the matrix, if element is == 1, it draws a square in the corresponding location on canvas
            if sandList[r][c] == 1:
                dudraw.filled_square(c,r,.5)
    for r in range(len(sandList)-1):
        for c in range(len(sandList[r])):
            if sandList[r][c] == 1 and sandList[r-1][c] == 0 and r > 0 and r < 100: #loops for every element in matrix, if element is ==1,                    
                sandList[r][c] = 0                                                  #checks if the element one row below is ==1, if so, it swaps the 0 and 1 to move the 'sand' downward
                sandList[r-1][c] = 1
    
    if dudraw.has_next_key_typed(): #checks if user types 'q'
        key = dudraw.next_key_typed()
    dudraw.show(10)     #displays canvas