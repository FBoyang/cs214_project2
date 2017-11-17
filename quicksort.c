#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>
#include "csv.h"
#include "tid_list.h"
#include "quicksort.h"

const int MIN_THREAD_LENGTH = 50;
const int MIN_LENGTH = 10;

void serial_quicksort(char ***matrix, int field_index, int low, int high);
int partition(char ***matrix, int field_index, int low, int high);
int compare(char *a, char *b);
int lexcmp(char *a, int alen, char *b, int blen);
int charcmp(char a, char b);
int strbegin(char *str);
int strend(char *str);
void insertionsort(char ***matrix, int field_index, int low, int high);

void *quicksort(void *arg)
{
	struct quicksort_args *args;
	struct quicksort_args low_args, high_args;
	int pivot;
	pthread_t tid;
	args = (struct quicksort_args *) arg;
	if (args->high - args->low < MIN_LENGTH) {
		insertionsort(args->matrix, args->field_index, args->low, args->high);
		return NULL;
	} else if (args->high - args->low < MIN_THREAD_LENGTH) {
		serial_quicksort(args->matrix, args->field_index, args->low, args->high);
		return NULL;
	}
	pivot = partition(args->matrix, args->field_index, args->low, args->high);
	low_args.matrix = args->matrix;
	low_args.tids = args->tids;
	low_args.field_index = args->field_index;
	low_args.low = args->low;
	low_args.high = pivot;
	high_args.matrix = args->matrix;
	high_args.tids = args->tids;
	high_args.field_index = args->field_index;
	high_args.low = pivot + 1;
	high_args.high = args->high;
	pthread_create(&tid, NULL, &quicksort, (void *) &low_args);
	quicksort((void *) &high_args);
	append_tid_list(args->tids, &tid, 1);
	pthread_join(tid, NULL);
	return NULL;
}

void serial_quicksort(char ***matrix, int field_index, int low, int high)
{
	int pivot;
	if (high - low < MIN_LENGTH) {
		insertionsort(matrix, field_index, low, high);
		return;
	}
	pivot = partition(matrix, field_index, low, high);
	serial_quicksort(matrix, field_index, low, pivot);
	serial_quicksort(matrix, field_index, pivot, high);
}

#ifdef _ALT_PARTITION
int partition(char ***matrix, int field_index, int low, int high)
{
	int i, j;
	int rand;
	char **tmp;
	rand = low + (unsigned long) random() % (high - low);
	tmp = matrix[low];
	matrix[low] = matrix[rand];
	matrix[rand] = tmp;
	i = low + 1;
	while (i < high && compare(matrix[i][field_index], matrix[low][field_index]) <= 0)
		i++;
	j = i;
	while (j < high && compare(matrix[j][field_index], matrix[low][field_index]) >= 0)
		j++;
	if (j < high) {
		tmp = matrix[i];
		matrix[i] = matrix[j];
		matrix[j] = tmp;
	}
	while (j < high) {
		while (compare(matrix[i][field_index], matrix[low][field_index]) <= 0)
			i++;
		while (j < high && compare(matrix[j][field_index], matrix[low][field_index]) >= 0)
			j++;
		if (j < high ) {
			tmp = matrix[i];
			matrix[i] = matrix[j];
			matrix[j] = tmp;
		}
	}
	tmp = matrix[i - 1];
	matrix[i - 1] = matrix[j];
	matrix[j] = tmp;
	return i - 1;
}
#else
int partition(char ***matrix, int field_index, int low, int high)
{
	int i, j;
	int rand;
	char **tmp;
	rand = low + (unsigned long) random() % (high - low);
	tmp = matrix[low];
	matrix[low] = matrix[rand];
	matrix[rand] = tmp;
	i = low + 1;
	j = high - 1;
	while (i < j) {
		while (i < high && compare(matrix[i][field_index], matrix[low][field_index]) <= 0)
			i++;
		while (j > low && compare(matrix[j][field_index], matrix[low][field_index]) >= 0)
			j--;
		if (i < j) {
			tmp = matrix[i];
			matrix[i] = matrix[j];
			matrix[j] = tmp;
		}
	}
	tmp = matrix[low];
	matrix[low] = matrix[j];
	matrix[j] = tmp;
	return j;
}
#endif

int compare(char *a, char *b)
{
	int ret;
	double ad, bd;
	bool ac, bc;
	int ab, bb;
	int ae, be;
	int alen, blen;
	bool aempty, bempty;
	char *endptr;
	ab = strbegin(a);
	ae = strend(a);
	bb = strbegin(b);
	be = strend(b);
	alen = ae - ab + 1;
	blen = be - bb + 1;
	aempty = (alen <= 0);
	bempty = (blen <= 0);
	if (aempty && bempty) {
		ret = 0;
	} else if (aempty) {
		ret = -1;
	} else if (bempty) {
		ret = 1;
	} else {
		ac = false;
		bc = false;
		ad = strtod(a + ab, &endptr);
		if (endptr == a + ae + 1)
			ac = true;
		bd = strtod(b + bb, &endptr);
		if (endptr == b + be + 1)
			bc = true;
		if (ac && bc) {
			if (fabs(ad - bd) < 0.0001)
				ret = 0;
			else if (ad < bd)
				ret = -1;
			else
				ret = 1;
		} else if (ac) {
			ret = -1;
		} else if (bc) {
			ret = 1;
		} else {
			ret = lexcmp(a + ab, alen, b+ bb, blen);
		}
	}
	return ret;
}

int lexcmp(char *a, int alen, char *b, int blen)
{
	int ret;
	int i, j;
	int cmp;
	i = 0;
	j = 0;
	ret = 2;
	while (ret == 2 && i < alen && j < blen) {
		while (a[i] < '\0' || isspace(a[i]))
			i++;
		while (b[j] < '\0' || isspace(b[j]))
			j++;
		if (i == alen && j == blen)
			ret = 0;
		else if (i == alen)
			ret = -1;
		else if (j == blen)
			ret = 1;
		else if ((cmp = charcmp(a[i], b[j])))
			ret = cmp;
		i++;
		j++;
	}
	if (ret == 2) {
		if (i >= alen && j >= blen)
			ret = 0;
		else if (i >= alen)
			ret = -1;
		else
			ret = 1;
	}
	return ret;
}

int charcmp(char a, char b)
{
	int ret;
	if (isalpha(a) && isalpha(b)) {
		if (toupper(a) == toupper(b)) {
			if (isupper(a) && isupper(b))
				ret = 0;
			else if (isupper(a))
				ret = -1;
			else if (isupper(b))
				ret = 1;
			else
				ret = 0;
		} else if (toupper(a) < toupper(b)) {
			ret = -1;
		} else {
			ret = 1;
		}
	} else if (toupper(a) == toupper(b)) {
		ret = 0;
	} else if (toupper(a) < toupper(b)) {
		ret = -1;
	} else {
		ret = 1;
	}
	return ret;
}

int strbegin(char *str)
{
	int i, len, begin;
	if (str)
		len = strlen(str);
	else
		len = 0;
	begin = len;
	i = 0;
	for (i = 0; i < len; i++) {
		if (str[i] > '\0' && !isspace(str[i])) {
			begin = i;
			break;
		}
	}
	return begin;
}

int strend(char *str)
{
	int i, len, end;
	if (str)
		len = strlen(str);
	else
		len = 0;
	end = -1;
	for (i = len - 1; i >= 0; i--) {
		if (str[i] > '\0' && !isspace(str[i])) {
			end = i;
			break;
		}
	}
	return end;
}

void insertionsort(char ***matrix, int field_index, int low, int high)
{
	int i, j;
	char **tmp;
	for (i = low + 1; i < high; i++) {
		tmp = matrix[i];
		for (j = i; j > 0 && compare(matrix[j - 1][field_index], tmp[field_index]) > 0; j--)
			matrix[j] = matrix[j - 1];
		matrix[j] = tmp;
	}
}
