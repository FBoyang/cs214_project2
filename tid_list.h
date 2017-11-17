#ifndef _TID_LIST_H
#define _TID_LIST_H

struct tid_list {
	pthread_t *list;
	pthread_mutex_t mutex;
	int num_tids;
	int capacity;
};

void initialize_tid_list(struct tid_list *tids);
void append_tid_list(struct tid_list *tids, pthread_t *new_tids, int num_new);
void print_tid_list(struct tid_list *tids, bool is_verbose);
void free_tid_list(struct tid_list *tids);

#endif
