#ifndef FIR_HPP
#define FIR_HPP
#include <cstdint>

#include "vicuna_crt.h"
#include "terminate_benchmark.h"
#include "uart.h"

void fir( int16_t * input, int16_t * filter, int32_t * output, uint32_t  input_len, uint32_t  filter_len, uint32_t  filter_channels);

#endif
