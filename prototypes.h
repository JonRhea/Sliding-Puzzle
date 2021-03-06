int gameInterface();
void firstSetUp();
void connectToServer(char *ip, long port);
void printBoardFromServer();
void readReturnValue();
void createServer(long port);
void firstSetUp(int clientCount);
void doMove();
void doQuit();
void doNew();
void doSave();
void doLoad();
int serverWait(int clientID);
int isMoveValid(int tile);
int * createGame();
void randomizeBoard();
void swap(int *a, int *b);
void printBoard();
void moveTile(int tile);
int checkWon();
int saveGame(char * filename);
int * loadGame(char * filename);
void teardown();