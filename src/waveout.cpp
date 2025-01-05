#include "waveout.h"
#include "dbg.h"

template <typename T>
void WaveOut::_FillBuffer(const float* bufferL, const float* bufferR, int numSamples) {}
template <>
void WaveOut::_FillBuffer<float>(const float* bufferL, const float* bufferR, int numSamples)
{
	BYTE* buffer = NULL;
	while (buffer == NULL)hr = renderService->GetBuffer(numSamples, &buffer);
	HResultCheckFailed(hr);
	for (int i = 0, j = 0; i < numSamples; ++i, j += 2)
	{
		((float*)buffer)[j + 0] = bufferL[i];
		((float*)buffer)[j + 1] = bufferR[i];
	}
	hr = renderService->ReleaseBuffer(numSamples, 0);
	HResultCheckFailed(hr);
}
template <>
void WaveOut::_FillBuffer<int16_t>(const float* bufferL, const float* bufferR, int numSamples)
{
	BYTE* buffer = NULL;
	while (buffer == NULL)hr = renderService->GetBuffer(numSamples, &buffer);
	HResultCheckFailed(hr);
	const float ToInt16 = (1 << 15);//û��limit����ʧ����Ϊ���ۣ�С�ı�
	for (int i = 0; i < numSamples << 1; i += 2)
	{
		((int16_t*)buffer)[i + 0] = bufferL[i] * ToInt16;
		((int16_t*)buffer)[i + 1] = bufferR[i] * ToInt16;
	}
	hr = renderService->ReleaseBuffer(numSamples, 0);
	HResultCheckFailed(hr);
}

WaveOut::WaveOut()
{
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HResultCheckFailed(hr);

	hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator,
		NULL,
		CLSCTX_ALL,
		IID_IMMDeviceEnumerator,
		(void**)&enumerator
	);
	HResultCheckFailed(hr);


	hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &renderer);
	HResultCheckFailed(hr);

	hr = enumerator->Release();
	HResultCheckFailed(hr);

	hr = renderer->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&renderClient);
	HResultCheckFailed(hr);

	hr = renderClient->GetMixFormat(&format);//����ģʽ��format�ǹ̶���
	HResultCheckFailed(hr);

	printf("SampleRate:%d\n", format->nSamplesPerSec);
	printf("Channel:%d\n", format->nChannels);
	printf("Bit:%d\n", format->wBitsPerSample);
	printf("FrameSize:%d\n", format->nBlockAlign);
}
WaveOut::~WaveOut()
{

}

void WaveOut::Init()//����ģʽ�Ĳ��˲����ʣ������ð�
{
	hr = renderClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 1 << 24, 0, format, NULL);
	HResultCheckFailed(hr);

	hr = renderClient->GetService(IID_IAudioRenderClient, (void**)&renderService);
	HResultCheckFailed(hr);
}

void WaveOut::Start()
{
	hr = renderClient->Start();
	HResultCheckFailed(hr);
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
	HResultCheckFailed(hr);
}
void WaveOut::Close()
{
	renderClient->Stop();
	renderService->Release();
	renderClient->Release();
	renderer->Release();
	CoUninitialize();
}