/*
Player.h
�v���C���[

15/05/27
Naoki Nakagawa
*/
#pragma once

// ����`
class Stage;

// �v���C���[�N���X
class Player
{
public:

	// �����o�ϐ�
	D3DXVECTOR3 rota;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 backPos;
	D3DXVECTOR3 frontPos;
	float length;
	vtx::Mesh *mesh;

	// �R���X�g���N�^
	Player();

	// �f�X�g���N�^
	~Player();

	// �X�V
	void Update(Stage &stage, bool &clearFlag);

	// �`��
	void Draw();

};
