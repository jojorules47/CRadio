/* 
 * Copyright 2018 gr-fm_debug author.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <gnuradio/io_signature.h>
#include "shared_tuner_impl.h"


namespace gr {
  namespace fm_debug {

    shared_tuner::sptr
    shared_tuner::make()
    {
      return gnuradio::get_initial_sptr
        (new shared_tuner_impl());
    }

    /*
     * The private constructor
     */
    shared_tuner_impl::shared_tuner_impl()
      : gr::sync_block("shared_tuner",
              gr::io_signature::make(0,0, 0),
              gr::io_signature::make(0, 0, sizeof(gr_complex)))
    {
      std::string tune_name = "tuner";
      shared_t* tune_memory = read_shared_memory(tune_name.c_str(), sizeof(kiss_t));
      tuner_s = (kiss_t*)tune_memory->memory;
      tune_out = (gr_complex*)tuner_s->buff;
      int block_size = (TUNE_MEM_SIZE) / 16;
      now = tuner_s->sample_num;
      offset = now/block_size;
      now = offset*block_size;
    }

    /*
     * Our virtual destructor.
     */
    shared_tuner_impl::~shared_tuner_impl()
    {
    }

    int
    shared_tuner_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *out = (gr_complex *) output_items[0];
      long samp_num = tuner_s->sample_num;
      while (samp_num < now + noutput_items){
			samp_num = tuner_s->sample_num;
			usleep(10);
      }
      long diff = samp_num - now;
      if(diff>256000){
	printf("Falling behind\n");
      }
      int start_offset = now % (TUNE_MEM_SIZE);
      if (start_offset + noutput_items > (TUNE_MEM_SIZE)) {
	     //memcpy for edge cases 
	     int end_number = (TUNE_MEM_SIZE) - start_offset;
	     memcpy(out, &tune_out[start_offset], end_number*sizeof(gr_complex));
	     memcpy(&out[end_number], tune_out, (noutput_items - end_number)*sizeof(gr_complex));
      } else {
	      memcpy(out, &tune_out[start_offset], noutput_items*sizeof(gr_complex));
      }

      now += noutput_items;


      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace fm_debug */
} /* namespace gr */

