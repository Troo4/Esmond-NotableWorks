#include <stdio.h>
#include <math.h>
#include <string.h>
#define HEAP_SIZE (128*8)
#define MANAGED_LIST_SIZE 128
#define FIRST_FIT 1
#define BEST_FIT 0

void duInitMalloc(int m);
void* duMalloc(int size, int index);
void duFree(void* ptr);

unsigned char heap1[HEAP_SIZE];
unsigned char heap2[HEAP_SIZE];
unsigned char oldHeap[HEAP_SIZE];
unsigned char* heaps[3];
unsigned int active = 0;
unsigned int mode;
void* managedList[MANAGED_LIST_SIZE];
int mlSize;

typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 = free
    int size; // size of the reserved block
    int managedIndex; // the unchanging index in the managed array
    int survivalAmt; // the number of times the block has moved between young heaps
    struct memoryBlockHeader* next;  // the next block in the integrated free list
} memoryBlockHeader;

memoryBlockHeader* freeListHead1;
memoryBlockHeader* freeListHead2;
memoryBlockHeader* freeListHead3;
memoryBlockHeader* freeListHeads[3];
void duInitMalloc(int m){
    heaps[0] = heap1;
    heaps[1] = heap2;
    heaps[2] = oldHeap;
    for (int i = 0; i<HEAP_SIZE;i++){
        (heaps[0])[i] = 0;
        (heaps[1])[i] = 0;
        (heaps[2])[i] = 0;
    }
    memoryBlockHeader* current1 = (memoryBlockHeader*)heaps[0];
    memoryBlockHeader* current2 = (memoryBlockHeader*)heaps[1];
    memoryBlockHeader* current3 = (memoryBlockHeader*)heaps[2];

    current1 -> size = HEAP_SIZE - sizeof(memoryBlockHeader);
    current1 -> next = NULL;
    current1->free = 1;
    current1->survivalAmt = 0;
    freeListHead1 = current1;
    current2 -> size = HEAP_SIZE - sizeof(memoryBlockHeader);
    current2 -> next = NULL;
    current2->free = 1;
    current2->survivalAmt = 0;
    freeListHead2 = current2;
    current3 -> size = HEAP_SIZE - sizeof(memoryBlockHeader);
    current3 -> next = NULL;
    current3 -> free = 1;
    current3->survivalAmt = 0;
    freeListHead3 = current3;
    freeListHeads[0] = freeListHead1;
    freeListHeads[1] = freeListHead2;
    freeListHeads[2] = freeListHead3;
    mode = m;
}
void allMemoryDump(void* heap){
    memoryBlockHeader* current = heap;
    printf("Current heap (0/1 young): %d\n", active);
    int size = 0;
    char freeChar = 'a';
    char usedChar = 'A';
    char graphicalStr[128];
    while(size<1024){

        if(current->free){
            printf("Free at %p, size %d\n",current, current->size);
            for(int i=0;i<((current->size) + sizeof(memoryBlockHeader))/8;i++){
                graphicalStr[(size)/8 + i] = freeChar;
            }
            freeChar++;
        }
        else{
            printf("Used at %p, size %d, surv %d\n",current, current->size, current->survivalAmt);
            for(int i=0;i<((current->size) + sizeof(memoryBlockHeader))/8;i++){
                graphicalStr[(size)/8 + i] = usedChar;
            }
            usedChar++;
        }
        size += sizeof(memoryBlockHeader) + current->size;
        if(size<1024){
            current = (memoryBlockHeader*) (heap + size);
        }
        
        
    }
    for(int i=0;i<128;i++){
        printf("%c",graphicalStr[i]);
    }
    printf("\n");
}
void freeMemoryDump(void* freeListHead){
    printf("MEMORY DUMP\n");
    memoryBlockHeader* temp = freeListHead;

    printf("freelist\n");
    while(temp != NULL){
        printf("Block at %p, Size %d\n",temp, temp->size);
        temp = temp->next;
    }
}
void mlMemoryDump(){
    printf("MANAGED LIST\n");
    for(int i=0;i<mlSize;i++){
        printf("ManagedList[%d] = %p\n",i, managedList[i]);
    }
}
void duMemoryDump(){
    allMemoryDump(heaps[active]);
    freeMemoryDump(freeListHeads[active]);
    printf("OLD HEAP\n");
    allMemoryDump(heaps[2]);
    freeMemoryDump(freeListHeads[2]);
    mlMemoryDump();
}
void* duMalloc(int num, int index){
    memoryBlockHeader* freeListHead = freeListHeads[index];
    int min;
    if (num%8 !=0){
        min = num + (8 - num%8);
    }else{
        min = num;
    }
    if(mode==FIRST_FIT){
        memoryBlockHeader* temp = freeListHead;
        memoryBlockHeader* prev = NULL;
        while(temp != NULL){
        if(temp->size >= min + sizeof(memoryBlockHeader)){
                
                memoryBlockHeader* used = temp;
                int bruh = temp->size;
                used->size = min;
                memoryBlockHeader* new = (memoryBlockHeader*)((unsigned char*)used + min + sizeof(memoryBlockHeader));
                
                new->size = bruh - min - sizeof(memoryBlockHeader);
            
                if(prev == NULL){
                    freeListHead = new;
                    freeListHeads[index] = freeListHead;
                    new->next = temp->next;
                } else{
                    prev->next = new;
                    new->next = temp->next;
                }
                used->free = 0;
                new->free = 1;
                return (void*)((unsigned char*)used + sizeof(memoryBlockHeader));
            }
            prev = temp;
            temp = temp->next;
        }
    }
    else{
        memoryBlockHeader* temp = freeListHead;
        memoryBlockHeader* prev = NULL;
        memoryBlockHeader* best = NULL;
        memoryBlockHeader* bestPrev = NULL;
        int diff = 1024; //default to sufficiently high ## such that any viable free block will have less of a difference
        while(temp != NULL){
            if(temp->size >= min + sizeof(memoryBlockHeader)){
                int tempDif = temp->size - (min + sizeof(memoryBlockHeader));
                printf("DIFS %d\n",tempDif);
                if(tempDif<diff){
                    best = temp;
                    diff = tempDif;
                    bestPrev = prev;
                    printf("BEST SIZE %d\n",diff);
                }
            }
            prev = temp;
            temp = temp->next;
        }
        if(best != NULL){
            memoryBlockHeader* used = best;
            int bruh = best->size;
            used->size = min;
            memoryBlockHeader* new = (memoryBlockHeader*)((unsigned char*)used + min + sizeof(memoryBlockHeader));
                
            new->size = bruh - min - sizeof(memoryBlockHeader);
            
            if(bestPrev == NULL){
                freeListHead = new;
                freeListHeads[index] = freeListHead;
                new->next = best->next;
            } else{
                bestPrev->next = new;
                new->next = best->next;
            }
            used->free = 0;
            new->free = 1;
            return (void*)((unsigned char*)used + sizeof(memoryBlockHeader));
        }
    }
    
    
    return NULL;
}
void duFree(void* tar){
    memoryBlockHeader* freeListHead;
    int um = 0; //keeps track of which heap we are freeing from
    if(tar >= ((void*)(heaps[2])) && tar < ((void*)((heaps[2]) + HEAP_SIZE))){
        // printf("HERE!!\n");
        um = 2;
        freeListHead = freeListHeads[2];
    } else{
        um = active;
        freeListHead = freeListHeads[active];
    }
    
    memoryBlockHeader* blockHeader = (memoryBlockHeader*)((unsigned char*)tar - sizeof(memoryBlockHeader));
    memoryBlockHeader* prev = NULL;
    memoryBlockHeader* temp = freeListHead;
    while(temp != NULL && temp < blockHeader){
        prev = temp;
        temp = temp->next;
    }
    if(prev == NULL){
        freeListHead = blockHeader;
        blockHeader->next = temp;
    } else{
        prev->next = blockHeader;
        blockHeader->next = temp;
    }
    blockHeader->survivalAmt = 0;
    blockHeader->free = 1;
    freeListHeads[um] = freeListHead;
}
void duManagedInitMalloc(int searchType){
    duInitMalloc(searchType);
    mlSize = 0;
}
void** duManagedMalloc(int size){
    void* entry = duMalloc(size, active);
    memoryBlockHeader* header = entry - sizeof(memoryBlockHeader);
    header->managedIndex = mlSize;
    managedList[mlSize] = entry;
    mlSize++;

    return &(managedList[mlSize-1]);

}
void duManagedFree(void** mptr){
    duFree(*mptr);
    *mptr = NULL;
}
void minorCollection(){
    //memcpy(pointerToMoveTo, pointerToFromFrom, numberOfBytesToMove);
    int size = 0;
    for(int i=0;i<mlSize;i++){
        if(managedList[i] != NULL){
            memoryBlockHeader* tempHeader = (memoryBlockHeader*)(managedList[i] - sizeof(memoryBlockHeader));
            
            // printf("here1\n");
            if((tempHeader->survivalAmt)+1 < 3){
                // printf("here2\n");
                unsigned int sizeToMove = (tempHeader -> size) + sizeof(memoryBlockHeader);
                // printf("here3\n");
                memoryBlockHeader* newLoc = (memoryBlockHeader*)memcpy((heaps[!active]) + size,tempHeader, sizeToMove);
                // printf("here4\n");
                (newLoc->survivalAmt)++;
                managedList[i] = ((void*)newLoc + sizeof(memoryBlockHeader));
                // printf("here5\n");
                size += sizeToMove;
            }
            else if((tempHeader->survivalAmt)+1 == 3){
                unsigned int sizeToMove = tempHeader -> size;
                void* dest = duMalloc(sizeToMove, 2) - sizeof(memoryBlockHeader);
                memoryBlockHeader* newLoc = (memoryBlockHeader*)memcpy(dest,tempHeader, sizeToMove + sizeof(memoryBlockHeader));
                (newLoc->survivalAmt)++;
                managedList[i] = ((void*)newLoc + sizeof(memoryBlockHeader));
            }
            
        }
    }
    memoryBlockHeader* newFree = (memoryBlockHeader*)((heaps[!active])+size);
    newFree->size = HEAP_SIZE - size - sizeof(memoryBlockHeader);
    newFree->next = NULL;
    newFree->free = 1;
    freeListHeads[!active] = newFree;
    active = !active;
}
void majorCollection(){
    //update managed list
    //update free list
    //defrags old heap by compressing it, walking through the list and removing free space
    
    int size = ((freeListHeads[2])->size) + sizeof(memoryBlockHeader) + (((void*)freeListHeads[2]) - ((void*)heaps[2])); //finds the total size of the free block + the size of the used blocks before it
    memoryBlockHeader* current = (memoryBlockHeader*)((heaps[2]) + size); //first block after first free block
    memoryBlockHeader* prevFree = freeListHeads[2];
    while(size < 1024){
        if(current->free){
            prevFree->next = current->next;
            prevFree->size += (current->size) + sizeof(memoryBlockHeader);
            size += (current->size) + sizeof(memoryBlockHeader);
            current = (memoryBlockHeader*)(size + heaps[2]);
        }
        else{
            int sizeToMove = (current->size) + sizeof(memoryBlockHeader);
            int tempSize = prevFree->size;
            memoryBlockHeader* tempNext = prevFree->next;
            memoryBlockHeader* newLoc = (memoryBlockHeader*)memcpy(prevFree, current, sizeToMove);
            memoryBlockHeader* newFree = ((void*)prevFree) + (newLoc-> size) + sizeof(memoryBlockHeader);
            newFree->size = tempSize;
            newFree->next = tempNext;
            newFree->free = 1;
            newFree->survivalAmt = 0;
            freeListHeads[2] = newFree;
            prevFree = newFree;
            managedList[newLoc->managedIndex] = (void*)newLoc + sizeof(memoryBlockHeader);
            size += (newLoc->size) + sizeof(memoryBlockHeader);
            current = (memoryBlockHeader*)(size + heaps[2]);
        }

    }
}