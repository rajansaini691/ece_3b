#include "sample.h"
#include "fft.h"

#define SAMPLE_FREQ 48828.125

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

#define N_ 512
#define M_ 9
float q[N_];
float w[N_];
float freq_get() {
	int decimation = (int) (SAMPLE_FREQ / (2 * oct_high[octave] * a_freq));
	if(decimation > 8) decimation = 8;
	if(octave < 4) decimation = 2;
	if(sample_get(q, N_ * decimation, decimation)) return 0.00;
	for(int i = 0; i < N_; i++) w[i] = 0;
	return fft(q, w, N_, M_, SAMPLE_FREQ /decimation, oct_low[octave] * a_freq, oct_high[octave] * a_freq);
}
