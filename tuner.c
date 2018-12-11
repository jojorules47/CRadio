#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include "kiss_fft.h"
#include "getSamps.h"
#include "tuner.h"

//#define TUNE_MEM_SIZE 262144

/*typedef struct{
	long sample_num;
	complex_float buff[TUNE_MEM_SIZE];
} kiss_t; */

static int do_exit = 0;

void sig_handler(int signo){
	if (signo==SIGINT){
		printf("\nReceived SIGINT\n");
		do_exit=1;
	}
}

int* genfilt(int size,int taps){ //int sample_rate, int shift
	//int zeroes = size - taps;
	int cnt;
	int* filt = calloc(size,sizeof(int));
	for(cnt=0;cnt<size;cnt++){
		if(cnt<taps){
			filt[cnt] = 1;
		}
		else{
			filt[cnt] = 0;
		}
	}

	//filt = filt *  e^(-j2PI*shift/sample_rate)

	return filt;
}	

int main() {
	int cnt, buf_number;
	const char* shm_name = "mem";
	const char* tune_name = "tuner";
	int ifft_N = 2048; // bin size to ~64k, or radio location as power of 2
	//int filt_size = 32768;
	//int taps = 109;
	uint32_t shm_size;
	long samp_num;
	//int* filt;
	const int N = 65536; // 2^16
	const int N2 = 32768; // N2 is devisible into SAMP_SIZE by 128
	shared_t* shm_memory;
	shared_t* tune_memory;
	tuner_info_t* tuner_info;
	complex_char* memory;

	shm_memory = read_shared_memory(shm_name,sizeof(tuner_info_t));
	tuner_info = shm_memory->memory;
	
	int buffer_size = sizeof(tuner_info->buffer) / sizeof(complex_char);
	
	tune_memory = init_shared_memory(tune_name, sizeof(kiss_t));
	kiss_t* tuner_s = (kiss_t*)tune_memory->memory;
	tuner_s->sample_num = 0;
	kiss_fft_cpx* tune_out = (kiss_fft_cpx*) tuner_s->buff;
	
	float* data = calloc(N, sizeof(tuner_info_t));

	kiss_fft_cfg cfg;
	kiss_fft_cpx *cx_in = calloc(N,sizeof(kiss_fft_cpx));
	kiss_fft_cpx *cx_out = calloc(N,sizeof(kiss_fft_cpx));

	cfg = kiss_fft_alloc(N,0,NULL,NULL);

	kiss_fft_cfg icfg;
	kiss_fft_cpx *icx_in = calloc(ifft_N,sizeof(kiss_fft_cpx));
	kiss_fft_cpx *icx_out = calloc(ifft_N,sizeof(kiss_fft_cpx));
	
	icfg = kiss_fft_alloc(ifft_N,1,NULL,NULL);

	long now = tuner_info->sample_num;
	long offset = now/N2;
	now = offset*N2;
	long next = now + N2;
	samp_num = now;
	memory = tuner_info->buffer;
	buf_number = 0;

	signal(SIGINT,sig_handler);
	int tuner_index = 0;

	// tuner frequency calculations
	float rf_freq = 162e6f;
	float desired_freq = 162.473e6f;
	float freq_offset = desired_freq - rf_freq;
	float hz_per_bin = 2e6f / (float)N;
	// this is valid as long as center is greater than tune
	// and less than samp rate / 2
	int center_bin = (int)(freq_offset / hz_per_bin);
	int start_bin = center_bin - ifft_N/4;
	
	while(!do_exit){
		while (samp_num < next) {
		    samp_num = tuner_info->sample_num;
		    usleep(10);
		    if(do_exit==1){
			    break;
		    }
		}
		long diff = samp_num - next;
		if(diff>256000){
			printf("Falling behind\n");
		}
		int bpos = now % SAMP_SIZE;
		if(!bpos){
			printf("Still running %ld %ld\n",diff,now);
		}
		
		for (cnt = 0; cnt < N2; cnt++) {
			cx_in[cnt+N2].r = (float) (memory[bpos+cnt].real-127.5f);
			cx_in[cnt+N2].i = (float) (memory[bpos+cnt].imag-127.5f);
		}

		kiss_fft(cfg,cx_in,cx_out);
		memcpy(cx_in, &cx_in[N2], N2*sizeof(kiss_fft_cpx));
		
		memset(icx_in, 0, ifft_N*sizeof(kiss_fft_cpx));

		memcpy(&icx_in[ifft_N/4], &cx_out[start_bin],ifft_N/2*sizeof(kiss_fft_cpx));
		kiss_fft(icfg,icx_in,icx_out);
		for(cnt=0;cnt<ifft_N;cnt+=2){
			icx_out[cnt].r = -icx_out[cnt].r;
			icx_out[cnt].i = -icx_out[cnt].i;
		}
		memcpy(&tune_out[tuner_index],&icx_out[ifft_N/4],ifft_N/2*sizeof(kiss_fft_cpx));
		now += N2;
		next = now + N2;
		tuner_index += ifft_N/2;
		tuner_s->sample_num += ifft_N/2;
		tuner_index = tuner_index % TUNE_MEM_SIZE; 
	}
	destroy_shared_memory(tune_memory);
	///free(filt);
	free(cx_in);
	free(cx_out);
	free(icx_in);
	free(icx_out);
	return 0;
}
