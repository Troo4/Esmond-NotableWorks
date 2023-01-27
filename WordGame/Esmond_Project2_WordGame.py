"""
    Word guessing game (Wordle)
    Filename: Esmond_Project2_WordGame.py
    Author: Truman Esmond
    Date: Jan 2023
    Course: 1352
    Assignment: Project 2 - Word game
    Collaborators: none
    Internet Source: none
"""
from random import randint
words = [] #declares an empty list that will store our list of viable words
with open("Q2/usaWords.txt", 'r') as a_file: #opens the file and puts every 5-letter word into a list: words
    for line in a_file:
        if len(line) == 6:
            words.append(line.strip())
word = words[randint(0,len(words))]
#word = 'eerie' # these two lines provide an override to test certain word circumstances
#word = 'arrow'
counter = 0 #counts the number of turns
guesses = {} #will store the users guess number, their guess, and the response
def validGuess(guess)-> bool:
    """
    checks if the user's guess is a valid word
    parameter:
    guess - takes the user's guess as a string
    returns: boolean, true if guess is valid, false if not
    """
    if len(guess) != 5: #if guess is not 5 letters, returns false
        return False
    for ele in words:
        if guess == ele: #if guess is in 'words', return true
            return True
def check(guess)->str:
    """
    checks users guess to the random word
    parameters:
    guess - takes user's guess as a string
    returns: a 5 letter string of either G: letter is correct and in correct place, Y: letter is correct but incorrectly placed, or B: letter is incorrect
    """
    answer = ''
    for i in range(5):
        if guess[i] == word[i]:
            answer += 'G' #adds 'G' to answer if it is the correct letter in the correct spot
        elif word.find(guess[i]) != -1 and guess.count(guess[i], 0, i+1) <= word.count(guess[i]): #adds 'Y' to the answer if it is the correct letter but incorrectly placed (will only add 'Y' for every occurence of the letter in the word, will not duplicate "Y" for only one letter in the word)
            answer += 'Y'
        else:
            answer += 'B' #adds 'B' to answer if it is not found in the word
    return answer
guess = 'start'
while counter <= 7 and check(guess) != 'GGGGG': #infinitely loops while the user does not exceed 6 attempts, and do not guess the answer
    guess = input("Guess: ")
    validGuess(guess) #validates guess
    while validGuess(guess) == False: #loops until user provides a valid guess
        guess = input('Please enter a valid, 5-letter word: ')
    guesses.update({f'Guess {counter}:': f'{guess} {check(guess)}'}) #updates the 'guesses' dictionary with the guess count, the guess itself, and the response
    for line in guesses:
        print(f'{line} {guesses.get(line)}', end= " ") #returns the guesses dictionary to the user after each guess
        print()
    counter += 1 #adds a turn
    if check(guess) == 'GGGGG': #stops loop if user guesses correctly
        break
if counter > 7 and check(guess) != 'GGGGG': #prints if user does not guess word after 6 tries
    print(f'You did not guess the word: {word}')
elif check(guess) == 'GGGGG': #prints how many turns it took for user to guess correctly
    print(f'You got it in {counter-1} guesses!')
