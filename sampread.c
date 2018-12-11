#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include "getSamps.h"
#include "tuner.h"
#include "kiss_fft.h"

#define PI 3.14159265

static int do_exit = 0;

void sig_handler(int signo){
	if (signo==SIGINT){
		printf("Received SIGINT\n");
                do_exit = 1;
	}
}

typedef struct{
	int len;
	char* data;
} sequences;

int main(){
	int cnt;
	int filtcnt;
	const char* shm_name = "mem";
	shared_t* shm_memory;
	tuner_info_t* tuner_info;
	complex_char* memory;
	long samp_num;
	int window = 512;
	sequences *seqone;

	shm_memory = read_shared_memory(shm_name, sizeof(tuner_info_t));
	tuner_info = shm_memory->memory;
	memory = tuner_info->buffer;
	int buffer_size = sizeof(tuner_info->buffer) / sizeof(complex_char);
	samp_num = tuner_info->sample_num;
	long next = samp_num + 200000;

	kiss_fft_cfg cfg;
	kiss_fft_cpx *cx_in = calloc(window,sizeof(kiss_fft_cpx));
	kiss_fft_cpx *cx_out = calloc(window,sizeof(kiss_fft_cpx));

	cfg = kiss_fft_alloc(window,0,NULL,NULL);

	char* filt = calloc(window,sizeof(char));
	for(cnt=0;cnt<window;cnt++){
		if(cnt>100){
			filt[cnt] = 0; //create lowpass freq response
		}
		else{
			filt[cnt] = 1;
		}
		double K = atof(&filt[cnt]);
		filt[cnt] = (char) ((1 / window) * sin(PI * cnt * K / window) / sin(PI * cnt / window)); // re-arranged IDFT creates impulse response (Lyons p179)
	}
	seqone = malloc(sizeof(seqone)+ window + sizeof(filt));
	seqone->len = window;
	seqone->data = filt;

	float *datar = calloc(window,sizeof(float));
	float *datai = calloc(window,sizeof(float));

	while(do_exit==0){
		while(samp_num < next){
			usleep(50000);
			samp_num = tuner_info->sample_num;
		}
		int end_idx = (int) (samp_num % buffer_size);
		int start_idx = end_idx - window; //if neg do two memcpys
		for(cnt = 0; cnt<window;cnt++){
			int idx = start_idx + cnt;
			if (idx<0) idx += buffer_size;
			datar[cnt] = (float) (memory[idx].real-127.5f);
			datai[cnt] = (float) (memory[idx].imag-127.5f);
		}
		//filter
		for(cnt=0;cnt<window;cnt++){
			datar[cnt] += seqone->data[cnt] * &datar[samp_num-cnt]; //W.I.P.
		}

		//decimate
		//demodulate
		next+=200000;
		signal(SIGINT,sig_handler);
	}

	free(cfg);
	free(cx_in);
	free(cx_out);
	free(datar);
	free(datai);
	free(filt);
	
	return 0;
}
