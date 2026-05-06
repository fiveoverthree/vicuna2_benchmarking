#ifndef MATMUL_HPP
#define MATMUL_HPP
#include <cstdint>

#include "vicuna_crt.h"
#include "terminate_benchmark.h"
#include "uart.h"

void add( const int8_t * pSrcA, const int8_t * pSrcB, int16_t * pDst, uint32_t vecLen);

#endif
