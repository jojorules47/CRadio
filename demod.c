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
#include "filter_taps.h"

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
	float* data_out = calloc(N/4,sizeof(float));
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
		//memcpy(cx_in, &cx_in[N/2], N/2*sizeof(kiss_fft_cpx));
		memcpy(data, &data[N/2], N/2*sizeof(float));
		for(cnt=0;cnt<N/2;cnt++){
			int idx = cnt + start_offset;
			complex_float data_cpx;
		        data_cpx.real = tune_out[idx].real * last.real - tune_out[idx].imag * last.imag;
			data_cpx.imag = tune_out[idx].real * last.imag + tune_out[idx].imag * last.real;
			//cx_in[cnt + N/2].r = atan2(data_cpx.imag,data_cpx.real);
			//cx_in[cnt + N/2].i = 0;
			data[cnt + N/2] = atan2(data_cpx.imag,data_cpx.real);
			last.real = tune_out[idx].real;
			last.imag = -tune_out[idx].imag;
		}
		
		for(cnt=0;cnt<N/2;cnt+=2){
			        data_out[cnt/2] = filter_taps[0] * data[cnt];
				for(a=1;a<FILTER_TAP_NUM;a++){
					data_out[cnt/2] += filter_taps[a] * data[cnt + a];
				}
				data_out[cnt/2] = data_out[cnt/2]/2.0f;
			//data_out[cnt/2] = data[cnt];
		} //Add decimation of 4

		/*kiss_fft(cfg,cx_in,cx_out);
		memcpy(icx_in, cx_out, N*sizeof(kiss_fft_cpx));
		//memcpy(&icx_in[10], &cx_out[10], (D/2-10) * sizeof(kiss_fft_cpx));
		//memcpy(&icx_in[D/2-10], &cx_out[N-D/2-1-10], (D/2-10) * sizeof(kiss_fft_cpx));
		
		kiss_fft(icfg, icx_in, icx_out);
		
		for(cnt=0;cnt<D/2;cnt++){
			data_out[cnt] = icx_out[cnt+D/4].r/(float)(D/2);
			//if(!cnt) printf("Val is %f\n", data_out[cnt]);
		} */

		fwrite(data_out, sizeof(float), N/4, stdout);
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
