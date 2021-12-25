#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int gameInterface();
void printBoardFromServer();
void readReturnValue();

extern int clientToServer[2];
extern int serverToClient[2];

enum command {cmd_pnt, cmd_move, cmd_quit, cmd_new, cmd_save, cmd_load};

/*
* Prints out UI of game.
* User inputs are sent to server for interpretation via enum
*/
int gameInterface(){
	//set up game and print board
	readReturnValue();
	printBoardFromServer();
	readReturnValue();
	
	
	//where the user's input will be stored
	char choice;
	
	//this is used to check if the board size is between 2 and 10 for option 'n'
	int okaySize;
	
	//the size of a new board entered by the user in option 'n'
	int newSize;
	
	//use for default case, so that client does not attempt to read returnValueOfGameWon before cmd is sent to client
	int goodCmd = 0;

	char filename[12] = "savefile.txt";
	
	enum command clientCmd;
	
	/*Six optipons for input:
	* 'p' - Prints the current board
	* 'm' - User is prompted to enter a tile to move, then moves that tile
	* 'q' - Quits the game (terminates the program)
	* 'n' - Prompts the user for a new board size, then creates a new board
	* 's' - Saves the current board to savefile.txt
	* 'l' - Loads a board from savefile.txt
	* Any other option resutls in an error message and an option to try again.*/
	while(choice != 'q'){

	
		//will be 1 if default (unknown command) is hit
		if(goodCmd == 0){
			//see if game has been won
			int returnValueOfGameWon;
			read(serverToClient[0], &returnValueOfGameWon, sizeof(int));
			if(returnValueOfGameWon == 1){
				printf("\n**You won the game!**\n");
				return 0;
			}//end if
			else{
				//do nothing
			}//end else
		}//end if
		//set goodCmd to 0 incase default was hit
		goodCmd = 0;
		
		printf("\nSelect and option:\np - Print the game state\nm - Move a tile\nq - Quit the game\nn - Create a new board\ns - Save the board\nl - Load a game from savefile\n>");
		scanf("%s" , &choice);//get input from the user
		
		switch(choice){
			case 'p':
				clientCmd = cmd_pnt;
				write(clientToServer[1], &clientCmd, sizeof(cmd_pnt));
				printBoardFromServer();
				readReturnValue();
			break;
			case 'm':
				clientCmd = cmd_move;
				write(clientToServer[1], &clientCmd, sizeof(cmd_move));
				printf("Enter a tile to move.\n>");
				int tile;
				scanf("%d", &tile);
				write(clientToServer[1], &tile, sizeof(int));
				readReturnValue();
			break;
			case 'q':
				clientCmd = cmd_quit;
				printf("Ending the game.\n");
				write(clientToServer[1], &clientCmd, sizeof(cmd_quit));
			break;
			case 'n':
				clientCmd = cmd_new;
				write(clientToServer[1], &clientCmd, sizeof(cmd_new));
				okaySize = 0;
				while(okaySize == 0){
					printf("Enter the size of the board (between 2 and 10).\n>");
					scanf("%d", &newSize);
					
					if(newSize < 2 || newSize > 10){
						printf("Board size must be between 2 and 10. Please try again.\n\n");
					}//end if
					else{
						okaySize = 1;
					}//end else
				}//end while
				write(clientToServer[1], &newSize, sizeof(int));
				printBoardFromServer();
				readReturnValue();
			break;
			case 's':
				clientCmd = cmd_save;
				write(clientToServer[1], &clientCmd, sizeof(cmd_save));
				write(clientToServer[1], filename, 12);
				readReturnValue();
			break;
			case 'l':
				clientCmd = cmd_load;
				write(clientToServer[1], &clientCmd, sizeof(cmd_load));
				write(clientToServer[1], filename, 12);
				readReturnValue(); //load game
				printBoardFromServer();
				readReturnValue(); //print board
			break;
			default: 
				printf("Unknown command. Please try again.\n");
				goodCmd = 1;
			
		}//end switch
	}//end while
}//end gameInterface

/*
* Read return values from server functions and perform certain actions depending on value
* Values are listed in the function
*/
void readReturnValue(){
	
	//read and store return value
	int returnValue;
	read(serverToClient[0], &returnValue, sizeof(int));
	
	switch(returnValue){
			case 0: //firstSetUp() finished
				printf("The game is set up!\n");
			break;
			case 2: //printBoard() finished
				//print no message, it will be clear that the board has printed.
			break;
			case 3: //isMoveValid() returned 0 (not valid)
				printf("The tile cannot be moved!\n");
			break;
			case 4: //isMoveValid() returned 1 (is valid)
				printf("The tile has been moved successfully!\n");
			break;
			case 5: //saveBoard() returned 0
				printf("Board has been saved successfully!\n");
			break;
			case 6: //saveBoard() returned 0
				printf("Board could not be saved!\n");
			break;	
			case 7: //loadGame() returned the new board
				printf("Game loaded successfully!\n");
				write(clientToServer[1], &returnValue, sizeof(int)); //so the previous message does not print in the board
			break;
			case 8: //loadGame() returned the old board (load failed)
				printf("Game could not be loaded!\n");
			break;	
			default: //should never be hit, print returnValue for debug
				printf("Default hit! %d\n", returnValue);
		
	}//end switch
}//end readReturnValue

/*
* Prints the board. The board and board size is send by the server
*/
void printBoardFromServer(){
	int done = 1;
	int boardSize;
	int *board;
	
	read(serverToClient[0], &boardSize, sizeof(boardSize));
	write(clientToServer[1], &done, sizeof(done));
	
	board = (int*)malloc((boardSize * boardSize) * sizeof(int));
	
	read(serverToClient[0], board, (boardSize * boardSize) * sizeof(int));
	write(clientToServer[1], &done, sizeof(done));
	
	for(int i = 0; i < boardSize * boardSize; i++){
		if(board[i]!= 0){
			printf("%3d ", board[i]);
		}//end if
		else
			//this 3 space print will create the free space in the printed board
			printf("    ");

		//print a new line whenever a row is finished printing
		//skip for i = 0 because program will crash if 0 % x is trying to be calculated
		if(i != 0){
			if(i % boardSize == boardSize - 1){
				printf("\n");
			}//end if
		}//end if
	}//end for
	free(board);//we are done with this board
	
	//tell server client has finished printing the board
	write(clientToServer[1], &done, sizeof(done));
}//end printBoardFromServer