#include <stdio.h>
#ifndef MEMORY_H
#define MEMORY_H
class Memory{
    private:
        int size;
        unsigned char* main;
    public:
        Memory(int s);
        unsigned char getByte(unsigned long addy);
        void setByte(unsigned long addy, unsigned char newVal);
        int getSize();
        void display();

};
#endif