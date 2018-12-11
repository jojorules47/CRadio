/* -*- c++ -*- */
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

#ifndef INCLUDED_FM_DEBUG_SHARED_TUNER_IMPL_H
#define INCLUDED_FM_DEBUG_SHARED_TUNER_IMPL_H

#include <fm_debug/shared_tuner.h>
#include "getSamps.h"
#include "tuner.h"

namespace gr {
  namespace fm_debug {

    class shared_tuner_impl : public shared_tuner
    {
     private:
      // Nothing to declare in this block.
      kiss_t* tuner_s;
      gr_complex* tune_out;
      long now;
      long offset;
      gr_complex last;
     public:
      shared_tuner_impl();
      ~shared_tuner_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace fm_debug
} // namespace gr

#endif /* INCLUDED_FM_DEBUG_SHARED_TUNER_IMPL_H */

