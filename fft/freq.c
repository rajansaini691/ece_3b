#include "freq.h"
#include "sample.h"
#include "fft.h"

float q[512];
float w[512];
float freq_get() {
	if(sample_get(q, 512, 1)) return 0.00;
	for(int i = 0; i < 512; i++) w[i] = 0;
	return fft(q, w, 512, 12, 48828.125);
}
