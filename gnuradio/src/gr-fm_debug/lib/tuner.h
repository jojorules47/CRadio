#ifndef __TUNER_H__
#define __TUNER_H__

#include <stdint.h>
//#include "kiss_fft.h"
#define SAMP_SIZE 4194304 // (16*32*16384)/sizeof(complex_char)
#define TUNE_MEM_SIZE 262144

typedef struct{
	unsigned char real;
	unsigned char imag;
} complex_char;

typedef struct {
	float real;
	float imag;
} complex_float;

typedef struct {
	uint32_t freq;
	uint32_t sample_rate;
	int gain;
	long sample_num;
	complex_char buffer[SAMP_SIZE];
} tuner_info_t;

typedef struct {
	long sample_num;
	complex_float buff[TUNE_MEM_SIZE];
} kiss_t;

#endif //__TUNER_H__
