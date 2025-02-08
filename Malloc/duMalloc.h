#ifndef DUMALLOC_H
#define DUMALLOC_H

// The interface for DU malloc and free


void duMemoryDump();

void duManagedInitMalloc(int searchType);
void** duManagedMalloc(int size);
void duManagedFree(void** mptr);
void minorCollection();
void majorCollection();
#endif