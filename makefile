slidingpuzzle: 	slidingpuzzle-v4.o sp-tcp-client.o sp-tcp-server.o sp-game.o
					gcc slidingpuzzle-v4.o sp-tcp-client.o sp-tcp-server.o sp-game.o -o slidingpuzzle-v4
									
slidingpuzzle-v4.o:   slidingpuzzle-v4.c
						gcc -c slidingpuzzle-v4.c
			
											
sp-tcp-client.o:	sp-tcp-client.c
						gcc -c sp-tcp-client.c
											
sp-tcp-server.o:	sp-tcp-server.c
						gcc -c sp-tcp-server.c
						
sp-game.o:	sp-game.c
				gcc -c sp-game.c				
							
											
clean:	
		rm sp-game.o sp-tcp-server.o sp-tcp-client.o slidingpuzzle-v4.o slidingpuzzle-v4			
