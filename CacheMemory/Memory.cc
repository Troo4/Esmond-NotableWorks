#include <stdio.h>
#include "Memory.h"

Memory::Memory(int s){
    this->size = s;
    this->main = new unsigned char[s];

    for(unsigned long i = 0; i<size;i++){
        //populates memory
        main[i] = i%255;
    }
}
unsigned char Memory::getByte(unsigned long addy){
    //returns byte at given address within the memory array
    return main[addy];
}
int Memory::getSize(){
    return this->size;
}
void Memory::setByte(unsigned long addy, unsigned char newVal){
    //sets byte at given address to given new value
    main[addy] = newVal;
}
void Memory::display(){
    printf("MAIN MEMORY\n");
    for(int i=0;i<size;i++){
        
        if(i!= 0 && i%16 ==0){
            printf("\n");
        }
        printf("%02x",main[i]);
        printf(" ");
    }
    printf("\n");
}