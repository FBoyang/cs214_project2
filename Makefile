CC = gcc
sources = csv.c dirwalk.c quicksort.c tid_list.c main.c
headers = csv.h dirwalk.h quicksort.h tid_list.h

.PHONY: all
all: sorter filesplit

sorter: $(sources) $(headers)
	gcc -pthread -lm -Wall -O -g -o sorter $(sources)

filesplit: filesplit.c
	gcc -Wall -O -g -o filesplit filesplit.c

.PHONY: clean
clean:
	rm -vf sorter filesplit
