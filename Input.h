/*
Input.h
����

15/05/28
Naoki Nakagawa
*/
#pragma once

namespace vtx
{
	// ����`
	class Key;
	class Mouse;
	class Gamepad;

	//--------------------------------------------------
	// �N���X
	//--------------------------------------------------

	// �C���v�b�g�N���X
	class Input
	{
	private:

		//LPDIRECTINPUT8 dInput;

	public:

		// �����o�ϐ�
		std::unique_ptr<Key> key;
		std::unique_ptr<Mouse> mouse;
		//std::unique_ptr<Gamepad> gamepad;

		// �R���X�g���N�^
		Input(HWND hWnd);

		// �f�X�g���N�^
		~Input();

		LPDIRECTINPUT8 GetDevice();

	};

	// �L�[�N���X
	class Key
	{
	private:

		// �����o�ϐ�
		LPDIRECTINPUTDEVICE8 dkey;
		BYTE state[256];
		BYTE old[256];

	public:

		// �R���X�g���N�^
		Key(LPDIRECTINPUT8 dInput, HWND hWnd);

		// �f�X�g���N�^
		~Key();
		
		// ��Ԃ̊Ď�
		void Check();

		// ��Ԃ��擾
		bool Get(int keyCode);

		// �L�[�A�b�v���擾
		bool GetUp(int keyCode);

		// �L�[�_�E�����擾
		bool GetDown(int keyCode);

	};

	// �}�E�X�N���X
	class Mouse
	{
	private:

		// �����o�ϐ�
		LPDIRECTINPUTDEVICE8 dmouse;
		DIMOUSESTATE2 state;
		DIMOUSESTATE2 old;
		POINT pos;

	public:

		// �R���X�g���N�^
		Mouse(LPDIRECTINPUT8 dInput, HWND hWnd);

		// �f�X�g���N�^
		~Mouse();

		// ��Ԃ̊Ď�
		void Check();

		// �{�^���̏�Ԃ��擾
		bool GetButton(int buttonCode);

		// �{�^���A�b�v���擾
		bool GetUpButton(int buttonCode);

		// �{�^���_�E�����擾
		bool GetDownButton(int buttonCode);

		// �}�E�X��X���W���擾
		int GetX();

		// �}�E�X��Y���W���擾
		int GetY();

	};

	// �Q�[���p�b�h�N���X
	class Gamepad
	{
	private:

		// �����o�ϐ�
		//LPDIRECTINPUTDEVICE8 dpad;
		DIJOYSTATE2 state;
		DIJOYSTATE2 old;

	public:

		// �R���X�g���N�^
		Gamepad(LPDIRECTINPUT8 dInput, HWND hWnd);

		// �f�X�g���N�^
		~Gamepad();

		// ��Ԃ̊Ď�
		void Check();

		// �X�e�B�b�N��X���̎擾
		float GetX();

		// �X�e�B�b�N��Y���̎擾
		float GetY();

	private:

		// �Q�[���p�b�h��񋓂���
		static BOOL CALLBACK EnumGamepad(const DIDEVICEINSTANCE *dInstance, VOID *context);

		// �X�e�B�b�N�Ȃǂ̎���񋓂���
		static BOOL CALLBACK EnumAxis(LPCDIDEVICEOBJECTINSTANCE oInstance, LPVOID ref);

	};
}
