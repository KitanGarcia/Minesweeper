#ifndef mine_functions
	#define mine_functions

typedef struct{
	int rows, cols, mines, end_game, ask_again, count, won_game;
	char* board;


} Board;

void start_game(int rows, int cols, int mines, int seed, int param); //starts game (only called once)
void print_board(Board* minesweep); //prints out board
void player_move(Board* minesweep); //deals with players move
int menu_options(Board* minesweep, int r_chosen, int c_chosen); //prints menu/options
void reveal_tile(Board* minesweep, int r_chosen, int c_chosen); //reveals tile
void check_above(Board* minesweep, int r_chosen, int c_chosen); 
void check_below(Board* minesweep, int r_chosen, int c_chosen);
void check_left(Board* minesweep, int r_chosen, int c_chosen); 
void check_right(Board* minesweep, int r_chosen, int c_chosen);
void check_below_right(Board* minesweep, int r_chosen, int c_chosen);
void check_below_left(Board* minesweep, int r_chosen, int c_chosen);
void check_above_right(Board* minesweep, int r_chosen, int c_chosen);
void check_above_left(Board* minesweep, int r_chosen, int c_chosen);
void check_won(Board* minesweep);

#endif
