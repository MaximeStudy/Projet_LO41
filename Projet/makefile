all: atelier

atelier : ./object/machine.o ./object/main.o ./object/superviseur.o
	gcc -o atelier ./object/machine.o ./object/main.o ./object/superviseur.o -pthread

./object/machine.o : ./src/machine.c
	gcc -o ./object/machine.o -c ./src/machine.c -pthread

./object/IHM.o : ./src/IHM.c
	gcc -o ./object/IHM.o -c ./src/IHM.c -pthread

./object/superviseur.o : ./src/superviseur.c
	gcc -o ./object/superviseur.o -c ./src/superviseur.c -pthread

./object/main.o: ./src/main.c ./header/machine.h ./header/IHM.h ./header/superviseur.h
	gcc -o ./object/main.o -c ./src/main.c -pthread

clean:
	rm -rf ./object/*.o

mrproper: clean
	rm -rf atelier
