#pragma once

namespace vtx
{
	// オーディオクラス
	class Audio
	{
	private:

		// メンバ変数
		IXAudio2 *xaudio;
		IXAudio2MasteringVoice *masterVoice;

	public:
			
		// メンバ変数
		IXAudio2SourceVoice *sourceVoice;

		// コンストラクタ
		Audio();

		// デストラクタ
		~Audio();

		// 読み込み
		int Load();

	};
}
