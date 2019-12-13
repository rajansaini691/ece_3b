#include "twiddle.h"

#define PI 3.14159265
#define LUT_SIZE 320
// lookup table for values of sin from 0 to pi/2 in pi/80 increments
const float lut[LUT_SIZE+1] = {
		0.        , 0.00490872, 0.00981732, 0.01472568, 0.01963369,
		       0.02454123, 0.02944817, 0.03435441, 0.03925982, 0.04416428,
		       0.04906767, 0.05396989, 0.0588708 , 0.0637703 , 0.06866826,
		       0.07356456, 0.0784591 , 0.08335174, 0.08824237, 0.09313088,
		       0.09801714, 0.10290104, 0.10778246, 0.11266129, 0.1175374 ,
		       0.12241068, 0.127281  , 0.13214826, 0.13701234, 0.14187312,
		       0.14673047, 0.1515843 , 0.15643447, 0.16128086, 0.16612338,
		       0.17096189, 0.17579628, 0.18062644, 0.18545224, 0.19027357,
		       0.19509032, 0.19990237, 0.2047096 , 0.2095119 , 0.21430915,
		       0.21910124, 0.22388805, 0.22866946, 0.23344536, 0.23821564,
		       0.24298018, 0.24773886, 0.25249158, 0.25723821, 0.26197864,
		       0.26671276, 0.27144045, 0.2761616 , 0.2808761 , 0.28558383,
		       0.29028468, 0.29497853, 0.29966528, 0.3043448 , 0.30901699,
		       0.31368174, 0.31833893, 0.32298845, 0.32763018, 0.33226402,
		       0.33688985, 0.34150757, 0.34611706, 0.3507182 , 0.3553109 ,
		       0.35989504, 0.3644705 , 0.36903718, 0.37359497, 0.37814376,
		       0.38268343, 0.38721389, 0.39173501, 0.3962467 , 0.40074883,
		       0.40524131, 0.40972403, 0.41419687, 0.41865974, 0.42311251,
		       0.42755509, 0.43198737, 0.43640924, 0.44082059, 0.44522133,
		       0.44961133, 0.4539905 , 0.45835873, 0.46271592, 0.46706195,
		       0.47139674, 0.47572016, 0.48003212, 0.48433252, 0.48862124,
		       0.49289819, 0.49716327, 0.50141636, 0.50565737, 0.5098862 ,
		       0.51410274, 0.5183069 , 0.52249856, 0.52667764, 0.53084403,
		       0.53499762, 0.53913832, 0.54326604, 0.54738066, 0.55148209,
		       0.55557023, 0.55964499, 0.56370626, 0.56775395, 0.57178796,
		       0.57580819, 0.57981455, 0.58380693, 0.58778525, 0.59174941,
		       0.5956993 , 0.59963485, 0.60355594, 0.60746249, 0.61135441,
		       0.61523159, 0.61909395, 0.62294139, 0.62677382, 0.63059115,
		       0.63439328, 0.63818013, 0.6419516 , 0.6457076 , 0.64944805,
		       0.65317284, 0.6568819 , 0.66057513, 0.66425244, 0.66791374,
		       0.67155895, 0.67518798, 0.67880075, 0.68239715, 0.68597711,
		       0.68954054, 0.69308736, 0.69661748, 0.70013081, 0.70362727,
		       0.70710678, 0.71056925, 0.7140146 , 0.71744274, 0.7208536 ,
		       0.72424708, 0.72762312, 0.73098162, 0.73432251, 0.7376457 ,
		       0.74095113, 0.74423869, 0.74750833, 0.75075995, 0.75399348,
		       0.75720885, 0.76040597, 0.76358476, 0.76674516, 0.76988708,
		       0.77301045, 0.7761152 , 0.77920124, 0.78226851, 0.78531693,
		       0.78834643, 0.79135693, 0.79434836, 0.79732065, 0.80027373,
		       0.80320753, 0.80612197, 0.80901699, 0.81189252, 0.81474848,
		       0.81758481, 0.82040144, 0.82319831, 0.82597533, 0.82873246,
		       0.83146961, 0.83418673, 0.83688375, 0.83956061, 0.84221723,
		       0.84485357, 0.84746954, 0.85006509, 0.85264016, 0.85519469,
		       0.85772861, 0.86024186, 0.86273439, 0.86520612, 0.86765701,
		       0.87008699, 0.87249601, 0.874884  , 0.87725091, 0.87959669,
		       0.88192126, 0.88422459, 0.88650662, 0.88876728, 0.89100652,
		       0.8932243 , 0.89542056, 0.89759523, 0.89974828, 0.90187965,
		       0.90398929, 0.90607715, 0.90814317, 0.91018732, 0.91220953,
		       0.91420976, 0.91618796, 0.91814408, 0.92007808, 0.92198992,
		       0.92387953, 0.92574689, 0.92759194, 0.92941463, 0.93121493,
		       0.9329928 , 0.93474818, 0.93648104, 0.93819134, 0.93987902,
		       0.94154407, 0.94318642, 0.94480605, 0.94640291, 0.94797697,
		       0.94952818, 0.95105652, 0.95256194, 0.9540444 , 0.95550388,
		       0.95694034, 0.95835373, 0.95974404, 0.96111122, 0.96245524,
		       0.96377607, 0.96507367, 0.96634802, 0.96759909, 0.96882685,
		       0.97003125, 0.97121229, 0.97236992, 0.97350412, 0.97461487,
		       0.97570213, 0.97676588, 0.9778061 , 0.97882275, 0.97981582,
		       0.98078528, 0.98173111, 0.98265328, 0.98355177, 0.98442657,
		       0.98527764, 0.98610498, 0.98690855, 0.98768834, 0.98844433,
		       0.98917651, 0.98988485, 0.99056934, 0.99122996, 0.9918667 ,
		       0.99247953, 0.99306846, 0.99363345, 0.9941745 , 0.9946916 ,
		       0.99518473, 0.99565388, 0.99609903, 0.99652019, 0.99691733,
		       0.99729046, 0.99763955, 0.9979646 , 0.99826561, 0.99854256,
		       0.99879546, 0.99902428, 0.99922904, 0.99940971, 0.99956631,
		       0.99969882, 0.99980724, 0.99989157, 0.99995181, 0.99998795, 1
};

#define TAYLOR 0 // Degree of taylor expansion to use; 0, 1, or 2
#define PI2 1.57079632679

struct cnum twiddle(float real, float im, int k, int b) {
	if(k == 0) return (struct cnum) {real, 0};
	k = k % (2 * b); // regularize to within 2 pi
	k = (2 * b) - k; // make it a positive angle
	int q = (k * 2) / b; // determine quadrant
	// flip based on quadrant
	if (q == 1) k = b - k;
	else if (q == 2) k -= b;
	else if (q == 3) k = (2 * b) - k;

	int idx = (k * (LUT_SIZE*2)) / b; // find index in lut
	if((k % b) > (b / 2)) idx++; // round up

#if TAYLOR
	float sin_a = lut[idx]; // lookup in lut
	float cos_a = lut[LUT_SIZE-idx];
	k = (80 * k) % (b * idx);
	b *= 80;
	float angle = ((PI2 * k) / b);
#endif /* TAYLOR */
#if TAYLOR == 1
	float sin = sin_a + cos_a * angle;
	float cos = cos_a - sin_a * angle;
#elif TAYLOR == 2
	float sin_b = angle - (angle * angle * angle / 6);
	float cos_b = 1 - (angle * angle / 2);
	float sin = sin_a * cos_b + cos_a * sin_b;
	float cos = cos_a * cos_b - sin_a * sin_b;
#else /* TAYLOR == .*/
	float sin = lut[idx]; // lookup in lut
	float cos = lut[LUT_SIZE-idx];
#endif /* TAYLOR == .*/
	return (struct cnum) {real * cos - im * sin, real * sin + im * cos};
}
