CC=gcc
RM=rm -f

LIB=sheap.so
OBJ=main.c

EXE=runner

all: $(LIB)

$(LIB): main.c sheap.c
	$(CC) $(OBJ) -o $(EXE)
	$(CC) -shared -fPIC sheap.c -o $(LIB)

clean:
	$(RM) $(EXE)
	$(RM) $(LIB)
