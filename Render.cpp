/*
Render.cpp
描画処理

15/05/21
Naoki Nakagawa
*/
#include "vtx.h"
#define SafeRelease(p) if (p) { p->Release(); p = nullptr; }
DWORD FloatToDWORD(float v) { return *((DWORD *)&v); }

namespace vtx
{
	//--------------------------------------------------
	// 描画クラス
	//--------------------------------------------------

	// コンストラクタ
	Render::Render(HWND hWnd, int width, int height, bool isFullScreen)
	{
		direct3d = Direct3DCreate9(D3D_SDK_VERSION);
		if (!direct3d)
		{
			return;
		}

		D3DPRESENT_PARAMETERS pp;
		ZeroMemory(&pp, sizeof(pp));
		pp.hDeviceWindow = hWnd;
		pp.BackBufferCount = 1;
		pp.BackBufferWidth = width;
		pp.BackBufferHeight = height;
		pp.BackBufferFormat = D3DFMT_UNKNOWN;
		pp.Windowed = !isFullScreen;
		pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		pp.MultiSampleQuality = 0;
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		pp.EnableAutoDepthStencil = true;
		pp.AutoDepthStencilFormat = D3DFMT_D16;
		pp.Flags = 0;
		pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

		if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d3d)))
		{
			direct3d->Release();
			return;
		}

		d3d->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		d3d->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		d3d->SetRenderState(D3DRS_ZENABLE, true);
		d3d->SetRenderState(D3DRS_ZWRITEENABLE, true);
		d3d->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
		d3d->SetRenderState(D3DRS_LIGHTING, true);
		d3d->SetRenderState(D3DRS_SPECULARENABLE, true);
		d3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		d3d->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3d->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		d3d->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		d3d->SetRenderState(D3DRS_ALPHAREF, 0x00000000);
		d3d->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		d3d->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3d->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		d3d->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		d3d->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		d3d->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
		d3d->SetRenderState(D3DRS_POINTSCALEENABLE, true);
		d3d->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(1.0f));
		d3d->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));
		d3d->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
		d3d->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
		d3d->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));

		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(D3DLIGHT9));

		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Direction = D3DXVECTOR3(1, -1, 1);
		light.Diffuse.r = 1.0f;
		light.Diffuse.g = 1.0f;
		light.Diffuse.b = 1.0f;

		d3d->SetLight(0, &light);
		d3d->LightEnable(0, true);
		d3d->SetRenderState(D3DRS_LIGHTING, true);
		d3d->SetRenderState(D3DRS_AMBIENT, 0xff030303);
	}

	// デストラクタ
	Render::~Render()
	{
		SafeRelease(d3d);
		SafeRelease(direct3d);
		OutputDebugString(_T("レンダーを破棄しました。\n"));
	}

	// 描画の開始
	void Render::Start()
	{
		d3d->BeginScene();
	}

	// 描画の終了
	void Render::End()
	{
		d3d->EndScene();
		d3d->Present(nullptr, nullptr, nullptr, nullptr);
	}

	// 画面のクリア
	void Render::Clear(DWORD color)
	{
		d3d->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
	}

	// 3Dデバイスの取得
	const LPDIRECT3DDEVICE9 Render::GetD3d()
	{
		return d3d;
	}

	// コンストラクタ
	Camera::Camera()
	{
		Create(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0));
	}

	// コンストラクタ
	Camera::Camera(D3DXVECTOR3 pos, D3DXVECTOR3 rota)
	{
		Create(pos, rota);
	}

	// デストラクタ
	Camera::~Camera()
	{

	}

	// 生成共通処理
	void Camera::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rota)
	{
		SetViewPort(0, 0, app->wnd->GetWidth(), app->wnd->GetHeight());

		Camera::pos = pos;
		Camera::rota = rota;

		// 射影行列の初期化
		D3DXMatrixIdentity(&m_projection);

		// 射影行列を作成
		D3DXMatrixPerspectiveFovLH(&m_projection, D3DXToRadian(45.0f), aspect, 1.0f, 100000.0f);

		// 射影行列をパイプラインに設定
		render->GetD3d()->SetTransform(D3DTS_PROJECTION, &m_projection);

		View();
	}

	// 撮影
	void Camera::View()
	{
		D3DXMATRIX *m_temp = new D3DXMATRIX;
		D3DXMatrixIdentity(&m_view);
		D3DXMatrixIdentity(m_temp);

		// 行列を回転する
		D3DXMatrixRotationYawPitchRoll(m_temp, rota.y, rota.x, rota.z);
		m_view *= *m_temp;

		// 行列を平行移動させる
		D3DXMatrixTranslation(m_temp, pos.x, pos.y, pos.z);
		m_view *= *m_temp;

		D3DXMatrixIdentity(m_temp);
		D3DXMatrixInverse(m_temp, nullptr, &m_view);	// カメラの逆行列を求める

		// ビュー行列を設定
		render->GetD3d()->SetTransform(D3DTS_VIEW, m_temp);

		delete m_temp;
	}

	// ビューポートの設定
	void Camera::SetViewPort(int x, int y, int width, int height)
	{
		D3DVIEWPORT9 viewPort;
		viewPort.X = x;
		viewPort.Y = y;
		viewPort.Width = width;
		viewPort.Height = height;
		viewPort.MinZ = 0.0f;
		viewPort.MaxZ = 1.0f;

		render->GetD3d()->SetViewport(&viewPort);

		aspect = (float)viewPort.Width / (float)viewPort.Height;
	}
}
