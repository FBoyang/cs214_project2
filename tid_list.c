#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include "tid_list.h"

void initialize_tid_list(struct tid_list *tids)
{
	tids->list = NULL;
	pthread_mutex_init(&tids->mutex, NULL);
	tids->num_tids = 0;
	tids->capacity = 0;
}

void append_tid_list(struct tid_list *tids, pthread_t *new_tids, int num_new)
{
	pthread_mutex_lock(&tids->mutex);
	if (tids->num_tids + num_new > tids->capacity) {
		tids->capacity = 2 * (tids->capacity + num_new);
		tids->list = realloc(tids->list, tids->capacity * sizeof(*tids->list));
	}
	memcpy(tids->list + tids->num_tids, new_tids, num_new * sizeof(*new_tids));
	tids->num_tids += num_new;
	pthread_mutex_unlock(&tids->mutex);
}

void print_tid_list(struct tid_list *tids, bool is_verbose)
{
	int i;
	fprintf(stdout, "Initial PID: %d", getpid());
	if (is_verbose) {
		fputs("\nTIDs of all child threads: ", stdout);
		for (i = 0; i < tids->num_tids; i++) {
			fprintf(stdout, "%ld", tids->list[i]);
			if (i < tids->num_tids - 1)
				fputc(',', stdout);
		}
	}
	fprintf(stdout, "\nTotal number of threads: %d\n", tids->num_tids);
}

void free_tid_list(struct tid_list *tids)
{
	free(tids->list);
	pthread_mutex_destroy(&tids -> mutex);
}
