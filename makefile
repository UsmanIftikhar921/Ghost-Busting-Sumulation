CC = gcc
OPT = -Wall
CFLAGS = -pthread
DEPS = defs.h
OBJ = main.o ghost.o hunter.o room.o building.o evidence.o

ghostHunter: $(OBJ)
	$(CC) $(OPT) -o ghostHunter ${OBJ} $(CFLAGS)

main.o: main.c defs.h
	$(CC) $(OPT) -c main.c

ghost.o: ghost.c defs.h
	$(CC) $(OPT) -c ghost.c defs.h

hunter.o: hunter.c defs.h
	$(CC) $(OPT) -c hunter.c defs.h

room.o: room.c defs.h
	$(CC) $(OPT) -c room.c defs.h

building.o: building.c defs.h
	$(CC) $(OPT) -c building.c defs.h

evidece.o: evidence.c defs.h
	$(CC) $(OPT) -c evidence.c defs.h
clean:
	rm -f ${OBJ} ghostHunter
