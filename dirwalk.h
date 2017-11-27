#ifndef _DIRWALK_H
#define _DIRWALK_H

#include "readcsv.h"
#include "tid_list.h"

struct dirwalk_args {
	struct csv *table;
	struct tid_list *tids;
	char *dir;
};

void *dirwalk(void *arg);

#endif
