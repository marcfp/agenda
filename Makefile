PACKAGE = @PACKAGE@
VERSION = @VERSION@
DEFS = -DLOCALEDIR=\"$(LOCALEDIR)\"

ECHO=echo
CC=gcc
CFLAGS=-g -W -Wall 
EXTERN=-c 
LIBS=-lm -lpq
INCLUDES=-I /usr/include/postgresql/
TARGET=Agenda_telefons
TARGET_GRAFIC=Agenda_telefons_Efl
PATH_FUNCIONS=funcions
FUNCIONS=funcions
GRAFIC=pkg-config --cflags --libs elementary evas ecore ecore-evas eina ecore-x
DATADIR = @DATADIR@
LOCALEDIR = $(DATADIR)/locale
#PACKAGE = @PACKAGE@

#DEFS = -DLOCALEDIR=\"$(LOCALEDIR)\" 
##--with-included-gettext



all:funcio consola grafic 

grafic:funcio grafic
	$(CC) -o $(TARGET_GRAFIC) $(INCLUDES) $(CFLAGS) $(TARGET_GRAFIC).c $(FUNCIONS).o `$(GRAFIC)` $(DEFS) $(LIBS)
consola:funcio consola
	$(CC) -o $(TARGET) $(INCLUDES) $(CFLAGS) $(TARGET).c $(FUNCIONS).o $(DEFS) $(LIBS)
funcio:	
	$(CC) $(CFLAGS) $(EXTERN) $(FUNCIONS)/$(FUNCIONS).c 
clean:
	$(RM) $(TARGET) $(TARGET_GRAFIC) $(FUNCIONS).o
