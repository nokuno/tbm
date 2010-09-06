/**
	@file Sun3D.cpp
	@brief CSun3D �N���X�̃C���v�������e�[�V����
*/
#include "stdafx.h"
#include "SunUtility.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunExeption.h"
#include "SunApp.h"
#include "Sun3D.h"

CSun3D Sun3D;
/**
	@brief �o�b�N�o�b�t�@�t�H�[�}�b�g
*/
static const D3DFORMAT BackBufferFormats[] = {
	//16bit
	D3DFMT_R5G6B5,			D3DFMT_A1R5G5B5,		D3DFMT_X1R5G5B5,
	D3DFMT_X4R4G4B4,		D3DFMT_A4R4G4B4,		D3DFMT_A8R3G3B2,
	//24bit
	D3DFMT_R8G8B8,
	//32bit
	D3DFMT_A8R8G8B8,		D3DFMT_X8R8G8B8,		D3DFMT_A2R10G10B10,
	D3DFMT_A8B8G8R8,		D3DFMT_X8B8G8R8,		D3DFMT_A2B10G10R10,
	//64bit
	D3DFMT_A16B16G16R16,	D3DFMT_A16B16G16R16F,
	//UNKNOWN
	D3DFMT_UNKNOWN,
};

/**
	 @brief�e�N�X�`���t�H�[�}�b�g
*/
static D3DFORMAT TextureFormats[] = {
	//16bit
	D3DFMT_R5G6B5,			D3DFMT_A1R5G5B5,		D3DFMT_X1R5G5B5,
	D3DFMT_X4R4G4B4,		D3DFMT_A4R4G4B4,		D3DFMT_A8R3G3B2,
	//24bit
	D3DFMT_R8G8B8,
	//32bit
	D3DFMT_A8R8G8B8,		D3DFMT_X8R8G8B8,		D3DFMT_A2R10G10B10,
	D3DFMT_A8B8G8R8,		D3DFMT_X8B8G8R8,		D3DFMT_A2B10G10R10,
	//64bit
	D3DFMT_A16B16G16R16,	D3DFMT_A16B16G16R16F,
	//UNKNOWN
	D3DFMT_UNKNOWN
};
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CSun3D::CSun3D()
{
	this->m_hDll	= NULL;
	this->m_pd3d	= NULL;
	this->m_pd3ddev	= NULL;
	this->m_bitmode	= 32;
	this->m_vsync	= 1;
	this->m_texbit	= 0;
	this->m_square	= 0;
	this->m_skip	= 2;
	this->m_interval= 3;
	this->m_minimum	= 30;
	this->m_scount	= 0;
	this->m_count	= 0;
	this->m_format	= D3DFMT_UNKNOWN;
	this->m_obfps = 60;
	::ZeroMemory(&this->m_d3dpp,sizeof D3DPRESENT_PARAMETERS);
	::ZeroMemory(&this->m_d3dcaps,sizeof D3DCAPS9);
	memset( this->m_renderState, 0xFF, sizeof(DWORD[0xFF]) );
	memset( this->m_textureState, 0xFF, sizeof(DWORD[0xFF]) );
	m_fvfState = ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1 );	//�Q�c�p���_�t�H�[�}�b�g
	m_fileformat = D3DXIFF_JPG;
}

CSun3D::~CSun3D()
{
	this->Exit();
}
/**
	@brief �R�c�S������
	
	 ������s���O�ɂr����������������Ă�K�v������
*/
int CSun3D::Init()
{
	//�R���t�B�O���[�h
	if(!this->LoadConfig())
		return 0;
	//�c�����������R�c������
	if(!this->CreateDirect3D())
		return 0;
	//�f�B�X�v���C���O�o��
	if(!this->CheckAdapterInformation())
		return 0;
	//Caps������
	if(!this->InitCaps())
		return 0;
	//Caps���O�o��
	if(!this->DeviceLog())
		return 0;
	//�t�H�[�}�g�̌���
	if(!Sun3D.DecideDisplay())
		return 0;
	//���t���b�V�����[�g�̌���
	if(!Sun3D.DecideRefreshRate())
		return 0;
	//�e�N�X�`���t�H�[�}�b�g�̌���
	if(!Sun3D.DecideTextureFormat())
		return 0;
	//���̑��̃p�����[�^�̌���
	if(!Sun3D.DecideOtherParameters())
		return 0;
	//���肳�ꂽ�p�����[�^�̃��O�o��
	if(!Sun3D.ParameterLog())
		return 0;
	//�f�o�C�X�쐬
	if(!this->CreateDevice())
		return 0;
	//�e��X�e�[�g������
	if(!this->InitState())
		return 0;
	//SunApp�ɑ���
	SunApp.SetVsync( this->m_vsync );
	this->m_time = ::timeGetTime();
	return 1;
}
/**
	@brief �������
*/
int CSun3D::Exit()
{
	try
	{
		SAFE_RELEASE(m_pd3ddev);
		SAFE_RELEASE(m_pd3d);
		::FreeLibrary(m_hDll);
		m_hDll = NULL;
	}catch(...){MsgBox("Direct3D�̉���ŕs���ȗ�O���܂����B");}
	return 1;
}
/**
	@brief �ď�����
*/
int CSun3D::ReInit()
{
	if(!this->Exit())
		return 0;
	if(!this->Init())
		return 0;
	return 1;
}
/**
	@brief �e��X�e�[�g������
*/
int CSun3D::InitState()
{
	D3DXMATRIX	proj;
	D3DXMATRIX	view;
	D3DXMATRIX	world;

	// �v���W�F�N�V�����s��
	D3DXMatrixPerspectiveFovLH( &proj,
		D3DX_PI / 4,	//y�����ւ̎���
		1.0f,			//�A�X�y�N�g��
		10.0f,			//�߃r���[����
		5000.0f );		//���r���[����
	
	// �r���[�s���ݒ�
	D3DXVECTOR3 vEye( 0, 0, -50 );	//���_
	D3DXVECTOR3 vAt( 0, 0, 0 );		//�����_
	D3DXVECTOR3 vUp( 0, 1, 0 );		//�����
	D3DXMatrixLookAtLH( &view, &vEye, &vAt, &vUp );

	// ���[���h�}�g���b�N�X�쐬
	D3DXMatrixRotationYawPitchRoll( &world, 0, 0, 0 );

	// �v���W�F�N�V�����s��ݒ�
	m_pd3ddev->SetTransform( D3DTS_PROJECTION, &proj );

	// �r���[�s���ݒ�
	m_pd3ddev->SetTransform( D3DTS_VIEW, &view );

	// ���[���h�}�g���b�N�X��ݒ�
	m_pd3ddev->SetTransform( D3DTS_WORLD, &world );

	// �F�̎Z�o���@�̐ݒ�
	this->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );		// �]�����̐F�̎Z�o���@
	this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );	// �]����̐F�̎Z�o���@

	//�@�����_�����O�X�e�[�g
	this->SetRenderState( D3DRS_LIGHTING, false );			//���C�e�B���O
	this->SetRenderState( D3DRS_ZENABLE, false );
	this->SetRenderState( D3DRS_ZWRITEENABLE, false );
	this->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	this->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	this->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	//�����`��
	this->SetRenderState( D3DRS_CLIPPING , true );			//�N���b�s���O
	this->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );

	//�A���`�F��
//	if( m_d3dcaps.LineCaps & D3DLINECAPS_ANTIALIAS )
//		this->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );

	m_pd3ddev->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_LINEAR);
	m_pd3ddev->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pd3ddev->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);


	// �e�N�X�`���̃u�����f�B���O���@�̐ݒ�
	this->SetTextureState(  D3DTSS_ALPHAOP, D3DTOP_MODULATE );	// �ȉ��̈����̐�������Z����
	this->SetTextureState(  D3DTSS_COLORARG1, D3DTA_TEXTURE );	// �e�N�X�`���̐F
	this->SetTextureState(  D3DTSS_COLORARG2, D3DTA_DIFFUSE );	// �f�B�t�[�Y�̐F

	//���_�t�H�[�}�b�g��DirectX�֒ʒm
	m_pd3ddev->SetFVF(( D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1 ));

	//�f�o�C�X�\�͂ɂ���Đ����`�e�N�X�`���������g�p
	if( m_d3dcaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY )
		m_square = 1;
	return 1;
}
/**
	@brief �R���t�B�O���[�h
*/
int CSun3D::LoadConfig()
{
	//��ʂ̃r�b�g���[�h
	switch(SunConfig.GetInt2("direct3d","bitmode",32))
	{
	case 16:
		m_bitmode = 16;
		break;
	case 24:
		m_bitmode = 24;
		break;
	case 32:
		m_bitmode = 32;
		break;
	case 64:
		m_bitmode = 64;
		break;
	default:
		SunApp.MsgBox("�r�b�g���[�h�̐ݒ肪�����Ă��܂��B\n��x�R���t�B�O�t�@�C�����폜���Ă��������B\n");
		break;
	}
	//�e�N�X�`���̃r�b�g���[�h
	switch(SunConfig.GetInt2("direct3d","texbit",0))
	{
	case 0:	//��ʂ̃r�b�g���[�h�Ɠ���
		m_texbit = 0;
		break;
	case 16:
		m_texbit = 16;
		break;
	case 24:
		m_texbit = 24;
		break;
	case 32:
		m_texbit = 32;
		break;
	case 64:
		m_texbit = 64;
		break;
	default:
		SunApp.MsgBox("�r�b�g���[�h�̐ݒ肪�����Ă��܂��B\n��x�R���t�B�O�t�@�C�����폜���Ă��������B\n");
		break;
	}

	//���������҂��̐ݒ�
	if( m_vsync != -1 ) {
		m_vsync	= SunConfig.GetInt2("direct3d","vsync",1);
	}
	else {
		m_vsync = 0;
	}

	//�����`�e�N�X�`���̋����g�p
	m_square = SunConfig.GetInt2("direct3d","square",0);

	//�X�i�b�v�V���b�g�̃t�@�C���`��
	string s;
	s = SunConfig.GetString2("direct3d","snapshot","jpg");
	if( s == "bmp" ) {
		m_fileformat = D3DXIFF_BMP;
	}
	else if( s == "jpg" ) {
		m_fileformat = D3DXIFF_JPG;
	}
	else if( s == "png" ) {
		m_fileformat = D3DXIFF_PNG;
	}
	else if( s == "tga" ) {
		m_fileformat = D3DXIFF_TGA;
	}
	else {
		m_fileformat = (D3DXIMAGE_FILEFORMAT)SunConfig.GetInt("direct3d","snapshot");
	}

	//�`��X�L�b�v
	m_skip = SunConfig.GetInt2("config.ini","direct3d","skip",2);
	m_interval = SunConfig.GetInt2("config.ini","direct3d","interval",1);
	m_minimum = SunConfig.GetInt2("config.ini","direct3d","minimum",30);
	m_scount = 0;
	return 1;
}
/**
	@brief GetUpdate
*/
int CSun3D::GetUpdate()
{
	return m_skip == 0 ||
		m_skip == 1 && ( m_obfps >= m_minimum || m_scount >= m_interval-1 ) ||
		m_skip == 2 && m_count % m_interval == 0;
}
/**
	@brief Present
*/
int CSun3D::Present()
{
	int time = ::timeGetTime();
	//���t���[���X�V
	this->m_obfps = 1000.0 / (time-this->m_time);
	this->m_time = time;
	this->m_count++;

	//�`��X�L�b�v�Ŕ�΂���镔��
	if( this->GetUpdate() ) {
		this->m_scount = 0;
		if(m_pd3ddev == NULL)
			return 0;
		HRESULT result;
		while( FAILED( result = m_pd3ddev->Present(NULL,NULL,NULL,NULL))) {
			SunLog.LogOut("Present���s : %d\n",result);
			result = m_pd3ddev->Reset(&m_d3dpp);
			if(FAILED(result)) {
				SunLog.LogOut("Reset���s : %d\n",result);
				return 0;
			}
		}
	}
	else {
		this->m_scount++;
	}
	return 1;
}
/**
	@brief �N���A�[
*/
int CSun3D::Clear(D3DCOLOR color)
{
	if(FAILED(m_pd3ddev->Clear(0,NULL,D3DCLEAR_TARGET ,color, 1.0f, 0)))
		return 0;
	return 1;
}
/**
	@brief BeginScene()
*/
int CSun3D::BeginScene()
{
	if(FAILED(m_pd3ddev->BeginScene()))
		return 0;
	return 1;
}
/**
	@brief EndScene()
*/
int CSun3D::EndScene()
{
	if(FAILED(m_pd3ddev->EndScene()))
		return 0;
	return 1;
}
/**
	@brief �e�N�X�`���T�C�Y�́H
*/
int CSun3D::GetTextureSize()
{
	return floor2(min(m_d3dcaps.MaxTextureWidth,m_d3dcaps.MaxTextureHeight));
}
/**
	@brief DLL���[�h�AD3D�I�u�W�F�N�g�擾
*/
int CSun3D::CreateDirect3D()
{
	PDIRECT3DCREATE9 pProcDirect3DCreate9 = NULL;

	// Load DLL
	this->m_hDll = ::LoadLibrary( "d3d9.dll" );
	if ( this->m_hDll == NULL )
	{
		throw SunExeption("DLL�̓ǂݍ��݂Ɏ��s\nDirectX9���������C���X�g�[������Ă��邩\n�m�F���ĉ�����\n");
	}

	// Load Direct3DCreate from DLL
	pProcDirect3DCreate9 = (PDIRECT3DCREATE9)::GetProcAddress( this->m_hDll, "Direct3DCreate9" );
	if ( pProcDirect3DCreate9 == NULL )
	{
		SunApp.MsgBox("DLL����̊֐��Ăяo���Ɏ��s\nDirectX9���������C���X�g�[������Ă��邩\n�m�F���ĉ�����" );
		return 0;
	}

	// Create Direct3D Object
	this->m_pd3d = pProcDirect3DCreate9( D3D_SDK_VERSION );
	if ( this->m_pd3d == NULL )
	{
		SunApp.MsgBox("Direct3D�I�u�W�F�N�g�̐����Ɏ��s\nDirectX9���������C���X�g�[������Ă��邩\n�m�F���ĉ�����" );
		return 0;
	}
	return 1;
}

/**
	@brief �f�o�C�X�\�͎擾
*/
int CSun3D::InitCaps()
{

	//�f�o�C�X�̔\�͂̒���
	//�f�t�H���g�̃f�o�C�X�̂ݒ������܂��B�i�r�f�I�J�[�h���Q�ȏ゠��ꍇ�͍l�����Ȃ��j

	//---�A�_�v�^�̃n�[�h�E�F�A�����擾���܂�
	if( m_pd3d->GetDeviceCaps(0,D3DDEVTYPE_HAL,&m_d3dcaps) != D3D_OK )
	{//�g�`�k�f�o�C�X�Ƃ��Ẵn�[�h�E�F�A���܂��������݂��Ȃ�
		SunLog.LogOut("HAL�f�o�C�X�𐶐��ł��܂���\n");
		if( m_pd3d->GetDeviceCaps(0,D3DDEVTYPE_REF,&m_d3dcaps) != D3D_OK )
		{//�q�d�e�f�o�C�X�Ƃ��Ẵn�[�h�E�F�A���܂��������݂��Ȃ�
			//���̏��������s����鎞�A�A�_�v�^�̃n�[�h�E�F�A�͂܂����������ăQ�[������ɓK���Ȃ��ƔF���ł���
			//�������A�v���O�����I��������̌����������Ȃ���O�ł���ׂɏ������p������
			//����ɂ���āA�A�_�v�^�̃n�[�h�E�F�A���͎擾�ł��Ă��Ȃ�
			SunApp.MsgBox("�f�o�C�X��Direct3D�ɑΉ����Ă��܂���B\n");
			return 0;
		}
	}
	return 1;
}
/**
	@brief �f�o�C�X�쐬
*/
int CSun3D::CreateDevice()
{
	//�n�[�h�E�F�A���_�����i���_�o�b�t�@�j�͂��Ȃ����j�Łi�QD������BTWINKBELL�Q�Ɓj
	//�g�`�k�f�o�C�X���������݂�
	if( FAILED(m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,SunApp.m_hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3ddev)))
	{
		SunLog.LogOut("HAL�f�o�C�X�𐶐��ł��܂���ł����AREF�œ��삵�܂�\n");
		//�q�d�e�f�o�C�X���������݂�
		if( FAILED(m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_REF,SunApp.m_hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,&m_d3dpp,&m_pd3ddev)))
		{
			//--�ǂ̃f�o�C�X���擾�ł��Ȃ�����
			SunApp.MsgBox("DirectX�f�o�C�X���擾�ł��܂���ł���\n");
			SAFE_RELEASE(m_pd3d);						//DirectX3D�I�u�W�F�N�g�����

			return FALSE;
		}

	}

	if( m_pd3ddev == NULL )
	{//�I�u�W�F�N�g�������s
		SunApp.MsgBox("DirectXD3D9�f�o�C�X�������s\n");
		return 0;
	}

	SunLog.LogOut("�g�p�ł���r�f�I�������F[%d] MB\n", m_pd3ddev->GetAvailableTextureMem()/(1024*1024) );

	return 1;
}
/**
	@brief �f�B�X�v���C���[�h�����肷��
*/
int CSun3D::DecideDisplay()
{
	D3DDISPLAYMODE displayMode;
	m_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&displayMode);		//���݂̃f�B�X�v���C���[�h�f�[�^�擾
	if(SunApp.m_window)
	{//�E�B���h�E���[�h�ł̓t�H�[�}�b�g���Œ肳���
		//�o�b�N�o�b�t�@�t�H�[�}�b�g���f�B�X�v���C�Ɠ�����
		m_d3dpp.BackBufferFormat = displayMode.Format;
		//�t�H�[�}�b�g�����ăr�b�g���[�h����E�E����
		if(displayMode.Format == D3DFMT_R5G6B5 ||
			displayMode.Format == D3DFMT_A1R5G5B5 ||
			displayMode.Format == D3DFMT_X1R5G5B5 ||
			displayMode.Format == D3DFMT_X4R4G4B4 ||
			displayMode.Format == D3DFMT_A4R4G4B4 ||
			displayMode.Format == D3DFMT_A8R3G3B2)
		{
			m_bitmode = 16;
		}
		else if(displayMode.Format == D3DFMT_R8G8B8)
		{//24�͏��Ȃ����낤�B
			m_bitmode = 24;
		}
		else if(displayMode.Format == D3DFMT_A8R8G8B8 ||
			displayMode.Format == D3DFMT_X8R8G8B8 ||
			displayMode.Format == D3DFMT_A2R10G10B10 ||
			displayMode.Format == D3DFMT_A8B8G8R8 ||
			displayMode.Format == D3DFMT_X8B8G8R8 ||
			displayMode.Format == D3DFMT_A2B10G10R10)
		{//32�͏d���Ȃ�̂��|�����ǁA�Ȃ�Ƃ�����ׂ��B
			m_bitmode = 32;
		}
		else if(displayMode.Format == D3DFMT_A16B16G16R16 ||
			displayMode.Format == D3DFMT_A16B16G16R16F)
		{//64�́A�����I�ɕ��y���邩��
			m_bitmode = 64;
		}
		else
		{//��̉�ʃ��[�h
			SunApp.MsgBox("���݂̉�ʃ��[�h�����ʂł��܂���B\n");
			return 0;
		}
		return 1;
	}
	else
	{//�t���X�N���[�����[�h�ł͎����Ńf�B�X�v���C���[�h�����߂���
		for ( long i = 0; BackBufferFormats[i] != D3DFMT_UNKNOWN; i++ )
		{
			if(FAILED(m_pd3d->CheckDeviceType( 0 , D3DDEVTYPE_HAL , BackBufferFormats[i], BackBufferFormats[i], SunApp.m_window)))
			{
				continue;
			}
			if(GetBackBufferFormatRank(m_d3dpp.BackBufferFormat) < GetBackBufferFormatRank(BackBufferFormats[i]))
			{
				m_d3dpp.BackBufferFormat = BackBufferFormats[i];
			}
		}
		if(this->m_d3dpp.BackBufferFormat == D3DFMT_UNKNOWN)
		{
			SunApp.MsgBox("���̃f�o�C�X�͗��p�\�ȃt�H�[�}�b�g������܂���B\n");
			return 0;
		}
	}
	return 1;
}
/**
	@brief �o�b�N�o�b�t�@�t�H�[�}�b�g�̕]��

	��{�I�ɃA���t�@�l�Ȃ����D��
*/
int CSun3D::GetBackBufferFormatRank(D3DFORMAT format)
{
	int ret=0;
	if(m_bitmode == 16)
	{
		switch(format)
		{
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 24)
	{
		switch(format)
		{
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 32)
	{
		switch(format)
		{
		case D3DFMT_X8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_A8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 64)
	{
		switch(format)
		{
		case D3DFMT_A16B16G16R16F:
			ret = 15;
			break;
		case D3DFMT_A16B16G16R16:
			ret = 14;
			break;
		case D3DFMT_X8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_A8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_R5G6B5:
			ret = 6;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 4;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 3;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 2;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		SunApp.MsgBox("�r�b�g���[�h�̐ݒ肪�����Ă��܂��B\n��x�R���t�B�O�t�@�C���������Ă��������B\n");
	}
	return ret;
}
/**
	@brief ���t���b�V�����[�g�̌���
*/
int CSun3D::DecideRefreshRate()
{
	//�E�B���h�E���[�h���Ƀ��t���b�V�����[�g��ݒ肷��ƃo�O��i;�t
	if(SunApp.m_window == 1)
		return 1;
	//�������������������Ȃ��ݒ�̂Ƃ�
	if(this->m_vsync == 0)
		return 1;
	HRESULT hr;
	int refreshRate = 0;
	// ���[�h���
	D3DDISPLAYMODE DisplayMode;
	DWORD ModeCount = this->m_pd3d->GetAdapterModeCount( D3DADAPTER_DEFAULT, m_d3dpp.BackBufferFormat);
	for ( DWORD j = 0; j < ModeCount; j++ )
	{
		hr = this->m_pd3d->EnumAdapterModes( D3DADAPTER_DEFAULT, m_d3dpp.BackBufferFormat, j, &DisplayMode );
		if SUCCEEDED( hr )
		{
			//640*480��������
			if(DisplayMode.Width == SunApp.m_width && DisplayMode.Height == SunApp.m_height)
			{
				//���t���b�V�����[�g�����60�ɋ߂�������
				if(abs(DisplayMode.RefreshRate-60) < abs(refreshRate-60))
				{
					refreshRate = DisplayMode.RefreshRate;
				}
			}
		}
	}
	m_d3dpp.FullScreen_RefreshRateInHz = refreshRate;
	return 1;
}
/**
	@brief �e�N�X�`���t�H�[�}�b�g�̌���
*/
int CSun3D::DecideTextureFormat()
{
	HRESULT hr;
	//�e�N�X�`���t�H�[�}�b�g���
	for ( long j = 0; TextureFormats[j] != D3DFMT_UNKNOWN; j++ )
	{
		hr = this->m_pd3d->CheckDeviceFormat(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						m_d3dpp.BackBufferFormat,
						0,
						D3DRTYPE_TEXTURE,
						TextureFormats[j] );
		if SUCCEEDED( hr )
		{
			//���݂̃e�N�X�`���t�H�[�}�b�g�ƕ]�����r
			if(GetTextureFormatRank(m_format) < GetTextureFormatRank(TextureFormats[j]))
			{
				m_format = TextureFormats[j];
			}
		}
	}
	return 1;
}
/**
	@brief �e�N�X�`���t�H�[�}�b�g�̕]��

  ��{�I�ɃA���t�@�l�����D��
*/
int CSun3D::GetTextureFormatRank(D3DFORMAT format)
{
	int ret = 0;
	int bitmode = ( m_texbit != 0 ? m_texbit : m_bitmode );
	if(bitmode == 16)
	{
		switch(format)
		{
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(bitmode == 24)
	{
		switch(format)
		{
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(bitmode == 32)
	{
		switch(format)
		{
		case D3DFMT_A8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_X8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else if(m_bitmode == 64)
	{
		switch(format)
		{
		case D3DFMT_A16B16G16R16F:
			ret = 15;
			break;
		case D3DFMT_A16B16G16R16:
			ret = 14;
			break;
		case D3DFMT_A8R8G8B8:
			ret = 13;
			break;
		case D3DFMT_A8B8G8R8:
			ret = 12;
			break;
		case D3DFMT_A2R10G10B10:
			ret = 11;
			break;
		case D3DFMT_A2B10G10R10:
			ret = 10;
			break;
		case D3DFMT_X8R8G8B8:
			ret = 9;
			break;
		case D3DFMT_X8B8G8R8:
			ret = 8;
			break;
		case D3DFMT_R8G8B8:
			ret = 7;
			break;
		case D3DFMT_A4R4G4B4:
			ret = 6;
			break;
		case D3DFMT_A1R5G5B5:
			ret = 5;
			break;
		case D3DFMT_A8R3G3B2:
			ret = 4;
			break;
		case D3DFMT_R5G6B5:
			ret = 3;
			break;
		case D3DFMT_X1R5G5B5:
			ret = 2;
			break;
		case D3DFMT_X4R4G4B4:
			ret = 1;
			break;
		default:
			ret = 0;
			break;
		}
	}
	else
	{
		SunApp.MsgBox("�r�b�g���[�h�̐ݒ肪�����Ă��܂��B\n��x�R���t�B�O�t�@�C���������Ă��������B\n");
	}
	return ret;
}
/**
	@brief ���̑��̃p�����[�^������
*/
int CSun3D::DecideOtherParameters()
{
	//---�\���̈�̐ݒ�	
	m_d3dpp.BackBufferWidth			= SunApp.m_width;				//���h�b�g���ݒ�
	m_d3dpp.BackBufferHeight		= SunApp.m_height;				//�c�h�b�g���ݒ�
	m_d3dpp.BackBufferCount			= 1;						//�o�b�N�o�b�t�@�̐�
	m_d3dpp.MultiSampleType			= D3DMULTISAMPLE_NONE;		//
	m_d3dpp.MultiSampleQuality		= D3DMULTISAMPLE_NONE;		//
//	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_FLIP;	//�t���b�v���x��
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//�t���b�v���x��
	m_d3dpp.Windowed				= SunApp.m_window;				//�E�C���h�E���[�h�œ���H
	m_d3dpp.hDeviceWindow			= SunApp.m_hWnd;				//�E�B���h�E�n���h��
	m_d3dpp.EnableAutoDepthStencil	= false;					//
	m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;				//�X�e���V���t�H�[�}�b�g
//	m_d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;			//�f�o�b�O�p�Ƀo�b�N�o�b�t�@�����b�N�\�ɂ���
//	m_d3dpp.FullScreen_RefreshRateInHz;							//�����͌���ς�
//	m_d3dpp.PresentationInterval;								//���ƂŃR���t�B�O�ɂ�茈��
	if(SunApp.m_window) //�E�B���h�E���[�h�Ő��������͎��Ȃ����Ƃɂ���
		m_vsync = 0;
	//��������R���t�B�O�ɉe�������
//	m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
	switch(m_vsync)
	{
	//VSYNC�񓯊��̏ꍇ
	case 0:
		if(m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE)
		{
			m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
		}
		//�f�o�C�X��D3DPRESENT_INTERVAL_IMMEDIATE�ɑΉ����Ă��Ȃ��Ƃ�
		else
		{
			m_vsync	= 1;
			m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
		}
		break;
	//VSYNC�����̏ꍇ
	case 1:
		m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
		break;
	default:
		SunApp.MsgBox("���������҂��̐ݒ肪�����Ă��܂��B\n��x�R���t�B�O�t�@�C�����폜���Ă��������B\n");
		return 0;
	}
	return 1;
}
/**
	@brief ���肳�ꂽ�p�����[�^�[�o��
*/
int CSun3D::ParameterLog()
{
	SunLog.LogOut("���肳�ꂽ�p�����[�^\n");
	SunLog.LogOut("	BackBufferFormat[ %s ]\n",this->GetFormatString(m_d3dpp.BackBufferFormat).data());
	SunLog.LogOut("	WindowMode[ %d ]\n",this->m_d3dpp.Windowed);
	SunLog.LogOut("	RefreshRate[ %d ]\n",this->m_d3dpp.FullScreen_RefreshRateInHz);
	SunLog.LogOut("	PresentationInterval[ %d ]\n",this->m_d3dpp.PresentationInterval);
	SunLog.LogOut("	Vsync[ %d ]\n",this->m_vsync);
	SunLog.LogOut("	TextureFormat[ %s ]\n",this->GetFormatString(m_format).data());
	return 1;
}
/**
	@brief �s�N�Z���t�H�[�}�b�g������擾
*/
string CSun3D::GetFormatString(D3DFORMAT Format)
{
	string ret;
	switch ( Format )
	{
	case D3DFMT_UNKNOWN:			ret = "UNKNOWN" ;		break;
	case D3DFMT_R8G8B8:				ret = "R8G8B8" ;		break;
	case D3DFMT_A8R8G8B8:			ret = "A8R8G8B8" ;		break;
	case D3DFMT_X8R8G8B8:			ret = "X8R8G8B8" ;		break;
	case D3DFMT_R5G6B5:				ret = "R5G6B5" ;		break;
	case D3DFMT_X1R5G5B5:			ret = "X1R5G5B5" ;		break;
	case D3DFMT_A1R5G5B5:			ret = "A1R5G5B5" ;		break;
	case D3DFMT_A4R4G4B4:			ret = "A4R4G4B4" ;		break;
	case D3DFMT_R3G3B2:				ret = "R3G3B2" ;		break;
	case D3DFMT_A8:					ret = "A8" ;			break;
	case D3DFMT_A8R3G3B2:			ret = "A8R3G3B2" ;		break;
	case D3DFMT_X4R4G4B4:			ret = "X4R4G4B4" ;		break;
	case D3DFMT_A2B10G10R10:		ret = "A2B10G10R10" ;	break;
	case D3DFMT_A8B8G8R8:			ret = "A8B8G8R8" ;		break;
	case D3DFMT_X8B8G8R8:			ret = "X8B8G8R8" ;		break;
	case D3DFMT_G16R16:				ret = "G16R16" ;		break;
	case D3DFMT_A2R10G10B10:		ret = "A2R10G10B10" ;	break;
	case D3DFMT_A16B16G16R16:		ret = "A16B16G16R16" ;	break;
	case D3DFMT_A8P8:				ret = "A8P8" ;			break;
	case D3DFMT_P8:					ret = "P8" ;			break;
	case D3DFMT_L8:					ret = "L8" ;			break;
	case D3DFMT_L16:				ret = "L16" ;			break;
	case D3DFMT_A8L8:				ret = "A8L8" ;			break;
	case D3DFMT_A4L4:				ret = "A4L4" ;			break;
	case D3DFMT_V8U8:				ret = "V8U8" ;			break;
	case D3DFMT_Q8W8V8U8:			ret = "Q8W8V8U8" ;		break;
	case D3DFMT_V16U16:				ret = "V16U16" ;		break;
	case D3DFMT_Q16W16V16U16:		ret = "Q16W16V16U16" ;	break;
	case D3DFMT_CxV8U8:				ret = "CxV8U8" ;		break;
	case D3DFMT_L6V5U5:				ret = "L6V5U5" ;		break;
	case D3DFMT_X8L8V8U8:			ret = "X8L8V8U8" ;		break;
	case D3DFMT_A2W10V10U10:		ret = "A2W10V10U10" ;	break;
	case D3DFMT_G8R8_G8B8:			ret = "G8R8_G8B8" ;		break;
	case D3DFMT_R8G8_B8G8:			ret = "R8G8_B8G8" ;		break;
	case D3DFMT_DXT1:				ret = "DXT1" ;			break;
	case D3DFMT_DXT2:				ret = "DXT2" ;			break;
	case D3DFMT_DXT3:				ret = "DXT3" ;			break;
	case D3DFMT_DXT4:				ret = "DXT4" ;			break;
	case D3DFMT_DXT5:				ret = "DXT5" ;			break;
	case D3DFMT_UYVY:				ret = "UYVY" ;			break;
	case D3DFMT_YUY2:				ret = "YUY2" ;			break;
	case D3DFMT_D16_LOCKABLE:		ret = "D16_LOCKABLE" ;	break;
	case D3DFMT_D32:				ret = "D32" ;			break;
	case D3DFMT_D15S1:				ret = "D15S1" ;			break;
	case D3DFMT_D24S8:				ret = "D24S8" ;			break;
	case D3DFMT_D24X8:				ret = "D24X8" ;			break;
	case D3DFMT_D24X4S4:			ret = "D24X4S4" ;		break;
	case D3DFMT_D32F_LOCKABLE:		ret = "D32F_LOCKABLE" ;	break;
	case D3DFMT_D24FS8:				ret = "D24FS8" ;		break;
	case D3DFMT_D16:				ret = "D16" ;			break;
	case D3DFMT_VERTEXDATA:			ret = "VERTEXDATA" ;	break;
	case D3DFMT_INDEX16:			ret = "INDEX16" ;		break;
	case D3DFMT_INDEX32:			ret = "INDEX32" ;		break;
	case D3DFMT_R16F:				ret = "R16F" ;			break;
	case D3DFMT_G16R16F:			ret = "G16R16F" ;		break;
	case D3DFMT_A16B16G16R16F:		ret = "A16B16G16R16F" ;	break;
	case D3DFMT_R32F:				ret = "R32F" ;			break;
	case D3DFMT_G32R32F:			ret = "G32R32F" ;		break;
	case D3DFMT_A32B32G32R32F:		ret = "A32B32G32R32F" ;	break;
	}

	return ret;
}
/**
	@brief �G���[������擾
*/
string CSun3D::GetErrorString(HRESULT hr)
{
	string ret;
	switch ( hr )
	{
	case D3D_OK:
		ret = "�G���[�͔������Ă��Ȃ��B" ;
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:
		ret = "���ݐݒ肳��Ă��郌���_�����O �X�e�[�g�͈ꏏ�Ɏg�p�ł��Ȃ��B" ;
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		ret = "���݂̃e�N�X�`�� �t�B���^�͈ꏏ�Ɏg�p�ł��Ȃ��B" ;
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		ret = "���݂̃e�N�X�`���͓����Ɏg�p�ł��Ȃ��B��ʂɃ}���`�e�N�X�`�� �f�o�C�X�ɂ����āA�����ɗL���ɂ��ꂽ�p���b�g���e�N�X�`���œ����p���b�g�����L����K�v������ꍇ�ɔ�������B" ;
		break;
	case D3DERR_DEVICELOST:
		ret = "�f�o�C�X�������Ă��āA�����_�ł͕����ł��Ȃ����߁A�����_�����O�͕s�\�ł���B" ;
		break;
	case D3DERR_DEVICENOTRESET:
		ret = "�f�o�C�X�̓��Z�b�g�ł��Ȃ��B" ;
		break;
	case D3DERR_DRIVERINTERNALERROR:
		ret = "�����h���C�o �G���[�B" ;
		break;
	case D3DERR_INVALIDCALL:
		ret = "���\�b�h�̌Ăяo���������ł���B���Ƃ��΁A���\�b�h�̃p�����[�^�ɖ����Ȓl���ݒ肳��Ă���ꍇ�ȂǁB" ;
		break;
	case D3DERR_INVALIDDEVICE:
		ret = "�v�����ꂽ�f�o�C�X�̎�ނ��L���łȂ��B" ;
		break;
	case D3DERR_MOREDATA:
		ret = "�w�肳�ꂽ�o�b�t�@ �T�C�Y�ɕێ��ł���ȏ�̃f�[�^�����݂���B" ;
		break;
	case D3DERR_NOTAVAILABLE:
		ret = "���̃f�o�C�X�́A�Ɖ�ꂽ�e�N�j�b�N���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_NOTFOUND:
		ret = "�v�����ꂽ���ڂ�������Ȃ������B" ;
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		ret = "Direct3D ���������s���̂ɏ\���ȃf�B�X�v���C ���������Ȃ��B" ;
		break;
	case D3DERR_TOOMANYOPERATIONS:
		ret = "�f�o�C�X���T�|�[�g���Ă���ȏ�̃e�N�X�`�� �t�B���^�����O�������A�A�v���P�[�V�������v�����Ă���B" ;
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:
		ret = "�A���t�@ �`���l���ɑ΂��Ďw�肳��Ă���e�N�X�`�� �u�����f�B���O�������A�f�o�C�X���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		ret = "�A���t�@ �`���l���ɑ΂��Ďw�肳��Ă���e�N�X�`�� �u�����f�B���O�������A�f�o�C�X���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:
		ret = "�J���[�l�ɑ΂��Ďw�肳��Ă���e�N�X�`�� �u�����f�B���O�������A�f�o�C�X���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		ret = "�J���[�l�ɑ΂��Ďw�肳��Ă���e�N�X�`�� �u�����f�B���O�������A�f�o�C�X���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		ret = "�f�o�C�X���w�肳�ꂽ�e�N�X�`���W���l���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		ret = "�f�o�C�X���w�肳�ꂽ�e�N�X�`�� �t�B���^���T�|�[�g���Ă��Ȃ��B" ;
		break;
	case D3DERR_WRONGTEXTUREFORMAT:
		ret = "�e�N�X�`�� �T�[�t�F�X�̃s�N�Z�� �t�H�[�}�b�g���L���łȂ��B" ;
		break;
	case E_FAIL:
		ret = "Direct3D �T�u�V�X�e�����Ō����s���̃G���[�����������B" ;
		break;
	case E_INVALIDARG:
		ret = "�����ȃp�����[�^���߂��Ă���֐��ɓn���ꂽ�B" ;
		break;
	case E_OUTOFMEMORY:
		ret = "Direct3D ���Ăяo�����������邽�߂̏\���ȃ����������蓖�Ă邱�Ƃ��ł��Ȃ������B" ;
		break;
	case D3DXERR_CANNOTATTRSORT:
		ret = "�œK���e�N�j�b�N�Ƃ��đ����̃\�[�g (D3DXMESHOPT_ATTRSORT) �̓T�|�[�g����Ă��Ȃ��B " ;
		break;
	case D3DXERR_CANNOTMODIFYINDEXBUFFER:
		ret = "�C���f�b�N�X �o�b�t�@��ύX�ł��Ȃ��B " ;
		break;
	case D3DXERR_INVALIDMESH:
		ret = "���b�V���������ł���B " ;
		break;
	case D3DXERR_SKINNINGNOTSUPPORTED:
		ret = "�X�L�j���O�̓T�|�[�g����Ă��Ȃ��B " ;
		break;
	case D3DXERR_TOOMANYINFLUENCES:
		ret = "�w�肳�ꂽ�e������������B " ;
		break;
	case D3DXERR_INVALIDDATA:
		ret = "�f�[�^�������ł���B" ;
		break;
	default:
		ret = "���m�̃G���[�B" ;
		break;
	}

	return ret;
}
/**
	@brief �f�o�C�X�\�͂𒲂ׂă��O���o��
*/
int CSun3D::DeviceLog()
{
	SunLog.LogOut("�O���t�B�b�N�f�o�C�X�\�̗͂�\n");

	//�h���C�o��D3DPRESENT_INTERVAL_IMMEDIATE���T�|�[�g���Ă��邩�H
	SunLog.LogCaps( m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE,"	�h���C�o�́AD3DPRESENT_INTERVAL_IMMEDIATE���T�|�[�g");

	//�h���C�o��D3DPRESENT_INTERVAL_ONE���T�|�[�g���Ă��邩�H
	SunLog.LogCaps( m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE,"	�h���C�o�́AD3DPRESENT_INTERVAL_ONE���T�|�[�g");

	//�o�b�N�o�b�t�@���X���b�v�s�\
	if(m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE == 0 && m_d3dcaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE == 0)
	{
		SunApp.MsgBox("���̃f�o�C�X�ł̓X���b�v���ł��܂���\n");
		return 0;
	}
	//�e�N�X�`���T�C�Y�͂Q�̗ݏ悩�H
	SunLog.LogCaps( m_d3dcaps.TextureCaps & D3DPTEXTURECAPS_POW2 , "	�e�N�X�`���T�C�Y�͂Q�̗ݏ悩�H");

	//�e�N�X�`���T�C�Y�͏�ɐ����`���H
	SunLog.LogCaps( m_d3dcaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY , "	�e�N�X�`���T�C�Y�͏�ɐ����`���H" );

	//�e�N�X�`���̍ő�T�C�Y�́H
	SunLog.LogOut("	�e�N�X�`���̍ő�T�C�Y�F[%d] �~ [%d]\n", m_d3dcaps.MaxTextureWidth , m_d3dcaps.MaxTextureHeight);

	//���Z����
	SunLog.LogCaps( m_d3dcaps.TextureOpCaps & D3DTEXOPCAPS_ADD ,"	���Z����");

	//�A���t�@�u�����h
	SunLog.LogCaps( m_d3dcaps.TextureOpCaps & D3DTEXOPCAPS_BLENDCURRENTALPHA ,"	�A���t�@�u�����h");

	//�v���~�e�B�u�̕`��\��
	SunLog.LogOut("	�v���~�e�B�u�̕`��\�ő吔:[%d]\n",m_d3dcaps.MaxPrimitiveCount);

	//---�f�o�C�X�̐��\�w�W�iD3DDEVTYPE �񋓌^
	SunLog.LogCaps( m_d3dcaps.DeviceType == D3DDEVTYPE_HAL,"	�n�[�h�E�F�A�A�N�Z�����[�V�������C���[");

	//�V�X�e���������o�b�t�@����r�f�I�������o�b�t�@�ւ̃C���[�W�]�����ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps  & D3DDEVCAPS_CANBLTSYSTONONLOCAL,"	�V�X�e�����r�f�I�̃C���[�W�]��");

	//---�����_�����O�R�}���h�������ɑ��M�ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_CANRENDERAFTERFLIP,"	���������_�����O");
	
	//---�V�X�e���������ɂ�����s�o�b�t�@�������ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_EXECUTESYSTEMMEMORY,"	�V�X�e���������̎��s�o�b�t�@");

	//---�r�f�I�������ɂ�����s�o�b�t�@�������ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_EXECUTEVIDEOMEMORY,"	�r�f�I�������̎��s�o�b�t�@");
	
	//---�f�o�C�X�ɂ́A�V�[���̃��X�^���p�̃n�[�h�E�F�A �A�N�Z�����[�V���������邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION,"	�V�[�����X�^�̃n�[�h�E�F�A�A�N�Z�����[�V����");
	
	//---�f�o�C�X�́A�n�[�h�E�F�A�Ńg�����X�t�H�[�� & ���C�e�B���O���T�|�[�g�ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT, "	�s��ϊ��A���������n�[�h�E�F�A�@�\" );
	
	//---�f�o�C�X�́A�T���x�W�F�A�X�v���C�����T�|�[�g���Ă��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_QUINTICRTPATCHES,"	�T���x�W�F�A�X�v���C���Ȑ��̃T�|�[�g");

	//---�r�f�I�������ȊO�Ƀe�N�X�`�����i�[�ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TEXTURENONLOCALVIDMEM ,"	�r�f�I�������ȊO�Ƀe�N�X�`�����i�[�ł��邩�H");

	//---�V�X�e������������e�N�X�`���𗘗p�ł��邩�H
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TEXTURESYSTEMMEMORY ,"	�V�X�e������������e�N�X�`���𗘗p�ł��邩�H");

	//---���_���Z�ς݂̃f�[�^�i�[��ɃV�X�e���������̃o�b�t�@�𗘗p�ł���
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TLVERTEXSYSTEMMEMORY ,"	�V�X�e���������o�b�t�@�ɒ��_�f�[�^��z�u");
	
	//---���_���Z�ς݂̃f�[�^�i�[��Ƀr�f�I�������̃o�b�t�@�𗘗p�ł���
	SunLog.LogCaps( m_d3dcaps.DevCaps & D3DDEVCAPS_TLVERTEXVIDEOMEMORY ,"	�r�f�I�������o�b�t�@�ɒ��_�f�[�^��z�u");
	
	//�A���`�G�C���A�X�𗘗p�ł���
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_ANTIALIAS , "	�A���`�G�C���A�X�𗘗p�ł���");

	//�\�[�X�u�����h�𗘗p�ł���
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_BLEND , "	�\�[�X�u�����h�𗘗p�ł���");

	//�t�H�O�𗘗p�ł���
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_FOG , "	�t�H�O�𗘗p�ł���");

	//�e�N�X�`���}�b�s���O�𗘗p�ł���
	SunLog.LogCaps( m_d3dcaps.LineCaps & D3DLINECAPS_TEXTURE , "	�e�N�X�`���}�b�s���O�𗘗p�ł���");
	return 1;
}
/**
	�f�o�C�X���O�֐�
*/
int CSun3D::CheckAdapterInformation()
{
	D3DADAPTER_IDENTIFIER9 Identifier;
	HRESULT hr;

	//
	// �A�_�v�^�[���
	//
	SunLog.LogOut( "�A�_�v�^�[���\n" );
	

	if(FAILED(m_pd3d->GetAdapterIdentifier( D3DADAPTER_DEFAULT, 0, &Identifier )))
	{
		SunApp.MsgBox("�A�_�v�^���𓾂��܂���ł����B�R�c�f�o�C�X���L���ɂȂ��Ă��邩�m�F���Ă��������B\n");
		return 0;
	}
	{
		SunLog.LogOut( "	Name [ %s ]\n", Identifier.Description );
		SunLog.LogOut( "	Driver [ %s ]\n", Identifier.Driver );
		SunLog.LogOut( "	Product [ %u ]\n", HIWORD(Identifier.DriverVersion.HighPart) );

		SunLog.LogOut( "�@�@Version [ %u.%u.%u ]\n",
			LOWORD(Identifier.DriverVersion.HighPart),
			HIWORD(Identifier.DriverVersion.LowPart),
			LOWORD(Identifier.DriverVersion.LowPart) );
		
		SunLog.LogOut( "	Windows Hardware Quality Lab (WHQL) �F�؃��x��\n" );

		if ( Identifier.WHQLLevel == 0 )
		{
			SunLog.LogOut( "	�F�؂���Ă��Ȃ��B\n" );			
		}
		else if ( Identifier.WHQLLevel == 1 )
		{
			SunLog.LogOut( "	WHQL �̔F�؂͎󂯂Ă��邪�A���p�ł�����t���͂Ȃ��B\n" );
		}
		else
		{
			SunLog.LogOut( "	%u.%u.%u\n",
				((Identifier.WHQLLevel>>16)&0xFF), ((Identifier.WHQLLevel>>8)&0x0F), ((Identifier.WHQLLevel>>0)&0x0F) );
			
		}
	}

	//
	// ���݂̉�ʃ��[�h�擾
	//
	SunLog.LogOut( "���݂̉�ʏ��\n" );
	D3DDISPLAYMODE WindowMode;
	if(FAILED( m_pd3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &WindowMode ) ) )
	{
		SunApp.MsgBox("���݂̉�ʏ��𓾂��܂���ł����B�R�c�f�o�C�X���L���ɂȂ��Ă��邩�m�F���Ă��������B\n");
		return 0;
	}
	{
		SunLog.LogOut( "�@�@�@Width [ %u ]\n", WindowMode.Width );
		SunLog.LogOut( "�@�@�@Height [ %u ]\n", WindowMode.Height );
		SunLog.LogOut( "�@�@�@RefreshRate [ %u ]\n", WindowMode.RefreshRate );
		SunLog.LogOut( "�@�@�@Format [ %s ]\n", this->GetFormatString(WindowMode.Format).data() );
	}

	//
	// �t���X�N���[�����Ɏg�p�\�ȉ�ʃ��[�h��񋓂���
	//
	SunLog.LogOut( "�g�p�\��[%d]�~[%d]��ʃ��[�h���\n" ,SunApp.m_width, SunApp.m_height);

	//�@�o�b�N�o�b�t�@�t�H�[�}�b�g�ŗ�
	int devFlag = 0;	//�Q�[�������s�ł���f�o�C�X���H
	for ( long i = 0; BackBufferFormats[i] != D3DFMT_UNKNOWN; i++ )
	{
		// �f�B�X�v���C���[�h���̎擾
		D3DDISPLAYMODE DisplayMode;
		DWORD ModeCount = this->m_pd3d->GetAdapterModeCount( D3DADAPTER_DEFAULT, BackBufferFormats[i] );
		if ( ModeCount > 0 )
		{
			int modeFlag = 0;	//���̃��[�h�̓Q�[���Ɏg�p�ł���H
			SunLog.LogOut( "�@BackBuffer Format [ %s ]\n", this->GetFormatString(BackBufferFormats[i]).data() );
			// ���[�h���
			for ( DWORD j = 0; j < ModeCount; j++ )
			{
				hr = this->m_pd3d->EnumAdapterModes( D3DADAPTER_DEFAULT, BackBufferFormats[i], j, &DisplayMode );
				if SUCCEEDED( hr )
				{
					//640*480��������
					if(DisplayMode.Width == SunApp.m_width && DisplayMode.Height == SunApp.m_height)
					{
						modeFlag += 1;
						SunLog.LogOut("		RefreshRate[%d]\n",DisplayMode.RefreshRate);
					}
				}
			}
			//�e�N�X�`���t�H�[�}�b�g���
			for ( long j = 0; TextureFormats[j] != D3DFMT_UNKNOWN; j++ )
			{
				hr = this->m_pd3d->CheckDeviceFormat(
								D3DADAPTER_DEFAULT,
								D3DDEVTYPE_HAL,
								BackBufferFormats[i],
								0,
								D3DRTYPE_TEXTURE,
								TextureFormats[j] );
				if SUCCEEDED( hr )
				{
					modeFlag += 1;
					SunLog.LogOut( "�@	�@�@TextureFormat[ %s ]\n", this->GetFormatString(TextureFormats[j]).data() );
				}
			}
			//���̃��[�h�ł�640*480�Ŏg�p�ł���e�N�X�`���t�H�[�}�b�g�����݂���
			devFlag = 1;
		}
	}
	//������ɔ����āE�E
	if(devFlag == 0)
	{
		SunApp.MsgBox("���̃f�o�C�X�ł�[%d]�~[%d]�Ŏg�p�ł���e�N�X�`���t�H�[�}�b�g������܂���\n",SunApp.m_width, SunApp.m_height);
		return 0;
	}

	return 1;
}
/**
	�e�N�X�`���ݒ�
*/
int CSun3D::SetTexture(LPDIRECT3DTEXTURE9 pd3dtex)
{
	if(FAILED(this->m_pd3ddev->SetTexture(0,pd3dtex)))
		return 0;
	return 1;
}
/**
	@brief �����_�����O�X�e�[�g

	���݂Ɠ����X�e�[�g�Ȃ�Ȃɂ����܂���B
*/
int CSun3D::SetRenderState( D3DRENDERSTATETYPE State, DWORD Param )
{
//	if ( m_renderState[State] != Param )
	{
		this->m_renderState[State] = Param;
		if(FAILED(this->m_pd3ddev->SetRenderState( State, Param )))
			return 0;
	}
	return 1;
}
/**
	@brief �e�N�X�`���X�e�[�g
	
	���݂Ɠ����X�e�[�g�Ȃ�Ȃɂ����܂���B
*/
int CSun3D::SetTextureState( D3DTEXTURESTAGESTATETYPE State, DWORD Param )
{
//	if ( this->m_textureState[State] != Param )
	{
		this->m_textureState[State] = Param;
		if(FAILED(this->m_pd3ddev->SetTextureStageState( 0, State, Param )))
			return 0;
	}
	return 1;
}
/*
//FVF�t���O�̃Z�b�g
//���݂Ɠ����X�e�[�g�Ȃ�Ȃɂ����܂���B
*/
int CSun3D::SetFVF(DWORD fvfState)
{
//	if ( this->m_fvfState != fvfState )
	{
		m_fvfState = fvfState;
		if(FAILED(this->m_pd3ddev->SetFVF(fvfState)))
			return 0;
	}
	return 1;
}
/**
//�J���[�L�[
//���݂Ɠ����X�e�[�g�Ȃ�Ȃɂ����܂���B
*/
int CSun3D::SetColorkeyEnable( bool Flag )
{
	if ( Flag )
	{
		this->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		this->SetRenderState( D3DRS_ALPHATESTENABLE, true );
		this->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATER );
		this->SetRenderState( D3DRS_ALPHAREF, 0 );
	}
	else
	{
		this->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
		this->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	}
	return 1;
}
/**
//�u�����f�B���O�^�C�v
//���݂Ɠ����X�e�[�g�Ȃ�Ȃɂ����܂���B
*/
int CSun3D::SetBlendingType( eBlendingType BlendingType )
{
	switch ( BlendingType )
	{
	case BLEND_NONE:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_NORMAL:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_ADD:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_MUL:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_REVERSE:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_INVDESTCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_BRILLIANT:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_DARKSIDE:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_MAX:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_MAX );
		break;

	case BLEND_MIN:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_MIN );
		break;

	case BLEND_TEST:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_DESTCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_TEST2:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;

	case BLEND_TRANS:
		this->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ZERO );
		this->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		this->SetRenderState( D3DRS_BLENDOP,   D3DBLENDOP_ADD );
		break;
	}
	return 1;
}

int CSun3D::SetViewport(D3DVIEWPORT9& viewport)
{
/*	if( FAILED( m_pd3ddev->SetViewport( &viewport ) ) ) {
		SunLog.LogOut("SetViewport�Ɏ��s\n");
		return 0;
	}
*/	return 1;
}
int CSun3D::MoveView(float x,float y,float z)
{
	D3DXMATRIX trans,view;
	D3DXMatrixTranslation(&trans,x,y,z);
	m_pd3ddev->GetTransform( D3DTS_VIEW, &view );
	D3DXMatrixMultiply(&view,&view,&trans);
	m_pd3ddev->SetTransform( D3DTS_VIEW, &view );

	return 1;
}
int CSun3D::SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX *pMatrix)
{
	if(FAILED( m_pd3ddev->SetTransform( State, pMatrix ) ))
		return 0;
	return 1;
}
int CSun3D::SetLight(DWORD Index,CONST D3DLIGHT9 *pLight)
{
	if(FAILED( m_pd3ddev->SetLight(Index,pLight)))
		return 0;
	if(FAILED( m_pd3ddev->LightEnable(Index,true)))
		return 0;
	return 1;
}