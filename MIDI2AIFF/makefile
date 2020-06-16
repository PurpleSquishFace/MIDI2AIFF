# Makefile for 13011362 (this is a comment) 

INCLUDES = -I ../include 
LIBS = $(PORTSF) -lm
PORTSF = ../lib/libportsf.a 
CC = gcc 

all: 13011362

13011362: $(PORTSF) 13011362.c 
	$(CC) -o 13011362 13011362.c $(INCLUDES) $(LIBS) 

$(PORTSF): 
	cd ../portsf; make libportsf.a; make install 

clean: rm -f *.o 
