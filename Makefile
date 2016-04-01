CC=gcc
CFLAGS=-g -Wall
LIBS=-lpq -lm
INCLUDES=-I /usr/include/postgresql/
TARGET=Agenda
PATH_FUNCIONS=../c/funcions/
PATH_FHEADER=../c/headers/
FUNCIONS=funcions

all:
	$(CC) -o $(TARGET) $(INCLUDES) $(LIBS) $(CFLAGS) $(TARGET).c $(PATH_FUNCIONS)$(FUNCIONS).c

clean:
	$(RM) $(TARGET)
