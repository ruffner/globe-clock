# Makefile for globegen bitmap conversion utility

COMPILER      = g++
OPTIONS       = -Wall
LINKER_OPT    = -L/usr/lib -lstdc++ -lm

all: globegen

globegen: globegen.cc
	$(COMPILER) $(OPTIONS) -o globegen globegen.cc $(LINKER_OPT)

clean:
	rm -f core *.o *.bak *stackdump *~
