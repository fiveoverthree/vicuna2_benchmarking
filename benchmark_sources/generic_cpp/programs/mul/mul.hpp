#ifndef MATMUL_HPP
#define MATMUL_HPP
#include <cstdint>

#include "vicuna_crt.h"
#include "terminate_benchmark.h"
#include "uart.h"

void mul( const int16_t * pSrcA, const int16_t * pSrcB, int32_t * pDst, uint32_t vecLen);

#endif
