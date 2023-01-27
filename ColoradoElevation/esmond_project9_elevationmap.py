"""
File Names: esmond_project9_elevationmap.py
Author: Truman Esmond
Date: Nov 2022
Course: COMP 1351
Assignment: Project 9- Colorado Elevations
Collaborators: N/A
Internet Source: None
"""

import dudraw
elevations = []
with open("CO_elevations_feet.txt", 'r') as a_file:  #opens the elevations file, sorts the value into 560 lists of 760 elevations
    for line in a_file:
        elevations.append(line.strip().split(" "))
max = 0 #declares a mximum elevation variable
min = 4000 # random value from list, will allow me to compare other values to it because 0 would be the min otherwise
for row in elevations:  
    for column in row:
        if int(column) > max:
            max = int(column)           #iterates over all of the elevations, finds the maximum elevation and minimum, saves them to a variable
        elif int(column) < min:
            min = int(column)
print(max) #outputs max to the user
def color(row: int, colm: int)-> int:
    """
    Purpose: Scales an elevation to a value between 0 and 255 depending on its place between the maximum and minimum elevation.
    Parameters: two ints, relating to the row and column of the elevations matrix, used to put the corresponding elevation in a 255 scale
    Return: an int between 0 and 255
    """
    
    segment = int(elevations[row][colm]) #finds the elevation
    range = max - min #declares the range between max and min
    place = segment - min #finds the range between the elevation and min
    return int((place/range)*255) #returns the segment out of the total range, multipied by 255

dudraw.set_canvas_size(760, 560)
dudraw.set_x_scale(0,760) #760 wide
dudraw.set_y_scale(0,560) #560 tall

def draw()->None:
    """
    Purpose: depicts the elevations matrix with rgb values for each pixel on a gray-scale, creates a topographical map of CO
    Parameters: none
    Return: none
    """
    xLoc = .5 #x coordinate
    yLoc = 560 - .5 #y coordinate
    for r in range(len(elevations)): #loops for the amount of rows in the matrix
        xLoc = .5 #resets x coordinate at the start of new row
        for c in range(len(elevations[r])): #loops for the amount of columns in each row
            dudraw.set_pen_color_rgb(color(r, c), color(r, c), color(r, c)) #sets the pen color to the appropriate RGB value based on the elevation index
            dudraw.filled_square(xLoc, yLoc, .5) #draws a square pixel on the canvas
            xLoc += 1 #moves one pixel over
        yLoc -= 1 #drops down one row
draw() #draws the inital map, with no particular elevation selected
while True: #loops infinitely
    if dudraw.mouse_is_pressed():
        dudraw.clear()
        draw()
        mouseX = int(dudraw.mouse_x())    #this outputs the exact elevation at a point the user clicks on in red in the bottom right corner of the map
        mouseY = int(dudraw.mouse_y())
        dudraw.set_pen_color(dudraw.RED)
    
        dudraw.text(720,50,str(elevations[mouseY][mouseX]))

    dudraw.show(60)
