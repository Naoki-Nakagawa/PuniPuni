/*
App.cpp
アプリケーション

15/05/21
Naoki Nakagawa
*/
#include "vtx.h"

namespace vtx
{
	//--------------------------------------------------
	// アプリケーションクラス
	//--------------------------------------------------

	// コンストラクタ
	App::App()
		: wnd(new Window())
	{

	}

	// コンストラクタ
	App::App(TCHAR *title, int width, int height, bool isFullScreen)
		: wnd(new Window(title, width, height, isFullScreen))
	{

	}

	// デストラクタ
	App::~App()
	{
		OutputDebugString(_T("アプリケーションを破棄しました。\n"));
	}

	//--------------------------------------------------
	// ウィンドウクラス
	//--------------------------------------------------

	// コンストラクタ
	Window::Window()
	{
		Create(_T("vtxライブラリ"), 800, 600, false);
	}

	// コンストラクタ
	Window::Window(TCHAR *title, int width, int height, bool isFullScreen)
	{
		Create(title, width, height, isFullScreen);
	}

	// デストラクタ
	Window::~Window()
	{
		OutputDebugString(_T("ウィンドウを破棄しました。\n"));
	}

	// メッセージループ
	bool Window::MessageLoop()
	{
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return false;
			}
		}

		input->key->Check();
		input->mouse->Check();
		//input->gamepad->Check();

		return true;
	}

	// ハンドルの取得
	const HWND Window::GetHandle()
	{
		return handle;
	}

	// タイトルの取得
	const TCHAR *Window::GetTitle()
	{
		return title;
	}

	// 幅の取得
	const int Window::GetWidth()
	{
		return width;
	}

	// 高さの取得
	const int Window::GetHeight()
	{
		return height;
	}

	// フルスクリーンフラグの取得
	const bool Window::GetFullScreen()
	{
		return isFullScreen;
	}

	// 生成共通処理
	void Window::Create(TCHAR *title, int width, int height, bool isFullScreen)
	{
		// メンバ変数の設定
		Window::title = title;
		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
		Window::width = rect.right - rect.left;
		Window::height = rect.bottom - rect.top;
		Window::isFullScreen = isFullScreen;

		// ウィンドウクラスの設定
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(wcex));
		wcex.lpfnWndProc = Proc;
		wcex.hInstance = GetModuleHandle(nullptr);
		wcex.lpszClassName = (TCHAR *)Window::title;
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpszMenuName = (TCHAR *)Window::title;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

		// ウィンドウクラスの登録
		if (!RegisterClassEx(&wcex))
		{
			return;
		}

		// ウィンドウの生成
		handle = CreateWindow(Window::title, Window::title, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, Window::width, Window::height, nullptr, nullptr, wcex.hInstance, nullptr);

		if (!handle)
		{
			return;
		}

		// ウィンドウの表示
		ShowWindow(handle, SW_SHOWNORMAL);
		ValidateRect(handle, 0);
	}

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_DESTROY)
		{
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}
