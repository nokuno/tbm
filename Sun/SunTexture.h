/**
	@file SunTexture.h
	@brief SunTexture �N���X�̃C���^�[�t�F�C�X
*/

#pragma once
/**
	@class SunTexture
	@brief �e�N�X�`���N���X

	�e�N�X�`���N���X�ł��B
	�`��ɂ̓X�v���C�g�N���X���K�v�ł��B�B
*/
class SunTexture
{
	friend class SunSprite;
public:
	LPDIRECT3DTEXTURE9	m_pd3dtex;	///<�e�N�X�`��
	D3DSURFACE_DESC		m_d3dsd;	///<�e�N�X�`�����
	D3DXIMAGE_INFO		m_d3dii;	///<���摜�̏��
public:
	SunTexture();
	SunTexture(char* filename);
	virtual ~SunTexture();
	int LoadTexture(char* filename);
	int Load(char* filename,D3DCOLOR color = COLOR_TRANS);
	int Load(char* filename,D3DCOLOR color,D3DFORMAT format);
	int LoadSquare(char* filename,D3DCOLOR color = COLOR_TRANS);
	int LoadSquare(char* filename,D3DCOLOR color,D3DFORMAT format);
	int LoadRect(char* filename,RECT &rect,D3DCOLOR color = COLOR_TRANS);
	int LoadRect(char* filename,RECT &rect,D3DCOLOR color,D3DFORMAT format);
	int LoadRectFromMemory(void* pBuffer,DWORD dwSize,RECT &rect,D3DCOLOR color,D3DFORMAT format);
	int LoadFromPack(char* filename,D3DCOLOR color,D3DFORMAT format);
	int LoadSquareFromPack(char* filename,D3DCOLOR color,D3DFORMAT format);
	int Create(int width,int height,D3DFORMAT format,D3DPOOL pool = D3DPOOL_DEFAULT);
	int Create(int width,int height);
	int DrawFont(char* pStr,int x,int y,int size,COLORREF color,const char* pFontName="�l�r �S�V�b�N");
	int Release();
	int Draw(int x,int y);
	int Draw(int x,int y,D3DCOLOR color);
	int Draw(int x,int y,D3DCOLOR color,float scale);
	int Draw(int x,int y,D3DCOLOR color,float scale,float angle);
	int DrawRect(int x,int y,RECT &rect);
	int DrawRect(int x,int y,RECT &rect,D3DCOLOR color);
	int DrawRect(int x,int y,RECT &rect,D3DCOLOR color,float scale,float angle=0);
	int DrawGradation(int x,int y,D3DCOLOR under,D3DCOLOR top);
	int DrawGradation(int x,int y,D3DCOLOR under,D3DCOLOR top,float scale);
	int DrawRectEx(float x,float y,RECT &rect,D3DCOLOR color=alpha(255),float scale=1,float angle=0);
	int DrawRectClip(int x,int y,RECT &rect,D3DCOLOR color=alpha(255));
	int DrawClip(int x,int y,D3DCOLOR color=alpha(255));
};
