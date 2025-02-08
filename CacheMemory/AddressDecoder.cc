#include <stdio.h>
#include "AddressDecoder.h"
#include <cmath>

AddressDecoder::AddressDecoder(int sc, int sb, int sa){
    this->cacheSize = sc;
    this->blockSize = sb;
    this->setAssoc = sa;
    this->numSets = sc/(sb*sa);

    this->blockBits = std::log2(this->blockSize);
    this->setBits = std::log2(this->numSets);
    this->tagBits = (sizeof(unsigned long)*8) - (this->blockBits) - (this->setBits);
}
unsigned long AddressDecoder::getSetIndex(unsigned long addy){
    //returns set index, detemines which set the memory should be put into the cache
    return ((addy<<(this->tagBits))>>((this->tagBits) + this->blockBits));
}
unsigned long AddressDecoder::getBlockOffset(unsigned long addy){
    //returns the block offset, used to access block data directly
    int shiftAmt = (this->tagBits) + this->setBits;
    return (addy<<shiftAmt)>>shiftAmt;
}
unsigned long AddressDecoder::getTag(unsigned long addy){
    //returns the tag of the address
    return (addy>>((this->setBits)+this->blockBits));
}
unsigned long AddressDecoder::getBlockAddress(unsigned long addy){
    //returns the tag + set index portion of the address (no offset)
    return (addy>>(this->blockBits))<<(this->blockBits);
}
unsigned long AddressDecoder::spliceAddress(unsigned long tag, unsigned long addy){
    //allows decoder to take the tag of a block and derive the entire address from it
    unsigned long noTag = getSetIndex(addy);
    noTag = noTag<<this->blockBits;
    unsigned long nTag = tag<<((this->setBits)+(this->blockBits));
    return nTag | noTag;
}