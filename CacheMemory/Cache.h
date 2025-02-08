#include <stdio.h>
#include "Memory.h"
#include "Set.h"
#include "AddressDecoder.h"
#include "PerformanceCounter.h"
#ifndef CACHE_H
#define CACHE_H

class Cache{
    private:
        Set** sets;
        int cacheSize;
        int blockSize;
        int setAssoc;
        int numSets;
        Memory* mainMem;
        AddressDecoder* decoder;
        PerformanceCounter* performanceCounter;
    public:
        Cache(Memory* m, int sc, int sb, int sa);
        unsigned char read(unsigned long addy);
        void write(unsigned long addy, unsigned char newVal);
        void display();
};

#endif