/**
	@file SunSurface.h
	@brief �T�[�t�F�[�X
*/
#pragma once

#include "SunTask.h"
/**
	@class SunSurface
	@brief �T�[�t�F�[�X
	
	���܂�g���@��͂���܂��񂪁A�T�[�t�F�[�X�N���X�ł��B
	�A���t�@���ō쐬����Ƃc�b���g�����e�L�X�g�`�悪�ł��܂���B
	�e�N�X�`������擾�ł���T�[�t�F�[�X�̓A���t�@���ł��B
	���ʂɍ��Ɠ��߂��ł��Ȃ������Ƀe�N�X�`���T�C�Y�̐������Ȃ��Ƃ��������b�g��������������܂��B
*/
class SunSurface  
{
	friend class SunSurfaceSprite;
public:
	LPDIRECT3DSURFACE9	m_pd3ds;
	D3DXIMAGE_INFO		m_d3dii;	//�I���W�i���C���[�W���
	D3DSURFACE_DESC		m_d3dsd;	//�e�N�X�`�����
public:
	SunSurface();
	SunSurface(char* pFile);
	virtual ~SunSurface();
	int Release();
	int GetBackBuffer();
	int Load(char* lpszFileName);
	int LoadFromPack(char* lpszFileName);
	int	Create(int width,int height);
	int	Create(int width,int height,D3DFORMAT format);
	int	Create(int width,int height,D3DFORMAT format,D3DPOOL pool);
		//�Z�`�]���B�]����͎������g�B�o�b�N�o�b�t�@�Ȃ�`��ɂȂ�B
		//�A���t�@�l���ō쐬�����ꍇ�͎��s���邱�Ƃɒ��ӁB
		int Blit(LPDIRECT3DSURFACE9 pd3ds,int x,int y,RECT &rcSrc);
		int Blit(SunSurface *ps,int x,int y,RECT &rcSrc);
		int Blit(SunSurface *ps,int x,int y);
		int Draw(int x,int y,RECT &rcSrc);		///<�����Ńo�b�N�o�b�t�@���擾���ē]��
		int Draw(int x,int y);
		void Draw(int x,int y,int layer);
		void Draw(int x,int y,RECT &rcSrc,int layer);
		//�t�H���g�`��
		int Text(const char* text,int x,int y,int size,COLORREF color,const char* pFontName="�l�r �S�V�b�N");
		//�h��Ԃ�
		int Fill( D3DCOLOR color, RECT &rect);
		int Fill( D3DCOLOR color);
		LPDIRECT3DSURFACE9 GetSurface(){return m_pd3ds;}
	//���̊֐��ō��ƃA���t�@���ɂȂ邱�Ƃɒ��ӁB
	int GetTextureSurface(LPDIRECT3DTEXTURE9 pd3dtex,int width,int height);
	//���̊֐��̓e�N�X�`������g���̂ŃT�[�t�F�[�XCreate�͂��Ȃ�
	int LoadRect(char* lpszFileName,RECT &rect,D3DCOLOR color);
	int LoadRectFromMemory(void* pBuffer,DWORD dwSize,RECT &rect,D3DCOLOR color);
	//�X�i�b�v�V���b�g�p
	int SaveSurface(LPCTSTR pDestFile,D3DXIMAGE_FILEFORMAT DestFormat,RECT *pSrcRect);
};
/**
	�T�[�t�F�[�X�X�v���C�g�N���X

	�`��L���[�Ή��̃X�v���C�g�ł��B
*/
class SunSurfaceSprite : public SunTask
{
private:
	SunSurface* m_pSurface;
	int m_x,m_y;
	RECT m_rect;
public:
	SunSurfaceSprite(SunSurface* pSurface,int x,int y);
	SunSurfaceSprite(SunSurface* pSurface,int x,int y,RECT rect);
	virtual int operator()();
};

//FPS�\���p�T�[�t�F�[�X
class SunFpsSurface : public SunTask
{
private:
	SunSurface m_surf;
public:
	SunFpsSurface(){}
	virtual ~SunFpsSurface(){};
	virtual int operator()();
	void Create(){m_surf.Create(48,15);}
};
extern SunFpsSurface SunFpsShow;

//�X�i�b�v�V���b�g�ۑ��֐�
int SaveSnapShot(D3DXIMAGE_FILEFORMAT DestFormat);
int SaveSnapShot();