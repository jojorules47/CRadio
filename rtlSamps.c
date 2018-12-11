#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//#include <math.h>
#include <signal.h>
#include <rtl-sdr.h>
#include "getSamps.h"
#include "tuner.h"

//typedef struct rtlsdr_dev rtlsdr_dev_t;
static int do_exit = 0;
static rtlsdr_dev_t *dev = NULL;

void sig_handler(int signo){
	if (signo==SIGINT){
		printf("Received SIGINT\n");
		do_exit = 1;
	}
}

void sdr_setup(tuner_info_t* tuner_info){
	uint32_t samp_rate = 2000000;
	uint32_t freq = 162000000;
	int gmode = 0; //manual: 0 auto: 1
	int gain = 290;
	uint32_t bw = 1400000;
	int error;

	tuner_info->freq = freq;
	tuner_info->sample_rate = samp_rate;
	tuner_info->gain = gain;

	error = rtlsdr_set_sample_rate(dev, samp_rate); //ret 0
	if(error!=0){
		printf("Error setting sample rate\n");
	}
	error = rtlsdr_set_center_freq(dev, freq);
	if(error){
		printf("Error tuning device\n");
	}

	error = rtlsdr_set_tuner_gain_mode(dev, gmode); //ret 0
	if(error!=0){
		printf("Error setting gain mode\n");
	}
	
	error = rtlsdr_set_tuner_gain(dev, gain); //ret 0
	if(error!=0){
		printf("Error setting gain\n");
	}
	
	error = rtlsdr_set_tuner_bandwidth(dev, bw); //ret 0 if error
	if(error){
		printf("Error setting bandwidth\n");
	}
}

int main() {
	uint32_t index;
	int num_e, r, n_read;
	uint32_t def_buf_len = (16 * 16384);
	//uint8_t buffer[def_buf_len];
	const char* shm_name = "mem";
	uint32_t shm_size = 16 * 32 * 16384;
	shared_t* shm_memory;
	tuner_info_t* tuner_info;
	char* memory;
	int cnt = 0;

	//index = rtlsdr_get_device_count();
        index = 0;
	if(index < 0){
		printf("Error setting device index\n");
	}

	//const char *name = rtlsdr_get_device_name(index);
	
	r = rtlsdr_open(&dev, index);
	if(r<0){
		printf("Failed to open rtlsdr device\n");
		exit(1);
	}
	shm_memory = init_shared_memory(shm_name, sizeof(tuner_info_t));
	tuner_info = shm_memory->memory;
	memory = (char*) tuner_info->buffer;
	tuner_info->sample_num = 0;

	sdr_setup(tuner_info);
	rtlsdr_reset_buffer(dev);
	num_e = 0;
	int samp_read_length = def_buf_len / sizeof(complex_char);

	//typedef void(*rtlsdr_read_async_cb_t)(unsigned char *buf, uint32_t len, void *ctx);
	while(!do_exit){
		/*rtlsdr_read_async(dev,
				  &cb, //callback function
				  NULL, //void *ctx
				  0, //buff num
				  def_buf_len); //buff len*/
		r = rtlsdr_read_sync(dev, &memory[cnt], def_buf_len, &n_read);
		if(r<0){
			printf("Sync read failed\n");
			break;
		}
		if((uint32_t)n_read < def_buf_len){
			num_e++;
			fprintf(stderr,"Short read, samples lost, %d\n",num_e);
			//break;
		}
		tuner_info->sample_num += samp_read_length;
		cnt += def_buf_len;
		cnt = cnt % shm_size;
		signal(SIGINT,sig_handler);
	}
	destroy_shared_memory(shm_memory);
	//unmap_shared_memory(shm_memory);
	rtlsdr_close(dev);
}
