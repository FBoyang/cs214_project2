CC = gcc
sources = csv.c dirwalk.c quicksort.c tid_list.c main.c
headers = csv.h dirwalk.h quicksort.h tid_list.h

.PHONY: all
all: sorter filesplit average

sorter: $(sources) $(headers)
	gcc -pthread -lm -Wall -g -o sorter $(sources)

filesplit: filesplit.c
	gcc -Wall -O -g -o filesplit filesplit.c

average: average.c
	gcc -lm -Wall -O -g -o average average.c

.PHONY: clean
clean:
	rm -vf sorter filesplit average
