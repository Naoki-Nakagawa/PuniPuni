#include "vtx.h"

namespace vtx
{
	//--------------------------------------------------
	// オーディオクラス
	//--------------------------------------------------

	// コンストラクタ
	Audio::Audio()
	{
		if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
			return;

		UINT flag = 0;
#if defined(DEBUG) | defined(_DEBUG)
		flag |= XAUDIO2_DEBUG_ENGINE;
#endif
		if (FAILED(XAudio2Create(&xaudio, flag)))
			return;

		if (FAILED(xaudio->CreateMasteringVoice(&masterVoice)))
			return;

		Load();
	}

	// デストラクタ
	Audio::~Audio()
	{
		OutputDebugString(_T("オーディオを破棄しました。"));
		sourceVoice->Stop();
		sourceVoice->DestroyVoice();
		masterVoice->DestroyVoice();
		if (!xaudio)xaudio->Release(); xaudio = nullptr;
		CoUninitialize();
	}

	int Audio::Load()
	{
		WAVEFORMATEX format = { 0 };
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = 1;
		format.wBitsPerSample = 16;
		format.nSamplesPerSec = 44100;
		format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
		format.nAvgBytesPerSec = format.nSamplesPerSec*format.nBlockAlign;

		if (FAILED(xaudio->CreateSourceVoice(&sourceVoice, &format)))
			return -1;

		std::vector<BYTE> data(format.nAvgBytesPerSec * 1);
		short *p = (short *)&data[0];
		for (size_t i = 0; i < data.size() / 2; i++)
		{
			float length = format.nSamplesPerSec / 440.0f;
			*p = (short)(32767 * sinf(i * D3DX_PI / length));
			p++;
		}

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = data.size();
		buffer.pAudioData = &data[0];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		sourceVoice->SubmitSourceBuffer(&buffer);
	}
}
