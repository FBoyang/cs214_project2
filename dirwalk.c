#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include <stdbool.h>
#include "tid_list.h"
#include "dirwalk.h"
#include "csv.h"

void *dirwalk(void *arg)
{
	struct dirwalk_args *args;
	DIR *dir;
	struct dirent *ent;
	char *entname;
	struct dirwalk_args **childargs;
	int num_subdirs;
	int subdir_capacity;
	struct csvargs **fileargs;
	int num_files;
	int file_capacity;
	pthread_t *tid_array;
	int num_tids;
	int tid_capacity;
	pthread_t tid;
	int i;
	args = (struct dirwalk_args *) arg;
	if ((dir = opendir(args->dir)) == NULL)
		return NULL;
	subdir_capacity = 4;
	childargs = malloc(subdir_capacity * sizeof(*childargs));
	num_subdirs = 0;
	file_capacity = 4;
	fileargs = malloc(file_capacity * sizeof(*fileargs));
	num_files = 0;
	tid_capacity = 4;
	tid_array = malloc(tid_capacity * sizeof(*tid_array));
	num_tids = 0;
	while ((ent = readdir(dir))) {
		entname = malloc((strlen(args->dir) + strlen(ent->d_name) + 2) * sizeof(char));
		sprintf(entname, "%s/%s", args->dir, ent->d_name);
		switch (ent->d_type) {
		case DT_DIR:
			if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
				free(entname);
				break;
			}
			if (num_subdirs >= subdir_capacity) {
				subdir_capacity *= 2;
				childargs = realloc(childargs, subdir_capacity * sizeof(*childargs));
			}
			childargs[num_subdirs] = malloc(sizeof(**childargs));
			childargs[num_subdirs]->table = args->table;
			childargs[num_subdirs]->tids = args->tids;
			childargs[num_subdirs]->dir = entname;
			pthread_create(&tid, NULL, &dirwalk, (void *) childargs[num_subdirs]);
			if (num_tids >= tid_capacity) {
				tid_capacity *= 2;
				tid_array = realloc(tid_array, tid_capacity * sizeof(*tid_array));
			}
			tid_array[num_tids] = tid;
			num_subdirs++;
			num_tids++;
			break;
		case DT_REG:
			if (strlen(ent->d_name) < 4 || strcmp(ent->d_name + strlen(ent->d_name) - 4, ".csv")) {
				free(entname);
				break;
			}
			if (num_files >= file_capacity) {
				file_capacity *= 2;
				fileargs = realloc(fileargs, file_capacity * sizeof(*fileargs));
			}
			fileargs[num_files] = malloc(sizeof(**fileargs));
			fileargs[num_files]->table = args->table;
			fileargs[num_files]->file = entname;
			pthread_create(&tid, NULL, &csvread, (void *) fileargs[num_files]);
			if (num_tids >= tid_capacity) {
				tid_capacity *= 2;
				tid_array = realloc(tid_array, tid_capacity * sizeof(*tid_array));
			}
			tid_array[num_tids] = tid;
			num_files++;
			num_tids++;
			break;
		default:
			free(entname);
		}
	}
	append_tid_list(args->tids, tid_array, num_tids);
	for (i = 0; i < num_tids; i++)
		pthread_join(tid_array[i], NULL);
	free(tid_array);
	for (i = 0; i < num_subdirs; i++) {
		free(childargs[i]->dir);
		free(childargs[i]);
	}
	free(childargs);
	for (i = 0; i < num_files; i++) {
		free(fileargs[i]->file);
		free(fileargs[i]);
	}
	free(fileargs);
	return NULL;
}
