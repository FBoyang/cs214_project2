#include <stdio.h>

int main(int argc, char **argv)
{
	float total;
	int minutes;
	float seconds;
	int i;
	total = 0.0f;
	i = 0;
	while (scanf("real %dm%fs\n", &minutes, &seconds) == 2) {
		total += 60 * minutes + seconds;
		i++;
	}
	printf("%f\n", total / i);
	return 0;
}
