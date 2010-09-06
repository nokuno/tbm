/**
	@file SunSprite.h
	@brief SunSprite �N���X�̃C���^�[�t�F�C�X
*/

#pragma once

#include "SunTexture.h"
/**
	@brief �P���ȂQ�c�X�v���C�g�p���_�\��
*/
typedef struct VXSPRITE
{
	float x, y, z, rhw;
	D3DCOLOR c;
	float u, v;
	//���_�t�H�[�}�b�g�t���O
	static const DWORD FVF;
} VXSPRITE;

/**
	@brief �X�v���C�g��b�N���X

	�����ɒ��_�\���́i�S���_�j��SunTexture�|�C���^�����B
	�|�C���^�̓X�}�[�g�|�C���^�ł���B
	���ۂɂ͂��ꎩ�̂ł͂Ȃ������e�N�X�`���p�̃X�v���C�g���g�p����B
*/

class SunSprite
{
private:
	//�f�[�^�����o
	VXSPRITE		m_vertex[4];
	SunTexture*		m_ptex;		///<�e�N�X�`���|�C���^
	static double	s_x, s_y;
public:
	SunSprite(SunTexture* ptex);
	virtual ~SunSprite();
	int Draw();										///<�`��
	int DrawEx();									///<�`��(��ʗh��Ή�)
	int SetTexture(SunTexture* ptex);				///<�e�N�X�`���ݒ�
	int CreateRect(RECT &rect);						///<�`���̈�ݒ�
	int CreateRectEx(float x,float y,float w,float h );			///<�`���̈�ݒ�i�����Ή��j
	int SetSourceRect(RECT &rcSrc);					///<�`�挳�̈�ݒ�
	int SetRepeat(float rx,float ry);				///<�`�挳�J��Ԃ���
	int SetColor(D3DCOLOR color);					///<�f�B�q���[�Y�F�ݒ�
	int SetGradationColor(D3DCOLOR under,D3DCOLOR top);	///<�O���f�[�V�����F�ݒ�
	int Move(float x,float y);						///<���s�ړ�
	int Rotate(float a,float x,float y);			///<�y����]
	int Scale(float mx,float my,float x,float y);	///<�g��k��
	static void SetOffset(float x,float y){s_x=x;s_y=y;}
//�����g�p�֐�
	int Init();
};
