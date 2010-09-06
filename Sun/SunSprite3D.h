/**
	@file SunSprite3D.h
	@brief SunSprite3D �N���X�̃C���^�[�t�F�C�X
*/

#pragma once

#include "SunTexture.h"
/**
	@brief 3D�X�v���C�g�p���_�\��
*/
typedef struct VXSPRITE3D
{
	float x, y, z;
	float nx, ny, nz;
	D3DCOLOR c;
	float u, v;
	//���_�t�H�[�}�b�g�t���O
	static const DWORD FVF;
} VXSPRITE3D;

/**
	@brief 3D�X�v���C�g�N���X

	�����ɒ��_�\���́i�S���_�j��SunTexture�|�C���^�����B
*/

class SunSprite3D
{
public:
	//�f�[�^�����o
	VXSPRITE3D		m_vertex[4];
	SunTexture*		m_ptex;		///<�e�N�X�`���|�C���^
public:
	SunSprite3D(SunTexture* ptex);
	virtual ~SunSprite3D();
	int Draw();										///<�`��
	int SetTexture(SunTexture* ptex);				///<�e�N�X�`���ݒ�
	int CreateRectXY(RECT &rect,float z);			///<XY���ʂɕ��s�ȋ�`�ݒ�
	int CreateRectYZ(RECT &rect,float x);			///<YZ���ʂɕ��s�ȋ�`�ݒ�
	int CreateRectZX(RECT &rect,float y);			///<ZX���ʂɕ��s�ȋ�`�ݒ�
	int SetSourceRect(RECT &rcSrc);					///<�`�挳�̈�ݒ�
	int SetRepeat(float rx,float ry);				///<�`�挳�J��Ԃ���
	int SetColor(D3DCOLOR color);					///<�f�B�q���[�Y�F�ݒ�
	int SetGradationColor(D3DCOLOR under,D3DCOLOR top);	///<�O���f�[�V�����F�ݒ�
	int Move(float x,float y,float z);				///<���s�ړ�
	int Rotate(float a,float x,float y);			///<�y����]
	int Scale(float mx,float my,float x,float y);	///<�g��k��
//�����g�p�֐�
	int Init();
};
