ECHO=echo
CC=gcc
CFLAGS=-g -W -Wall 
EXTERN=-c 
LIBS=-lpq -lm
INCLUDES=-I /usr/include/postgresql/
TARGET=Agenda_telefons
TARGET_GRAFIC=Agenda_telefons_Efl
PATH_FUNCIONS=funcions
FUNCIONS=funcions
GRAFIC=pkg-config --cflags --libs elementary evas ecore ecore-evas eina ecore-x

all:funcio consola grafic 

grafic:funcio grafic
	$(CC) -o $(TARGET_GRAFIC) $(INCLUDES) $(LIBS) $(CFLAGS) $(TARGET_GRAFIC).c $(FUNCIONS).o `$(GRAFIC)`
consola:funcio consola
	$(CC) -o $(TARGET) $(INCLUDES) $(LIBS) $(CFLAGS) $(TARGET).c $(FUNCIONS).o
funcio:	
	$(CC) $(CFLAGS) $(EXTERN) $(FUNCIONS)/$(FUNCIONS).c 
clean:
	$(RM) $(TARGET) $(TARGET_GRAFIC) $(FUNCIONS).o
