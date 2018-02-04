#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mine_functions.h"

int main(int argc, char *argv[]) {
	int rows = 0, cols = 0, mines = 0, seed = 0; //define command line parameters

	if(argc < 5){
		start_game(rows, cols, mines, seed, 1);
		return 0;
	} // too few parameters
	else if(argc > 5){
		start_game(rows, cols, mines, seed, 2);
		return 0;
	} // too many parameters
	rows = atoi(argv[1]);
	cols = atoi(argv[2]);
	mines = atoi(argv[3]);
	//if(argv[4] != NULL)
	seed = atoi(argv[4]);

	start_game(rows, cols, mines, seed, 0);

	return 0;
}
