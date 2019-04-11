CC=gcc
RM=rm -f

LIB=sheap.so
OBJ=sizetable.c pool_hash_table.c
MAIN=main.c

EXE=runner

all: $(LIB)

$(LIB): sizetable.c sheap.c pool_hash_table.c main.c
	$(CC) $(MAIN) -o $(EXE)
	$(CC) -c $(OBJ)
	$(CC) -shared -fPIC sheap.c -o $(LIB)

clean:
	$(RM) $(EXE)
	$(RM) $(LIB)
