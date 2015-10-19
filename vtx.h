/*
vtx.h
ライブラリのヘッダファイル

15/05/21
Naoki Nakagawa
*/
#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// C++系
#include <vector>
#include <memory>
#include <random>
// Windows系
#include <Windows.h>
#include <tchar.h>
// DirectX系
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <xaudio2.h>
// vtxライブラリ系
#include "App.h"
#include "Render.h"
#include "Resource.h"
#include "Input.h"
#include "Audio.h"
#include "Math.h"

namespace vtx
{
	extern std::unique_ptr<App> app;
	extern std::unique_ptr<Render> render;
	extern std::unique_ptr<Input> input;
	extern std::unique_ptr<Audio> audio;
	extern std::unique_ptr<Math> math;
	
	//--------------------------------------------------
	// クラス
	//--------------------------------------------------

	// ライブラリクラス
	class Lib
	{
	public:

		// コンストラクタ
		Lib();

		// コンストラクタ
		Lib(TCHAR *title, int width, int height, bool isFullScreen);

		// デストラクタ
		~Lib();

	};
}
