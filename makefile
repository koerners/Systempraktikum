CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SYSC = main.o performConnection.o editStrings.o config.o thinker.o
VERSION = sysprak-client

sysprak-client: $(SYSC)
	$(CC) $(CFLAGS) -o $(VERSION) $(SYSC)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

performConnection.o: performConnection.c
	$(CC) $(CFLAGS) -c performConnection.c

editStrings.o: editStrings.c
	$(CC) $(CFLAGS) -c editStrings.c
	
config.o: config.c
	$(CC) $(CFLAGS) -c config.c

thinker.o: thinker.c
	$(CC) $(CFLAGS) -c thinker.c
	
clean:
	rm $(SYSC) sysprak-client -f
	
play:
	make ; ./$(VERSION) -g $(GAME_ID) -p $(PLAYER)
	
valgrindOhnePlayer:
	make ; valgrind --leak-check=full --trace-children=yes ./$(VERSION) -g $(GAME_ID)

VmitPlayervalgrind:
	make ; valgrind --leak-check=full --trace-children=yes ./$(VERSION) -g $(GAME_ID) -p $(PLAYER)