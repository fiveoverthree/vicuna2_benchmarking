#ifndef MATMUL_HPP
#define MATMUL_HPP
#include <cstdint>

#include "vicuna_crt.h"
#include "terminate_benchmark.h"
#include "uart.h"

void matmul(int8_t * pSrcA, int8_t * pSrcB, int32_t * pDst, uint32_t numColsA, uint32_t numRowsA, uint32_t numColsB);

#endif
