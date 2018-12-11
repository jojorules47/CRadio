#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>
#include "nanomsg/nn.h"
#include "nanomsg/pubsub.h"
#include "nanomsg/ws.h"
#include "getSamps.h"
#include "tuner.h"
#include "kiss_fft.h"

static int do_exit = 0;

void sig_handler(int signo){
	if (signo==SIGINT){
	 	printf("Received SIGINT\n");
		do_exit = 1;
	}
}

int fillBuffer(char* buf, int max_size, float* data, int numpts) {
    int cnt = 0;
    int idx = 0;
    //idx += sprintf(&buf[idx], "{\"x\":0,\"y\",1}");
    //return idx;

    idx += sprintf(&buf[idx], "{\"x\":[");

    for (cnt = 0; cnt < numpts-1; cnt++) {
        idx += sprintf(&buf[idx], "%d,", cnt);
        if (idx >= max_size - 100) {
            return idx;
        }
    }
    idx += sprintf(&buf[idx], "%d],\"y\":[", cnt);
    if (idx >= max_size - 100) {
        return idx;
    }
 
    for (cnt = 0; cnt < numpts-1; cnt++) {
        idx += sprintf(&buf[idx], "%f,", data[cnt]);
    }
    idx += sprintf(&buf[idx], "%f]}", data[cnt]);
    if (idx >= max_size - 100) {
        return idx;
    }
 
    return idx;
}

int plotData(int sock, float* data, int numpts) {
    char buf[10000];
    int sz_d = fillBuffer(buf, 10000, data, numpts);
    if (sz_d >= 10000 - 100) {
        return -1;
    }
    //printf("SERVER: PUBLISHING DATE %s\n", buf);
    int bytes = nn_send(sock, buf, sz_d, 0);
    if (bytes < 0) {
        fprintf(stderr, "Could not send data");
        return -2;
    }
    return 0;
}

int* genfilter(int N){
	int cnt;
	int *ret = (int*)malloc(N*sizeof(int));
	for(cnt=0;cnt<N;cnt++){
		if(cnt<50 || cnt>100){
			*(ret + cnt) = 0;
		}
		else{
			*(ret + cnt) = 1;
		}
	}
	return ret;
}



int main (int argc, const char *argv[]){
    	int rc;
    	int sock;
    	int N = 256;
    	int cnt;
	const char* shm_name = "mem";
	shared_t* shm_memory;
	tuner_info_t* tuner_info;
	complex_char* memory;
	long samp_num;
	unsigned int pcnt = 0;
	_Bool freq = 1;
	int* filt;

	/*  Open the socket anew. */
	sock = nn_socket (AF_SP, NN_PUB);

	rc = nn_bind (sock, "ws://0.0.0.0:3001");
	//nn_setsockopt (sock, NN_WS_MSG_TYPE, NN_WS_MSG_TYPE_TEXT, NULL, 0);

	shm_memory = read_shared_memory(shm_name, sizeof(tuner_info_t));
	tuner_info = shm_memory->memory;
	memory = tuner_info->buffer;
	int buffer_size = sizeof(tuner_info->buffer) / sizeof(complex_char);
	printf("%ld\n",buffer_size);
	samp_num = tuner_info->sample_num;
	long next = samp_num + 200000;
	
	kiss_fft_cfg cfg;
	kiss_fft_cpx *cx_in = calloc(N,sizeof(kiss_fft_cpx));
	kiss_fft_cpx *cx_out = calloc(N,sizeof(kiss_fft_cpx));
	
	cfg = kiss_fft_alloc(N,0,NULL,NULL);

	float *data = calloc(N, sizeof(float));
	filt = genfilter(N);

	while(1) {
		while(samp_num < next){
			usleep(50000);
			samp_num = tuner_info->sample_num;
		}
		int end_idx = (int) (samp_num % buffer_size);
		// if start idx is neg, do two memcpy's
		int start_idx = end_idx - N;
		for (cnt = 0; cnt < N; cnt++) {
			int idx = start_idx + cnt;
			if (idx<0) idx += buffer_size;
			if(freq==1){
				cx_in[cnt].r = (float) (memory[idx].real-127.5f);
				cx_in[cnt].i = (float) (memory[idx].imag-127.5f);
			}
			else{
			data[cnt] = (float) (memory[idx].real-127.5f);
			}
		}

	
	if(freq==1){
		kiss_fft(cfg,cx_in,cx_out);
		for(cnt=0;cnt<N;cnt++){
			float mag = cx_out[cnt].r * cx_out[cnt].r + cx_out[cnt].i * cx_out[cnt].i;
			//mag = mag * filt[cnt];
			data[cnt] = mag;
		}
	}
	int val = plotData(sock, data, N);
	if (val) {
		fprintf(stderr, "error sending data\n");
		break;
        }
	next += 200000;
	if (pcnt++>100){
		printf("Still getting samps\n");
		pcnt = 0;
	}
	//printf("test\n");
	signal(SIGINT,sig_handler);
    }
	free(cfg);
	free(cx_in);
	free(cx_out);
	free(data);
	return 0;
}

