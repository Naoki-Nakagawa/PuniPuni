/*
Stage.h
ステージ

15/05/27
Naoki Nakagawa
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "vtx.h"
#include "Stage.h"
using namespace vtx;

// コンストラクタ
Block::Block()
{
	pos = D3DXVECTOR2(0, 0);
	type = 0;
}

// コンストラクタ
Block::Block(D3DXVECTOR2 pos, int type)
{
	Block::pos = pos;
	Block::type = type;
}

// コンストラクタ
Stage::Stage()
{
	mesh = new Mesh[2];
	mesh[0].Load(_T("Mesh/Block00.x"));
	mesh[1].Load(_T("Mesh/Block01.x"));
	texture = new Texture[1];
	texture[0].Load(_T("Texture/Pink.png"));
}

// デストラクタ
Stage::~Stage()
{
	delete[] mesh;
	delete[] texture;
}

// 更新
void Stage::Load(const char *filePath)
{
	block.clear();

	std::ifstream ifs(filePath);
	std::string line;

	if (!ifs.fail())
	{
		while (getline(ifs, line))
		{
			std::string token;
			float param[3];
			std::istringstream str(line);

			for (int i = 0; i < ARRAYSIZE(param); i++)
			{
				std::stringstream ss;
				getline(str, token, ',');
				ss << token;
				ss >> param[i];
			}

			block.push_back(Block(D3DXVECTOR2(param[0], param[1]), param[2]));
		}
	}
}

// 描画
void Stage::Draw()
{
	for (unsigned int i = 0; i < block.size(); i++)
	{
		mesh[block[i].type].Draw(&D3DXVECTOR3(block[i].pos.x, block[i].pos.y, 0), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1), texture[0].Get());
	}
}

// ステージと点が触れているか (戻り値はテンキーの上下左右に相当する)
int Stage::CheckHit(float &x, float &y, float r)
{
	for (unsigned int i = 0; i < block.size(); i++)
	{
		if ((x <= block[i].pos.x + 0.5f + r && x >= block[i].pos.x - 0.5f - r) &&
			(y <= block[i].pos.y + 0.5f && y >= block[i].pos.y - 0.5f) ||
			(x <= block[i].pos.x + 0.5f && x >= block[i].pos.x - 0.5f) &&
			(y <= block[i].pos.y + 0.5f + r && y >= block[i].pos.y - 0.5f - r) ||
			(pow(x - block[i].pos.x - 0.5f, 2) + pow(y - block[i].pos.y - 0.5f, 2) <= pow(r, 2)) ||
			(pow(x - block[i].pos.x + 0.5f, 2) + pow(y - block[i].pos.y - 0.5f, 2) <= pow(r, 2)) ||
			(pow(x - block[i].pos.x - 0.5f, 2) + pow(y - block[i].pos.y + 0.5f, 2) <= pow(r, 2)) ||
			(pow(x - block[i].pos.x + 0.5f, 2) + pow(y - block[i].pos.y + 0.5f, 2) <= pow(r, 2)))
		{
			if (abs(x - block[i].pos.x) > abs(y - block[i].pos.y))
			{
				if (x < block[i].pos.x)return 4;
				else return 6;
			}
			else
			{
				if (y < block[i].pos.y)return 2;
				else return 8;
			}
		}
	}

	return false;
}
