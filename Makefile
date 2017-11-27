CC = gcc
sources = readcsv.c mergesort.c dirwalk.c tid_list.c main.c
header = dirwalk.h tid_list.h mergesort.h readcsv.h

.PHONY:	all

all: sorter

sorter: $(sources)
	gcc -pthread -lm -Wall -O -g -o sorter $(sources)

.PHONY: clean
clean:
	rm -vf sorter

