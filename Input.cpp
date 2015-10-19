/*
Input.cpp
入力

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
	// インプットクラス
	//--------------------------------------------------

	// コンストラクタ
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

	// デストラクタ
	Input::~Input()
	{
		SafeRelease(dInput);
	}

	LPDIRECTINPUT8 Input::GetDevice()
	{
		return dInput;
	}

	//--------------------------------------------------
	// キークラス
	//--------------------------------------------------

	// コンストラクタ
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

	// デストラクタ
	Key::~Key()
	{
		dkey->Unacquire();
		SafeRelease(dkey);
	}

	// 状態の監視
	void Key::Check()
	{
		std::memcpy(old, state, sizeof(state));
		dkey->GetDeviceState(256, state);

		dkey->Acquire();
	}

	// 状態を取得
	bool Key::Get(int keyCode)
	{
		return state[keyCode] & 0xf0;
	}

	// キーアップを取得
	bool Key::GetUp(int keyCode)
	{
		if (!(state[keyCode] & 0xf0)
			&& (old[keyCode] & 0xf0))
		{
			return true;
		}

		return false;
	}

	// キーダウンを取得
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
	// マウスクラス
	//--------------------------------------------------

	// コンストラクタ
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

	// デストラクタ
	Mouse::~Mouse()
	{
		dmouse->Unacquire();
		SafeRelease(dmouse);
	}

	// 状態の監視
	void Mouse::Check()
	{
		// マウスのボタンの状態の取得
		old = state;
		dmouse->GetDeviceState(sizeof(DIMOUSESTATE2), &state);

		// マウスの座標の取得
		RECT wndRect;
		GetCursorPos(&pos);
		GetWindowRect(app->wnd->GetHandle(), &wndRect);
		pos.x -= wndRect.left;
		pos.y -= wndRect.top;

		dmouse->Acquire();
	}

	// マウスのボタンの状態を取得
	bool Mouse::GetButton(int buttonCode)
	{
		if (state.rgbButtons[buttonCode])
		{
			return true;
		}

		return false;
	}

	// ボタンアップを取得
	bool Mouse::GetUpButton(int buttonCode)
	{
		if (!state.rgbButtons[buttonCode]
			&& old.rgbButtons[buttonCode])
		{
			return true;
		}

		return false;
	}

	// ボタンダウンを取得
	bool Mouse::GetDownButton(int buttonCode)
	{
		if (state.rgbButtons[buttonCode]
			&& !old.rgbButtons[buttonCode])
		{
			return true;
		}

		return false;
	}

	// マウスの座標を取得
	int Mouse::GetX()
	{
		return pos.x;
	}

	// マウスの座標を取得
	int Mouse::GetY()
	{
		return pos.y;
	}

	//--------------------------------------------------
	// ゲームパッドクラス
	//--------------------------------------------------

	// コンストラクタ
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

	// デストラクタ
	Gamepad::~Gamepad()
	{
		dpad->Unacquire();
		SafeRelease(dpad);
	}

	// 状態の監視
	void Gamepad::Check()
	{
		dpad->GetDeviceState(sizeof(DIJOYSTATE2), &state);

		dpad->Acquire();
	}

	// スティックのX軸の取得
	float Gamepad::GetX()
	{
		return state.lX * 0.0001f;
	}

	// スティックのY軸の取得
	float Gamepad::GetY()
	{
		return state.lY * 0.0001f;
	}

	// ゲームパッドを列挙する
	BOOL CALLBACK Gamepad::EnumGamepad(const DIDEVICEINSTANCE *dInstance, VOID *context)
	{
		if (FAILED(dInput->CreateDevice(dInstance->guidInstance, &dpad, nullptr)))
		{
			return DIENUM_CONTINUE;
		}

		return DIENUM_STOP;
	}

	// スティックなどの軸を列挙する
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
