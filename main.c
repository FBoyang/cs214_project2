
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "readcsv.h"
#include "tid_list.h"
#include "dirwalk.h"
#include "mergesort.h"

int get_field_index(char *field_name);

int main(int argc, char **argv)
{
	int c;
	int field_index;
	char *field_name;
	char *input_dir;
	char *output_dir;
	bool is_verbose;
	struct csv table;
	struct csv stable;
	struct tid_list tids;
	struct dirwalk_args dwargs;
	struct mergesort_args msargs;
	field_index = -1;
	input_dir = ".";
	output_dir = ".";
	is_verbose = true;
	while ((c = getopt(argc, argv, "c:d:o:qv")) != -1) {
		switch (c) {
		case 'c':
			field_name = optarg;
			field_index = get_field_index(optarg);
			break;
		case 'd':
			input_dir = optarg;
			break;
		case 'o':
			output_dir = optarg;
			break;
		case 'q':
			is_verbose = false;
			break;
		case 'v':
			is_verbose = true;
		/* we will let getopt() deal with errors */
		}
	}
	if (field_index == -1) {
		fputs("please provide a field by which to sort\n", stderr);
	} else {
		initialize_csv(&table);
		initialize_csv(&stable);
		initialize_tid_list(&tids);
		dwargs.table = &table;
		dwargs.tids = &tids;
		dwargs.dir = input_dir;
		dirwalk((void *) &dwargs);
		msargs.matrix = table.matrix;
		msargs.tids = &tids;
		msargs.field_index = field_index;
		msargs.low = 0;
		msargs.high = table.num_rows;
		smatrix = malloc(sizeof(char **) * table.num_rows);
		mergesort((void *) &msargs);

		stable.matrix = smatrix;
		stable.num_rows = table.num_rows;
		stable.row_capacity = table.row_capacity;		
		print_csv(&stable, field_name, output_dir);
		print_tid_list(&tids, is_verbose);
		free_csv(&table);
		free_tid_list(&tids);
	}
	return 0;
}
