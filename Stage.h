/*
Stage.h
�X�e�[�W

15/05/27
Naoki Nakagawa
*/
#pragma once

class Block
{
public:

	D3DXVECTOR2 pos;
	int type;

	// �R���X�g���N�^
	Block();

	// �R���X�g���N�^
	Block(D3DXVECTOR2 pos, int type);

};

// �X�e�[�W�N���X
class Stage
{
public:

	// �����o�ϐ�
	vtx::Mesh *mesh;
	vtx::Texture *texture;
	std::vector<Block> block;

	// �R���X�g���N�^
	Stage();

	// �f�X�g���N�^
	~Stage();

	// �X�V
	void Load(const char *filePath);

	// �`��
	void Draw();

	// �X�e�[�W�Ɠ_���G��Ă��邩 (�߂�l�̓e���L�[�̏㉺���E�ɑ�������)
	int CheckHit(float &x, float &y, float r);

};
