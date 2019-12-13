/*
 * stream_grabber.c
 *
 *  Created on: Nov 10, 2017
 *      Author: davidmc
 */

#include <stdint.h>
#include "xparameters.h"
#include "sample.h"

static volatile uint32_t* const reg_start =
		(uint32_t*)(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR);
static volatile uint32_t* const reg_samples =
		(uint32_t*)(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR);
static volatile uint32_t* const read_addr =
		(uint32_t*)(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR+4);
static volatile int32_t* const read_val =
		(int32_t*)(XPAR_MIC_BLOCK_STREAM_GRABBER_0_BASEADDR+8);

static uint8_t fresh_sample = 0; // determines if sample has been read from before

void sample_start(){
	fresh_sample = 1;
	*reg_start = 0; //Value doesn't matter, the write does the magic
}

uint16_t sample_n() {
	if(fresh_sample)
		return *reg_samples;
	else
		return 0x0000;
}

int32_t sample_read(uint16_t i)
{
	fresh_sample = 0;
	*read_addr = i;
	return *read_val;
}

uint8_t sample_get(float* q, uint16_t samples, uint8_t decimation) {
	/** copies the requested number of samples with appropriate decimation
	 * into float array. returns 0x00 on succeess, if 0xFF then sampling is
	 * incomplete.
	 **/
	if(!fresh_sample || *reg_samples < samples) return 0xFF;
	int qi = 0;
	for(int i = 0; i < samples; i += decimation) {
		uint32_t sum = 0;
		for(int d = i; d < i + decimation && i < samples; d++) {
			sum += sample_read(d);
		}
		q[qi] = 3.3 * sum / (67108864.0 * decimation);
		qi++;
	}
	return 0x00;
}
