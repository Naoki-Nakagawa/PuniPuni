/*
Stage.h
ステージ

15/05/27
Naoki Nakagawa
*/
#pragma once

class Block
{
public:

	D3DXVECTOR2 pos;
	int type;

	// コンストラクタ
	Block();

	// コンストラクタ
	Block(D3DXVECTOR2 pos, int type);

};

// ステージクラス
class Stage
{
public:

	// メンバ変数
	vtx::Mesh *mesh;
	vtx::Texture *texture;
	std::vector<Block> block;

	// コンストラクタ
	Stage();

	// デストラクタ
	~Stage();

	// 更新
	void Load(const char *filePath);

	// 描画
	void Draw();

	// ステージと点が触れているか (戻り値はテンキーの上下左右に相当する)
	int CheckHit(float &x, float &y, float r);

};
