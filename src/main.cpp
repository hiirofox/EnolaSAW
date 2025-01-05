#include "waveout.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define BufferNumSamples 256

float bufl[BufferNumSamples];
float bufr[BufferNumSamples];

WaveOut wo;
int main()
{
	wo.Init();
	wo.Start();
	int sampleRate = wo.GetSampleRate();
	float t = 0;
	float dt = 440.0 / sampleRate;
	for (;;)
	{
		for (int i = 0; i < BufferNumSamples; ++i)
		{
			bufl[i] = cosf(t * 2.0 * M_PI);
			bufr[i] = cosf(t * 2.0 * M_PI);
			t += dt;
			t -= (int)t;
		}
		wo.FillBuffer(bufl, bufr, BufferNumSamples);
	}
}