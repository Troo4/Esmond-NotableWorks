#include <stdio.h>
#include "Memory.h"
#include "AddressDecoder.h"
#include "PerformanceCounter.h"
#include "Block.h"
#include "Set.h"

Set::Set(Memory* m, int sa, int sb, AddressDecoder* d, PerformanceCounter* pc){
    this->mainMem = m;
    this->setAssoc = sa;
    this->blockSize = sb;
    this->decoder = d;
    this->performanceCounter = pc;
    this->blocks = new Block*[this->setAssoc];
    for(int i=0; i<(this->setAssoc);i++){
        this->blocks[i] = new Block(m, sb);
    }
}
unsigned char Set::read(unsigned long addy){
    //gets the tag, searches for it and finds a block to read to using helper methods
    unsigned long tag = this->decoder->getTag(addy);
    unsigned long block = findBlock(tag, addy);
    //calls and returns the block read method
    return (this->blocks[block])->read(this->decoder->getBlockOffset(addy));
}
void Set::write(unsigned long addy, unsigned char newVal){
    //gets the tag, searches for it and finds a block to write to using helper methods
    unsigned long tag = this->decoder->getTag(addy);
    unsigned long block = findBlock(tag, addy);
    //calls block write method
    this->blocks[block]->write(this->decoder->getBlockOffset(addy),newVal);
}
void Set::display(){
    printf("   Blocks:\n");
    for(int i=0;i<this->setAssoc;i++){
        printf("   %d:\n",i);
        this->blocks[i]->display();
    }

}
unsigned long Set::findBlock(unsigned long tag, unsigned long addy){
    //finds the block to either read or write to, by finding matching, empty, or lru block
    unsigned long returnBlock;
    unsigned long block = this->matchingBlock(tag);
    if(block != -1){
        //if there is a matching block, it is a hit and the block is retained to be returned
        this->performanceCounter->hit();
        returnBlock = block;
        
    }
    else{
        //if there is no matching block, check for empty blocks to read into cache
        unsigned long empty = this->emptyBlock();
        if(empty != -1){
            //if there is an empty block
            unsigned long blockAddy = this->decoder->getBlockAddress(addy);
            // printf("BEFORE LOADMEM\n");
            this->blocks[empty]->loadFromMemory(blockAddy);
            this->blocks[empty]->setValid(1);
            this->blocks[empty]->updateTime();
            this->blocks[empty]->setTag(tag);
            this->performanceCounter->miss();
            
            returnBlock = empty;
        }
        else{
            //if no empty blocks, go to evict
            unsigned long lruBlock = this->findLRU();

            evictLRU(addy, lruBlock);
            this->performanceCounter->miss();
            returnBlock = lruBlock;
        }
    }
    //returns block to read or write to
    return returnBlock;
}

unsigned long Set::matchingBlock(unsigned long tag){
    for(unsigned long i=0;i<this->setAssoc;i++){
        if(this->blocks[i]->isValid() && this->blocks[i]->getTag() == tag){
            //matching block found, return block
            return i;
        }
    }
    //no matching blocks
    return -1;
}
unsigned long Set::emptyBlock(){
    for(unsigned long i=0;i<this->setAssoc;i++){
        if(!(this->blocks[i]->isValid())){
            //empty block found
            return i;
        }
    }
    //no empty block
    return -1;
}
unsigned long Set::findLRU(){
    unsigned long lru = 0; //initializes LRU to first block for an intitial comparator

    for(unsigned long i = 1; i< this->setAssoc; i++){
        if(((this->blocks)[i]->lastAccessed())<this->blocks[lru]->lastAccessed()){
            //if time is less than lru (meaning accessed less recently), block becomes lru
            lru = i;
        }
    }
    return lru;
}
void Set::evictLRU(unsigned long addy, unsigned long lruBlock){ //breakpoint*****
    //check if LRU is dirty, if so save to memory then evict
    if(this->blocks[lruBlock]->isDirty()){
        unsigned long lruTag = this->blocks[lruBlock]->getTag();
        //gets the address of the LRU block in memory by splicing its tag with the address of the new one (just need set index)
        unsigned long oldBlockAddy = this->decoder->spliceAddress(lruTag,addy);
        //saves it to memory
        this->blocks[lruBlock]->saveToMemory(oldBlockAddy);
        //make note of writeback
        this->performanceCounter->writeback();

    }
    //replace the old block w new
    //gets block address in memory to load the whole block
    unsigned long blockAddress = this->decoder->getBlockAddress(addy);
    //loads block
    this->blocks[lruBlock]->loadFromMemory(blockAddress);
    this->blocks[lruBlock]->setTag(this->decoder->getTag(addy));
    this->blocks[lruBlock]->updateTime();


}

