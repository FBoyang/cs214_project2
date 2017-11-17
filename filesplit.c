#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	FILE *infile;
	FILE **outfiles;
	int num_ofs;
	int c;
	char ofname[16];
	char *line;
	size_t n;
	char **linearr;
	int num_lines;
	int line_capacity;
	int i, j;
	infile = NULL;
	num_ofs = 0;
	while ((c = getopt(argc, argv, "i:n:")) != -1) {
		switch (c) {
		case 'i':
			infile = fopen(optarg, "r");
			break;
		case 'n':
			num_ofs = atoi(optarg);
		/* we will let getopt() deal with errors */
		}
	}
	if (infile == NULL || num_ofs <= 0) {
		fputs("error: please provide an input file and a number of output files", stderr);
	}
	outfiles = malloc(num_ofs * sizeof(*outfiles));
	for (i = 0; i < num_ofs; i++) {
		sprintf(ofname, "f%03d.csv", i + 1);
		outfiles[i] = fopen(ofname, "w");
	}
	line = NULL;
	n = 0;
	getline(&line, &n, infile);
	for (i = 0; i < num_ofs; i++)
		fputs(line, outfiles[i]);
	line_capacity = 4;
	linearr = malloc(line_capacity * sizeof(*linearr));
	num_lines = 0;
	while (getline(&line, &n, infile) != -1) {
		if (num_lines >= line_capacity) {
			line_capacity *= 2;
			linearr = realloc(linearr, line_capacity * sizeof(*linearr));
		}
		linearr[num_lines] = line;
		line = NULL;
		n = 0;
		num_lines++;
	}
	fclose(infile);
	for (i = 0; i < num_ofs && num_lines > 0; i++, num_lines--) {
		j = random() % num_lines;
		fputs(linearr[j], outfiles[i]);
		free(linearr[j]);
		linearr[j] = linearr[num_lines - 1];
	}
	for (; num_lines > 0; num_lines--) {
		i = random() % num_ofs;
		j = random() % num_lines;
		fputs(linearr[j], outfiles[i]);
		free(linearr[j]);
		linearr[j] = linearr[num_lines - 1];
	}
	for (i = 0; i < num_ofs; i++)
		fclose(outfiles[i]);
	free(outfiles);
	free(linearr);
}
