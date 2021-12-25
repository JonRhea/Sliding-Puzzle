#include "includes.h"
#include "prototypes.h"

#define PORT 3490

enum command {cmd_pnt, cmd_move, cmd_quit, cmd_new, cmd_save, cmd_load};

int numberPort;
int enteredInfo;
int sock = 0;
struct sockaddr_in serv_addr;

/**
* Creates a socket to connect and attemps to connect to a server
* @param *ip: The ip the user entered, this must not be NULL
* @param port: The port entered by the user. Can be 0, which means use default port 3490
*/	
void connectToServer(char *ip, long port){
	
	//create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("\n Socket creation error \n");
        exit(1);
    }//end if
   
	//set ipv4
    serv_addr.sin_family = AF_INET;
	
	//if port is 0, then no port was entered
	if(port == 0){
		enteredInfo = 0;
		serv_addr.sin_port = htons(PORT);
	}//end if
	//if port was entered, use it
	else{
		enteredInfo = 1;
		numberPort = port;
		serv_addr.sin_port = numberPort;
	}//end else
	
	//convert ip string
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0){
		printf("Invalid address. Address not supported or not entered!\n");
		exit(1);
	}//end if
	
	//attempt connection
    printf("Client: trying to connect...\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        exit(1);
    }//end if
	else{
		if(enteredInfo == 0){
			printf("Client: connected to %s on port %d\n", ip, PORT);
		}//end if
		else{
			printf("Client: connected to %s on port %d\n", ip, numberPort);
		}//end else
	}//end else
	
	gameInterface();
}//end connectToServer

/**
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
			recv(sock, &returnValueOfGameWon, sizeof(int), 0);
			if(returnValueOfGameWon == 1){
				printf("\n**You won the game!**\n");
				return 0;
			}//end if
			else{
				//printf("The game has not been won!\n");
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
				send(sock, &clientCmd, sizeof(cmd_pnt), 0);
				printBoardFromServer();
				readReturnValue();
			break;
			case 'm':
				clientCmd = cmd_move;
				send(sock, &clientCmd, sizeof(cmd_move), 0);
				printf("Enter a tile to move.\n>");
				int tile;
				scanf("%d", &tile);
				send(sock, &tile, sizeof(int), 0);
				readReturnValue();
			break;
			case 'q':
				clientCmd = cmd_quit;
				printf("Ending the game.\n");
				send(sock, &clientCmd, sizeof(cmd_quit), 0);
				close(sock);
			break;
			case 'n':
				clientCmd = cmd_new;
				send(sock, &clientCmd, sizeof(cmd_new), 0);
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
				send(sock, &newSize, sizeof(int), 0);
				printBoardFromServer();
				readReturnValue();
			break;
			case 's':
				clientCmd = cmd_save;
				send(sock, &clientCmd, sizeof(cmd_save), 0);
				send(sock, filename, 12, 0);
				readReturnValue();
			break;
			case 'l':
				clientCmd = cmd_load;
				send(sock, &clientCmd, sizeof(cmd_load), 0);
				send(sock, filename, 12, 0);
				readReturnValue(); //load game
				
			break;
			default: 
				printf("Unknown command. Please try again.\n");
				goodCmd = 1;
			
		}//end switch
	}//end while
}//end gameInterface

/**
* Read return values from server functions and perform certain actions depending on value
* Values are listed in the function
*/
void readReturnValue(){
	
	//read and store return value
	int returnValue;
	recv(sock, &returnValue, sizeof(int), 0);
	//printf("returnValue: %d\n", returnValue);
	
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
				send(sock, &returnValue, sizeof(int), 0); //so the previous message does not print in the board
				printBoardFromServer();
				readReturnValue(); //print board
			break;
			case 8: //loadGame() returned the old board (load failed)
				printf("Game could not be loaded!\n");
			break;	
			default: //should never be hit, print returnValue for debug
				printf("Default hit! %d\n", returnValue);
		
	}//end switch
}//end readReturnValue

/**
* Prints the board. The board and board size is send by the server
*/
void printBoardFromServer(){
	int done = 1;
	int boardSize;
	int *board;
	
	recv(sock, &boardSize, sizeof(boardSize), 0);
	send(sock, &done, sizeof(done), 0);
		
	board = (int*)malloc((boardSize * boardSize) * sizeof(int));
	
	recv(sock, board, (boardSize * boardSize) * sizeof(int), 0);
	send(sock, &done, sizeof(done), 0);
	
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
	send(sock, &done, sizeof(done), 0);
}//end printBoardFromServer