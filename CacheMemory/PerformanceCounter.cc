#include <stdio.h>
#include "PerformanceCounter.h"

PerformanceCounter::PerformanceCounter(){
    this->accesses = 0;
    this->hits = 0;
    this->misses = 0;
    this->writebacks = 0;
}
void PerformanceCounter::hit(){
    this->hits++;
    this->accesses++;
}
void PerformanceCounter::miss(){
    this->misses++;
    this->accesses++;
}
void PerformanceCounter::writeback(){
    this->writebacks++;
}
int PerformanceCounter::getAccesses(){
    return this->accesses;
}
int PerformanceCounter::getHits(){
    return this->hits;
}
int PerformanceCounter::getMisses(){
    return this->misses;
}
int PerformanceCounter::getWritebacks(){
    return this->writebacks;
}
double PerformanceCounter::getMissperc(){
    return ((double)this->misses/(double)this->accesses)*100.0;
}
double PerformanceCounter::getWritebackperc(){
    return ((double)this->writebacks/(double)this->accesses)*100.0;
}
void PerformanceCounter::display(){
    printf("Accesses: %d\n", this->accesses);
    printf("Hits: %d\n", this->hits);
    printf("Misses: %d\n", this->misses);
    printf("Writebacks: %d\n", this->writebacks);
    printf("Miss Percentage: %.2f\n", this->getMissperc());
    printf("Writeback Percentage: %.2f\n", this->getWritebackperc());
}

