/*
Math.h
数学的な処理

15/05/23
Naoki Nakagawa
*/
#include "vtx.h"

namespace vtx
{
	//--------------------------------------------------
	// 計算クラス
	//--------------------------------------------------

		// コンストラクタ
	Math::Math()
	{

	}

		// デストラクタ
	Math::~Math()
	{

	}

		// 整数の乱数
	int Math::Random(int min, int max)
	{
		std::uniform_int_distribution<int> range(min, max);
		return range(mt);
	}

		// 浮動小数の乱数
	float Math::Random(float min, float max)
	{
		std::uniform_real_distribution<float> range(min, max);
		return range(mt);
	}
}
