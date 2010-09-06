
#pragma once

#include "SunTask.h"
/**
	@brief �P���ȂQ�c�G�t�F�N�g�p���_�\��
*/
typedef struct VXEFFECT
{
	float x, y, z, w;
	D3DCOLOR c;
	//���_�t�H�[�}�b�g�t���O
	static const DWORD FVF;
} VXEFFECT;
/**
	@brief �G�t�F�N�g�N���X

	�P���Ȓ��_�\���݂̂̂������o�Ɏ��N���X�B
	��{�I�ɂ��ꎩ�̂��g�킸�A���b�p�[���g���B

    @todo	------------------------
	�ǉ��������`�󂪂�������B
	�R�c�����O�i�h��G�t�F�N�g�j�Ƃ��A���[�U�[�n�A���ˏ�|���S���Ƃ��B
	��U�|���S���Ƃ��A��������킩��Ȃ����n�G�t�F�N�g�Ƃ��B
*/
class SunEffect
{
public:
	vector<VXEFFECT>	m_vertex;		///<���_�\����
	D3DPRIMITIVETYPE	m_primType;		///<�`��^�C�v
public:
	SunEffect();
	virtual ~SunEffect();
	int Resize(int size);		//�T�C�Y�ύX
	int SetColor(D3DCOLOR color);
	//�`��֘A
	int DrawBase(D3DPRIMITIVETYPE PrimitiveType, int PrimitiveCount, int offset);	///<�`�搔�w��`��
	int DrawBase2(D3DPRIMITIVETYPE PrimitiveType, int vertexnum, int offset);		///<�`�搔�w��`��
	int DrawBase(D3DPRIMITIVETYPE PrimitiveType);			///<�S�ĕ`��
	int Draw();
	//���_�ϊ��֘A
	int Move(float x,float y);						//���s�ړ�
	int Rotate(float r,float x,float y);			//�y����]
	int Scale(float dx,float dy,float x,float y);	//�g��k��
	int RotateY(float r,float x);			//�x����]
	//���_�����֘A
	int CreateRectBase(RECT& rect,D3DCOLOR color,int offset=0,int interval=1);
	int CreateRegularBase(int x,int y,int n,int r,D3DCOLOR color,int offset=0,int interval=1);		///<�����p�`�i�I�t�Z�b�g�{�Ԋu�w��j
	int CreateRegularBase(int x,int y,int z,int n,int r,D3DCOLOR color,int offset,int interval);		///<�����p�`
	int CreateParabolaBase(int x,int y,int n,float a,int lastx,D3DCOLOR color,int offset=0,int interval=1);

	void SetType(D3DPRIMITIVETYPE type){m_primType=type;}
};
/**
	@brief �����O�G�t�F�N�g

	�����Ƃ��ėp���������Ǝv����A�����O�̃G�t�F�N�g�B
*/
class SunEffectRing
{
public:
	int m_x,m_y;		//���S���W
	int m_inRadius;		//�������a
	int m_outRadius;	//�O�����a
	DWORD m_inColor;	//���F�i�f�t�H���g�͓����j
	DWORD m_outColor;	//�O�F�i�f�t�H���g�͔��j
	int m_num;			//���_��
public:
	SunEffectRing();
	int Draw();
};
/**
	@brief �y�����O�G�t�F�N�g

	�K�[�h�G�t�F�N�g��p��
*/
class SunEffectZRing
{
public:
	int m_x,m_y;		//���S���W
	int m_z;			//�O���̉��s��
	float m_angle;		//�x����]�̊p�x
	int m_inRadius;		//�������a
	int m_outRadius;	//�O�����a
	DWORD m_inColor;	//���F�i�f�t�H���g�͓����j
	DWORD m_outColor;	//�O�F�i�f�t�H���g�͔��j
	int m_num;			//���_��
public:
	SunEffectZRing();
	int Draw();
};
/**
	@brief ���������[�U�[�G�t�F�N�g

	�i���̏��łɃ}�X�^�[�X�p�[�N�I�I
	���Ȃ݂Ɂu�������v�̉p�P���Parabola
	�P�F�Ȃ̂Ŏ��ۂɂ͂Q�̂ق����g�����H
*/
class SunEffectLaser
{
public:
	int m_x,m_y;		//���S���W
	float m_a;			//���ix=a*y*y;)
	int m_num;			//
	int m_lastx;		//�ő�w���W�i��ʊO�Ȃ�640��0�j
	DWORD m_color;		//�F�i�f�t�H���g�͔��j
	float m_angle;		//�y����]�j�i�f�t�H���g�O�j
public:
	SunEffectLaser();
	int Draw();
};

/**
	@brief ���������[�U�[�G�t�F�N�g

	�i���̏��łɃ}�X�^�[�X�p�[�N�I�I
	���Ȃ݂Ɂu�������v�̉p�P���Parabola
	���k�ƊO�k�����B
*/
class SunEffectLaser2
{
public:
	int m_x,m_y;		//���S���W
	float m_ina;			//�����W���ix=a*y*y;)
	float m_outa;			//�O���W��
	int m_num;			//
	int m_lastx;		//�ő�w���W�i��ʊO�Ȃ�640��0�j
	DWORD m_inColor;	//�F�i�f�t�H���g�͔��j
	DWORD m_outColor;	//�F�i�f�t�H���g�͓����j
	float m_angle;		//�y����]�j�i�f�t�H���g�O�j
public:
	SunEffectLaser2();
	int Draw();
};
/**
	@breaf �~�`���C���[�t���[��
*/
class SunEffectCircle
{
public:
	int m_x,m_y;		//���S���W
	int m_radius;		//�������a
	DWORD m_color;		//���F�i�f�t�H���g�͓����j
	int m_num;			//���_��
public:
	SunEffectCircle();
	int Draw();
};
/**
	@brief ���N�g�G�t�F�N�g

	��ʑS�̂��t���b�V������̂Ɏg���ARECT�G�t�F�N�g�B
*/
class SunEffectRect
{
public:
	RECT m_rect;
	DWORD m_color;	//�F
public:
	SunEffectRect();
	int Draw();
};
/**
	@brief �������[�U�[�G�t�F�N�g
*/
class SunEffectLineLaser
{
public:
	float m_sx,m_sy;	///< �n�_
	float m_ex,m_ey;	///< �I�_
	float m_iw;			///< ����
	float m_ow;			///< �O��
	DWORD m_inColor;	///< ���F
	DWORD m_outColor;	///< �O�F
public:
	SunEffectLineLaser();
	int Draw();
};
/**
	@brief �Q�[�W�G�t�F�N�g

	�{�X�̃��C�t�\���ȂǂɎg���Q�[�W�G�t�F�N�g�B
*/
class SunEffectGauge
{
public:
	RECT	m_rect;		//�S�̗̂̈�
	double	m_point;	//�O�`�P�͈̔͂ŁA�Q�[�W�̗�
	DWORD	m_upColor;	//��̐F
	DWORD	m_downColor;	//���̐F
	int		m_yoko;		//�^�e�����R��
public:
	SunEffectGauge();
	int Draw();
};
/**
	@brief ���ڃG�t�F�N�g

	�~�X�e�B�A�����g���i�΁j�A���ڃG�t�F�N�g�B
*/
class SunEffectTorime
{
public:
	int m_x,m_y;		//���S���W�i�����@���W�����j
	int m_inRadius;		//���~���a
	int m_outRadius;		//�O�~���a
	DWORD m_inColor;		//�F�i�f�t�H���g�͍��j
	DWORD m_outColor;		//�F�i�f�t�H���g�͍��j
	int m_num;			//���~�̒��_��
public:
	SunEffectTorime();
	int Draw();
};
