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
#include "kiss_fft.h"

#define FILTER_TAP_NUM 33

static int do_exit = 0;

void sig_handler(int signo){
        if (signo==SIGINT){
                printf("\nReceived SIGINT\n");
                do_exit=1;
        }
}

int main() { 
	int cnt, buf_number, block_size, a; 
	const char* tune_name = "tuner";
	long samp_num;
	//int N = 65536;
	//int N2 = 32768;
	//int ifft_N = 1024;
	float real;
	float imag;
	shared_t* tune_memory;
	//complex_char* memory;

	tune_memory = read_shared_memory(tune_name, sizeof(kiss_t));
	kiss_t* tuner_s = (kiss_t*)tune_memory->memory;
	complex_float* tune_out = tuner_s->buff;

	//FILE *afile = fopen("afile.dat","wb");

	block_size = (TUNE_MEM_SIZE) / 32;
	long now = tuner_s->sample_num;
	long offset = now/block_size;
	now = offset*block_size;
	long next = now + block_size;
	samp_num = now;
	buf_number = 0;

	static double filter_taps[FILTER_TAP_NUM] = {-0.016253292838097254,
		-0.05743840964970983,
		-0.05596439344342629,
		0.0001530388985161355,
		0.03393232492900409,
		-0.006171498578367088,
		-0.03114962670706923,
		0.015513336012730857,
		0.03248162595456588,
		-0.02916823667891449,
		-0.034591216714420045,
		0.05143456613294535,
		0.03651811191114433,
		-0.09849574481007074,
		-0.03776111670143315,
		0.315807809989598,
		0.5383404961323581,
		0.315807809989598,
		-0.03776111670143315,
		-0.09849574481007074,
		0.03651811191114433,
		0.05143456613294535,
		-0.034591216714420045,
		-0.02916823667891449,
		0.03248162595456588,
		0.015513336012730857,
		-0.03114962670706923,
		-0.006171498578367088,
		0.03393232492900409,
		0.0001530388985161355,
		-0.05596439344342629,
		-0.05743840964970983,
		-0.016253292838097254};

	// this is for N/D decimation
	int N = 2*block_size;
	int D = 2*block_size;
	
	kiss_fft_cfg cfg;
	kiss_fft_cpx *cx_in = calloc(N,sizeof(kiss_fft_cpx));
	kiss_fft_cpx *cx_out = calloc(N,sizeof(kiss_fft_cpx));
	cfg = kiss_fft_alloc(N,0,NULL,NULL);
	
	kiss_fft_cfg icfg;
	kiss_fft_cpx *icx_in = calloc(D,sizeof(kiss_fft_cpx));
	kiss_fft_cpx *icx_out = calloc(D,sizeof(kiss_fft_cpx));
	icfg = kiss_fft_alloc(D,1,NULL,NULL);

	signal(SIGINT,sig_handler);
	float* data_out = calloc(D/2,sizeof(float));
	float* data = calloc(N,sizeof(float));

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
		//printf("On sample %ld\n", samp_num);
		long diff = samp_num - next;
		//if(diff>256000){
		//	printf("Falling behind\n");
		//}
		int start_offset = now % (TUNE_MEM_SIZE);
		memcpy(cx_in, &cx_in[N/2], N/2*sizeof(kiss_fft_cpx));
		for(cnt=0;cnt<N/2;cnt++){
			int idx = cnt + start_offset;
			complex_float data_cpx;
		        data_cpx.real = tune_out[idx].real * last.real - tune_out[idx].imag * last.imag;
			data_cpx.imag = tune_out[idx].real * last.imag + tune_out[idx].imag * last.real;
			cx_in[cnt + N/2].r = atan2(data_cpx.imag,data_cpx.real);
			cx_in[cnt + N/2].i = 0;
			//data[cnt] = atan2(data_cpx.imag,data_cpx.real);
			last.real = tune_out[idx].real;
			last.imag = -tune_out[idx].imag;
		}
		
		/*for(cnt=0;cnt<N/2;cnt++){
			for(a=0;a<FILTER_TAP_NUM;a++){
				data_out[cnt] += filter_taps[FILTER_TAP_NUM - 1 - a] * data[cnt + a];
			}
		}*/

		kiss_fft(cfg,cx_in,cx_out);
		memcpy(icx_in, cx_out, N*sizeof(kiss_fft_cpx));
		//memcpy(&icx_in[10], &cx_out[10], (D/2-10) * sizeof(kiss_fft_cpx));
		//memcpy(&icx_in[D/2-10], &cx_out[N-D/2-1-10], (D/2-10) * sizeof(kiss_fft_cpx));
		
		kiss_fft(icfg, icx_in, icx_out);
		
		for(cnt=0;cnt<D/2;cnt++){
			data_out[cnt] = icx_out[cnt+D/4].r/(float)(D/2);
			//if(!cnt) printf("Val is %f\n", data_out[cnt]);
		}

		fwrite(data_out, sizeof(float), D/2, stdout);
		//fflush(stdout);

		now += block_size;
		next = now + block_size;
	}
	//free(datar);
	//free(datai);
	free(data);
	free(data_out);
	free(cx_in);
	free(cx_out);
	free(icx_in);
	free(icx_out);
	//fclose(afile);
	return 0;
}
