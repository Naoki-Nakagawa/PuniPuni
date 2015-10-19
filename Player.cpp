/*
Player.cpp
�v���C���[

15/05/27
Naoki Nakagawa
*/
#include "vtx.h"
#include "Player.h"
#include "Stage.h"
using namespace vtx;

// �R���X�g���N�^
Player::Player()
{
	rota = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
	backPos = D3DXVECTOR3(0, 0, 0);
	frontPos = D3DXVECTOR3(0, 0, 0);
	length = 0.0f;
	mesh = new vtx::Mesh(_T("Mesh/Player.x"));
}

// �f�X�g���N�^
Player::~Player()
{
	delete mesh;
}

// �X�V
void Player::Update(Stage &stage, bool &clearFlag)
{
	// �X�P�[���̕ω��l
	static D3DXVECTOR3 scaleSp = D3DXVECTOR3(0, 0, 0);
	// �N���b�N�����ǂ���
	static bool isClick = false;

	// �N���b�N���Ă���ԐL�т�
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

	// ��[�������ɓ������Ă�����{�̂������񂹂�
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

	// ��[�̍��W���v�Z����
	frontPos.x = cos(rota.z) * length + backPos.x;
	frontPos.y = sin(rota.z) * length + backPos.y;

	// �`���߂�
	scaleSp.x += ((length + 1) - scale.x) * 0.5f;
	scaleSp.y += (1.0f - scale.y) * 0.5f;
	scaleSp.z += (1.0f - scale.z) * 0.5f;
	scale += scaleSp *= 0.75f;	// ���l�͔����̒l�B0���ᔽ���B1��������
}

// �`��
void Player::Draw()
{
	static std::unique_ptr<Mesh> arrow(new Mesh(_T("Mesh/Arrow.x")));
	static std::unique_ptr<Texture> redTexture(new Texture(_T("Texture/Red.png")));
	D3DXVECTOR3 arrowRota = D3DXVECTOR3(0, 0, atan2(vtx::input->mouse->GetX() - app->wnd->GetWidth() * 0.5f, vtx::input->mouse->GetY() - app->wnd->GetHeight() * 0.5f) + D3DXToRadian(-90));
	float distance = pow(pow(vtx::input->mouse->GetX() - app->wnd->GetWidth() * 0.5f, 2) + pow(vtx::input->mouse->GetY() - app->wnd->GetHeight() * 0.5f, 2), 0.5) * 0.03f;
	D3DXVECTOR3 arrowPos = D3DXVECTOR3(cos(arrowRota.z) * distance, sin(arrowRota.z) * distance, 0);
	arrow->Draw(&(arrowPos + backPos), &arrowRota, &D3DXVECTOR3(1, 1, 1), redTexture->Get());

	// ��[�Ɩ{�̂̍��W�̕��ς̍��W�ɕ`��
	mesh->Draw(&((frontPos + backPos) * 0.5f), &rota, &scale, nullptr);
}
