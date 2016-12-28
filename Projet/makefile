all: atelier

atelier : ./object/machine.o ./object/main.o ./object/superviseur.o ./object/robot.o ./object/convoyeur.o ./object/IHM.o
	gcc -o atelier ./object/machine.o ./object/main.o ./object/superviseur.o ./object/robot.o ./object/convoyeur.o ./object/IHM.o -pthread

./object/machine.o : ./src/machine.c
	gcc -o ./object/machine.o -c ./src/machine.c -pthread

./object/convoyeur.o : ./src/robot.c
	gcc -o ./object/convoyeur.o -c ./src/convoyeur.c -pthread

./object/robot.o : ./src/robot.c
	gcc -o ./object/robot.o -c ./src/robot.c -pthread

./object/IHM.o : ./src/IHM.c
	gcc -o ./object/IHM.o -c ./src/IHM.c -pthread

./object/superviseur.o : ./src/superviseur.c
	gcc -o ./object/superviseur.o -c ./src/superviseur.c -pthread

./object/main.o: ./src/main.c ./header/machine.h ./header/robot.h ./header/superviseur.h
	gcc -o ./object/main.o -c ./src/main.c -pthread

clean:
	rm -rf ./object/*.o

mrproper: clean
	rm -rf atelier

run : atelier
		@echo "\n Run the executable"
		make mrproper
		make
		./atelier
