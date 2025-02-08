project2: Memory.o AddressDecoder.o PerformanceCounter.o Cache.o Set.o Block.o Simulation.o
	g++ Memory.o AddressDecoder.o PerformanceCounter.o Cache.o Set.o Block.o Simulation.o -o proj2
Memory.o: Memory.cc Memory.h
	g++ Memory.cc -c
Cache.o: Cache.cc Cache.h Memory.h Set.h AddressDecoder.h PerformanceCounter.h
	g++ Cache.cc -c
Set.o: Set.cc Set.h Memory.h AddressDecoder.h PerformanceCounter.h Block.h
	g++ Set.cc -c
Block.o: Block.cc Block.h Memory.h
	g++ Block.cc -c
AddressDecoder.o: AddressDecoder.cc AddressDecoder.h
	g++ AddressDecoder.cc -c
PerformanceCounter.o: PerformanceCounter.cc PerformanceCounter.h
	g++ PerformanceCounter.cc -c
Simulation.o: Simulation.cc Memory.h
	g++ Simulation.cc -c

clean:c
	rm -f *.o
	rm -f proj2