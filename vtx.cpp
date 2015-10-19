/*
vtx.cpp
ライブラリ

15/05/21
Naoki Nakagawa
*/
#include "vtx.h"

namespace vtx
{
	std::unique_ptr<App> app;
	std::unique_ptr<Render> render;
	std::unique_ptr<Input> input;
	std::unique_ptr<Audio> audio;
	std::unique_ptr<Math> math;

	//--------------------------------------------------
	// ライブラリクラス
	//--------------------------------------------------

	// コンストラクタ
	Lib::Lib()
	{
		// メモリリーク検出を開始する
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		app.reset(new App());
		render.reset(new Render(app->wnd->GetHandle(), app->wnd->GetWidth(), app->wnd->GetHeight(), app->wnd->GetFullScreen()));
		input.reset(new Input(app->wnd->GetHandle()));
		audio.reset(new Audio());
		math.reset(new Math());
	}

	// コンストラクタ
	Lib::Lib(TCHAR *title, int width, int height, bool isFullScreen)
	{
		// メモリリーク検出を開始する
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		app.reset(new App(title, width, height, isFullScreen));
		render.reset(new Render(app->wnd->GetHandle(), width, height, isFullScreen));
		input.reset(new Input(app->wnd->GetHandle()));
		audio.reset(new Audio());
		math.reset(new Math());
	}

	// デストラクタ
	Lib::~Lib()
	{
		// デバッグビルド時に終了確認用のビープ音を鳴らす
#if defined(DEBUG) | defined(_DEBUG)
		MessageBeep(MB_OK);
#endif
		OutputDebugString(_T("ライブラリを破棄しました。\n"));
	}
}
