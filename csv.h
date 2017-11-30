#ifndef _CSV_H
#define _CSV_H

struct csv {
	char ***matrix;
	pthread_mutex_t mutex;
	int num_rows;
	int row_capacity;
};

struct csvargs {
	struct csv *table;
	char *file;
};

void initialize_csv(struct csv *table);
void *csvread(void *arg);
void append_csv(struct csv *table, char ***new_entries, int num_new);
void print_csv(struct csv *table, char *field_name, char *output_dir);
void free_csv(struct csv *table);

#endif
