// SunTexture.cpp: SunTexture �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunExeption.h"
#include "SunFile.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "Sun3D.h"
#include "SunSurface.h"
#include "SunTexture.h"
#include "SunSprite.h"
#include "SunSprite3D.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SunTexture::SunTexture()
{
	m_pd3dtex = NULL;
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
	::ZeroMemory(&m_d3dii,sizeof(D3DXIMAGE_INFO));
}

SunTexture::SunTexture(char* filename)
{
	m_pd3dtex=NULL;
	this->LoadTexture(filename);
}
SunTexture::~SunTexture()
{
	this->Release();
}
SunTexture::LoadTexture(char *filename){
	if( !Sun3D.m_square )
		return this->LoadFromPack(filename,COLOR_TRANS,Sun3D.m_format);
	else
		return this->LoadSquareFromPack(filename,COLOR_TRANS,Sun3D.m_format);
}

//!���
int SunTexture::Release()
{
	SAFE_RELEASE(m_pd3dtex);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
	return 1;
}
/**
	@brief �ǂݍ���
*/
int SunTexture::Load(char *filename,D3DCOLOR color,D3DFORMAT format)
{
	this->Release();
	if(FAILED(::D3DXGetImageInfoFromFile(filename,&m_d3dii)))	//�I���W�i�����
		throw SunExeption("[ %s ] �t�@�C����������܂���B\n",filename);
	if(FAILED(D3DXCreateTextureFromFileExA(
				Sun3D.m_pd3ddev,
				filename,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				1,
				0,
				format,
				D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				color,
				NULL,
				NULL,
				&m_pd3dtex )))
	{
		throw SunExeption("%s�F�e�N�X�`���ǂݍ��݃G���[\n",filename);
	}
	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);			//�e�N�X�`�����
	SunLog.LogOut("[ %s ] [ %d �~ %d ]�@�摜�ǂݍ��ݐ���\n",filename,m_d3dii.Width,m_d3dii.Height);
	return 1;
}

//�t�H�[�}�b�g�Œ�
int SunTexture::Load(char *filename,D3DCOLOR color)
{
	return this->Load(filename,color,Sun3D.m_format);
}

//�����`�e�N�X�`���փ��[�h
int SunTexture::LoadSquare(char* filename,D3DCOLOR color,D3DFORMAT format)
{
	this->Release();
	if(FAILED(::D3DXGetImageInfoFromFile(filename,&m_d3dii)))	//�I���W�i�����
		throw SunExeption("[ %s ] �t�@�C����������܂���B\n",filename);
	int width = m_d3dii.Width;
	int height = m_d3dii.Height;
	int size = ceil2(max(width,height));
	RECT rect = {0,0,width,height};
	if(!this->Create(size,size,format))		//�����`�̃e�N�X�`���쐬
		return 0;
	SunSurface surf;	//�e�N�X�`���T�[�t�F�[�X
	surf.GetTextureSurface(m_pd3dtex,width,height);	//�e�N�X�`���������Ŏg���T�[�t�F�[�X�擾
	if(!surf.LoadRect(filename,rect,color))
		return 0;

	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);			//�e�N�X�`�����
	return 1;
}
//�t�H�[�}�b�g�Œ�
int SunTexture::LoadSquare(char *filename,D3DCOLOR color)
{
	return this->LoadSquare(filename,color,Sun3D.m_format);
}
/**
	@brief �̈�w�胍�[�h

	������SunSurface::LoadRect���g�����A������͊֐�����Create���Ă���B
	Create����T�C�Y�͎w�肳�ꂽ�̈���傫���B
*/
int SunTexture::LoadRect(char* filename,RECT &rect,D3DCOLOR color,D3DFORMAT format)
{
	this->Release();
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int size = ceil2(max(width,height));
	if(!this->Create(size,size,format))		//�����`�̃e�N�X�`���쐬
		return 0;
	SunSurface surf;	//�e�N�X�`���T�[�t�F�[�X
	surf.GetTextureSurface(m_pd3dtex,width,height);	//�e�N�X�`���������Ŏg���T�[�t�F�[�X�擾
	if(!surf.LoadRect(filename,rect,color))
		return 0;

	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);			//�e�N�X�`�����
	return 1;
}
//�t�H�[�}�b�g�Œ�
int SunTexture::LoadRect(char *filename, RECT &rect, D3DCOLOR color)
{
	return this->LoadRect(filename, rect, color, Sun3D.m_format);
}
//����������ǂݍ���
int SunTexture::LoadRectFromMemory(void* pBuffer,DWORD dwSize,RECT &rect,D3DCOLOR color,D3DFORMAT format)
{
	this->Release();
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	int size = ceil2(max(width,height));
	if(!this->Create(size,size,format))		//�����`�̃e�N�X�`���쐬
		return 0;
	SunSurface surf;	//�e�N�X�`���T�[�t�F�[�X
	surf.GetTextureSurface(m_pd3dtex,width,height);	//�e�N�X�`���������Ŏg���T�[�t�F�[�X�擾
	if(!surf.LoadRectFromMemory(pBuffer,dwSize,rect,color))
		return 0;

	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);			//�e�N�X�`�����
	return 1;
}
//�p�b�N�t�@�C�����烍�[�h
int SunTexture::LoadFromPack(char* filename,D3DCOLOR color,D3DFORMAT format)
{
	SunFileEx file;
	if(file.Open(filename) == 0)
		throw SunExeption("[ %s ] �t�@�C����������܂���B\n",filename);
	if(file.Read() == 0)
		throw SunExeption("[ %s ] �t�@�C�����ǂݍ��߂܂���B\n",filename);
	this->Release();
	if(FAILED(::D3DXGetImageInfoFromFileInMemory(file.GetBuffer(),file.GetSize(),&m_d3dii)))	//�I���W�i�����
		throw SunExeption("[ %s ] �t�@�C�����摜�ł͂���܂���B\n",filename);

	if(FAILED(D3DXCreateTextureFromFileInMemoryEx(
				Sun3D.m_pd3ddev,
				file.GetBuffer(),
				file.GetSize(),
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				1,
				0,
				format,
				D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				color,
				NULL,
				NULL,
				&m_pd3dtex )))
	{
		throw SunExeption("%s�F�e�N�X�`���ǂݍ��݃G���[\n",filename);
	}
	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);			//�e�N�X�`�����
	return 1;
}
//�p�b�N�t�@�C�����烍�[�h
int SunTexture::LoadSquareFromPack(char* filename,D3DCOLOR color,D3DFORMAT format)
{
	SunFileEx file;
	if(file.Open(filename) == 0)
		throw SunExeption("[ %s ] �t�@�C����������܂���B\n",filename);
	if(file.Read() == 0)
		throw SunExeption("[ %s ] �t�@�C�����ǂݍ��߂܂���B\n",filename);
	if(FAILED(::D3DXGetImageInfoFromFileInMemory(file.GetBuffer(),file.GetSize(),&m_d3dii)))	//�I���W�i�����
		throw SunExeption("[ %s ] �t�@�C�����摜�ł͂���܂���B\n",filename);

	int width = m_d3dii.Width;
	int height = m_d3dii.Height;
	int size = ceil2(max(width,height));
	RECT rect = {0,0,width,height};
	this->Release();
	if(!this->Create(size,size,format))		//�����`�̃e�N�X�`���쐬
		return 0;
	SunSurface surf;	//�e�N�X�`���T�[�t�F�[�X
	surf.GetTextureSurface(m_pd3dtex,width,height);	//�e�N�X�`���������Ŏg���T�[�t�F�[�X�擾
	if(!surf.LoadRectFromMemory(file.GetBuffer(),file.GetSize(),rect,color))
		return 0;

	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);			//�e�N�X�`�����
	return 1;
}
/**
	@brief ��̃e�N�X�`���쐬
*/
int SunTexture::Create(int width,int height, D3DFORMAT format, D3DPOOL pool)
{
	this->Release();
	if(FAILED(Sun3D.m_pd3ddev->CreateTexture(
		width,
		height,
		0,
		0,
		format,
		pool,
		&m_pd3dtex,
		NULL
		)))
	{
		throw SunExeption("[%d �~ %d]�e�N�X�`���o�b�t�@���m�ۂł��܂���ł����B\n",width,height);
	}
	m_pd3dtex->GetLevelDesc(0,&m_d3dsd);
	return 1;
}
//�t�H�[�}�b�g�Œ�
int SunTexture::Create(int width,int height)
{
	return this->Create(width, height, Sun3D.m_format,D3DPOOL_DEFAULT);
}
/**
	�t�H���g�쐬

	�e�N�X�`��������Ă���ĂԂ���
*/
int SunTexture::DrawFont(char* pStr,int x,int y,int size,COLORREF color,const char* pFontName)
{
	SunSurface surf;	//�e�N�X�`���T�[�t�F�[�X
	surf.GetTextureSurface(m_pd3dtex, m_d3dsd.Width, m_d3dsd.Height);
	SunSurface font;	//�ꎞ�T�[�t�F�[�X
	//font.Create(m_d3dsd.Width, m_d3dsd.Height);
	font.Create(m_d3dsd.Width-x, m_d3dsd.Height-y);
	font.Fill(xrgb(255,0,255));
	font.Text(pStr,0,0,size,color,pFontName);
	
	RECT rcDest={x,y,m_d3dsd.Width, m_d3dsd.Height};
	if(FAILED(D3DXLoadSurfaceFromSurface(surf.m_pd3ds,NULL,&rcDest,font.m_pd3ds,NULL,NULL,D3DX_FILTER_NONE,xrgb(255,0,255))))
		throw("�t�H���g�T�[�t�F�[�X->�e�N�X�`���]�����s\n");
	return 1;
}

//-------------------��������`��n---------------------------------

int SunTexture::Draw(int x,int y)
{
	SunSprite sprite(this);
	RECT rect={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	sprite.CreateRect(rect);
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	sprite.SetSourceRect(src);
	sprite.SetColor(alpha(255));
	sprite.DrawEx();
	return 1;
}

int SunTexture::Draw(int x,int y,D3DCOLOR color)
{
	SunSprite sprite(this);
	RECT rect={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	sprite.CreateRect(rect);
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	sprite.DrawEx();
	return 1;
}

int SunTexture::Draw(int x,int y,D3DCOLOR color,float scale)
{
	SunSprite sprite(this);
	RECT rect={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	sprite.CreateRect(rect);
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	sprite.Scale(scale,scale,x+m_d3dsd.Width/2,y+m_d3dsd.Height/2);
	sprite.DrawEx();
	return 1;
}
int SunTexture::Draw(int x,int y,D3DCOLOR color,float scale,float angle)
{
	SunSprite sprite(this);
	RECT rect={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	sprite.CreateRect(rect);
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	sprite.Rotate(angle,x+m_d3dii.Width/2,y+m_d3dii.Height/2);
	sprite.Scale(scale,scale,x+m_d3dsd.Width/2,y+m_d3dsd.Height/2);
	sprite.DrawEx();
	return 1;
}
int SunTexture::DrawRect(int x,int y,RECT &rect)
{
	SunSprite sprite(this);
	RECT dest={x,y,x+rect.right-rect.left,y+rect.bottom-rect.top};
	sprite.CreateRect(dest);
	RECT src=rect;
	sprite.SetSourceRect(src);
	sprite.SetColor(alpha(255));
	sprite.DrawEx();
	return 1;
}
int SunTexture::DrawRect(int x,int y,RECT &rect,D3DCOLOR color)
{
	SunSprite sprite(this);
	RECT dest={x,y,x+rect.right-rect.left,y+rect.bottom-rect.top};
	sprite.CreateRect(dest);
	RECT src=rect;
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	sprite.DrawEx();
	return 1;
}
int SunTexture::DrawRect(int x,int y,RECT &rect,D3DCOLOR color,float scale,float angle)
{
	SunSprite sprite(this);
	RECT dest={x,y,x+rect.right-rect.left,y+rect.bottom-rect.top};
	sprite.CreateRect(dest);
	RECT src=rect;
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	float cx = dest.left + (dest.right-dest.left)/2;
	float cy = dest.top + (dest.bottom-dest.top)/2;
	sprite.Rotate(angle,cx,cy);
	sprite.Scale(scale,scale,cx,cy);
	sprite.DrawEx();
	return 1;
}
int SunTexture::DrawGradation(int x,int y,D3DCOLOR under,D3DCOLOR top)
{
	SunSprite sprite(this);
	RECT rect={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	sprite.CreateRect(rect);
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	sprite.SetSourceRect(src);
	sprite.SetGradationColor(under,top);
	sprite.DrawEx();
	return 1;
}
int SunTexture::DrawGradation(int x,int y,D3DCOLOR under,D3DCOLOR top,float scale)
{
	SunSprite sprite(this);
	RECT rect={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	sprite.CreateRect(rect);
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	sprite.SetSourceRect(src);
	sprite.SetGradationColor(under,top);
	sprite.Scale(scale,scale,x+m_d3dsd.Width/2,y+m_d3dsd.Height/2);
	sprite.DrawEx();
	return 1;
}
//�����_���W�ɑΉ�
int SunTexture::DrawRectEx(float x,float y,RECT &rect,D3DCOLOR color,float scale,float angle)
{
	SunSprite sprite(this);
	RECT dest={x,y,x+rect.right-rect.left,y+rect.bottom-rect.top};
	sprite.CreateRectEx(x,y,rect.right-rect.left,rect.bottom-rect.top);
	RECT src=rect;
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	float cx = dest.left + (dest.right-dest.left)/2;
	float cy = dest.top + (dest.bottom-dest.top)/2;
	sprite.Rotate(angle,cx,cy);
	sprite.Scale(scale,scale,cx,cy);
	sprite.DrawEx();
	return 1;
}
//�N���b�v�@�\��
int SunTexture::DrawRectClip(int x,int y,RECT &rect,D3DCOLOR color)
{
	RECT clip = {24,48,600,448};
	SunSprite sprite(this);
	RECT dest={x,y,x+rect.right-rect.left,y+rect.bottom-rect.top};
	RECT src=rect;
	if( dest.left < clip.left ) {
		src.left += clip.left - dest.left;
		dest.left = clip.left;
	}
	if( dest.top < clip.top ) {
		src.top += clip.top - dest.top;
		dest.top = clip.top;
	}
	if( dest.right > clip.right ) {
		src.right += clip.right - dest.right;
		dest.right = clip.right;
	}
	if( dest.bottom > clip.bottom ) {
		src.bottom += clip.bottom - dest.bottom;
		dest.bottom = clip.bottom;
	}
	sprite.CreateRect(dest);
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	sprite.DrawEx();
	return 1;
}
//�N���b�v�@�\��
int SunTexture::DrawClip(int x,int y,D3DCOLOR color)
{
	RECT clip = {24,48,600,448};
	SunSprite sprite(this);
	RECT dest={x,y,x+m_d3dsd.Width,y+m_d3dsd.Height};
	RECT src={0,0,m_d3dsd.Width,m_d3dsd.Height};
	if( dest.left < clip.left ) {
		src.left += clip.left - dest.left;
		dest.left = clip.left;
	}
	if( dest.top < clip.top ) {
		src.top += clip.top - dest.top;
		dest.top = clip.top;
	}
	if( dest.right > clip.right ) {
		src.right += clip.right - dest.right;
		dest.right = clip.right;
	}
	if( dest.bottom > clip.bottom ) {
		src.bottom += clip.bottom - dest.bottom;
		dest.bottom = clip.bottom;
	}
	sprite.CreateRect(dest);
	sprite.SetSourceRect(src);
	sprite.SetColor(color);
	sprite.DrawEx();
	return 1;
}