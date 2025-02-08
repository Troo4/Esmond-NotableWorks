#include <stdio.h>
#include "Memory.h"
#include "Block.h"
#include <chrono> // to bring in the chrono library

Block::Block(Memory* m, int bs){
    this->mainMem = m;
    this->blockSize = bs;
    this->valid = 0;
    this->dirty = 0;
    this->tag = 0;
    this->lastAccess = 0;
    this->data = new unsigned char[this->blockSize];
    
}
unsigned char Block::read(unsigned long offset){
    //updates time, ensures a valid block, and returns the data at the offset
    updateTime();
    this->valid = 1;
    return this->data[offset];
}  
void Block::write(unsigned long offset, unsigned char newVal){
    //updates time, status, cleanliness, and changes the data at the block offset
    updateTime();
    this->valid = 1;
    this->dirty = 1;
    this->data[offset] = newVal;
}
void Block::loadFromMemory(unsigned long addy){
    //loads each byte in the block into the block object
    for(int i =0;i<this->blockSize;i++){
        this->data[i]= this->mainMem->getByte(addy+i);
    }
    this->dirty = 0;
}
void Block::saveToMemory(unsigned long addy){
    //saves each byte back to the memory block
    for(int i=0; i<this->blockSize;i++){
        this->mainMem->setByte(addy + i, this->data[i]);
    }

}
void Block::display(){
    printf("    valid: %d   tag: %ld   dirty: %d   timestamp: %ld\n", this->valid, this->tag, this->dirty, this->lastAccess);
    printf("    ");
    for(int i=0; i<this->blockSize;i++){
        printf("%02x",this->data[i]);
        printf(" ");
    }
    printf("\n");
}
//getters and setters
bool Block::isValid(){
    return this->valid;
}
bool Block::isDirty(){
    return this->dirty;
}
long Block::lastAccessed(){
    return this->lastAccess;
}
void Block::updateTime(){
    // to make an instance of a high resolution clock called m_clock
    std::chrono::high_resolution_clock m_clock;
    // to get the number of nanoseconds that have elapsed since the epoch (Jan 1, 1970) as a long
    long nanoSec = std::chrono::duration_cast<std::chrono::nanoseconds>(m_clock.now().time_since_epoch()).count();
    this->lastAccess = nanoSec;
}
unsigned long Block::getTag(){
    return this->tag;
}
void Block::setTag(unsigned long nt){
    this->tag = nt;
}
void Block::setValid(int setting){
    this->valid = setting;
}
void Block::setDirty(int cleanliness){
    this->dirty = cleanliness;
}

