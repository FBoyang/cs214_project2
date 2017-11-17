#ifndef _QUICKSORT_H
#define _QUICKSORT_H

#include "tid_list.h"

struct quicksort_args {
	char ***matrix;
	struct tid_list *tids;
	int field_index;
	int low;
	int high;
};

void *quicksort(void *arg);

#endif
