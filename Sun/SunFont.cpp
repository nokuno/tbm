// SunFont.cpp: SunFont �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "Sun3D.h"
#include "SunSurface.h"
#include "SunTexture.h"
#include "SunExeption.h"
#include "SunSprite.h"
#include "SunFont.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////
SunFont::SunFont(){
	m_interval = 5;
}
SunFont::SunFont(char* pStr,int fontSize,const char* pFontName,int interval)
{
	m_interval = interval;
//	if( !Sun3D.m_square )
//		Create(pStr,fontSize,pFontName);
//	else
		CreateList(pStr,fontSize,pFontName);
}

SunFont::~SunFont()
{
	this->Release();
}
/**
	�t�H���g�e�N�X�`���쐬
	
	�����`�ȊO�̃e�N�X�`������������̂Ƃ��āA������𒷕��`�e�N�X�`���ɓ���܂��B
	���R2�̗ݏ搧��������̂ŁA������512�𒴂���e�N�X�`���ɂȂ�Ƃ��Ȃ薳�ʂł��c
	�Ȃ��Asquare�I�v�V������ON�̂Ƃ��͂���ɏc���𖳑ʂɂ��Đ����`�e�N�X�`�����g���܂��B
*/
int SunFont::Create(char* pStr,int fontSize,const char* pFontName)
{
	this->Release();
	const char* p=pStr;				//�����|�C���^
	if( p != NULL && *p != '\0')
	{
		m_str = pStr;

		//�V�K�쐬
		SunFontTexture* pTex = new SunFontTexture;

		//�e�N�X�`��������
		if(FAILED(CreateTextureFont( pTex, pStr, fontSize, fontSize, pFontName, alpha(255))))
			return 0;

		//���X�g�ɒǉ�
		m_lstpTex.push_back(pTex);
	}
	return 1;
}
/**
	�t�H���g�e�N�X�`�����X�g�쐬
	
	�����`�e�N�X�`�����g���āA1����=1�e�N�X�`���Ƃ��ăe�N�X�`�����X�g�Ɋi�[���܂��B
	�e�N�X�`���̕����ɂ���Ăǂꂾ���d���Ȃ邩�̓t�H���g�Ɍ��炸��������Ƃ���c
*/
int SunFont::CreateList(char* pStr,int fontSize,const char* pFontName)
{
	this->Release();
	m_str = pStr;
	const char* p=pStr;				//�����|�C���^
	while( *p != '\0')
	{
		//�V�K�쐬
		SunFontTexture* pTex = new SunFontTexture;
		//�S�p�����p���ŏꍇ����
		BOOL zenkaku = ::IsDBCSLeadByte( *p );
		if(zenkaku)
		{//�S�p
			char buffer[3];
			buffer[0] = *p;
			buffer[1] = *(p+1);
			buffer[2] = '\0';
			p+=2;	//�Q�o�C�g�i�߂�
			//�����`��
			CreateTextureFont( pTex, buffer, fontSize, fontSize, pFontName, alpha(255));
		}
		else
		{//���p
			char buffer[2];
			buffer[0] = *p;
			buffer[1] = '\0';
			p+=1;	//�P�o�C�g�i�߂�
			//�����`��
			CreateTextureFont( pTex, buffer, fontSize, fontSize, pFontName, alpha(255));
		}
		//���X�g�ɒǉ�
		m_lstpTex.push_back(pTex);
	}
	return 1;
}
/**
	�J��
*/
int SunFont::Release()
{
	for(SunFontItr i=m_lstpTex.begin(); i!=m_lstpTex.end(); ++i) {
		SAFE_DELETE(*i);
	}
	m_lstpTex.resize(0);
	m_str.resize(0);
	return 1;
}
/**
	�`��
*/
int SunFont::Draw(int x,int y,D3DCOLOR color,double scale,double angle)
{
	double x2 = x, y2 = y;
	double mm = 0;
	const char* p = m_str.c_str();				//�����|�C���^
	for(SunFontItr i = m_lstpTex.begin(); i != m_lstpTex.end(); ++i)
	{
		//�Ƃ肠�����`��
		if( *i != NULL ) {
			(*i)->m_tex.Draw(x2,y2,color,scale,Deg2Rad(angle));
		}
		//�S�p�����p���ŏꍇ����
		BOOL zenkaku = ::IsDBCSLeadByte( *p );
		double mx=0,my=0;
		if(zenkaku)
		{//�S�p
			// ���̕����̕`��ʒu�ֈړ�
			mx = (*i)->m_gm.gmCellIncX;
			my = (*i)->m_gm.gmCellIncY;
			x2 += mx * Cos(angle) + my * Sin(angle);
			y2 += mx * Sin(angle) + my * Cos(angle);
			p+=2;	//�Q�o�C�g�i�߂�
		}
		else
		{//���p
			if(*p == '\n')//�G�X�P�[�v�V�[�N�G���X
			{
				mx = -mm;
				mm = 0;
				my = m_tm.tmHeight + m_tm.tmExternalLeading + m_tm.tmInternalLeading + m_interval;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			else{
				// ���̕����̕`��ʒu�ֈړ�
				mx = (*i)->m_gm.gmCellIncX;
				my = (*i)->m_gm.gmCellIncY;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			p += 1;	//�P�o�C�g�i�߂�
		}
		mm += mx;
	}
	return 1;
}
/**
	�O���f�[�V����
*/
int SunFont::DrawGradation(int x,int y,D3DCOLOR under,D3DCOLOR top,double scale,double angle)
{
	double x2 = x, y2 = y;
	double mm = 0;
	const char* p = m_str.c_str();				//�����|�C���^
	for(SunFontItr i = m_lstpTex.begin(); i != m_lstpTex.end(); ++i)
	{
		//�Ƃ肠�����`��
		if( *i != NULL ) {
			(*i)->m_tex.DrawGradation(x2,y2,under,top,scale);
		}
		//�S�p�����p���ŏꍇ����
		BOOL zenkaku = ::IsDBCSLeadByte( *p );
		double mx=0,my=0;
		if(zenkaku)
		{//�S�p
			// ���̕����̕`��ʒu�ֈړ�
			mx = (*i)->m_gm.gmCellIncX;
			my = (*i)->m_gm.gmCellIncY;
			x2 += mx * Cos(angle) + my * Sin(angle);
			y2 += mx * Sin(angle) + my * Cos(angle);
			p+=2;	//�Q�o�C�g�i�߂�
		}
		else
		{//���p
			if(*p == '\n')//�G�X�P�[�v�V�[�N�G���X
			{
				mx = -mm;
				mm = 0;
				my = m_tm.tmHeight + m_tm.tmExternalLeading + m_tm.tmInternalLeading + m_interval;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			else{
				// ���̕����̕`��ʒu�ֈړ�
				mx = (*i)->m_gm.gmCellIncX;
				my = (*i)->m_gm.gmCellIncY;
				x2 += mx * Cos(angle) + my * Sin(angle);
				y2 += mx * Sin(angle) + my * Cos(angle);
			}
			p += 1;	//�P�o�C�g�i�߂�
		}
		mm += mx;
	}
	return 1;
}
/**
	�O���f�[�V�����i��͔��A���͎w��F�j�{�e���e�L�X�g�`��
*/
int SunFont::DrawFormat(int x,int y,D3DCOLOR color,BYTE a){
	D3DCOLOR under = (color & 0x00FFFFFF) | argb(a,0,0,0);
	this->Draw(x+1,y+2,argb(a,0,0,0),1);
	this->DrawGradation(x,y,under,alpha(a),1);
	return 1;
}
/**
	�ӂ��t���`��
*/
int SunFont::DrawFlame(int x,int y,D3DCOLOR c1,D3DCOLOR c2) {
	this->Draw(x+1,y,c2);
	this->Draw(x-1,y,c2);
	this->Draw(x,y+1,c2);
	this->Draw(x,y-1,c2);
	this->Draw(x+1,y+1,c2);
	this->Draw(x-1,y+1,c2);
	this->Draw(x+1,y-1,c2);
	this->Draw(x-1,y-1,c2);
	this->Draw(x,y,c1);
	return 1;
}
/**
	�A���`�G�C���A�X���t�H���g�`��֐�
*/
HRESULT SunFont::CreateTextureFont( SunFontTexture* pTexture, char *str, int w, int h, LPCTSTR font_name ,D3DCOLOR color)
{
	HRESULT hr = S_OK;;

	// �f�o�C�X�R���e�L�X�g�̝s��
	HWND hDskWnd = GetDesktopWindow();
	HDC hDskDC = GetDC( hDskWnd );
	HDC hdc = CreateCompatibleDC( hDskDC );

	// �t�H���g�̍쐬
	HFONT hFont;
	hFont = CreateFont(
		h,							// �t�H���g�̍���
		w / 2,						// �t�H���g�̉����H
		0,							// �����O�ł悵
		0,							// �����O�ł悵
		FW_SEMIBOLD,				// �t�H���g�̑���
		FALSE,						// �Α̂��ǂ���
		FALSE,						// �����̃^�C�v
		FALSE,						// �ł�������
		SHIFTJIS_CHARSET,			// �t�H���g�̕����Z�b�g
		OUT_DEFAULT_PRECIS,			// �o�͐��x
		CLIP_DEFAULT_PRECIS,		// �N���b�s���O
		ANTIALIASED_QUALITY,		// �o�͕i��
		DEFAULT_PITCH,				// pitch and family
		font_name					// �t�H���g�̎�ށi NULL �ł��� �j
	);
	
	//�t�H���g��I��
	SelectObject( hdc, hFont );

	// ���ݑI������Ă���t�H���g�̎擾
	GetTextMetrics( hdc, &m_tm );

	//�ő�o�b�t�@�T�C�Y
	DWORD dwMaxBufferSize = 0;

	//�e�N�X�`���T�C�Y
	int width = 0;
	int height = m_tm.tmHeight;
	
	// ���݂̕����ւ̃|�C���^
	const BYTE *ptr = ( const BYTE*)str;

	//�e�N�X�`���T�C�Y�p���[�v
	while(true)
	{
		// �X�g�����O���̕������擾
		UINT nChar = _mbsnextc( ptr );
		
		//������I���iNULL�I�[�j
		if(nChar == '\0')
			break;

		// �����`��p�s��
		MAT2 mat2 = 
		{
			{ 0, 1, }, 
			{ 0, 0, },
			{ 0, 0, }, 
			{ 0, 1, }
		};

		// �o�b�t�@�T�C�Y�̎擾
		DWORD dwBufferSize = GetGlyphOutline( hdc, nChar, GGO_GRAY8_BITMAP, &pTexture->m_gm, 0, NULL, &mat2 );
		
		//�o�b�t�@�T�C�Y���X�V
		if(dwBufferSize > dwMaxBufferSize)
			dwMaxBufferSize = dwBufferSize;

		//�����X�V
		width += pTexture->m_gm.gmCellIncX;

		// ���̕����ֈړ�
		ptr = _mbsinc( ptr );
	}

	// �o�b�t�@�̍쐬
	LPBYTE hBuffer = (LPBYTE)GlobalAlloc(GPTR, dwMaxBufferSize);

	if(hBuffer == NULL)
		return E_FAIL;

	width = ceil2( width );
	height = ceil2( height );
	int size = ceil2(max(width,height));
	//�����`�I�v�V�����ŏꍇ�킯
	if( !Sun3D.m_square )
		pTexture->m_tex.Create( width, height, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);
	else
		pTexture->m_tex.Create( size, size, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED);

	//�e�N�X�`�������b�N
	D3DLOCKED_RECT d3dRect;
	hr = pTexture->m_tex.m_pd3dtex->LockRect(0,&d3dRect,NULL,0);

	if(FAILED(hr))
		return hr;

	//���b�N���ꂽ�̈�ւ̃|�C���^
	BYTE* pBuffer = (BYTE*)d3dRect.pBits;
	DWORD* pdwBuffer = (DWORD*)d3dRect.pBits;

	//�����`��ʒu
	int mojiX = 0;
	int mojiY = 0;

	// ���݂̕����ւ̃|�C���^
	ptr = ( const BYTE*)str;

	//�������ƂɃ��[�v
	while(true)
	{
		// �X�g�����O���̕������擾
		UINT nChar = _mbsnextc( ptr );
		
		//������I���iNULL�I�[�j
		if(nChar == '\0')
			break;

		// �����`��p�s��
		MAT2 mat2 = 
		{
			{ 0, 1, }, 
			{ 0, 0, },
			{ 0, 0, }, 
			{ 0, 1, }
		};

		// �o�b�t�@�T�C�Y�̎擾
		DWORD dwBufferSize = GetGlyphOutline( hdc, nChar, GGO_GRAY8_BITMAP, &pTexture->m_gm, 0, NULL, &mat2 );

		//�󔒕����łȂ��ꍇ
		if(iswspace(nChar) == 0)
		{
			if(dwBufferSize == 0)
				return E_FAIL;

			// ���ۂɃo�b�t�@�Ɏ擾
			// �A�E�g���C�����O���[�X�P�[���ō쐬
			DWORD dwResult = GetGlyphOutline( hdc, nChar, GGO_GRAY8_BITMAP, &pTexture->m_gm, dwBufferSize, ( LPVOID)hBuffer, &mat2 );

			if(dwResult == GDI_ERROR)
				return E_FAIL;

			// �����̍����ƕ��̎擾
			int dw = pTexture->m_gm.gmBlackBoxX;
			int dh = pTexture->m_gm.gmBlackBoxY;

			// �s�b�`�����Ƃ߂܂��i�R�Q�r�b�g��؂�j
			int pitch = ( pTexture->m_gm.gmBlackBoxX + 3 ) & 0xfffc;

			// ���������̊J�n�ʒu�̎擾
			int offsetX = mojiX + pTexture->m_gm.gmptGlyphOrigin.x;
			int offsetY = mojiY + m_tm.tmAscent - pTexture->m_gm.gmptGlyphOrigin.y;

			// �����̕��ƍ����̐��������[�v
			for( int i = 0; i < dh; i++ )
			{
				for( int ii = 0; ii < dw; ii++ )
				{
					BYTE alpha, *p_alpha;		// �A���t�@�l

					// �F�̎擾
					p_alpha = ( (( LPBYTE)hBuffer + ( i * pitch ) + ii ));
					alpha = *( p_alpha );

					// ����ꂽ�r�b�g�}�b�v�́i�Ȃ����j0-64�̂U�T�i�K�Ȃ̂�0-255�ɕϊ�
					if(alpha != 0)
					{
						alpha = (alpha-1) * 4;
					}

					//�F�̑��
					BYTE a = color>>24 & 0xff;
					BYTE r = color>>16 & 0xff;
					BYTE g = color>>8 & 0xff;
					BYTE b = color & 0xff;

					pdwBuffer[ (i+offsetY) * d3dRect.Pitch/4 + (ii+offsetX) ] = D3DCOLOR_ARGB(alpha * a / 255, r, g, b);
				}
			}
		}

		// ���̕����̕`��ʒu�ֈړ�
		mojiX += pTexture->m_gm.gmCellIncX;
		mojiY += pTexture->m_gm.gmCellIncY;

		// ���̕����ֈړ�
		ptr = _mbsinc( ptr );
	}
	// �P�������̃o�b�t�@�̉��
	GlobalFree( hBuffer );

	// �t�H���g�̉��
	DeleteObject( hFont );

	//�e�N�X�`���̃A�����b�N
	pTexture->m_tex.m_pd3dtex->UnlockRect(0);

	//�f�o�C�X�R���e�L�X�g�̊J��
	ReleaseDC( hDskWnd, hDskDC );
	DeleteDC( hdc );

	return S_OK;
}
