CC=gcc
RM=rm -f

LIB=sheap.so
OBJ=sizetable.c sizetablemain.c

EXE=runner

all: $(LIB)

$(LIB): sizetable.c sizetablemain.c sheap.c
	$(CC) $(OBJ) -o $(EXE)
	$(CC) -shared -fPIC sheap.c -o $(LIB)

clean:
	$(RM) $(EXE)
	$(RM) $(LIB)
