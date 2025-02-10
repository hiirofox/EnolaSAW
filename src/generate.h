#pragma once

#include <math.h>

class VCO { //这个vco有毛病肿么办qwq
private:
	double t;
public:
	double WaveTable(double f, double wt) {
		t += f;
		if (wt < 1.0)
			return (1.0 - wt) * sin(t) + (wt)*asin(sin(t));
		else if (wt < 2.0)
			return (2.0 - wt) * asin(sin(t)) + (wt - 1.0) * (sin(t) > 0 ? 1 : -1);
		else
			return (3.0 - wt) * (sin(t) > 0 ? 1 : -1) + (wt - 2.0) * atan(tan(t * 0.5));

		return 0;
	}
	void reset() {
		t = 0;
	}
};

double quicknote(int name, int octave, double offset) {
	int nn=0;
	switch (name) {
	case 1:nn = 0; break;
	case 2:nn = 2; break;
	case 3:nn = 4; break;
	case 4:nn = 5; break;
	case 5:nn = 7; break;
	case 6:nn = 9; break;
	case 7:nn = 11; break;
	}
	nn += octave * 12 + offset;
	return nn;
}
class NOTE {
public:
	unsigned char note; //44.0 = A3
	unsigned char velo; //velocity 0~255
	unsigned short ins;
	unsigned short begn; //tick begin
	unsigned short dura; //tick mount
};

double note2freq(double note) {
	return 440.0 * pow(2.0, 1.0 / 12 * (note - 44));
}
//name音名 octave八度 offset升降+微分

#define BufferNumSamples 1024
#define M_PI 3.1415926535897932384626
class INST {  //亟待改进（抛弃）的合成器
private:
	double freq;
	double velocity;
	double attack;
	double release;
	double t;
	double v;
	int mode;
	int state;
	float te;
public:
	void press() {
		state = 1;
	}
	void remov() {
		state = 0;
	}
	void init(int m) {
		v = 0;
		mode = m;
		state = 0;
		attack = 0.1;
		release = 0.00008;
	}
	void SetEnv(double a, double r) {
		attack = a;
		release = r;
	}
	void SetMode(int m) {
		mode = m;
	}
	void in(NOTE ni) {
		freq = note2freq(ni.note);
		velocity = 1.0 * ni.velo/16;
	}
	void treset() {
		t = 0;
	}
	double proc(int sampleRate) {
		if (state == 1) {
			v += attack;
			if (v >= 1)v = 1;
		}
		if (state == 0) {
			v -= release;
			if (v <= 0)v = 0;
		}
		t += freq / 2 / sampleRate;
		t -= (int)t;
		te = 0;
		switch (mode) {
		case 0: {
			te = cosf(t * 2.0 * M_PI);
			break;
		}
		case 1: {
			te = cosf(t * 2.0 * M_PI) / abs(cosf(t * 2.0 * M_PI));
			break;
		}
		}
		return v * velocity * te;
	}
};