#include "sample.h"
#include "fft.h"


float oct_low[10] = {0.03716272,  0.07432544,  0.14865089,  0.29730178,  0.59460356,
	                 1.18920712,  2.37841423,  4.75682846,  9.51365692, 19.02731384};
float oct_high[10]= {0.07015388,  0.14030776,  0.28061551,  0.56123102,  1.12246205,
	                 2.2449241 ,  4.48984819,  8.97969639, 17.95939277, 35.91878555};
static uint8_t octave = 3;
static uint16_t a_freq = 440;


void set_octave(uint8_t o)
{
	octave = o;
}
void set_a(uint16_t a)
{
	a_freq = a;
}


float q[512];
float w[512];
float freq_get() {
	if(sample_get(q, 512, 1)) return 0.00;
	for(int i = 0; i < 512; i++) w[i] = 0;
	// return fft(q, w, 512, 9, 48828.125, oct_low[octave] * a_freq, oct_high[octave] * a_freq);
	return fft(q, w, 512, 9, 48828.125, 100, 1000);

}
