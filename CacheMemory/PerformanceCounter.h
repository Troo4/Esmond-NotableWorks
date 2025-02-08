#include <stdio.h>


#ifndef PERFORMANCECOUNTER_H
#define PERFORMANCECOUNTER_H

class PerformanceCounter{
    private:
        int accesses;
        int hits;
        int misses;
        int writebacks;
        
    public:
        PerformanceCounter();
        //setters
        void hit();
        void miss();
        void writeback();
        //getters
        int getAccesses();
        int getHits();
        int getMisses();
        int getWritebacks();
        double getMissperc();
        double getWritebackperc();

        void display();


        

        
};
#endif