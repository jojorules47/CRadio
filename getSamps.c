#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include "getSamps.h"

long get_shared_memory_size(const char *designator){
	int md;
	struct stat file_stat;
	int mode = S_IRWXU | S_IRWXG;

	md = shm_open(designator, O_CREAT | O_RDWR, mode);
	if (md == -1) {
		printf("Got an error opening shm\n");
		return -1;
	}
	if (fstat(md,&file_stat)){
		perror("fstat error\n");
		return -1;
	}
	close(md);
	return file_stat.st_size;
}

shared_t *init_shared_memory(const char* designator, long size){
	shared_t *ret = (shared_t*) calloc(1, sizeof(shared_t));
	strncpy(ret->designator, designator, MAX_DES);
	int mode = S_IRWXU | S_IRWXG;
	int ferror;

	ret->size = size;
	
	ret->md = shm_open(designator, O_CREAT | O_RDWR | O_TRUNC, mode);
	if(ret->md ==-1){
		printf("Got an error opening shm\n");
		return NULL;
	}
	ferror = ftruncate(ret->md, size);

	if (ferror == -1) {
		printf("Error in ftrunctate\n");
		return NULL;
	}

	ret->memory = mmap(0,size,PROT_READ | PROT_WRITE, MAP_SHARED, ret->md, 0);
	if (ret->memory == MAP_FAILED) {
		printf("Error on mmap\n");
		exit(1);
	}
	memset(ret->memory,0,size);
	return ret;
}

shared_t *map_shared_memory(const char* designator){
	shared_t *ret = (shared_t*) calloc(1,sizeof(shared_t));
	int mode = S_IRWXU | S_IRWXG;
	int ferror;
	struct stat file_stat;
	strncpy(ret->designator, designator, MAX_DES);

	ret->md = shm_open(designator, O_CREAT | O_RDWR | O_TRUNC, mode);
	if (ret->md == -1){
		printf("Got an error opening shm\n");
		return NULL;
	}
	if (fstat(ret->md, &file_stat)){
		perror("fstat error\n");
		return NULL;
	}
	ret->size = file_stat.st_size;
	if (!ret->size){
		printf("Size DNE\n");
		return NULL;
	}
	ret->memory = mmap(0,ret->size,PROT_READ | PROT_WRITE, MAP_SHARED,ret->md,0);
	if (ret->memory == MAP_FAILED) {
			printf("error on mmap\n");
			exit(1);
	}

	return ret;

}

shared_t* read_shared_memory(const char *designator, long size){
	shared_t *ret = (shared_t*) calloc(1,sizeof(shared_t));
	int mode = S_IRUSR | S_IRGRP;
	struct stat file_stat;
	strncpy(ret->designator, designator, MAX_DES);
	
	ret->md = shm_open(ret->designator, O_RDONLY, mode);
	if(ret->md == -1){
		printf("Got an error opening shm\n");
		return NULL;
	}
	if (fstat(ret->md, &file_stat)){
		perror("fstat error\n");
		return NULL;
	}
	ret->size = file_stat.st_size;
	if(!ret->size){
		printf("Size DNE\n");
		return NULL;
	}
	ret->memory = mmap(0,ret->size, PROT_READ, MAP_SHARED,ret->md,0);
	if (ret->memory == MAP_FAILED) {
		printf("error on mmap\n");
		exit(1);
	}

	return ret;
}

int destroy_shared_memory(shared_t* memory){
	int err;
	err = munmap(memory->memory, memory->size);
	if (err) {
		printf("Error unmapping the memory-dest\n");
		return err;
	}
	err = shm_unlink(memory->designator);
	if (err) {
		printf("Error unlinking the memory\n");
		return err;
	}
	free(memory);
	return err;
}

int unmap_shared_memory(shared_t* memory){
	int err;
	err = munmap(memory->memory,memory->size);
	if (err){
		printf("Error unmapping the memory-umap\n");
		return err;
	}
	close(memory->md);
	free(memory);
	return err;
}
