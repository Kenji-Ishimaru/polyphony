
# could add to CFLAGS to turn on warnings if you are using gcc
WARNS=-Wall

# change path if not running test from installed directory location
INCS=-I /usr/local/include/iverilog
# maybe want -O<something> and/or -g, if you use -O use -march=
CFLAGS= -fPIC -Wall $(INCS)
LFLAGS= -G -shared -export-dynamic

# change to your compiler
CC=gcc

# make rules for dynamic libaries
pli_boot.so:	pli_src.o
	$(LD) $(LFLAGS) pli_src.o $(LFLAGS) -o pli_boot.so

pli_src.o:	pli_src.c
	$(CC) $(CFLAGS) -c pli_src.c

