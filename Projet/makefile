all: atelier

atelier: atelier.o main.o superviseur.o
	gcc -o atelier atelier.o main.o superviseur.o -pthread

atelier.o : atelier.c
	gcc -o atelier.o -c atelier.c -pthread

IHM.o : IHM.c
	gcc -o IHM.o -c IHM.c -pthread

superviseur.o : superviseur.c
	gcc -o superviseur.o -c superviseur.c -pthread

main.o: main.c atelier.h IHM.h superviseur.h
	gcc -o main.o -c main.c -pthread

clean:
	rm -rf *.o

mrproper: clean
	rm -rf atelier
