/*
Input.cpp
����

15/05/28
Naoki Nakagawa
*/
#include "vtx.h"
#define SafeRelease(p) if (p) { p->Release(); p = nullptr; }

namespace vtx
{
	LPDIRECTINPUT8 dInput;
	LPDIRECTINPUTDEVICE8 dpad;

	//--------------------------------------------------
	// �C���v�b�g�N���X
	//--------------------------------------------------

	// �R���X�g���N�^
	Input::Input(HWND hWnd)
	{
		if (FAILED(DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&dInput, nullptr)))
		{
			return;
		}

		mouse.reset(new Mouse(dInput, hWnd));
		key.reset(new Key(dInput, hWnd));
		//gamepad.reset(new Gamepad(dInput, hWnd));
	}

	// �f�X�g���N�^
	Input::~Input()
	{
		SafeRelease(dInput);
	}

	LPDIRECTINPUT8 Input::GetDevice()
	{
		return dInput;
	}

	//--------------------------------------------------
	// �L�[�N���X
	//--------------------------------------------------

	// �R���X�g���N�^
	Key::Key(LPDIRECTINPUT8 dInput, HWND hWnd)
	{
		if (FAILED(dInput->CreateDevice(GUID_SysKeyboard, &dkey, nullptr)))
		{
			return;
		}

		if (FAILED(dkey->SetDataFormat(&c_dfDIKeyboard)))
		{
			return;
		}

		if (FAILED(dkey->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			return;
		}
	}

	// �f�X�g���N�^
	Key::~Key()
	{
		dkey->Unacquire();
		SafeRelease(dkey);
	}

	// ��Ԃ̊Ď�
	void Key::Check()
	{
		std::memcpy(old, state, sizeof(state));
		dkey->GetDeviceState(256, state);

		dkey->Acquire();
	}

	// ��Ԃ��擾
	bool Key::Get(int keyCode)
	{
		return state[keyCode] & 0xf0;
	}

	// �L�[�A�b�v���擾
	bool Key::GetUp(int keyCode)
	{
		if (!(state[keyCode] & 0xf0)
			&& (old[keyCode] & 0xf0))
		{
			return true;
		}

		return false;
	}

	// �L�[�_�E�����擾
	bool Key::GetDown(int keyCode)
	{
		if ((state[keyCode] & 0xf0)
			&& !(old[keyCode] & 0xf0))
		{
			return true;
		}

		return false;
	}

	//--------------------------------------------------
	// �}�E�X�N���X
	//--------------------------------------------------

	// �R���X�g���N�^
	Mouse::Mouse(LPDIRECTINPUT8 dInput, HWND hWnd)
	{
		if (FAILED(dInput->CreateDevice(GUID_SysMouse, &dmouse, nullptr)))
		{
			return;
		}

		if (FAILED(dmouse->SetDataFormat(&c_dfDIMouse2)))
		{
			return;
		}

		if (FAILED(dmouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			return;
		}

		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_ABS;

		if (FAILED(dmouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
		{
			return;
		}
	}

	// �f�X�g���N�^
	Mouse::~Mouse()
	{
		dmouse->Unacquire();
		SafeRelease(dmouse);
	}

	// ��Ԃ̊Ď�
	void Mouse::Check()
	{
		// �}�E�X�̃{�^���̏�Ԃ̎擾
		old = state;
		dmouse->GetDeviceState(sizeof(DIMOUSESTATE2), &state);

		// �}�E�X�̍��W�̎擾
		RECT wndRect;
		GetCursorPos(&pos);
		GetWindowRect(app->wnd->GetHandle(), &wndRect);
		pos.x -= wndRect.left;
		pos.y -= wndRect.top;

		dmouse->Acquire();
	}

	// �}�E�X�̃{�^���̏�Ԃ��擾
	bool Mouse::GetButton(int buttonCode)
	{
		if (state.rgbButtons[buttonCode])
		{
			return true;
		}

		return false;
	}

	// �{�^���A�b�v���擾
	bool Mouse::GetUpButton(int buttonCode)
	{
		if (!state.rgbButtons[buttonCode]
			&& old.rgbButtons[buttonCode])
		{
			return true;
		}

		return false;
	}

	// �{�^���_�E�����擾
	bool Mouse::GetDownButton(int buttonCode)
	{
		if (state.rgbButtons[buttonCode]
			&& !old.rgbButtons[buttonCode])
		{
			return true;
		}

		return false;
	}

	// �}�E�X�̍��W���擾
	int Mouse::GetX()
	{
		return pos.x;
	}

	// �}�E�X�̍��W���擾
	int Mouse::GetY()
	{
		return pos.y;
	}

	//--------------------------------------------------
	// �Q�[���p�b�h�N���X
	//--------------------------------------------------

	// �R���X�g���N�^
	Gamepad::Gamepad(LPDIRECTINPUT8 dInput, HWND hWnd)
	{
		if (FAILED(dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumGamepad, nullptr, DIEDFL_ATTACHEDONLY)))
		{
			return;
		}

		if (FAILED(dpad->SetDataFormat(&c_dfDIJoystick2)))
		{
			return;
		}

		if (FAILED(dpad->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			return;
		}

		if (FAILED(dpad->EnumObjects(EnumAxis, nullptr, DIDFT_AXIS)))
		{
			return;
		}
	}

	// �f�X�g���N�^
	Gamepad::~Gamepad()
	{
		dpad->Unacquire();
		SafeRelease(dpad);
	}

	// ��Ԃ̊Ď�
	void Gamepad::Check()
	{
		dpad->GetDeviceState(sizeof(DIJOYSTATE2), &state);

		dpad->Acquire();
	}

	// �X�e�B�b�N��X���̎擾
	float Gamepad::GetX()
	{
		return state.lX * 0.0001f;
	}

	// �X�e�B�b�N��Y���̎擾
	float Gamepad::GetY()
	{
		return state.lY * 0.0001f;
	}

	// �Q�[���p�b�h��񋓂���
	BOOL CALLBACK Gamepad::EnumGamepad(const DIDEVICEINSTANCE *dInstance, VOID *context)
	{
		if (FAILED(dInput->CreateDevice(dInstance->guidInstance, &dpad, nullptr)))
		{
			return DIENUM_CONTINUE;
		}

		return DIENUM_STOP;
	}

	// �X�e�B�b�N�Ȃǂ̎���񋓂���
	BOOL CALLBACK Gamepad::EnumAxis(LPCDIDEVICEOBJECTINSTANCE oInstance, LPVOID ref)
	{
		DIPROPRANGE range;
		ZeroMemory(&range, sizeof(range));
		range.diph.dwSize = sizeof(range);
		range.diph.dwHeaderSize = sizeof(range.diph);
		range.diph.dwObj = oInstance->dwType;
		range.diph.dwHow = DIPH_BYID;
		range.lMin = -10000;
		range.lMax = 10000;

		if (FAILED(dpad->SetProperty(DIPROP_RANGE, &range.diph)))
		{
			return DIENUM_STOP;
		}

		return DIENUM_CONTINUE;
	}

}
