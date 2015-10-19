/*
Player.cpp
プレイヤー

15/05/27
Naoki Nakagawa
*/
#include "vtx.h"
#include "Player.h"
#include "Stage.h"
using namespace vtx;

// コンストラクタ
Player::Player()
{
	rota = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	backPos = D3DXVECTOR3(0, 0, 0);
	frontPos = D3DXVECTOR3(0, 0, 0);
	length = 0.0f;
	mesh = new vtx::Mesh(_T("Mesh/Player.x"));
}

// デストラクタ
Player::~Player()
{
	delete mesh;
}

// 更新
void Player::Update(Stage &stage, bool &clearFlag)
{
	// スケールの変化値
	static D3DXVECTOR3 scaleSp = D3DXVECTOR3(0, 0, 0);
	// クリック中かどうか
	static bool isClick = false;

	// クリックしている間伸びる
	float distance = pow(pow(vtx::input->mouse->GetX() - app->wnd->GetWidth() * 0.5f, 2) + pow(vtx::input->mouse->GetY() - app->wnd->GetHeight() * 0.5f, 2), 0.5) * 0.03f;
	if (vtx::input->mouse->GetDownButton(0))
	{
		isClick = true;
	}
	if (vtx::input->mouse->GetUpButton(0))
	{
		isClick = false;
	}

	if (isClick &&
		distance > 1.0f)
	{
		rota.z = atan2(vtx::input->mouse->GetX() - app->wnd->GetWidth() * 0.5f, vtx::input->mouse->GetY() - app->wnd->GetHeight() * 0.5f) + D3DXToRadian(-90);
		length += (distance - scale.x) * 0.1f;
	}
	else
	{
		length = 0;
	}

	// 先端が何かに当たっていたら本体を引き寄せる
	if (length > 0.5f)
	{
		for (int i = 0; i < stage.block.size(); i++)
		{
			if (pow(stage.block[i].pos.x - frontPos.x, 2) + pow(stage.block[i].pos.y - frontPos.y, 2) <= pow(0.8f, 2))
			{
				while (pow(stage.block[i].pos.x - frontPos.x, 2) + pow(stage.block[i].pos.y - frontPos.y, 2) <= pow(0.8f, 2))
				{
					frontPos.x -= cos(rota.z) * 0.01f;
					frontPos.y -= sin(rota.z) * 0.01f;
				}
				isClick = false;
				backPos = frontPos;
				length = 0.0f;

				if (stage.block[i].type == 1)
					clearFlag = true;

				break;
			}
		}
	}

	// 先端の座標を計算する
	frontPos.x = cos(rota.z) * length + backPos.x;
	frontPos.y = sin(rota.z) * length + backPos.y;

	// 形状を戻す
	scaleSp.x += ((length + 1) - scale.x) * 0.5f;
	scaleSp.y += (1.0f - scale.y) * 0.5f;
	scaleSp.z += (1.0f - scale.z) * 0.5f;
	scale += scaleSp *= 0.75f;	// 数値は反発の値。0が低反発。1が高反発
}

// 描画
void Player::Draw()
{
	static std::unique_ptr<Mesh> arrow(new Mesh(_T("Mesh/Arrow.x")));
	static std::unique_ptr<Texture> redTexture(new Texture(_T("Texture/Red.png")));
	D3DXVECTOR3 arrowRota = D3DXVECTOR3(0, 0, atan2(vtx::input->mouse->GetX() - app->wnd->GetWidth() * 0.5f, vtx::input->mouse->GetY() - app->wnd->GetHeight() * 0.5f) + D3DXToRadian(-90));
	float distance = pow(pow(vtx::input->mouse->GetX() - app->wnd->GetWidth() * 0.5f, 2) + pow(vtx::input->mouse->GetY() - app->wnd->GetHeight() * 0.5f, 2), 0.5) * 0.03f;
	D3DXVECTOR3 arrowPos = D3DXVECTOR3(cos(arrowRota.z) * distance, sin(arrowRota.z) * distance, 0);
	arrow->Draw(&(arrowPos + backPos), &arrowRota, &D3DXVECTOR3(1, 1, 1), redTexture->Get());

	// 先端と本体の座標の平均の座標に描画
	mesh->Draw(&((frontPos + backPos) * 0.5f), &rota, &scale, nullptr);
}
