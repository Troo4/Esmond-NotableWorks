#include <stdio.h>
#include "Memory.h"
#include "Cache.h"
#include "Set.h"
#include "AddressDecoder.h"
#include "PerformanceCounter.h"
Cache::Cache(Memory* m, int sc, int sb, int sa){
    this->cacheSize = sc;
    this->blockSize = sb;
    this->setAssoc = sa;
    this->numSets = sc/(sb*sa);
    this->sets = new Set*[numSets];
    this->mainMem = m;
    this->decoder = new AddressDecoder(sc,sb,sa);
    this->performanceCounter = new PerformanceCounter();
    this->sets = new Set*[this->numSets];
    for(int i=0;i<numSets;i++){
        //creates new set object, passing memory object, set associativity, size of blocks, and the decoder instance
        this->sets[i] = new Set(m, sa, sb, this->decoder, this->performanceCounter);
    }
}
unsigned char Cache::read(unsigned long addy){
    //gets set index w decoder, calls the read for the set with the address
    unsigned long setInd = (this->decoder)->getSetIndex(addy);
    return ((this->sets)[setInd])->read(addy);
}
void Cache::write(unsigned long addy, unsigned char newVal){
    //gets the index w decoder, calls the write 
    unsigned long setInd = (this->decoder)->getSetIndex(addy);
    return((this->sets)[setInd])->write(addy, newVal);
}
void Cache::display(){
    printf("CACHE:\n");
    for(int i=0;i<this->numSets;i++){
        printf(" SET: %d\n",i);
        sets[i]->display();
    }
    this->performanceCounter->display();
}