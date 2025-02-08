#include <stdio.h>
#include "Memory.h"
#include "AddressDecoder.h"
#include "PerformanceCounter.h"
#include "Block.h"

#ifndef SET_H
#define SET_H

class Set{
    private:
        int setAssoc;
        int blockSize;
        Memory* mainMem;
        AddressDecoder* decoder;
        PerformanceCounter* performanceCounter;
        Block** blocks;

        //private helper methods
        unsigned long findBlock(unsigned long tag, unsigned long addy);
        unsigned long matchingBlock(unsigned long tag);
        unsigned long emptyBlock();
        unsigned long findLRU();
        void evictLRU(unsigned long addy, unsigned long lruBlock);

    public:
        Set(Memory* m, int sa, int sb, AddressDecoder* d, PerformanceCounter* pc);
        unsigned char read(unsigned long addy);
        void write(unsigned long addy, unsigned char newVal);
        void display();
        
};

#endif