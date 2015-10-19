/*
Render.h
描画処理

15/05/21
Naoki Nakagawa
*/
#pragma once

namespace vtx
{

	//--------------------------------------------------
	// クラス
	//--------------------------------------------------

	// 描画クラス
	class Render
	{
		//------------------------------------------------------------------

	private:

		// メンバ変数
		LPDIRECT3D9			direct3d;
		LPDIRECT3DDEVICE9	d3d;		//描画デバイス

	public:

		// コンストラクタ
		Render(HWND hWnd, int width, int height, bool isFullScreen);

		//------------------------------------------------------------------

		// デストラクタ
		~Render();

		// 描画の開始
		void Start();

		// 描画の終了
		void End();

		// 画面のクリア
		void Clear(DWORD color);

		// 3Dデバイスの取得
		const LPDIRECT3DDEVICE9 GetD3d();

	};

	// カメラクラス
	class Camera
	{
	private:

		// メンバ変数
		float aspect;
		D3DXMATRIX m_view;
		D3DXMATRIX m_projection;

	public:

		// メンバ変数
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rota;

		// コンストラクタ
		Camera();

		// コンストラクタ
		Camera(D3DXVECTOR3 pos, D3DXVECTOR3 rota);

		// デストラクタ
		~Camera();

		// 生成共通処理
		void Create(D3DXVECTOR3 pos, D3DXVECTOR3 rota);

		// 撮影
		void View();

		// ビューポートの設定
		void SetViewPort(int x, int y, int width, int height);

	};

}
