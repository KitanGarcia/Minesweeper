#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "mine_functions.h"

void start_game(int rows, int cols, int mines, int seed, int param){
	Board* minesweep = malloc(sizeof(Board)); //creates pointer to game state
	int r = 0, c = 0;
	char* board = malloc(sizeof(char)*rows*cols);

	int temp_mines = mines;
	//int end_game = 0; //flag for end of game

	if(param == 1){
		printf("Not enough arguments. Usage:\n./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
		return;
	} //not enough parameters
	else if(param == 2){
		printf("Too many arguments. Usage:\n./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
		return;
	} //too many parameters

	minesweep->rows = rows;
	minesweep->cols = cols;
	minesweep->mines = mines;
	minesweep->board = board;
	minesweep->end_game = 0; //checks if end of game
	minesweep->won_game = 0; //checks if game is won
	minesweep->ask_again = 1; //sets to true
	minesweep->count = 0; //sets count to 0

	if(seed!=0){
		srand(seed);
		//printf("Went here\n");
	}
	else{
		//printf("Went here\n");
		//time_t mytime;
		//mytime = time(NULL);
		int t = (int)(time(NULL));
		//printf("%d\n", t);
		srand(t);
	} //sets seed for random either based of user or time

	for(r=0; r<rows; r++){
		for(c=0; c<cols; c++){
			minesweep->board[cols * r + c] = '#';
		}
	} //fill the board with #s

	while(temp_mines!=0){
		r = rand() % rows;
		//printf("\nr = %d\n", r);
		c = rand() % cols;
		//printf("c = %d\n\n", c);

		if(minesweep->board[cols * (rows - r - 1) + c]!='*'){
			minesweep->board[cols * (rows - r - 1) + c] = '*'; //set mines
			temp_mines--;
			printf("Placing mine at %d, %d\n", r, c);
		}
		
	} //places mines

	//printf("Went here\n");

	while(minesweep->end_game!=1){
		//printf("Went here\n");
		print_board(minesweep);
		player_move(minesweep);
		check_won(minesweep);
	} //plays game
	
	if((minesweep->end_game==1) && (minesweep->won_game == 0)){
		print_board(minesweep);
		printf("You Lost :(\n");
	}

	else if(minesweep->won_game==1){
		print_board(minesweep);
		printf("You Won!!\n");
	}
	//printf("\n%d %d %d %d\n", rows, cols, mines, seed);

}

void check_won(Board* minesweep){
	int r = 0, c = 0;
	int rows = minesweep->rows;
	int cols = minesweep->cols;

	minesweep->won_game = 1; //game set to won (below sets back to 0 if not won)

	for(r=0; r < rows; r++){	
		for(c=0; c < cols; c++){
			if((minesweep->board[cols * r + c]=='#') || (minesweep->board[cols * r + c]=='!') || (minesweep->board[cols * r + c]=='?') || (minesweep->board[cols * r + c]=='^') || (minesweep->board[cols * r + c]=='*')){
				minesweep->won_game = 0;
				//printf("LOST GAME\n");
			}
		}
	}

	if(minesweep->won_game==1){
		minesweep->end_game = 1;
	}
}//check is game is won

void player_move(Board* minesweep){
	int rows = minesweep->rows;
	int cols = minesweep->cols;

	char input[100]; //input 
	char* token = NULL; //token string for whitespace
	int r = rows, c = cols; //set to smthg so can enter while loop below

	while( minesweep->ask_again == 1 ){
			printf("Enter a row between 0-%d and a column between 0-%d: ", minesweep->rows-1, minesweep->cols-1);
			fgets(input, 100, stdin);
			token = strtok(input, " ");
			r = atoi(token);
			token = strtok(NULL, " ");
			c = atoi(token); //takes in row and column value
			//printf("%d\n%d\n", r, c);
			while( ((r >= rows) || (c >= cols) || (r < 0) || (c < 0)) || ((minesweep->count == 0) && isdigit(minesweep->board[cols * (rows - r -1) + c])) ){
				if((minesweep->count == 0) && isdigit(minesweep->board[cols * (rows - r -1) + c]))
					printf("This tile is already revealed.\n");
				printf("Enter row a row between 0-%d and a column between 0-%d: ", minesweep->rows-1, minesweep->cols-1);
				fgets(input, 100, stdin);
				token = strtok(input, " ");
				r = atoi(token);
				token = strtok(NULL, " ");
				c = atoi(token); //takes in row and column value
			}
			minesweep->ask_again = menu_options(minesweep, r, c);

			while ( getchar() != '\n' );//clears buffer
	} //keeps asking for input until valid row and column

	minesweep->ask_again = 1; //reset ask_again
}

int menu_options(Board* minesweep, int r_chosen, int c_chosen){
	int rows = minesweep->rows;
	int cols = minesweep->cols;
	int action = 0;
	char current_tile = '0'; //current tile

	r_chosen = rows - 1 - r_chosen; //rows are backwards so it sets it right
	current_tile = minesweep->board[cols * r_chosen + c_chosen]; //stores the current tile chosen

	printf("Enter Action\n");
	switch(current_tile){
		case '*':
		case '#':
			printf("0. Reveal\n");
			printf("1. Question\n");
			printf("2. Mark\n");
			printf("3. Cancel\n");
			printf("Action: ");
			scanf("%d", &action);
			if(action==0){
				reveal_tile(minesweep, r_chosen, c_chosen); //reveals tile
			}
			else if(action==1){
				if(minesweep->board[cols * r_chosen + c_chosen]=='*')
					minesweep->board[cols * r_chosen + c_chosen] = '^';
				else
					minesweep->board[cols * r_chosen + c_chosen] = '?';
			}
			else if(action==2){
				minesweep->mines--; //1 less mine left
				if(minesweep->board[cols * r_chosen + c_chosen]=='*')
					minesweep->board[cols * r_chosen + c_chosen] = '@';
				else
					minesweep->board[cols * r_chosen + c_chosen] = '!';
			}
			else{
				return 1; //ask user for input again
			} //any other number including 3
			break;
		case '@':
		case '!':
			printf("0. UnMark\n");
			printf("1. Cancel\n");
			printf("Action: ");
			scanf("%d", &action);
			if(action==0){
				minesweep->mines++; //add 1 more mine back
				if(minesweep->board[cols * r_chosen + c_chosen]=='@')
					minesweep->board[cols * r_chosen + c_chosen] = '*';
				else
					minesweep->board[cols * r_chosen + c_chosen] = '#';
			}
			if(action==1){
				return 1; //ask user for input again
			}
			break;
		case '^':
		case '?':
			printf("0. UnQuestion\n");
			printf("1. Cancel\n");
			printf("Action: ");
			scanf("%d", &action);
			if(action==0){
				if(minesweep->board[cols * r_chosen + c_chosen]=='^')
					minesweep->board[cols * r_chosen + c_chosen] = '*';
				else
					minesweep->board[cols * r_chosen + c_chosen] = '#';
			}
			if(action==1){
				return 1; //ask user for input again
			}
			break;
		//default :
	} //diff menu options

	return 0;
}

void reveal_tile(Board* minesweep, int r_chosen, int c_chosen){
	int rows = minesweep->rows;
	int cols = minesweep->cols;

	if(minesweep->board[cols * r_chosen + c_chosen]=='*'){
		minesweep->end_game = 1;
	}

	else{
		if(r_chosen==0){
			if(c_chosen==0){
				check_right(minesweep, r_chosen, c_chosen);
				check_below_right(minesweep, r_chosen, c_chosen);
				minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
				if(minesweep->count==0){
					if(minesweep->board[cols * r_chosen + c_chosen+1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen+1);
					if(minesweep->board[cols * (r_chosen+1) + c_chosen]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen);
					if(minesweep->board[cols * (r_chosen+1) + (c_chosen+1)]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen+1);
				}//check other tiles if count is 0
			}
			else if(c_chosen==cols-1){
				check_left(minesweep, r_chosen, c_chosen);
				check_below_left(minesweep, r_chosen, c_chosen);
				minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
				if(minesweep->count==0){
					if(minesweep->board[cols * r_chosen + c_chosen-1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen-1);
					if(minesweep->board[cols * (r_chosen+1) + c_chosen]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen);
					if(minesweep->board[cols * (r_chosen+1) + (c_chosen-1)]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen-1);
				}//check other tiles if count is 0
			}
			else{
				check_right(minesweep, r_chosen, c_chosen);
				check_left(minesweep, r_chosen, c_chosen);
				check_below(minesweep, r_chosen, c_chosen);
				minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
				if(minesweep->count==0){
					if(minesweep->board[cols * r_chosen + c_chosen-1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen-1);
					if(minesweep->board[cols * r_chosen + c_chosen+1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen+1);
					if(minesweep->board[cols * (r_chosen+1) + c_chosen]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen);
					if(minesweep->board[cols * (r_chosen+1) + (c_chosen-1)]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen-1);
					if(minesweep->board[cols * (r_chosen+1) + (c_chosen+1)]=='#')
						reveal_tile(minesweep, r_chosen+1, c_chosen+1);
				}//check other tiles if count is 0
			}
		}//if top row on board

		else if(r_chosen==rows-1){
			if(c_chosen==0){
				check_right(minesweep, r_chosen, c_chosen);
				check_above_right(minesweep, r_chosen, c_chosen);
				minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
				if(minesweep->count==0){
					if(minesweep->board[cols * r_chosen + c_chosen+1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen+1);
					if(minesweep->board[cols * (r_chosen-1) + c_chosen]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen);
					if(minesweep->board[cols * (r_chosen-1) + (c_chosen+1)]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen+1);
				}//check other tiles if count is 0
			}
			else if(c_chosen==cols-1){
				check_left(minesweep, r_chosen, c_chosen);
				check_above_left(minesweep, r_chosen, c_chosen);
				minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
				if(minesweep->count==0){
					if(minesweep->board[cols * r_chosen + c_chosen-1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen-1);
					if(minesweep->board[cols * (r_chosen-1) + c_chosen]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen);
					if(minesweep->board[cols * (r_chosen-1) + (c_chosen-1)]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen-1);
				}//check other tiles if count is 0
			}
			else{
				check_right(minesweep, r_chosen, c_chosen);
				check_left(minesweep, r_chosen, c_chosen);
				check_above(minesweep, r_chosen, c_chosen);
				minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
				if(minesweep->count==0){
					if(minesweep->board[cols * r_chosen + c_chosen-1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen-1);
					if(minesweep->board[cols * r_chosen + c_chosen+1]=='#')
						reveal_tile(minesweep, r_chosen, c_chosen+1);
					if(minesweep->board[cols * (r_chosen-1) + c_chosen]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen);
					if(minesweep->board[cols * (r_chosen-1) + (c_chosen-1)]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen-1);
					if(minesweep->board[cols * (r_chosen-1) + (c_chosen+1)]=='#')
						reveal_tile(minesweep, r_chosen-1, c_chosen+1);
				}//check other tiles if count is 0
			}
		}//if bottom row

		else if(c_chosen==0){
			check_above_right(minesweep, r_chosen, c_chosen);
			check_below_right(minesweep, r_chosen, c_chosen);
			check_right(minesweep, r_chosen, c_chosen);
			minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
			if(minesweep->count==0){
				if(minesweep->board[cols * r_chosen + c_chosen+1]=='#')
					reveal_tile(minesweep, r_chosen, c_chosen+1);
				if(minesweep->board[cols * (r_chosen-1) + c_chosen]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen);
				if(minesweep->board[cols * (r_chosen+1) + c_chosen]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen);
				if(minesweep->board[cols * (r_chosen-1) + (c_chosen+1)]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen+1);
				if(minesweep->board[cols * (r_chosen+1) + (c_chosen+1)]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen+1);
			}//check other tiles if count is 0
		}//if far left

		else if(c_chosen==cols-1){
			check_above_left(minesweep, r_chosen, c_chosen);
			check_below_left(minesweep, r_chosen, c_chosen);
			check_left(minesweep, r_chosen, c_chosen);
			minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
			if(minesweep->count==0){
				if(minesweep->board[cols * r_chosen + c_chosen+1]=='#')
					reveal_tile(minesweep, r_chosen, c_chosen+1);
				if(minesweep->board[cols * (r_chosen-1) + c_chosen]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen);
				if(minesweep->board[cols * (r_chosen+1) + c_chosen]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen);
				if(minesweep->board[cols * (r_chosen-1) + (c_chosen-1)]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen-1);
				if(minesweep->board[cols * (r_chosen+1) + (c_chosen-1)]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen-1);
			}//check other tiles if count is 0
		}//if far right
		
		else{
			check_above(minesweep, r_chosen, c_chosen);
			check_below(minesweep, r_chosen, c_chosen);
			check_left(minesweep, r_chosen, c_chosen);
			check_right(minesweep, r_chosen, c_chosen);
			minesweep->board[cols * r_chosen + c_chosen] = minesweep->count + 48; //sets tile to count of mines
			if(minesweep->count==0){
				//printf("this tile is zero\n");
				if(minesweep->board[cols * r_chosen + c_chosen+1]=='#') //all these check to make sure not already revealed 
					reveal_tile(minesweep, r_chosen, c_chosen+1);	
				if(minesweep->board[cols * r_chosen + c_chosen-1]=='#')
					reveal_tile(minesweep, r_chosen, c_chosen-1);
				if(minesweep->board[cols * (r_chosen + 1) + c_chosen]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen);
				if(minesweep->board[cols * (r_chosen - 1) + c_chosen]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen);
				if(minesweep->board[cols * (r_chosen - 1) + (c_chosen - 1)]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen-1);
				if(minesweep->board[cols * (r_chosen - 1) + (c_chosen + 1)]=='#')
					reveal_tile(minesweep, r_chosen-1, c_chosen+1);
				if(minesweep->board[cols * (r_chosen + 1) + (c_chosen - 1)]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen-1);
				if(minesweep->board[cols * (r_chosen + 1) + (c_chosen + 1)]=='#')
					reveal_tile(minesweep, r_chosen+1, c_chosen+1);
			}//check other tiles if count is 0
		} //if anywhere else on board

	}

	//printf("COUNT: %d\n", minesweep->count);
	minesweep->count = 0; //reset mine count

}

void check_above(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*(r_chosen-1) + c_chosen]=='*') || (minesweep->board[cols*(r_chosen-1) + c_chosen]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen-1) + (c_chosen - 1)]=='*') || (minesweep->board[cols*(r_chosen-1) + (c_chosen - 1)]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen-1) + (c_chosen + 1)]=='*') || (minesweep->board[cols*(r_chosen-1) + (c_chosen + 1)]=='@'))
		minesweep->count++;

} //checks above tiles

void check_below(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*(r_chosen+1) + c_chosen]=='*') || (minesweep->board[cols*(r_chosen+1) + c_chosen]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen+1) + (c_chosen - 1)]=='*') || (minesweep->board[cols*(r_chosen+1) + (c_chosen - 1)]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen+1) + (c_chosen + 1)]=='*') || (minesweep->board[cols*(r_chosen+1) + (c_chosen + 1)]=='@'))
		minesweep->count++;
} //checks below tiles

void check_left(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*r_chosen + (c_chosen-1)]=='*') || (minesweep->board[cols*r_chosen + (c_chosen-1)]=='@'))
		minesweep->count++;	

} //checks left tiles

void check_right(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*r_chosen + (c_chosen+1)]=='*') || (minesweep->board[cols*r_chosen + (c_chosen+1)]=='@'))
		minesweep->count++;
} //checks right tiles

void check_below_right(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*(r_chosen+1) + c_chosen]=='*') || (minesweep->board[cols*(r_chosen+1) + c_chosen]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen+1) + (c_chosen + 1)]=='*') || (minesweep->board[cols*(r_chosen+1) + (c_chosen + 1)]=='@'))
		minesweep->count++;	
}

void check_below_left(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*(r_chosen+1) + c_chosen]=='*') || (minesweep->board[cols*(r_chosen+1) + c_chosen]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen+1) + (c_chosen - 1)]=='*') || (minesweep->board[cols*(r_chosen+1) + (c_chosen - 1)]=='@'))
		minesweep->count++;	
	
}

void check_above_right(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*(r_chosen-1) + c_chosen]=='*') || (minesweep->board[cols*(r_chosen-1) + c_chosen]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen-1) + (c_chosen + 1)]=='*') || (minesweep->board[cols*(r_chosen-1) + (c_chosen + 1)]=='@'))
		minesweep->count++;	
	
}

void check_above_left(Board* minesweep, int r_chosen, int c_chosen){
	int cols = minesweep->cols;

	if((minesweep->board[cols*(r_chosen-1) + c_chosen]=='*') || (minesweep->board[cols*(r_chosen-1) + c_chosen]=='@'))
		minesweep->count++;
	if((minesweep->board[cols*(r_chosen-1) + (c_chosen - 1)]=='*') || (minesweep->board[cols*(r_chosen-1) + (c_chosen - 1)]=='@'))
		minesweep->count++;	
	
}

void print_board(Board* minesweep){
	int r = 0, c = 0;
	int rows = minesweep->rows;
	int cols = minesweep->cols;

	if(minesweep->end_game==1){
		for(r=0; r < rows; r++){
			printf("%d", rows-1-r);
			for(c=0; c < cols; c++){
				if((minesweep->board[cols * r + c]=='@') || (minesweep->board[cols * r + c]=='*'))
					printf(" *");
				else{
					reveal_tile(minesweep, r, c);
					printf(" %c", minesweep->board[cols * r + c]);
				}
			}
			printf("\n");	
		}
		printf(" ");
	    for(c = 0; c < cols; c++){
	            printf(" %d", c);
	    }
	    printf("\n");

	}

	else{
		//if(minesweep->mines < 0)
			//printf("There are 0 mines left\n"); //prints out how many mines are left
		//else
		printf("There are %d mines left\n", minesweep->mines); //prints out how many mines are left
		for(r=0; r < rows; r++){
			printf("%d", rows-1-r);
			for(c=0; c < cols; c++){
				if(minesweep->board[cols * r + c]=='@')
					printf(" !");
				else if(minesweep->board[cols * r + c]=='^')
					printf(" ?");
				else if(minesweep->board[cols * r + c]!='*')
					printf(" %c", minesweep->board[cols * r + c]); //only prints if not mine
				else
					printf(" #");
			}
		printf("\n");
		} //fill the board with #s

		printf(" ");
	    for(c = 0; c < cols; c++){
	            printf(" %d", c);
	    }
	    printf("\n");
	} //regular gameplay board output
} //prints boardgame out

