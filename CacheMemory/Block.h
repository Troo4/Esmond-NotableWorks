#include <stdio.h>
#include "Memory.h"
#include "AddressDecoder.h"
#include <chrono> // to bring in the chrono library

#ifndef BLOCK_H
#define BLOCK_H

class Block{
    private:
        Memory* mainMem;
        int blockSize;
        unsigned char* data;
        bool valid;
        bool dirty;
        unsigned long tag;
        long lastAccess;
        AddressDecoder* decoder;
    public:
        //major functions
        Block(Memory* m, int bs);
        unsigned char read(unsigned long offset );
        void write(unsigned long offset, unsigned char newVal);
        void loadFromMemory(unsigned long addy);
        void saveToMemory(unsigned long addy);
        void display();
        //setters and getters
        bool isValid();
        bool isDirty();
        long lastAccessed();
        void updateTime();
        unsigned long getTag();
        void setTag(unsigned long tag);
        void setValid(int setting);
        void setDirty(int cleanliness);




};

#endif