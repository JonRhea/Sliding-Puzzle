#include "includes.h"
#include "prototypes.h"

/**
* Starts the program in either server mode (-s with optional port)
* or in client mode (-c with required IP address and optional port)
* @return 0 The program has ended.
*/
int main(int argc, char* argv[]){
	char mode[3];
	if(argc > 1){
		strcpy(mode, argv[1]);
	}//end if
	
	//server mode
	if(strcmp(mode, "-s") == 0){
		long port = 0;
		if(argc == 3){
			char *pointer;
			port = strtol(argv[2], &pointer, 10);
		}//end if
		createServer(port);
	}//end if
	
	//client mode
	else if(strcmp(mode, "-c") == 0){
		long port = 0;
		char *ip = NULL;
		if(argc == 4){
			ip = argv[2];
			char *pointer;
			port = strtol(argv[3], &pointer, 10);
		}//end if
		if(argc == 3){
			ip = argv[2];
		}//end if
		connectToServer(ip, port);
	}//end if
	else{
		printf("Arugment error: Enter one of the following ways...\nRunning as server: slidingpuzzle-v4 -s [PORT]\nRunning as client: slidingpuzzle-v4 -c IP [PORT]\n");
	}//end else
	
	return 0;
}//end main