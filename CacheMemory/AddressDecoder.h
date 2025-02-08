#include <stdio.h>

#ifndef ADDRESSDECODER_H
#define ADDRESSDECODER_H

class AddressDecoder{
    private:
        int cacheSize;
        int blockSize;
        int setAssoc;
        int numSets;

        int blockBits;
        int setBits;
        int tagBits;

    public:
        AddressDecoder(int sc, int sb, int sa);
        unsigned long getSetIndex(unsigned long addy);
        unsigned long getBlockOffset(unsigned long addy);
        unsigned long getTag(unsigned long addy);
        unsigned long getBlockAddress(unsigned long addy);
        unsigned long spliceAddress(unsigned long tag, unsigned long addy);
};

#endif