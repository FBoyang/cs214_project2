#include <stdio.h>
#include <math.h>

int main(int argc, char **argv)
{
	float total;
	float squares;
	int minutes;
	float seconds;
	int i;
	float avg;
	total = 0.0f;
	squares = 0.0f;
	i = 0;
	while (scanf("real %dm%fs\n", &minutes, &seconds) == 2) {
		total += seconds;
		squares += seconds * seconds;
		i++;
	}
	avg = total / i;
	printf("average %f, std dev %f\n", avg, sqrt(squares/i - avg*avg));
	return 0;
}
