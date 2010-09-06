// SunEffect.cpp: SunEffect �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunExeption.h"
#include "SunApp.h"
#include "Sun3D.h"
#include "SunEffect.h"

const DWORD VXEFFECT::FVF = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SunEffect::SunEffect()
{
	m_primType = D3DPT_TRIANGLESTRIP;
}

SunEffect::~SunEffect()
{
	m_vertex.clear();
}
/**
	@breaf �|���S�����w��`��
	�|���S�����ƍŏ��̃|���S���̃C���f�b�N�X���w�肵�ĕ`�悵�܂��B
*/
int SunEffect::DrawBase(D3DPRIMITIVETYPE PrimitiveType, int PrimitiveCount, int offset)
{
	//�f�o�C�X�̍ő�`�搔�𗽉킵�Ă��肵�āB
	if(PrimitiveCount > (int)Sun3D.m_d3dcaps.MaxPrimitiveCount)
		return 0;
	//FVF�w��
	if(!Sun3D.SetFVF(VXEFFECT::FVF))	//�e���v���[�g�����̂e�u�e�w��
		return 0;
	//�e�N�X�`�����Z�b�g
	if(!Sun3D.SetTexture(NULL))
		return 0;
	//���悢��`��
	if(FAILED(Sun3D.m_pd3ddev->DrawPrimitiveUP(
										PrimitiveType,
										(UINT)PrimitiveCount,
										&this->m_vertex[offset],
										sizeof(VXEFFECT)
										)))
	{
		SunLog.LogOut("�v���~�e�B�u�`�掸�s\n");
		return 0;
	}
	return 1;
}
/**
	@breaf ���_���w��`��
	���_���ƍŏ��̃|���S���̃C���f�b�N�X���w�肵�ĕ`�悵�܂��B
*/
int SunEffect::DrawBase2(D3DPRIMITIVETYPE PrimitiveType, int vertexnum, int offset)
{
	//�����́u���_�̐��v�ł��邱�Ƃɒ��ӂ��ăv���~�e�B�u����n���B
	//���Ƃ��΁A�R�p�`�t�@���܂��̓X�g���b�v�̏ꍇ�A���_���[�Q���v���~�e�B�u���ɓ�����B
	int size;
	switch(PrimitiveType)
	{
	case D3DPT_POINTLIST:
		size = vertexnum;
		break;
	case D3DPT_LINELIST:
		size = vertexnum/2;
		break;
	case D3DPT_LINESTRIP:
		size = vertexnum-1;
		break;
	case D3DPT_TRIANGLELIST:
		size = vertexnum/3;
		break;
	case D3DPT_TRIANGLESTRIP:
		size = vertexnum-2;
		break;
	case D3DPT_TRIANGLEFAN:
		size = vertexnum-2;
		break;
	}
	if(size <= 0)
		return 1;
	if(!this->DrawBase(PrimitiveType,size,0))
		return 0;
	return 1;
}

/**
	@breaf �S�ĕ`��
	m_vertex�ɓ����Ă钸�_�S�Ďg���ĕ`�悵�܂��B
*/
int SunEffect::DrawBase(D3DPRIMITIVETYPE PrimitiveType)
{
	//���_����m_vertex�̃T�C�Y���A�I�t�Z�b�g�ɂO��n��
	if(!this->DrawBase2(PrimitiveType,m_vertex.size(),0))
		return 0;
	return 1;
}
/**
	@breaf �T�C�Y�ύX
*/
int SunEffect::Resize(int size)
{
	VXEFFECT vx;
	vx.x = 0;
	vx.y = 0;
	vx.z = 0;
	vx.w = 1;
	vx.c = alpha(255);
	m_vertex.resize(size,vx);
	return 1;
}

/**
	@brief ���s�ړ�
*/
int SunEffect::Move(float x,float y)
{
	for(int i=0;i<m_vertex.size();++i)
	{
		m_vertex[i].x += x;
		m_vertex[i].y += y;
	}
	return 1;
}
/**
	@brief �y����]
*/
int SunEffect::Rotate(float r,float x,float y)
{
	for(int i=0;i<m_vertex.size();++i)
	{
		float dx = m_vertex[i].x - x;
		float dy = m_vertex[i].y - y;
		float d = sqrt(dx*dx + dy*dy);	//����
		float a = atan2(dy,dx);			//�Ίp
		m_vertex[i].x = x + d * cos(a + r);
		m_vertex[i].y = y + d * sin(a + r);
	}
	return 1;
}
/**
	@brief �g��k��
*/
int SunEffect::Scale(float dx,float dy,float x,float y)
{
	for(int i=0;i<m_vertex.size();++i)
	{
		float lx = m_vertex[i].x - x;
		float ly = m_vertex[i].y - y;
		m_vertex[i].x = x + lx * dx;
		m_vertex[i].y = y + ly * dy;
	}
	return 1;
}
/**
	@brief �x����]
*/
int SunEffect::RotateY(float r,float x)
{
	for(int i=0;i<m_vertex.size();++i)
	{
		float dx = m_vertex[i].x - x;
		float dz = m_vertex[i].z - 0;
		float d = sqrt(dx*dx + dz*dz);	//����
		float a = atan2(dz,dx);			//�Ίp
		m_vertex[i].x = x + d * cos(a + r);
		m_vertex[i].z = 0 + d * sin(a + r);
	}
	return 1;
}
/**
	@brief RECT�쐬
*/
int SunEffect::CreateRectBase(RECT& rect,D3DCOLOR color,int offset,int interval)
{
	m_vertex[offset].x = rect.left;
	m_vertex[offset].y = rect.top;
	m_vertex[offset].c = color;
	m_vertex[offset+1*interval].x = rect.right;
	m_vertex[offset+1*interval].y = rect.top;
	m_vertex[offset+1*interval].c = color;
	m_vertex[offset+2*interval].x = rect.right;
	m_vertex[offset+2*interval].y = rect.bottom;
	m_vertex[offset+2*interval].c = color;
	m_vertex[offset+3*interval].x = rect.left;
	m_vertex[offset+3*interval].y = rect.bottom;
	m_vertex[offset+3*interval].c = color;
	return 1;
}
/**
	@brief �����p�`�쐬

	�O���݂̂Ȃ̂Łu�ʁv��`�悷��ɂ͒��S�_�������FAN���g������
	�g�p�O�ɂm�{�P�ɂq�������������邱��
*/
int SunEffect::CreateRegularBase(int x,int y,int n,int r,D3DCOLOR color,int offset,int interval)
{
	for(int i=0;i<n+1;++i)
	{
		int index = offset + i * interval;
		float a = (float) 2 *PAI * i/n;
		m_vertex[index].x = (float)x + (float)r * sin(a);
		m_vertex[index].y = (float)y + (float)r * cos(a);
		m_vertex[index].c = color;
	}
	return 1;
}
/**
	@brief �����p�`�쐬

	�y�l�ݒ�\
	�g�p�O�ɂm�{�P�ɂq�������������邱��
*/
int SunEffect::CreateRegularBase(int x,int y,int z,int n,int r,D3DCOLOR color,int offset,int interval)
{
	for(int i=0;i<n+1;++i)
	{
		int index = offset + i * interval;
		float a = (float) 2 *PAI * i/n;
		m_vertex[index].x = (float)x + (float)r * sin(a);
		m_vertex[index].y = (float)y + (float)r * cos(a);
		m_vertex[index].z = z;
		m_vertex[index].c = color;
	}
	return 1;
}
/**
	@brief �������쐬

	�g�p�O�ɂm�{�P�ɂq�������������邱��
*/
int SunEffect::CreateParabolaBase(int x,int y,int n,float a,int lastx,D3DCOLOR color,int offset,int interval)
{
	if(n%2)//n���
	{
		n-=1;
		for(int i=-n/2;i<=n/2;++i)
		{
			int index = offset+(i+n/2)*interval;
			m_vertex[index].x = (float)x + (float)(i*i)/(n/2*n/2)*(lastx-x);
			m_vertex[index].y = (float)y + (float)i*a;
			m_vertex[index].c = color;
		}
	}
	else//n������
	{
		for(int i=-n/2;i<=n/2;++i)
		{
			int index = offset+(i+n/2)*interval;
			m_vertex[index].x = (float)x + (float)(i*i)/(n/2*n/2)*(lastx-x);
			m_vertex[index].y = (float)y + (float)i*a;
			m_vertex[index].c = color;
		}
	}
	return 1;
}

int SunEffect::Draw()
{
	return this->DrawBase(m_primType);
}

//�R���X�g���N�^
SunEffectRing::SunEffectRing()
{
	m_x=320;m_y=240;
	m_inRadius=100;
	m_outRadius=200;
	m_inColor=alpha(0);
	m_outColor=alpha(255);
	m_num=20;
}
//�`��
int SunEffectRing::Draw()
{
	SunEffect effect;
	effect.Resize(m_num*2+2);
	effect.SetType(D3DPT_TRIANGLESTRIP);
	//���~
	effect.CreateRegularBase(m_x,m_y,m_num,m_inRadius,m_inColor,0,2);
	//�O�~
	effect.CreateRegularBase(m_x,m_y,m_num,m_outRadius,m_outColor,1,2);
	effect.Draw();
	return 1;
}
//�R���X�g���N�^
SunEffectZRing::SunEffectZRing()
{
	m_x=320;m_y=240;
	m_z=10;
	m_angle=-PAI/(2.0f+0.2f);
	m_inRadius=50;
	m_outRadius=80;
	m_inColor=alpha(0);
	m_outColor=alpha(255);
	m_num=20;
}
//�`��
int SunEffectZRing::Draw()
{
	SunEffect effect;
	effect.Resize(m_num*2+2);
	effect.SetType(D3DPT_TRIANGLESTRIP);
	//���~
	effect.CreateRegularBase(m_x,m_y,m_num,m_inRadius,m_inColor,0,2);
	//�O�~
	effect.CreateRegularBase(m_x,m_y,m_z,m_num,m_outRadius,m_outColor,1,2);
	//�x����]
	effect.RotateY(m_angle,m_x);
	effect.Draw();
	return 1;
}
//�R���X�g���N�^
SunEffectLaser::SunEffectLaser()
{
	m_x=320;m_y=240;
	m_a = 10;
	m_color=alpha(255);
	m_lastx=640;
	m_num=20;
	m_angle=0;
}
//�`��
int SunEffectLaser::Draw()
{
	SunEffect effect;
	effect.Resize(m_num+1);
	effect.SetType(D3DPT_TRIANGLEFAN);
	//�������쐬
	effect.CreateParabolaBase(m_x,m_y,m_num,m_a,m_lastx,m_color);
	//�y����]
	effect.Rotate(m_angle,m_x,m_y);
	effect.Draw();
	return 1;
}
//�R���X�g���N�^
SunEffectLaser2::SunEffectLaser2()
{
	m_x=320;m_y=240;
	m_ina = 10;
	m_outa = 20;
	m_inColor=alpha(255);
	m_outColor=alpha(0);
	m_lastx=640;
	m_num=20;
	m_angle=0;
}
//�`��
int SunEffectLaser2::Draw()
{
	//���k
	{
		SunEffect effect;
		effect.Resize(m_num+1);
		effect.SetType(D3DPT_TRIANGLEFAN);
		//�������쐬
		effect.CreateParabolaBase(m_x,m_y,m_num,m_ina,m_lastx,m_inColor);
		//�y����]
		effect.Rotate(m_angle,m_x,m_y);
		effect.Draw();
	}
	//�O�k
	{
		SunEffect effect;
		effect.Resize(m_num*2+2);
		effect.SetType(D3DPT_TRIANGLESTRIP);
		//�������쐬
		effect.CreateParabolaBase(m_x,m_y,m_num,m_ina,m_lastx,m_inColor,0,2);
		effect.CreateParabolaBase(m_x,m_y,m_num,m_outa,m_lastx,m_outColor,1,2);
		//�y����]
		effect.Rotate(m_angle,m_x,m_y);
		effect.Draw();
	}
	return 1;
}
//�R���X�g���N�^
SunEffectCircle::SunEffectCircle()
{
	m_x=320;m_y=240;
	m_radius=200;
	m_color=alpha(255);
	m_num=30;
}
//�`��
int SunEffectCircle::Draw()
{
	SunEffect effect;
	effect.Resize(m_num+1);
	effect.SetType(D3DPT_LINESTRIP);
	effect.CreateRegularBase(m_x,m_y,m_num,m_radius,m_color,0,1);
	effect.Draw();
	return 1;
}
//�R���X�g���N�^
SunEffectRect::SunEffectRect()
{
	SetRect(&m_rect,0,0,640,480);
	m_color=alpha(255);
}
//�`��
int SunEffectRect::Draw()
{
	SunEffect effect;
	effect.Resize(4);
	effect.SetType(D3DPT_TRIANGLEFAN);
	effect.CreateRectBase(m_rect,m_color);
	effect.Draw();
	return 1;
}
//�R���X�g���N�^
SunEffectLineLaser::SunEffectLineLaser()
{
	m_sx = 0;
	m_sy = 240;
	m_ex = 640;
	m_ey = 240;
	m_iw = 5;
	m_ow = 10;
	m_inColor = alpha(255);
	m_outColor = alpha(0);
}
//�`��
int SunEffectLineLaser::Draw()
{
	float dd = sqrt( pow2( m_sx - m_ex ) + pow2( m_sy - m_ey ) );
	float dx = ( m_sy - m_ey ) / dd;
	float dy = -( m_sx - m_ex ) / dd;
	float dx1 = dx * m_iw;
	float dy1 = dy * m_iw;
	float dx2 = dx * (m_iw+m_ow);
	float dy2 = dy * (m_iw+m_ow);
	{
		SunEffect effect;
		effect.Resize(4);
		effect.SetType(D3DPT_TRIANGLESTRIP);
		effect.m_vertex[0].x = m_sx + dx1;
		effect.m_vertex[0].y = m_sy + dy1;
		effect.m_vertex[0].c = m_inColor;;
		effect.m_vertex[1].x = m_ex + dx1;
		effect.m_vertex[1].y = m_ey + dy1;
		effect.m_vertex[1].c = m_inColor;;
		effect.m_vertex[2].x = m_sx - dx1;
		effect.m_vertex[2].y = m_sy - dy1;
		effect.m_vertex[2].c = m_inColor;
		effect.m_vertex[3].x = m_ex - dx1;
		effect.m_vertex[3].y = m_ey - dy1;
		effect.m_vertex[3].c = m_inColor;
		effect.Draw();
	}
	{
		SunEffect effect;
		effect.Resize(4);
		effect.SetType(D3DPT_TRIANGLESTRIP);
		effect.m_vertex[0].x = m_sx + dx1;
		effect.m_vertex[0].y = m_sy + dy1;
		effect.m_vertex[0].c = m_inColor;;
		effect.m_vertex[1].x = m_ex + dx1;
		effect.m_vertex[1].y = m_ey + dy1;
		effect.m_vertex[1].c = m_inColor;;
		effect.m_vertex[2].x = m_sx + dx2;
		effect.m_vertex[2].y = m_sy + dy2;
		effect.m_vertex[2].c = m_outColor;
		effect.m_vertex[3].x = m_ex + dx2;
		effect.m_vertex[3].y = m_ey + dy2;
		effect.m_vertex[3].c = m_outColor;
		effect.Draw();
	}
	{
		SunEffect effect;
		effect.Resize(4);
		effect.SetType(D3DPT_TRIANGLESTRIP);
		effect.m_vertex[0].x = m_sx - dx1;
		effect.m_vertex[0].y = m_sy - dy1;
		effect.m_vertex[0].c = m_inColor;;
		effect.m_vertex[1].x = m_ex - dx1;
		effect.m_vertex[1].y = m_ey - dy1;
		effect.m_vertex[1].c = m_inColor;;
		effect.m_vertex[2].x = m_sx - dx2;
		effect.m_vertex[2].y = m_sy - dy2;
		effect.m_vertex[2].c = m_outColor;
		effect.m_vertex[3].x = m_ex - dx2;
		effect.m_vertex[3].y = m_ey - dy2;
		effect.m_vertex[3].c = m_outColor;
		effect.Draw();
	}
	return 1;
}

//�R���X�g���N�^
SunEffectGauge::SunEffectGauge()
{
	SetRect(&m_rect,608,48,616,448);
	m_point = 1;
	m_upColor=xrgb(0,0,0);
	m_downColor=xrgb(255,255,255);
	m_yoko = 0;
}
//�`��
int SunEffectGauge::Draw()
{
	SunEffect effect;
	effect.Resize(4);
	if( m_yoko == 0 ) {
		effect.SetType(D3DPT_TRIANGLEFAN);
		effect.m_vertex[0].x = m_rect.left;
		effect.m_vertex[0].y = m_rect.bottom - m_point * (m_rect.bottom - m_rect.top );
		effect.m_vertex[0].c = m_upColor;
		effect.m_vertex[1].x = m_rect.right;
		effect.m_vertex[1].y = m_rect.bottom - m_point * (m_rect.bottom - m_rect.top );
		effect.m_vertex[1].c = m_upColor;
		effect.m_vertex[2].x = m_rect.right;
		effect.m_vertex[2].y = m_rect.bottom;
		effect.m_vertex[2].c = m_downColor;
		effect.m_vertex[3].x = m_rect.left;
		effect.m_vertex[3].y = m_rect.bottom;
		effect.m_vertex[3].c = m_downColor;
		effect.Draw();
	}
	else {
		effect.SetType(D3DPT_TRIANGLEFAN);
		effect.m_vertex[0].x = m_rect.left;
		effect.m_vertex[0].y = m_rect.top;
		effect.m_vertex[0].c = m_upColor;
		effect.m_vertex[1].x = m_rect.left + m_point * (m_rect.right - m_rect.left );
		effect.m_vertex[1].y = m_rect.top;
		effect.m_vertex[1].c = m_upColor;
		effect.m_vertex[2].x = m_rect.left + m_point * (m_rect.right - m_rect.left );
		effect.m_vertex[2].y = m_rect.bottom;
		effect.m_vertex[2].c = m_downColor;
		effect.m_vertex[3].x = m_rect.left;
		effect.m_vertex[3].y = m_rect.bottom;
		effect.m_vertex[3].c = m_downColor;
		effect.Draw();
	}
	return 1;
}
//�R���X�g���N�^
SunEffectTorime::SunEffectTorime()
{
	m_inRadius = 60;
	m_outRadius = 120;
	m_inColor = 0;
	m_outColor = xrgb(0,0,0);
	m_num = 20;
}
//�`��
int SunEffectTorime::Draw()
{
	SunEffect effect;

	RECT rect = {0,0,640,480};
	effect.Resize((m_num+1)*8+1);
	for( int i = 0; i < m_num*5/4+1; i++ ) {
		int index = i * 2;
		effect.m_vertex[index].c = m_outColor;
		if( i < (m_num+1)/4 ) {
			effect.m_vertex[index].x = 640;
			effect.m_vertex[index].y = 480;
		}
		else if( i < 2 * (m_num+1)/4 ) {
			effect.m_vertex[index].x = 640;
			effect.m_vertex[index].y = 0;
		}
		else if( i < 3 * (m_num+1)/4 ) {
			effect.m_vertex[index].x = 0;
			effect.m_vertex[index].y = 0;
		}
		else if( i < 4 * (m_num+1)/4 ) {
			effect.m_vertex[index].x = 0;
			effect.m_vertex[index].y = 480;
		}
		else {
			effect.m_vertex[index].x = 640;
			effect.m_vertex[index].y = 480;
		}
	}
	effect.CreateRegularBase(m_x,m_y,m_num,m_outRadius,m_outColor,1,2);	
	effect.Resize((m_num+1)*2+1);
	effect.Draw();

	
	effect.Resize((m_num+1)*2);
	effect.SetType(D3DPT_TRIANGLESTRIP);
	effect.CreateRegularBase(m_x,m_y,m_num,m_inRadius,m_inColor,0,2);
	effect.CreateRegularBase(m_x,m_y,m_num,m_outRadius,m_outColor,1,2);
	effect.Draw();
	return 1;
}
