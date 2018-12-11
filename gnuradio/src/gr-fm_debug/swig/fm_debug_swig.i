/* -*- c++ -*- */

#define FM_DEBUG_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "fm_debug_swig_doc.i"

%{
#include "fm_debug/shared_tuner.h"
%}


%include "fm_debug/shared_tuner.h"
GR_SWIG_BLOCK_MAGIC2(fm_debug, shared_tuner);
