/**
	@file Sun3D.h
	@brief �R�c�֘A
*/
#pragma once

//�u�����f�B���O���[�h
enum eBlendingType
{
	BLEND_NONE,			///< �A���t�@�u�����h�����i���߂��Ȃ��j
	BLEND_NORMAL,		///< �ʏ�
	BLEND_ADD,			///< ���Z
	BLEND_MUL,			///< ��Z
	BLEND_REVERSE,		///< ���]
	BLEND_BRILLIANT,	///< �����Ă�
	BLEND_DARKSIDE,		///< �Ă�����
	BLEND_MAX,			///< ��r�i���j
	BLEND_MIN,			///< ��r�i�Áj

	BLEND_TEST,			///< �J���p
	BLEND_TEST2,		///< �J���p
	BLEND_TRANS,		///< ����
};
///Direct3D�I�u�W�F�N�g�����֐��p�֐��|�C���^�^
typedef IDirect3D9 * (WINAPI *PDIRECT3DCREATE9)( unsigned int );
/**
	@class CSun3D
	@brief	�R�c�����N���X
	
	�c�����������R�c�֘A�̃N���X�B
	�c�����������R�c���c�k�k���瓮�I�ɓǂݍ��ށB
	�R�c�f�o�C�X�́u�I���v�u�쐬�v�u�\�͂̃��O�o�́v�Ȃǃf�o�C�X�֘A�������B
	�����ŃR���t�B�O��ǂݍ��ށB

    @todo	------------------------
	���݂͂Q�c�p�̋@�\�̂ݎ������Ă���܂��B
	�R�c�p�̃��C�g�Ȃǂ̐ݒ�@�\�͂��ƂŎ�������K�v������܂��B
	�e�N�X�`���̑��d�ݒ���͂����@�\�Ƃ��B
    @bug	------------------------
*/
class CSun3D
{
	friend class SunSurface;
	friend class SunTexture;
	friend class SunSpriteBase;
	friend class SunImage;
public:
//�f�[�^�����o
	HINSTANCE			m_hDll;		///<DLL�̃n���h��
	LPDIRECT3D9			m_pd3d;		///<D3D
	LPDIRECT3DDEVICE9	m_pd3ddev;	///<DEVICE
	int	m_bitmode;	///<��ʂ̃r�b�g���[�h
	int	m_texbit;	///<�e�N�X�`���̃r�b�g���[�h
	int	m_vsync;	///<��������
	int	m_square;	///<�����`�e�N�X�`��
	int	m_skip;		///<�`��X�L�b�v���[�h
	int	m_interval;	///<�`��X�L�b�v��
	int	m_minimum;	///<�`��X�L�b�vFPS
	int	m_scount;	///<�`��X�L�b�v�J�E���g
	int	m_count;	///<�J�E���g��
	int m_time;		///<�O�t���[����timeGetTime�̒l
	double	m_obfps;	///<�e�o�r�i���t���[���ϑ��l�j

	D3DPRESENT_PARAMETERS	m_d3dpp;	///<�������̂Ƃ������K�v�H
	D3DCAPS9	m_d3dcaps;	///<�f�o�C�X�\��
	D3DFORMAT	m_format;	///<�g�p����e�N�X�`���t�H�[�}�b�g
	DWORD	m_renderState[0xFF];	///< �����_�����O�X�e�[�g
	DWORD	m_textureState[0xFF];	///< �e�N�X�`���X�e�[�g
	DWORD	m_fvfState;				///<FVF�̃t���O
	D3DXIMAGE_FILEFORMAT	m_fileformat;	///<�X�i�b�v�V���b�g�̌`��
public:
//��{�@�\
	CSun3D();						///<�����o�̏�����
	virtual ~CSun3D();				///<�d�������i�j���Ăт܂��B
	int Init();						///<������
	int Exit();						///<�I��
	int ReInit();					///<�ď�����
	int LoadConfig();				///<�R���t�B�O���[�h
	int Clear(D3DCOLOR color);		///<�o�b�N�o�b�t�@���N���A
	int GetUpdate();				///<��ʂ��X�V���邩�ǂ���
	int Present();					///<�o�b�N�o�b�t�@���v���[���e�[�V����
	int BeginScene();				///<�V�[���J�n
	int EndScene();					///<�V�[���I��
	int	GetTextureSize();			///<�ő�e�N�X�`���T�C�Y�擾
//�����g�p
	int CreateDirect3D();			///<�c�����������R�c�쐬
	int CreateDevice();				///<�f�o�C�X�쐬
	int DecideDisplay();			///<m_d3dpp�������֌W
	int DecideRefreshRate();		///<m_d3dpp�������֌W
	int DecideTextureFormat();		///<m_d3dpp�������֌W
	int DecideOtherParameters();	///<m_d3dpp�������֌W
	int InitState();				///<�e��X�e�[�g�̏�����
//���O�֘A	
	int CheckAdapterInformation();	///<�f�B�X�v���C���O�o��
	int InitCaps();					///<�b�`�o�r������
	int	DeviceLog();				///<�b�`�o�r���O�o��
	int ParameterLog();				///<���肳�ꂽ�p�����[�^�̏o��
	int GetBackBufferFormatRank(D3DFORMAT format);	///<�o�b�N�o�b�t�@�t�H�[�}�b�g�̕]��
	int GetTextureFormatRank(D3DFORMAT format);		///<�e�N�X�`���t�H�[�}�b�g�̕]��
	string GetFormatString(D3DFORMAT Format);		///<�s�N�Z���t�H�[�}�b�g������擾
	string GetErrorString(HRESULT hr);				///<�G���[������擾
//�e�N�X�`���֘A
	int SetTexture(LPDIRECT3DTEXTURE9 pd3dtex);		///<�e�N�X�`���ݒ�
	int SetColorkeyEnable(bool flag);				///<�J���[�L�[�ݒ�
	int SetRenderState( D3DRENDERSTATETYPE State, DWORD Param );		///<�����_�[�X�e�[�g
	int SetTextureState(D3DTEXTURESTAGESTATETYPE State, DWORD Param );	///<�e�N�X�`���X�e�[�g
	int SetBlendingType( eBlendingType BlendingType );					//�u�����f�B���O���[�h
	int SetFVF( DWORD fvfState);						///<FVF�t���O�Z�b�g
	int GetVsync(){return m_vsync;}
	int SetViewport(D3DVIEWPORT9& viewport);
//3D�֘A
	int MoveView(float x,float y,float z);
	int SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX *pMatrix);
	int SetLight(DWORD Index,CONST D3DLIGHT9 *pLight);
};

extern CSun3D Sun3D;