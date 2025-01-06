#include <thread>
#include <functional>

#include "waveout.h"
#include "window.h"
//#include "component.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace Enola;

class MainComponent : public Window//主组件类，可以理解为把主窗口当成一个组件
{
private:
public:
};

class AudioProcessor
{
private:
	WaveOut wo;
	std::thread audioThread;
#define MaxNumSamplesOfBlock 4096	
	int numSamples = 512;
	float bufl[MaxNumSamplesOfBlock];
	float bufr[MaxNumSamplesOfBlock];

	int isClose = 0;
protected:
	void InitAudioProcessor()//waveout start
	{
		wo.Init();
		wo.Start();
		PrepareToPlay(wo.GetSampleRate(), numSamples);
		audioThread = std::thread([this]() {
			while (!isClose)
			{
				ProcessNextBlock(bufl, bufr, numSamples);
				wo.FillBuffer(bufl, bufr, numSamples);
			}
			});
	}
	void CloseAudioProcessor()//waveout stop
	{
		wo.Stop();
		wo.Close();
		isClose = 1;
		audioThread.join();
	}
public:
	void SetNumSamplesOfBlock(int numSamplesOfBlock)
	{
		numSamples = numSamplesOfBlock;
	}
	virtual void PrepareToPlay(int sampleRate, int numSamplesOfBlock)
	{
		DBG("sampleRate:%d numSamplesOfBlock:%d\n", sampleRate, numSamplesOfBlock);
	}
	virtual void ProcessNextBlock(float* wavbufl, float* wavbufr, int numSamples)
	{
		DBG("ProcessNextBlock\n");
	}
};

class EnolaAudioProcessor : public AudioProcessor
{
private:
	float t = 0;
	float dt = 440.0 / 48000;
public:
	void Init()
	{
		InitAudioProcessor();
	}
	void Close()
	{
		CloseAudioProcessor();
	}
	void PrepareToPlay(int sampleRate, int numSamplesOfBlock) override
	{
		DBG("EnolaAudioProcessor sampleRate:%d numSamplesOfBlock:%d\n", sampleRate, numSamplesOfBlock);
	}
	void ProcessNextBlock(float* wavbufl, float* wavbufr, int numSamples) override
	{
		for (int i = 0; i < numSamples; i++)
		{
			wavbufl[i] = sin(t * 2.0 * M_PI);
			wavbufr[i] = sin(t * 2.0 * M_PI);
			t += dt;
			t -= (int)t;
		}
	}
};


EnolaAudioProcessor wo;
int main()
{
	glfwInitHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
	if (!glfwInit()) {
		return -1; // 如果初始化失败，返回错误码
	}

	wo.Init();
	while (1);
	wo.Close();
}