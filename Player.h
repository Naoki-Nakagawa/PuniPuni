/*
Player.h
プレイヤー

15/05/27
Naoki Nakagawa
*/
#pragma once

// 仮定義
class Stage;

// プレイヤークラス
class Player
{
public:

	// メンバ変数
	D3DXVECTOR3 rota;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 backPos;
	D3DXVECTOR3 frontPos;
	float length;
	vtx::Mesh *mesh;

	// コンストラクタ
	Player();

	// デストラクタ
	~Player();

	// 更新
	void Update(Stage &stage, bool &clearFlag);

	// 描画
	void Draw();

};
