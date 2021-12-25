#include "includes.h"
#include "prototypes.h"

#define PORT 3490

enum command {cmd_pnt, cmd_move, cmd_quit, cmd_new, cmd_save, cmd_load};

int server_fd;
int client_socket;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);

int clientCount = 0;

/**
* Creates a socket for a server and loop infitely, accepting clients
* @param port: The port entered by the user. Can be 0, which means use default port 3490
*/
void createServer(long port){
       
	//create socket 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Server socket failed");
        exit(1);
    }//end if
       
	//set Sockopt
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("Server setsockopt failed");
        exit(1);
    }//end if
	
	//set ipv4 at default address (127.0.0.1 in this project)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
	
	//if port was not entered, use default
	if(port == 0){
		address.sin_port = htons(PORT);
	}//end if
	//if port was entered, use it
	else{
		int numberPort;
		numberPort = port;
		address.sin_port = numberPort;
	}//end else
           
	//bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("Server bind failed");
        exit(1);
    }//end if
	
	//listen 
    if (listen(server_fd, 3) < 0){
        perror("Server listen failed");
        exit(1);
    }//end if
	
	//begin accepting clients
	while(1){
		printf("Server: Waiting for client...Press Ctr C to quit server\n");
		if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
			perror("Error excepting client");
			exit(1);
		}//end if
		else{
			clientCount++;
			pid_t child_pid;
			if((child_pid = fork()) == -1){
				perror("Failed to fork.");
				exit(1);
			}//end if
			else if(child_pid){//parent 
				//do nothing, continue looping to accept clients
			}//end if
			else if(child_pid == 0){//child
				printf("Server: Client [%d] conneced from %s\n", clientCount, "127.0.0.1"); //manually set "127.0.0.1" b/c it won't change for this project
				firstSetUp(clientCount);
				//close client when done
				close(client_socket);
			}//end if
		}//end else
	}//end while
}//end createServer

/**
* Wait for input command from client, interpret it, adn executes corresponding functions
* @param clientID: ID number of client 
*/
int serverWait(int clientID){

	enum command clientCmd;
	
	while(clientCmd != cmd_quit){
		//check to see if game is won
		if(checkWon() == 1){
			int returnValue = 1;
			send(client_socket, &returnValue, sizeof(int), 0); 
			teardown();
			return 0;
		}//end if
		else{
			int returnValue = 0;
			send(client_socket, &returnValue, sizeof(int), 0); 
		}//end else
			
		//read command from client
		recv(client_socket, &clientCmd, sizeof(clientCmd), 0);
		
		printf("Server: Command %d recieved from client[%d]...\n", clientCmd, clientID);
			
		if(clientCmd == cmd_pnt){
			printBoard(); 
		}//end if
		else if(clientCmd == cmd_move){
			doMove();
		}//end if
		else if(clientCmd == cmd_quit){
			doQuit();
		}//end if
		else if(clientCmd == cmd_new){
			doNew();
		}//end if
		else if(clientCmd == cmd_save){
			doSave();
		}//end if
		else if(clientCmd == cmd_load){
			doLoad();
		}//end if
		else{ //default case, although this should never be hit b/c client takes care of unknown command
			//do nothing, error handeled by client
		}//end else		
	}//end while
}//end serverWait

