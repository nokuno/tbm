// SunFont.h: SunFont �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SunTexture.h"

struct SunFontTexture {
	SunTexture		m_tex;
	GLYPHMETRICS	m_gm;
};
/**
	�t�H���g�N���X

	�S�p�����P���P�e�N�X�`���Ƃ��āA�e�N�X�`�����X�g�ɕۑ����܂��B
	��Ƀt�H���g������Ă����΂����̃e�N�X�`���̕`��Ȃ̂ŁA
	�p�t�H�[�}���X�͉摜�t�@�C�����g���̂ƕς��Ȃ��͂��B
	����A�e�N�X�`������������Ă镪�x�����ǁi���j
*/
class SunFont
{
public:
	list<SunFontTexture*>	m_lstpTex;
	string		m_str;			///< ������
	TEXTMETRIC	m_tm;			///< ���݂̃t�H���g���
	int			m_interval;		///< �s��
public:
	SunFont();
	SunFont(char* pStr,int fontSize=16,const char* pFontName="�l�r �S�V�b�N",int interval = 4);
	virtual ~SunFont();
	int Release();
	int Create(char* pStr,int fontSize=16,const char* pFontName="�l�r �S�V�b�N");
	int CreateList(char* pStr,int fontSize=16,const char* pFontName="�l�r �S�V�b�N");
	int Draw(int x,int y,D3DCOLOR color=alpha(255),double scale=1,double angle=0);
	int DrawGradation(int x,int y,D3DCOLOR under=D3DCOLOR_XRGB(255,0,0),D3DCOLOR top=alpha(255),double scale=1,double angle=0);
	int DrawFormat(int x,int y,D3DCOLOR color=D3DCOLOR_XRGB(255,0,0),BYTE a=255);
	int DrawFlame(int x,int y,D3DCOLOR c1=D3DCOLOR_XRGB(255,255,255),D3DCOLOR c2=D3DCOLOR_XRGB(0,0,0));

	//�A���`�G�C���A�X���t�H���g�`��֐�
	HRESULT CreateTextureFont( SunFontTexture* pTexture, char *str, int w, int h, LPCTSTR font_name, D3DCOLOR color );

};
typedef list<SunFontTexture*>::iterator SunFontItr;

