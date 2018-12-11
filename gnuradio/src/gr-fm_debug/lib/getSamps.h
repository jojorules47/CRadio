#ifndef __GETSAMPS_H__
#define __GETSAMPS_H__

#include <stdio.h>

#define MAX_DES 1024

typedef struct {
	long size;
	char designator[MAX_DES];
	int md;
	void* memory;
} shared_t;

#ifdef __cplusplus
extern "C"{
#endif

long get_shared_memory_size(const char *);
shared_t *init_shared_memory(const char *, long);
shared_t *map_shared_memory(const char *);
int destroy_shared_memory(shared_t*);
int unmap_shared_memory(shared_t*);

shared_t *read_shared_memory(const char *, long);

#ifdef __cplusplus
}
#endif

#endif
