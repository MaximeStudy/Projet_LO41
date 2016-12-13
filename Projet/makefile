all: atelier

atelier: atelier.o main.o
	gcc -o atelier atelier.o main.o -pthread

atelier.o : atelier.c
	gcc -o atelier.o -c atelier.c -pthread

main.o: main.c atelier.h
		gcc -o main.o -c main.c -pthread

clean:
	rm -rf *.o

mrproper: clean
	rm -rf atelier
