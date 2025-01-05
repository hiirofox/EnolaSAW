#include "waveout.h"

template <typename T>
void WaveOut::_FillBuffer(const float* bufferL, const float* bufferR, int numSamples) {}
template <>
void WaveOut::_FillBuffer<float>(const float* bufferL, const float* bufferR, int numSamples)
{
	BYTE* buffer = NULL;
	while (buffer == NULL)hr = renderService->GetBuffer(numSamples, &buffer);
	assert(SUCCEEDED(hr));
	for (int i = 0, j = 0; i < numSamples; ++i, j += 2)
	{
		((float*)buffer)[j + 0] = bufferL[i];
		((float*)buffer)[j + 1] = bufferR[i];
	}
	hr = renderService->ReleaseBuffer(numSamples, 0);
	assert(SUCCEEDED(hr));
}
template <>
void WaveOut::_FillBuffer<int16_t>(const float* bufferL, const float* bufferR, int numSamples)
{
	BYTE* buffer = NULL;
	while (buffer == NULL)hr = renderService->GetBuffer(numSamples, &buffer);
	assert(SUCCEEDED(hr));
	const float ToInt16 = (1 << 15);//没有limit，损失音量为代价，小心别爆
	for (int i = 0; i < numSamples << 1; i += 2)
	{
		((int16_t*)buffer)[i + 0] = bufferL[i] * ToInt16;
		((int16_t*)buffer)[i + 1] = bufferR[i] * ToInt16;
	}
	hr = renderService->ReleaseBuffer(numSamples, 0);
	assert(SUCCEEDED(hr));
}

WaveOut::WaveOut()
{
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator,
		NULL,
		CLSCTX_ALL,
		IID_IMMDeviceEnumerator,
		(void**)&enumerator
	);
	assert(SUCCEEDED(hr));


	hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &renderer);
	assert(SUCCEEDED(hr));

	hr = enumerator->Release();
	assert(SUCCEEDED(hr));

	hr = renderer->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&renderClient);
	assert(SUCCEEDED(hr));

	hr = renderClient->GetMixFormat(&format);//共享模式的format是固定的
	assert(SUCCEEDED(hr));

	printf("SampleRate:%d\n", format->nSamplesPerSec);
	printf("Channel:%d\n", format->nChannels);
	printf("Bit:%d\n", format->wBitsPerSample);
	printf("FrameSize:%d\n", format->nBlockAlign);
}
WaveOut::~WaveOut()
{

}

void WaveOut::Init()//共享模式改不了采样率，将就用吧
{
	hr = renderClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 1 << 24, 0, format, NULL);
	assert(SUCCEEDED(hr));

	hr = renderClient->GetService(IID_IAudioRenderClient, (void**)&renderService);
	assert(SUCCEEDED(hr));
}

void WaveOut::Start()
{
	hr = renderClient->Start();
	assert(SUCCEEDED(hr));
}

int WaveOut::GetSampleRate()
{
	return format->nSamplesPerSec;
}

void WaveOut::FillBuffer(const float* bufferL, const float* bufferR, int numSamples)
{
	if (format->wBitsPerSample == 16) _FillBuffer<int16_t>(bufferL, bufferR, numSamples);
	if (format->wBitsPerSample == 32) _FillBuffer<float>(bufferL, bufferR, numSamples);
}

void WaveOut::Stop()
{
	hr = renderClient->Stop();
	assert(SUCCEEDED(hr));
}
void WaveOut::Close()
{
	renderClient->Stop();
	renderService->Release();
	renderClient->Release();
	renderer->Release();
	CoUninitialize();
}