CC=gcc
RM=rm -f

LIB=sheap.so
OBJ=sizetable.c pool_hash_table.c util.c flist.c
MAIN=main.c

EXE=runner

all: $(LIB)

$(LIB): sizetable.c sheap.c pool_hash_table.c main.c util.c flist.c
	$(CC) $(MAIN) -o $(EXE)
	$(CC) -shared -fPIC sheap.c $(OBJ) -o $(LIB)

clean:
	$(RM) $(EXE)
	$(RM) *.o
	$(RM) $(LIB)
