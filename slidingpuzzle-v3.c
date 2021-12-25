#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int gameInterface();
void firstSetUp();

int clientToServer[2];
int serverToClient[2];

/*
* Starts the program, forks the client and server, and where the program will end.
* @return 0 The program has ended.
*/
int main(){
	
	//pipe clientToServer
	if(pipe(clientToServer) == -1){
		perror("Failed pipe clientToServer.\n");
		return 1;
	}//end if
	
	//pipe serverToClient
	if(pipe(serverToClient) == -1){
		perror("Failed pipe serverToClient.\n");
		return 1;
	}//end if
	
	pid_t pid;
	
	switch(pid = fork()){
		case -1:
			perror("Failiure to fork.\n");
			exit(1);
		break;
		//case 0 will be the client (child)
		case 0:
			gameInterface();
		break;
		//default will be the server (parent)
		default:
			firstSetUp();
	}//end switch
	
	return 0;
}//end main