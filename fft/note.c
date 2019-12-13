#include "note.h"
//#include "lcd.h"

//array to store note names for findNote
static char notes[12][3]={"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

//finds and prints note of frequency and deviation from note
note_data findNote(float f) {
	float c=261.63;
	float r;
	int oct=4;
	int note=0;
	note_data data;

	//determine which octave frequency is in
	if(f >= c) {
		while(f > c*2) {
			c=c*2;
			oct++;
		}
	}
	else { //f < C4
		while(f < c) {
			c=c/2;
			oct--;
		}
	
	}

	//find note below frequency
	//c=middle C
	r=c*root2;
	while(f > r) {
		c=c*root2;
		r=r*root2;
		note++;
	}

	//determine which note frequency is closest to
	if((f-c) <= (r-f)) { //closer to left note
		data.cents = (int) (f - c + 0.5) / 100;
	}
	else { //f closer to right note
		note++;
		if(note >=12) note=0;
		data.cents = (int) (r - f + 0.5) / 100;
	}

	data.note = &(notes[note]);
	data.oct = oct;

	return data;
}
