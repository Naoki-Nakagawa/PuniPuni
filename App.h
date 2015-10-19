/*
App.h
アプリケーション

15/05/21
Naoki Nakagawa
*/
#pragma once

namespace vtx
{
	// 仮定義
	class Window;

	//--------------------------------------------------
	// クラス
	//--------------------------------------------------

	// アプリケーションクラス
	class App
	{
	public:

		// メンバ変数
		std::unique_ptr<Window> wnd;

		// コンストラクタ
		App();

		// コンストラクタ
		App(TCHAR *title, int width, int height, bool isFullScreen);

		// デストラクタ
		~App();

	};

	// ウィンドウクラス
	class Window
	{
	private:

		// メンバ変数
		HWND	handle;
		TCHAR	*title;
		int		width;
		int		height;
		bool	isFullScreen;

	public:

		// コンストラクタ
		Window();

		// コンストラクタ
		Window(TCHAR *title, int width, int height, bool isFullScreen);

		// デストラクタ
		~Window();

		// メッセージループ
		bool MessageLoop();

		// ハンドルの取得
		const HWND GetHandle();

		// タイトルの取得
		const TCHAR *GetTitle();

		// 幅の取得
		const int GetWidth();

		// 高さの取得
		const int GetHeight();

		// フルスクリーンフラグの取得
		const bool GetFullScreen();

	private:

		// 生成共通処理
		void Create(TCHAR *title, int width, int height, bool isFullScreen);

		// プロシージャ
		static LRESULT CALLBACK Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	};
}
