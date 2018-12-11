#!/usr/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/jojo/cradio/gnuradio/src/gr-fm_debug/lib
export PATH=/home/jojo/cradio/gnuradio/src/gr-fm_debug/build/lib:$PATH
export LD_LIBRARY_PATH=/home/jojo/cradio/gnuradio/src/gr-fm_debug/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-fm_debug 
