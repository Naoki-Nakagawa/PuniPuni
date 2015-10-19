/*
Render.cpp
描画処理

15/05/21
Naoki Nakagawa
*/
#include "vtx.h"
#define SafeRelease(p) if (p) { p->Release(); p = nullptr; }

struct ParticleVertex
{
	D3DXVECTOR3 pos;
	float size;
};

namespace vtx
{
	//--------------------------------------------------
	// メッシュクラス
	//--------------------------------------------------

	// コンストラクタ
	Mesh::Mesh()
	{

	}

	// コンストラクタ
	Mesh::Mesh(const TCHAR *filePath)
	{
		Load(filePath);
	}

	// デストラクタ
	Mesh::~Mesh()
	{
		SafeRelease(buffer);
		SafeRelease(mesh);
	}

	// 読み込み
	void Mesh::Load(const TCHAR *filePath)
	{
		if (FAILED(D3DXLoadMeshFromX(filePath, D3DXMESH_SYSTEMMEM, render->GetD3d(), nullptr, &buffer, nullptr, &materialNum, &mesh)))
		{
			return;
		}
	}

	// 描画
	void Mesh::Draw(D3DXVECTOR3 *position, D3DXVECTOR3 *rotation, D3DXVECTOR3 *scale, LPDIRECT3DTEXTURE9 texture)
	{
		D3DXMATRIX *m_world = new D3DXMATRIX();
		D3DXMATRIX *m_temp = new D3DXMATRIX();

		D3DXMatrixIdentity(m_world);
		D3DXMatrixIdentity(m_temp);

		// 行列を拡大縮小する
		D3DXMatrixScaling(m_temp, scale->x, scale->y, scale->z);
		*m_world *= *m_temp;

		// 行列を回転する
		D3DXMatrixRotationYawPitchRoll(m_temp, rotation->y, rotation->x, rotation->z);
		*m_world *= *m_temp;

		// 行列を平行移動させる
		D3DXMatrixTranslation(m_temp, position->x, position->y, position->z);
		*m_world *= *m_temp;

		render->GetD3d()->SetTransform(D3DTS_WORLD, m_world);

		render->GetD3d()->SetTexture(0, texture);

		// モデルの描画
		D3DXMATERIAL *materials = (D3DXMATERIAL *)buffer->GetBufferPointer();
		for (DWORD i = 0; i < materialNum; i++)
		{
			// マテリアルの設定
			render->GetD3d()->SetMaterial(&materials[i].MatD3D);

			// 分割されたメッシュの描画
			mesh->DrawSubset(i);
		}

		delete m_temp;
		delete m_world;
	}

	// コンストラクタ
	Particle::Particle()
	{
		// 100個分の頂点バッファを確保
		if (FAILED(render->GetD3d()->CreateVertexBuffer(100 * sizeof(ParticleVertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_PSIZE, D3DPOOL_MANAGED, &buffer, nullptr)))
		{
			return;
		}
	}

	// デストラクタ
	Particle::~Particle()
	{
		SafeRelease(buffer);
	}

	// パーティクルをセット
	void Particle::Set(D3DXVECTOR3 pos,
		D3DXVECTOR3 vec,
		float gravity,
		float size,
		int duration)
	{
		ParticleTransform instanceTransform = {
			pos,
			vec,
			gravity,
			size,
			duration,
		};
		transform.push_back(instanceTransform);
	}

	// 指定した座標に向かって進む
	void Particle::Go(D3DXVECTOR3 pos, float power)
	{
		for (int i = 0; i < transform.size(); i++)
		{
			transform[i].vec += (pos - transform[i].pos) * power;
		}
	}

	void Particle::Draw(LPDIRECT3DTEXTURE9 texture)
	{
		for (int i = 0; i < transform.size(); i++)
		{
			if (transform[i].duration <= 0)
			{
				transform.erase(transform.begin() + i);
				i--;
			}
		}

		render->GetD3d()->SetRenderState(D3DRS_ZENABLE, false);
		render->GetD3d()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		render->GetD3d()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		render->GetD3d()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		render->GetD3d()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		int iMax = transform.size() / 100;
		for (int i = 0; i <= iMax; i++)
		{
			ParticleVertex *v;
			buffer->Lock(0, 0, (LPVOID*)&v, 0);
			for (int j = 0; j < 100; j++)
			{
				if (transform.size() <= i * 100 + j)
				{
					break;
				}

				v[j].pos = transform[i * 100 + j].pos += transform[i * 100 + j].vec += D3DXVECTOR3(0, transform[i * 100 + j].gravity, 0);
				v[j].size = transform[i * 100 + j].size;

				transform[i * 100 + j].duration--;
			}
			buffer->Unlock();

			render->GetD3d()->SetTexture(0, texture);

			D3DXMATRIX m_world;
			D3DXMatrixIdentity(&m_world);
			render->GetD3d()->SetTransform(D3DTS_WORLD, &m_world);

			render->GetD3d()->SetStreamSource(0, buffer, 0, sizeof(ParticleVertex));
			render->GetD3d()->SetFVF(D3DFVF_XYZ | D3DFVF_PSIZE);
			render->GetD3d()->DrawPrimitive(D3DPT_POINTLIST, 0, transform.size());
		}

		render->GetD3d()->SetRenderState(D3DRS_ZENABLE, true);
		render->GetD3d()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		render->GetD3d()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		render->GetD3d()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		render->GetD3d()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}

	//--------------------------------------------------
	// テクスチャクラス
	//--------------------------------------------------

	// コンストラクタ
	Texture::Texture()
	{

	}

	// コンストラクタ
	Texture::Texture(const TCHAR *filePath)
	{
		Load(filePath);
	}

	// デストラクタ
	Texture::~Texture()
	{
		SafeRelease(texture);
	}

	// テクスチャの読み込み
	void Texture::Load(const TCHAR *filePath)
	{
		if (FAILED(D3DXCreateTextureFromFile(render->GetD3d(), filePath, &texture)))
		{
			return;
		}
	}

	// テクスチャの取得
	LPDIRECT3DTEXTURE9 Texture::Get()
	{
		return texture;
	}
}
