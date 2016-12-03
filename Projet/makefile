all: atelier

atelier: atelier.o main.o
	gcc -o atelier atelier.o main.o

atelier.o : atelier.c
	gcc -o atelier.o -c atelier.c

main.o: main.c atelier.h
		gcc -o main.o -c main.c

clean:
	rm -rf *.o

mrproper: clean
	rm -rf atelier
