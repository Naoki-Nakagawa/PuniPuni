#pragma once

namespace vtx
{
	// �I�[�f�B�I�N���X
	class Audio
	{
	private:

		// �����o�ϐ�
		IXAudio2 *xaudio;
		IXAudio2MasteringVoice *masterVoice;

	public:
			
		// �����o�ϐ�
		IXAudio2SourceVoice *sourceVoice;

		// �R���X�g���N�^
		Audio();

		// �f�X�g���N�^
		~Audio();

		// �ǂݍ���
		int Load();

	};
}
