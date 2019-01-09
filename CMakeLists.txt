cmake_minimum_required(VERSION 3.11.4)
project (c_sdr)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(rtlSamps rtlSamps.c getSamps.c)
target_link_libraries(rtlSamps -lrt -lrtlsdr)

add_executable(wstest websocket.c getSamps.c kiss_fft.c)
target_link_libraries(wstest -lrt -lm -lnanomsg)

add_executable(tuner tuner.c kiss_fft.c getSamps.c)
target_link_libraries(tuner -lrt -lm)

add_executable(demod demod.c kiss_fft.c getSamps.c)
target_link_libraries(demod -lrt -lm)
