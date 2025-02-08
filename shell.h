#include <stdio.h>

#ifndef SHELL_H
#define SHELL_H

char** parse(char* inp);
void cd(char* path);
void execute_commands(char** cmd);
int isInput(char** inp, char** names);
int isOutput(char** inp, char** names);
int isBackground(char** inp);
void handleChild(int sig);
void command(char** cmd);
int main();



#endif