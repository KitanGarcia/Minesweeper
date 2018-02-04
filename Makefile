mine_sweeper.out: main.o mine_functions.o
	gcc main.o mine_functions.o -o mine_sweeper.out

main.o: main.c mine_functions.h
	gcc -Wall -g -c main.c

mine_functions.o: mine_functions.c mine_functions.h
	gcc -Wall -g -c mine_functions.c

clean:
	rm *.o mine_sweeper.out
