/**
	@file SunSprite3D.cpp
	@brief SunSprite3D �N���X�̃C���v�������e�[�V����
*/
#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "Sun3D.h"
#include "SunTexture.h"
#include "SunSprite3D.h"
#include "SunExeption.h"


const DWORD VXSPRITE3D::FVF = ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE| D3DFVF_TEX1 );
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
SunSprite3D::SunSprite3D(SunTexture* ptex)
{
	this->Init();
	m_ptex		= ptex;
	RECT rect = {0,0,ptex->m_d3dsd.Width,ptex->m_d3dsd.Height};
	this->SetSourceRect(rect);
}
int SunSprite3D::Init()
{
//	m_ptex	= NULL;
//	::ZeroMemory(m_vertex,sizeof(m_vertex));
	for(int i=0;i<4;++i)
	{
		m_vertex[i].c = xrgb(255,255,255);
	}
	return 1;
}
SunSprite3D::~SunSprite3D()
{

}
/**
	@brief �`��
*/
int SunSprite3D::Draw()
{
	if(m_ptex == NULL)
		return 0;
	if(!Sun3D.SetTexture(m_ptex->m_pd3dtex))
		return 0;
	//FVF�w��
	if(!Sun3D.SetFVF(VXSPRITE3D::FVF))	//�e���v���[�g�����̂e�u�e�w��
		return 0;
	//���悢��`��
	if(FAILED(Sun3D.m_pd3ddev->DrawPrimitiveUP(
										D3DPT_TRIANGLEFAN,
										2,
										&this->m_vertex[0],
										sizeof(VXSPRITE3D)
										)))
	{
		throw SunExeption("�v���~�e�B�u�`�掸�s\n");
	}
	return 1;
}
/**
	@brief �e�N�X�`���ݒ�
*/
int SunSprite3D::SetTexture(SunTexture* ptex)
{
	m_ptex = ptex;
	return 1;
}
/**
	@brief XY���ʂɕ��s�ȋ�`�쐬
*/
int SunSprite3D::CreateRectXY(RECT &rect,float z)
{
	//����
	m_vertex[0].x = rect.left;
	m_vertex[0].y = rect.top;
	m_vertex[0].z = z;
	//�E��
	m_vertex[1].x = rect.right;
	m_vertex[1].y = rect.top;
	m_vertex[1].z = z;
	//�E��
	m_vertex[2].x = rect.right;
	m_vertex[2].y = rect.bottom;
	m_vertex[2].z = z;
	//����
	m_vertex[3].x = rect.left;
	m_vertex[3].y = rect.bottom;
	m_vertex[3].z = z;
	return 1;
}
/**
	@brief YZ���ʂɕ��s�ȋ�`�쐬
*/
int SunSprite3D::CreateRectYZ(RECT &rect,float x)
{
	//����
	m_vertex[0].y = rect.left;
	m_vertex[0].z = rect.top;
	m_vertex[0].x = x;
	//�E��
	m_vertex[1].y = rect.right;
	m_vertex[1].z = rect.top;
	m_vertex[1].x = x;
	//�E��
	m_vertex[2].y = rect.right;
	m_vertex[2].z = rect.bottom;
	m_vertex[2].x = x;
	//����
	m_vertex[3].y = rect.left;
	m_vertex[3].z = rect.bottom;
	m_vertex[3].x = x;
	return 1;
}
/**
	@brief ZX���ʂɕ��s�ȋ�`�쐬
*/
int SunSprite3D::CreateRectZX(RECT &rect,float y)
{
	//����
	m_vertex[0].z = rect.left;
	m_vertex[0].x = rect.top;
	m_vertex[0].y = y;
	//�E��
	m_vertex[1].z = rect.right;
	m_vertex[1].x = rect.top;
	m_vertex[1].y = y;
	//�E��
	m_vertex[2].z = rect.right;
	m_vertex[2].x = rect.bottom;
	m_vertex[2].y = y;
	//����
	m_vertex[3].z = rect.left;
	m_vertex[3].x = rect.bottom;
	m_vertex[3].y = y;
	return 1;
}
/**
	@brief ���_�F�̐ݒ�
*/
int SunSprite3D::SetColor(D3DCOLOR color)
{
	for(int i=0;i<4;++i)
		m_vertex[i].c = color;
	return 1;
}
/**
	@brief ���_�F�̐ݒ�
*/
int SunSprite3D::SetGradationColor(D3DCOLOR under,D3DCOLOR top)
{
	m_vertex[0].c = top;
	m_vertex[1].c = top;
	m_vertex[2].c = under;
	m_vertex[3].c = under;
	return 1;
}
/**
	@brief �e�N�X�`���̈�ݒ�

	������t�u���W�̐ݒ�ł��B
	�e�N�X�`�����Z�b�g����Ă���K�v������܂��B
*/
int SunSprite3D::SetSourceRect(RECT &rcSrc)
{
	//�e�N�X�`�����Z�b�g����Ă��Ȃ���Ύ��s
	if(m_ptex == NULL)
		return 0;
	//���
	float width		= m_ptex->m_d3dsd.Width;
	float height	= m_ptex->m_d3dsd.Height;
	//UV�ݒ�
	//---����
	m_vertex[0].u = (float)rcSrc.left		/ width;
	m_vertex[0].v = (float)rcSrc.top		/ height;
	//---�E��
	m_vertex[1].u = (float)(rcSrc.right-1)	/ width;
	m_vertex[1].v = (float)rcSrc.top		/ height;
	//---�E��
	m_vertex[2].u = (float)(rcSrc.right-1)	/ width;
	m_vertex[2].v = (float)(rcSrc.bottom-1)	/ height;
	//---����
	m_vertex[3].u = (float)rcSrc.left		/ width;
	m_vertex[3].v = (float)(rcSrc.bottom-1)	/ height;
	return 1;
}

/**
	@brief �e�N�X�`���J��Ԃ��ݒ�

	�摜�S�̂��g�����Ƃ�O��ɁA�J��Ԃ��e�N�X�`����ݒ肵�܂��B
	�e�N�X�`�����Z�b�g����Ă���K�v������܂��B
*/
int SunSprite3D::SetRepeat(float rx,float ry)
{
	//�e�N�X�`�����Z�b�g����Ă��Ȃ���Ύ��s
	if(m_ptex == NULL)
		return 0;
	//UV�ݒ�
	//---����
	m_vertex[0].u = 0;
	m_vertex[0].v = 0;
	//---�E��
	m_vertex[1].u = rx;
	m_vertex[1].v = 0;
	//---�E��
	m_vertex[2].u = rx;
	m_vertex[2].v = ry;
	//---����
	m_vertex[3].u = 0;
	m_vertex[3].v = ry;
	return 1;
}
/**
	@brief �y����]

	���S�_�͐�΍��W�ł��B
*/
int SunSprite3D::Rotate(float a,float x,float y)
{
	for(int i=0;i<4;++i)
	{
		float dx = m_vertex[i].x - x;
		float dy = m_vertex[i].y - y;
		float d = sqrt(dx*dx + dy*dy);	//����
		float ang = atan2(dy,dx);		//�Ίp
		m_vertex[i].x = x + d * cos(a + ang);
		m_vertex[i].y = y + d * sin(a + ang);
	}
	return 1;
}
/**
	@brief �g��k��

	���S�_�͐�΍��W�ł��B
*/
int SunSprite3D::Scale(float mx,float my,float x,float y)
{
	for(int i=0;i<4;++i)
	{
		float lx = m_vertex[i].x - x;
		float ly = m_vertex[i].y - y;
		m_vertex[i].x = x + lx * mx;
		m_vertex[i].y = y + ly * my;
	}
	return 1;
}
/**
	@brief ���s�ړ�
*/
int SunSprite3D::Move(float x,float y,float z)
{
	for(int i=0;i<4;++i)
	{
		m_vertex[i].x += x;
		m_vertex[i].y += y;
		m_vertex[i].z += z;
	}
	return 1;
}