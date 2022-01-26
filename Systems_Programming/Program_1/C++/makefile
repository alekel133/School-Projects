ReverseCPP.out: Main.o List.o
	g++  Main.o List.o -o ReverseCPP.out

list.o: List.cpp List.h Node.h
	g++ -c List.o

main.o: Main.cpp List.h
	g++ -c Main.cpp

clean:
	rm *.o
