/*
Resource.h
リソース

15/05/27
Naoki Nakagawa
*/
#pragma once

namespace vtx
{
	//--------------------------------------------------
	// 構造体
	//--------------------------------------------------

	// パーティクルの座標
	struct ParticleTransform
	{
		D3DXVECTOR3 pos;	// 座標
		D3DXVECTOR3 vec;	// ベクトル
		float gravity;		// 重力加速度
		float size;			// 大きさ
		int duration;		// 持続時間
	};

	//--------------------------------------------------
	// クラス
	//--------------------------------------------------

	// メッシュクラス
	class Mesh
	{
	private:

		// メンバ変数
		LPD3DXMESH mesh;
		LPD3DXBUFFER buffer;
		DWORD materialNum;

	public:

		// コンストラクタ
		Mesh();

		// コンストラクタ
		Mesh(const TCHAR *filePath);

		// デストラクタ
		~Mesh();

		// 読み込み
		void Load(const TCHAR *filePath);

		// 描画
		void Draw(D3DXVECTOR3 *position, D3DXVECTOR3 *rotation, D3DXVECTOR3 *scale, LPDIRECT3DTEXTURE9 texture);

	};

	// パーティクル
	class Particle
	{
	private:

		LPDIRECT3DVERTEXBUFFER9	buffer;
		std::vector<ParticleTransform> transform;

	public:

		// コンストラクタ
		Particle();

		// デストラクタ
		~Particle();

		// パーティクルをセット
		void Set(D3DXVECTOR3 pos,
			D3DXVECTOR3 vec,
			float gravity,
			float size,
			int duration);

		// 指定した座標に向かって進む
		void Go(D3DXVECTOR3 pos, float power);

		// 描画
		void Draw(LPDIRECT3DTEXTURE9 texture);

	};

	// テクスチャクラス
	class Texture
	{
	private:

		// メンバ変数
		LPDIRECT3DTEXTURE9 texture;

	public:

		// コンストラクタ
		Texture();

		// コンストラクタ
		Texture(const TCHAR *filePath);

		// デストラクタ
		~Texture();

		// テクスチャの読み込み
		void Load(const TCHAR *filePath);
		
		// テクスチャの取得
		LPDIRECT3DTEXTURE9 Get();

	};

}
