hex: main.o HexaBit.o
	g++ -o hex main.o HexaBit.o

main.o: HexaBit.h main.cpp
	g++ -c main.cpp

HexaBit.o: HexaBit.h HexaBit.cpp
	g++ -c HexaBit.cpp

clean:
	rm -f *.o hex
