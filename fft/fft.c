#include "fft.h"
#include "twiddle.h"

static float new_[512];
static float new_im[512];
#define BIN 95.367431640625

// n - 512 (buffer size)
float fft(float* q, float* w, int n, int m, float sample_f, float low, float high) {
	int a,b,r,d,e,c;
	int k,place;
	a=n/2;
	b=1;
	int i,j;
	float real=0,imagine=0;
	float max,frequency;

	// Ordering algorithm
	for(i=0; i<(m-1); i++){
		d=0;
		for (j=0; j<b; j++){
			for (c=0; c<a; c++){	
				e=c+d;
				new_[e]=q[(c*2)+d];
				new_im[e]=w[(c*2)+d];
				new_[e+a]=q[2*c+1+d];
				new_im[e+a]=w[2*c+1+d];
			}
			d+=(n/b);
		}
		for (r=0; r<n;r++){
			q[r]=new_[r];
			w[r]=new_im[r];
		}
		b*=2;
		a=n/(2*b);
	}
	//end ordering algorithm

	b=1;
	k=0;
	for (j=0; j<m; j++){	
	//MATH
		for(i=0; i<n; i+=2){
			if (i%(n/b)==0 && i!=0)
				k++;
			struct cnum tw = twiddle(q[i+1], w[i+1], k, b);
			new_[i] = q[i] + tw.real;
			new_im[i] = w[i] + tw.im;
			new_[i+1] = q[i] - tw.real;
			new_im[i+1] = w[i] - tw.im;

		}
		for (i=0; i<n; i++){
			q[i]=new_[i];
			w[i]=new_im[i];
		}
	//END MATH

	//REORDER
		for (i=0; i<n/2; i++){
			new_[i]=q[2*i];
			new_[i+(n/2)]=q[2*i+1];
			new_im[i]=w[2*i];
			new_im[i+(n/2)]=w[2*i+1];
		}
		for (i=0; i<n; i++){
			q[i]=new_[i];
			w[i]=new_im[i];
		}
	//END REORDER	
		b*=2;
		k=0;		
	}

	//find magnitudes
	max=0;
	place=1;
	int low_bin = (int) ((low + .5)/ BIN);
	int high_bin = (int) ((high + .5)/ BIN);
	for(i=low_bin; i<high_bin && i<(n/2); i++) {
		new_[i]=q[i]*q[i]+w[i]*w[i];
		if(max < new_[i]) {
			max=new_[i];
			place=i;
		}
	}
	

	frequency = BIN*place;

	//curve fitting for more accuarcy
	//assumes parabolic shape and uses three point to find the shift in the parabola
	//using the equation y=A(x-x0)^2+C
	float y1=new_[place-1],y2=new_[place],y3=new_[place+1];
	float x0=BIN+(2*BIN*(y2-y1))/(2*y2-y1-y3);
	x0=x0/BIN-1;

	if(x0 <0 || x0 > 2) { //error
		xil_printf("Quadratic fit error!\r\n");
		return -1;
	}
	if(x0 <= 1)  {
		frequency=frequency-(1-x0)*BIN;
	}
	else {
		frequency=frequency+(x0-1)*BIN;
	}
	
	return frequency;
}
