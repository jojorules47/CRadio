#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
//#include "kiss_fft.h"
#include "getSamps.h"
#include "tuner.h"

static int do_exit = 0;

void sig_handler(int signo){
        if (signo==SIGINT){
                printf("\nReceived SIGINT\n");
                do_exit=1;
        }
}

int main() { 
	int cnt, buf_number, block_size; 
	const char* tune_name = "tuner";
	long samp_num;
	int N = 65536;
	int N2 = 32768;
	//int ifft_N = 1024;
	float real;
	float imag;
	shared_t* tune_memory;
	//complex_char* memory;

	tune_memory = read_shared_memory(tune_name, sizeof(kiss_t));
	kiss_t* tuner_s = (kiss_t*)tune_memory->memory;
	complex_float* tune_out = tuner_s->buff;

	FILE *afile = fopen("afile.dat","wb");

	block_size = (TUNE_MEM_SIZE) / 16;
	long now = tuner_s->sample_num;
	long offset = now/block_size;
	now = offset*block_size;
	long next = now + block_size;
	samp_num = now;
	buf_number = 0;

	signal(SIGINT,sig_handler);
	//int tuner_index = 0;
	//float* datar = calloc(block_size,sizeof(complex_float));
	//float* datai = calloc(block_size,sizeof(complex_float));
	float* data = calloc(block_size,sizeof(complex_float));
	complex_float last;
	last.real = 0;
	last.imag = 0;
	while(!do_exit){
		while (samp_num < next){
			samp_num = tuner_s->sample_num;
			usleep(10);
			if (do_exit==1){
				break;
			}
		}
		printf("On sample %ld\n", samp_num);
		long diff = samp_num - next;
		if(diff>256000){
			printf("Falling behind\n");
		}
		int start_offset = now % (TUNE_MEM_SIZE);
		for(cnt=0;cnt<block_size;cnt++){
			int idx = cnt + start_offset;
			complex_float data_cpx;
		       data_cpx.real = tune_out[idx].real * last.real - tune_out[idx].imag * last.imag;
			data_cpx.imag = tune_out[idx].real * last.imag + tune_out[idx].imag * last.real;
			data[cnt] = atan2(data_cpx.imag,data_cpx.real);
			last.real = tune_out[idx].real;
			last.imag = tune_out[idx].imag;
		}

		fwrite(data, sizeof(float), block_size, afile);
		fflush(afile);

		now += block_size;
		next = now + block_size;
	}
	//free(datar);
	//free(datai);
	free(data);
	fclose(afile);
	return 0;
}
