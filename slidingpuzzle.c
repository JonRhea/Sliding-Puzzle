#include <stdio.h>
int isMoveValid(int tile);
void createGame();
void printBoard();
void moveTile(int tile);
void endGame();

#define boardSize 4
//The highest number in the matrix will always be the rows times the columns, minus 1.
int maxNumbers = (boardSize * boardSize) - 1;
int board[boardSize][boardSize];

/*
* Starts the program, containts the loop, and terminates the program
* @return 0 The program has ended.
*/
int main(){
	
	printf("Setting up the game.\n");
	createGame();
	
	//where the user's input will be stored
	char choice;
	
	/*Three options for input:
	* 'p' - Prints the current board
	* 'm' - User is prompted to enter a tile to move, then moves that tile
	* 'q' - Quits the game (terminates the program)
	* Any other option resutls in an error message and an option to try again.*/
	while(choice != 'q'){
		printf("\nSelect and option:\np - Print the game state\nm - Move a tile\nq - Quit the game\n>");
		scanf("%s" , &choice);//get input from the user
		
		if(choice == 'p'){
			printBoard(board);
		}//end if
		else if(choice == 'm'){
			//the tile the user entered to be moved, may or may not be legal
			int tile;
			printf("Enter a tile to move.\n>");
			scanf("%d", &tile);
			if(isMoveValid(tile) == 1){
				moveTile(tile);
			}//end if
		}//end if
		else if(choice == 'q'){
			endGame();
		}//end if	
		else {
			printf("Unknown command. Please try again.\n");
		}//end else
	}//end while
		
	return 0;
}//end main

/*
* Creates the board using the boardSize value
* As of part 1, the board will be inversly sorted
*/
void createGame(){
	int numberCounter = maxNumbers;
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			board[i][j] = numberCounter;
			--numberCounter;
		}//end for
	}//end for
	
}//end createGame

/*
* Prints the current state of the board
* @param board[][boardSize]: The current board
*/
void printBoard(int board[][boardSize]){
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			if(board[i][j] != 0)
				printf("%3d ", board[i][j]);
			else
				//this 3 space print will create the free space in the printed board
				printf("    ");

			//print a new line whenever a row is finished printing
			if(j == boardSize - 1){
				printf("\n");
			}//end if
		}//end for
	}//end for
	
}//end printBoard

/*
* Checks whether a tile can be moved (if a zero is next to it)
* @param tile: tile number to move
* @return 1 if tile is moveable, 0 if not
*/
int isMoveValid(int tile){
	//check if free tile was entered
	if(tile == 0){
		printf("The free tile cannot be moved!\n");
		return 0;
	}//end if
	
	//check if a tile that does not exist was entered.
	if((tile < 0)){
		printf("Negative numbers cannot be used in this game.\n");
		return 0;
	}//end if
	if(tile > maxNumbers){
		printf("Tile does not exist.\n");
		return 0;
	}//end if
	
	//loop through the board (2D array)
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			if(board[i][j] == tile){
				
				//check to see if the above tile is the free tile
				//if tile is in the top row (i = 0), do not check above 
				if((i - 1) >= 0){ 
					if(board[i - 1][j] == 0)
						return 1;
				}//end if
				
				//check to see if the right tile is the free tile
				//if tile is in the right most column (j = boardSize - 1), do not check right 
				if((j + 1) < boardSize){ 
					if(board[i][j + 1] == 0)
						return 1;
				}//end if
				
				//check to see if the below tile is the free tile
				//if tile is in the bottom row (i = boardSize - 1), do not check below 
				if((i + 1) < boardSize){
					if(board[i + 1][j] == 0)
						return 1;
				}//end if
				
				//check to see if the left tile is the free tile
				//if tile is in the left most column (j = 0), do not check left 
				if((j - 1) >= 0){
					if(board[i][j - 1] == 0)
						return 1;
				}//end if
				
				//if all four checks fail, the tile is not moveable.
				printf("The tile cannot be moved.\n");
				return 0;
			}//end if		
		}//end for
	}//end for
}//end isMoveValid

/*
* Moves a tile to the free spot (aka 0)
* @param tile: The tile to be moved
*/
void moveTile(int tile){
	//initialize the placeholder for the row and column of the free space
	int zeroTileRow;
	int zeroTileColumn;
	
	//initialize the placeholder for the row and column of the tile
	int moveTileRow;
	int moveTileColumn;
	
	//loop through the board (2D array)
	for(int i = 0; i < boardSize; i++){
		for(int j = 0; j < boardSize; j++){
			//get coordinates of the tile to be moved
			if(board[i][j] == tile){
				moveTileRow = i;
				moveTileColumn = j;
			}//end if
			
			//get coordinates of the empty space
			if(board[i][j] == 0){
				zeroTileRow = i;
				zeroTileColumn = j;
			}//end if
		}//end for
	}//end for

	//put the empty space where the tile is
	board[moveTileRow][moveTileColumn] = 0;
	//put the tile where the empty space is
	board[zeroTileRow][zeroTileColumn] = tile;
	
	printf("The tile has been moved successfully.\n");
}//end if

/*
* Prints a message that the game is in the process of teardown
*/
void endGame(){
	printf("Ending the game.\n");
}//end endGame


