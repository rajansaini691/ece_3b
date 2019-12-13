#ifndef __SAMPLE_H__
#define __SAMPLE_H__
#pragma once

#include <stdint.h>

void sample_start();
uint16_t sample_n();
int32_t sample_read(uint16_t i);
uint8_t sample_ready();
uint8_t sample_get(float* q, uint16_t samples, uint8_t decimation);
 
#endif /* __SAMPLE_H__ */
