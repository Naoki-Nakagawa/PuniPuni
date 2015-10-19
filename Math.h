/*
Math.h
数学的な処理

15/05/23
Naoki Nakagawa
*/
#pragma once

namespace vtx
{
	//--------------------------------------------------
	// クラス
	//--------------------------------------------------

	// 計算クラス
	class Math
	{
	private:

		// メンバ変数
		std::mt19937 mt;

	public:

		// コンストラクタ
		Math();

		// デストラクタ
		~Math();

		// 整数の乱数
		int Random(int min, int max);

		// 浮動小数の乱数
		float Random(float min, float max);

	};

}
