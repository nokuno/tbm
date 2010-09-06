// SunSurface.cpp: SunSurface �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunTask.h"
#include "SunExeption.h"
#include "SunFile.h"
#include "SunApp.h"
#include "Sun3D.h"
#include "SunSurface.h"

SunFpsSurface SunFpsShow;
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SunSurface::SunSurface()
{
	m_pd3ds = NULL;
	::ZeroMemory(&m_d3dii,sizeof D3DXIMAGE_INFO);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
}

SunSurface::SunSurface(char* pFile)
{
	m_pd3ds = NULL;
	::ZeroMemory(&m_d3dii,sizeof D3DXIMAGE_INFO);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
	this->LoadFromPack(pFile);
}

SunSurface::~SunSurface()
{
	this->Release();
}

int SunSurface::Release()
{
	SAFE_RELEASE(m_pd3ds);
	::ZeroMemory(&m_d3dii,sizeof D3DXIMAGE_INFO);
	::ZeroMemory(&m_d3dsd,sizeof D3DSURFACE_DESC);
	return 1;
}
//�o�b�N�o�b�t�@�𓾂�
int SunSurface::GetBackBuffer()
{
	this->Release();
	if(FAILED(Sun3D.m_pd3ddev->GetBackBuffer(
		0,
		0,
		D3DBACKBUFFER_TYPE_MONO,
		&m_pd3ds)))
	{
		throw SunExeption("�o�b�N�o�b�t�@�T�[�t�F�C�X���擾�ł��܂���ł����B\n");
	}
	m_pd3ds->GetDesc(&m_d3dsd);		//�T�[�t�F�[�X���
	return 1;
}
//�Z�[�u
int SunSurface::SaveSurface(LPCTSTR pDestFile,D3DXIMAGE_FILEFORMAT DestFormat,RECT *pSrcRect)
{
	if(FAILED(D3DXSaveSurfaceToFile(pDestFile,DestFormat,m_pd3ds,NULL,pSrcRect))){
		return 0;
	}
	return 1;
}
//D3DSURFACE8����SunSurface�ւ̓]��
int SunSurface::Blit( LPDIRECT3DSURFACE9 pd3ds,int x,int y,RECT &rcSrc)
{
	if(m_pd3ds == NULL || pd3ds == NULL)
		return 0;
	RECT rcDest={x,y,x+rcSrc.right-rcSrc.left,y+rcSrc.bottom-rcSrc.top};
	if(FAILED(Sun3D.m_pd3ddev->StretchRect(
		pd3ds,			//�]����
		&rcSrc,
		m_pd3ds,		//�]����͎������g
		&rcDest,
		D3DTEXF_NONE)))
	{
		throw SunExeption("�]�����s\n");
	}
	return 1;
}
//SunSurface���m�̓]��
int SunSurface::Blit(SunSurface *ps,int x,int y,RECT &rcSrc)
{
	if(m_pd3ds == NULL || ps == NULL)
		return 0;
	return Blit(ps->GetSurface(),x,y,rcSrc);
}
//�\�[�X�̈��S�̂ɌŒ肵�ē]��
int SunSurface::Blit(SunSurface *ps,int x,int y)
{
	RECT rcSrc = {0,0,m_d3dsd.Width,m_d3dsd.Height};
	return Blit(ps->GetSurface(),x,y,rcSrc);
}
//�h��Ԃ�
int SunSurface::Fill( D3DCOLOR color, RECT &rect)
{
	if(FAILED(Sun3D.m_pd3ddev->ColorFill( m_pd3ds, &rect, color)))
	{
		throw SunExeption("�h��Ԃ��`�掸�s\n");
	}
	return 1;
}
//�h��Ԃ�
int SunSurface::Fill( D3DCOLOR color)
{
	if(FAILED(Sun3D.m_pd3ddev->ColorFill( m_pd3ds, NULL, color)))
	{
		throw SunExeption("�h��Ԃ��`�掸�s\n");
	}
	return 1;
}
//�C���[�W���T�[�t�F�C�X�փ��[�h�i���̂Ƃ����ƍ��������߂�j
int SunSurface::Load(char *lpszFileName)
{
	this->Release();

	//�C���[�W���̎擾
	if(FAILED(::D3DXGetImageInfoFromFile(lpszFileName,&this->m_d3dii)))
	{     
		throw SunExeption("%s : �t�@�C�������݂��܂���B\n",lpszFileName);
	}
	//��̃T�[�t�F�C�X�쐬
	if(!this->Create(m_d3dii.Width,m_d3dii.Height))
	{
		SAFE_RELEASE(m_pd3ds);
		throw SunExeption("%s : �T�[�t�F�C�X���������m�ۂł��܂���ł����B\n",lpszFileName);
	}
	//�T�[�t�F�C�X�ɂa�l�o��ǂݍ���
	if(FAILED(D3DXLoadSurfaceFromFile(
		m_pd3ds,
		NULL,
		NULL,
		lpszFileName,
		NULL,
		D3DX_DEFAULT,
		0xFF000000,
		NULL)))
	{
		//�t�@�C���ɂȂ���΃��\�[�X����
		if(FAILED(D3DXLoadSurfaceFromResource(
			m_pd3ds,
			NULL,
			NULL,
			NULL,
			lpszFileName,
			NULL,
			D3DX_DEFAULT,
			0xFF000000,
			NULL)))
		{
			SAFE_RELEASE(m_pd3ds);
			throw SunExeption("%s : �T�[�t�F�C�X��ǂݍ��߂܂���ł����B\n",lpszFileName);
		}
	}

	m_pd3ds->GetDesc(&m_d3dsd);		//�T�[�t�F�[�X���
	//���O�o��
	SunLog.LogOut("%s ( %d �~ %d ):�T�[�t�F�C�X��ǂݍ��݂܂����B\n",lpszFileName,m_d3dii.Width,m_d3dii.Height);
	return 1;
}
/**
	�p�b�N�t�@�C�����烍�[�h
	�Ƃ����Ă����t�@�C�������݂���΂�������g���܂�
	�i�r�����ei�����̎d�l�j
*/
int SunSurface::LoadFromPack(char *lpszFileName)
{
	SunFileEx file;
	if(file.Open(lpszFileName) == 0)
	{
		throw SunExeption("%s : �t�@�C�������݂��܂���B\n",lpszFileName);
	}
	file.Read();
	//�C���[�W���̎擾
	if(FAILED(::D3DXGetImageInfoFromFileInMemory(file.GetBuffer(),file.GetSize(),&this->m_d3dii)))
	{     
		throw SunExeption("%s : �t�@�C�������݂��܂���B\n",lpszFileName);
	}
	//��̃T�[�t�F�C�X�쐬
	if(!this->Create(m_d3dii.Width,m_d3dii.Height))
	{
		SAFE_RELEASE(m_pd3ds);
		throw SunExeption("%s : �T�[�t�F�C�X���������m�ۂł��܂���ł����B\n",lpszFileName);
	}
	//�T�[�t�F�C�X�ɂa�l�o��ǂݍ���
	if(FAILED(D3DXLoadSurfaceFromFileInMemory(
		m_pd3ds,
		NULL,
		NULL,
		file.GetBuffer(),
		file.GetSize(),
		NULL,
		D3DX_DEFAULT,
		0xFF000000,
		NULL)))
	{
		SAFE_RELEASE(m_pd3ds);
		throw SunExeption("%s : �T�[�t�F�C�X��ǂݍ��߂܂���ł����B\n",lpszFileName);
	}
	return 1;
}
//�쐬
int	SunSurface::Create(int width,int height,D3DFORMAT format,D3DPOOL pool)
{
	this->Release();
	if(FAILED( Sun3D.m_pd3ddev->CreateOffscreenPlainSurface(
		width,
		height,
		format,
		pool,
		&m_pd3ds,
		NULL)))
	{
		throw SunExeption("�I�t�X�N���[���T�[�t�F�[�X���m�ۂł��܂���B\n");
	}
	m_pd3ds->GetDesc(&m_d3dsd);		//�T�[�t�F�[�X���
	m_d3dii.Width = width;
	m_d3dii.Height = height;
	m_d3dii.Format = format;
	return 1;
}

//�쐬
int	SunSurface::Create(int width,int height,D3DFORMAT format)
{
	return this->Create(width,height,format,D3DPOOL_DEFAULT);
}
//��̃T�[�t�F�C�X���쐬
int SunSurface::Create(int width,int height)
{
	//�ŗD��̓o�b�N�o�b�t�@�Ɠ����t�H�[�}�b�g�i���߂͂ł��Ȃ��d�l�j
	if(!this->Create(width,height,Sun3D.m_d3dpp.BackBufferFormat))
	{
		if(Sun3D.m_bitmode == 16)
		{//16bit
			if(!this->Create(width,height,D3DFMT_R5G6B5))
				if(!this->Create(width,height,D3DFMT_X1R5G5B5))
					if(!this->Create(width,height,D3DFMT_X4R4G4B4))
						return 0;
		}
		else if(Sun3D.m_bitmode == 32)
		{//32bit
		if(!this->Create(width,height,Sun3D.m_d3dpp.BackBufferFormat))
			if(!this->Create(width,height,D3DFMT_X8R8G8B8))
				return 0;
		}
		else
		{//else
			return 0;
		}
	}
	return 1;
}
//�`��i������GetBackBuffer����j
int SunSurface::Draw(int x,int y,RECT &rcSrc)
{
	SunSurface back;
	back.GetBackBuffer();
	return back.Blit(this,x,y,rcSrc);
}
//�`��i������GetBackBuffer����j
int SunSurface::Draw(int x,int y)
{
	SunSurface back;
	back.GetBackBuffer();
	RECT rcSrc = {0,0,m_d3dsd.Width,m_d3dsd.Height};
	return back.Blit(this,x,y,rcSrc);
}
//�e�N�X�`������쐬
int SunSurface::GetTextureSurface(LPDIRECT3DTEXTURE9 pd3dtex,int width,int height)
{
	if(pd3dtex == NULL)
		return 0;
	this->Release();
	if(FAILED(pd3dtex->GetSurfaceLevel(0,&m_pd3ds)))
	{
		throw SunExeption("�e�N�X�`���T�[�t�F�[�X�̎擾�Ɏ��s\n");
	}
	m_d3dii.Width = width;
	m_d3dii.Height = height;
	m_pd3ds->GetDesc(&m_d3dsd);		//�T�[�t�F�[�X���
	return 1;
}
/**
	@brief �̈�w�胍�[�h
	���̊֐��̓e�N�X�`������g���̂ŃT�[�t�F�[�XCreate�͂��Ȃ�
	���R���[�h�����Ȃ��B
*/
int SunSurface::LoadRect(char* lpszFileName,RECT &rect,D3DCOLOR color)
{
	if(m_pd3ds == NULL)
		return 0;
	int width = rect.right-rect.left;
	int height = rect.bottom - rect.top;
	if(width > m_d3dsd.Width || height > m_d3dsd.Height)
		return 0;
	if(FAILED(D3DXLoadSurfaceFromFile(
		m_pd3ds,
		NULL,
		NULL,			//�Ȃ������s����̂�Dest��NULL
		lpszFileName,
		&rect,
		D3DX_FILTER_NONE,
		color,
		NULL)))
		return 0;
	return 1;
}
/**
	@brief �̈�w�胍�[�hFrom�������[
	���̊֐��̓e�N�X�`������g���̂ŃT�[�t�F�[�XCreate�͂��Ȃ�
	���R���[�h�����Ȃ��B
*/
int SunSurface::LoadRectFromMemory(void* pBuffer,DWORD dwSize,RECT &rect,D3DCOLOR color)
{
	if(m_pd3ds == NULL)
		return 0;
	int width = rect.right-rect.left;
	int height = rect.bottom - rect.top;
	if(width > m_d3dsd.Width || height > m_d3dsd.Height)
		return 0;
	if(FAILED(D3DXLoadSurfaceFromFileInMemory(
		m_pd3ds,
		NULL,
		NULL,			//�Ȃ������s����̂�Dest��NULL
		pBuffer,
		dwSize,
		&rect,
		D3DX_FILTER_NONE,
		color,
		NULL)))
		return 0;
	return 1;
}
/**
	@breif SunSurfaceSprite�N���X�̃R���X�g���N�^
*/
SunSurfaceSprite::SunSurfaceSprite(SunSurface* pSurface,int x,int y)
{
	m_pSurface=pSurface;
	m_x=x;m_y=y;
	SetRect(&m_rect,0,0,pSurface->m_d3dsd.Width,pSurface->m_d3dsd.Height);
}
SunSurfaceSprite::SunSurfaceSprite(SunSurface* pSurface,int x,int y,RECT rect)
{
	m_pSurface=pSurface;
	m_x=x;m_y=y;
	m_rect = rect;
}
/**
	@brief �T�[�t�F�[�X�X�v���C�g�̕`��
*/
int SunSurfaceSprite::operator ()()
{
	return m_pSurface->Draw(m_x,m_y);
}
//
int SunFpsSurface::operator ()()
{
	if(SunApp.m_fpsshow > 0)
	{
		m_surf.Fill(xrgb(0,0,0));
		m_surf.Text((char*)SunApp.GetFPSString().c_str(),0,0,15,RGB(250,250,250));
		SunApp.m_fpsshow --;
	}
	m_surf.Draw(295,455);
//	m_surf.Draw(640-100,480-30,100);
	return 1;
}
//�����`��iGetDC���p��
int SunSurface::Text(const char* text,int x,int y,int size,COLORREF color,const char* pFontName)
{
	if(m_pd3ds == NULL)
		return 0;
	HFONT hFont = CreateFont(
		size,0,0,0,800,
		0,
		0,
		0,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
		pFontName);
	HDC hdc;
	if(FAILED(m_pd3ds->GetDC(&hdc)))
		throw SunExeption("�����`�掸�s�FTEXT=%s\n",text);
	SelectObject(hdc, hFont);
	SetTextColor(hdc,color);
	SetBkMode(hdc, TRANSPARENT);
	if(::TextOut(hdc,x,y,text,strlen(text))==NULL)
		throw SunExeption("�����`�掸�s�FTEXT=%s\n",text);
	m_pd3ds->ReleaseDC(hdc);
	if(FAILED(DeleteObject(hFont)))
		throw SunExeption("�����`�掸�s�FTEXT=%s\n",text);
	return 1;
}
//�X�i�b�v�V���b�g�ۑ��֐�
int SaveSnapShot(D3DXIMAGE_FILEFORMAT DestFormat)
{
	::ResetCurrent();
	::CreateDirectory("snapshot");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	char filename[MAX_PATH];
	char kakutyosi[5];
	switch( DestFormat ) {
	case D3DXIFF_BMP:
		strcpy( kakutyosi, ".bmp" );
		break;
	case D3DXIFF_JPG:
		strcpy( kakutyosi, ".jpg" );
		break;
	case D3DXIFF_PNG:
		strcpy( kakutyosi, ".png" );
		break;
	case D3DXIFF_TGA:
		strcpy( kakutyosi, ".tga" );
		break;
	default:
		strcpy( kakutyosi, ".xxx" );
		break;
	}
	for( int i = 0; i < 1000; ++i ) {
		sprintf( filename, "snapshot/tbm%03d%s", i, kakutyosi );
		hFind = FindFirstFile(filename, &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			SunSurface surf;
			surf.GetBackBuffer();
			surf.SaveSurface(filename,DestFormat,NULL);
			break;
		}
	}
	return 1;
}
//�X�i�b�v�V���b�g�ۑ��֐��i�R���t�B�O����t�@�C���`�����g���j
int SaveSnapShot()
{
	return SaveSnapShot( Sun3D.m_fileformat );
}