#ifndef __TWIDDLE_H__
#define __TWIDDLE_H__
struct cnum {
	float real;
	float im;
};
struct cnum twiddle(float real, float im, uint16_t k, uint16_t b);
#endif /* __TWIDDLE_H__ */
