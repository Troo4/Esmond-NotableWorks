
// Simple C++ version of Cache Memory Simulator
// All classes are used like Java does their classes
// Arrays (not Vectors) are the only data structure and they
// only hold pointers to objects rather than the objects themselves
// No operator overloading, inheritance, or objects on the runtime stack
// Basically, it is as simple a C++ setup as possible
#include <stdio.h>
#include "Memory.h"
#include "Cache.h"
void testNwayDetail() {
    // This is a test the test an N-way associative cache
    // It is a full test that will test all the features of the cache
    // First reading Misses and Hits
    // Then LRU replacement policy
    // Then writes that are hits and misses
    // Then reads/writes that have to do writebacks
    // It is meant to make sure the code is working correctly
    // It is not meant to be a performance test
    printf("-------------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------\n");
    printf("-------------------------------------------------------------------\n");
    printf("Cache Simulator Nway Detailed Test\n");
    // My main memory has 32 bytes
    Memory* m = new Memory(32);
    // Cache with 16 bytes, 4 bytes per block, 2-way associative
    Cache* c = new Cache(m, 16, 4, 2);
    m->display();
    c->display();
    // First check that the reads work
    // The first read is always a miss
    // Miss in set 0
    printf("-------------------------------------------------------------------\n");
    printf("READ 0\n");
    printf("%d\n", c->read(0));
    m->display();
    c->display();
    // Next check a cache hit
    // Hit in set 0
    printf("-------------------------------------------------------------------\n");
    printf("READ 2\n");
    printf("%d\n", c->read(2));
    m->display();
    c->display();
    // Now do 3 more reads to fill up the cache
    // This is read miss one
    // Miss in set 1
    printf("-------------------------------------------------------------------\n");
    printf("READ 4\n");
    printf("%d\n", c->read(4));
    m->display();
    c->display();
    // This is read miss two
    // This also checks that it will read in the block
    // starting at 8 rather than 10 since 8-11 includes 10
    // Miss in set 0
    printf("-------------------------------------------------------------------\n");
    printf("READ 10\n");
    printf("%d\n", c->read(10));
    m->display();
    c->display();
    // This is read miss three
    // Miss in set 1
    printf("-------------------------------------------------------------------\n");
    printf("READ 12\n");
    printf("%d\n", c->read(12));
    m->display();
    c->display();
    // At this point both blocks are filled in both sets
    // So the next miss will have to replace a block
    // This is to change the LRU block in set 0 from block 0 to block 1
    // It will impact what happens in the next read below
    printf("-------------------------------------------------------------------\n");
    printf("READ 1\n");
    printf("%d\n", c->read(1));
    m->display();
    c->display();
    // This replaces the block at 1 of set 0 since it is LRU
    printf("-------------------------------------------------------------------\n");
    printf("READ 16\n");
    printf("%d\n", c->read(16));
    m->display();
    c->display();
    // Now check that the writes work
    // First write to something that is already in the cache
    // This should change the value from 0x06 to 0x66
    // This should also mark the block as dirty
    printf("-------------------------------------------------------------------\n");
    printf("WRITE 6\n");
    c->write(6, 0x66);
    m->display();
    c->display();
    // Next write to something that is not in the cache and has
    // to be loaded but the evicted is not dirty
    // Should load in the block starting at 20
    // Should replace the block at 1 of set 1 (since it is LRU)
    // Should change the value from 0x15 to 0x55
    printf("-------------------------------------------------------------------\n");
    printf("WRITE 21\n");
    c->write(21, 0x55);
    m->display();
    c->display();
    // Finally write to something that is no in the cache and has
    // to be loaded and the evicted block is dirty
    // Note that at this point both blocks in set 1 are dirty
    // so any read or write that loads into set 1 will evict a dirty block
    // We will do a read first that does a writeback
    // This should evict block 0 of set 1
    printf("-------------------------------------------------------------------\n");
    printf("READ 12\n");
    c->read(12);
    m->display();
    c->display();
    // And finally a write that does a writeback
    // This should evict block 1 of set 1 since it is now LRU
    // Changes 1e to ee
    printf("-------------------------------------------------------------------\n");
    printf("WRITE 30\n");
    c->write(30, 0xEE);
    m->display();
    c->display();
    // Note that when it is complete there is still a dirty block
    // We could write it back to the memory at this point
    // But since the program is about to end, memory will be not
    // used again so there is no need to writeback the cache to it
}
int main() {
    // Correctness test
    testNwayDetail();
    // Performance tests go here...
}
