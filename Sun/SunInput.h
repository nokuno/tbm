// SunInput.h: SunInput �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

//���͏��
#define INP_NEUTRAL	-1
#define INP_RIGHT	0
#define INP_LEFT	1
#define INP_DOWN	2
#define INP_UP		3
#define INP_B0		4
#define INP_B1		5
#define INP_B2		6
#define INP_B3		7
#define INP_B4		8
#define INP_B5		9
#define INP_B6		10
#define INP_B7		11
#define INP_MAX		12

//�v���[���[���i�L�[�R���t�B�O�̐��ɉe���j
#define PLAYER_NUM 1

/**
	@brief �L�[�{�[�h����
*/
class SunKeyboard
{
	friend class CSunInput;
private:
	LPDIRECTINPUTDEVICE8	m_pdidKeyboard;
	BYTE		m_bKeyState[256];		///< �L�[�{�[�h���
	BYTE		m_bKeyStateOld[256];	///< �L�[�{�[�h��ԁi�P�t���O�j
	int		m_button[INP_MAX-4];		///< �{�^���R���t�B�O�i�����L�[�̂S�������j
public:
	SunKeyboard();
	virtual ~SunKeyboard();
	int Release();
	int GetInputState(int inp);	///<
	int GetInputEvent(int inp);	///< �}�N���iINP_**)����擾
	int UpdateInput();
	int ClearInput();
	int Init(LPDIRECTINPUT8);
	int GetButtonData();		///< ���f�[�^�擾
	int GetKeyboardState();		///< DIK����擾
};
/**
	@brief �}�E�X����
*/
class SunMouse
{
	friend class CSunInput;
private:
	LPDIRECTINPUTDEVICE8	m_pdidMouse;
	DIMOUSESTATE2 m_dims;		//�}�E�X���
	DIMOUSESTATE2 m_dimsOld;	//�}�E�X��ԁi�P�t���O�j
	float m_x,m_y,m_z;			//�}�E�X�|�C���^�̐�Έʒu
public:
	SunMouse();
	virtual ~SunMouse();
	int Release();
	int UpdateInput();
	int ClearInput();
	int Init(LPDIRECTINPUT8);
	int GetButtonState(int inp){return m_dims.rgbButtons[inp];}
	int GetButtonEvent(int inp){return m_dims.rgbButtons[inp] && !m_dimsOld.rgbButtons[inp];}
};
/**
	@brief �W���C�X�e�B�b�N����
*/
class SunJoystick
{
	friend class CSunInput;
private:
	LPDIRECTINPUTDEVICE8	m_pdidJoystick;
	DIJOYSTATE2		m_diJoyState;		///< �W���C�X�e�B�b�N���
	DIJOYSTATE2		m_diJoyStateOld;	///< �W���C�X�e�B�b�N���(�P�t���O�j
	TCHAR	m_name[MAX_PATH];			///< ���O
	int		m_number;					///< �W���C�X�e�B�b�N�ԍ�
	int		m_button[INP_MAX-4];		///< �{�^���R���t�B�O�i�����L�[�̂S�������j
	int		m_asobi;					///< �L�[�̗V��
public:
	SunJoystick();
	virtual ~SunJoystick();
	int Release();
	int Init(CSunInput* pInput,const DIDEVICEINSTANCE* pdidInstance);
	int UpdateInput();
	int GetInputState(int inp);	///< �}�N���iINP_**)����擾
	int GetInputEvent(int inp);	///< �}�N���iINP_**)����擾
	int GetButtonData();		///< ���f�[�^�擾
	int ClearInput();
	static BOOL CALLBACK EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext );
};
typedef list<SunJoystick*>::iterator SunJoyItr;

/**
	@brief ���ׂĂ̓���
*/
class CSunInput
{
	friend SunJoystick;
public:
	LPDIRECTINPUT8			m_pDI;				///<
	SunKeyboard				m_keyboard;			///<�L�[�{�[�h
	SunMouse				m_mouse;			///<�}�E�X
	list<SunJoystick*>		m_lstpJoystick;		///<�W���C�X�e�B�b�N���X�g
	int		m_state[INP_MAX];		///<�P�t���O�̏��
	int		m_device[PLAYER_NUM];	///<�P�o�C�Q�o�̎g�p�f�o�C�X�ԍ��i-1:�L�[�{�[�h�A0�ȏ�F�W���C�X�e�B�b�N)
	int		m_usekeyboard;				///< �L�[�{�[�h���p�t���O
	int		m_usemouse;					///< �}�E�X���p�t���O
	int		m_usejoystick;				///< �W���C�X�e�B�b�N���p�t���O
public:
	CSunInput();
	virtual ~CSunInput();
	int Release();				///<
	int	InitInput();			///< �g��Ȃ��B�������p
	int ReInit();				///< �i�āj�������A��������g������
	int GetInputState(int inp);	///< ���̏u�Ԃ̓��͏��
	int GetInputEvent(int inp);	///< �����ꂽ�u�Ԃ����o
	int GetInputHanasi(int inp);///< ���������o
	int GetInputState2(int player,int inp);	///< �����v���[���[�Ή�
	int GetInputEvent2(int player,int inp);	///< �����v���[���[�Ή�
	int GetMouseX();			///<
	int GetMouseY();			///<
	int GetMouseZ();			///<
	int GetMouseState(int inp){return m_mouse.GetButtonState(inp);}
	int GetMouseEvent(int inp){return m_mouse.GetButtonEvent(inp);}
	void SetMouseXY(int x,int y);
	void UseKeyboard(int use){m_usekeyboard=use;}
	void UseJoystick(int use){m_usekeyboard=use;}
	void UseMouse(int use){m_usemouse=use;}
	int UpdateInput();			///< �X�V
	int ClearInput();			///<
	int GetButtonData();			///< ���f�[�^�擾�i������ĂȂ����-1��Ԃ�
	int GetButtonData(int device);	///< �f�o�C�X�w��
	int GetButtonData2(int player);	///< �����v���[���[�Ή�
	int GetKeyState(int key);		///< DIK_*����擾
	int GetKeyEvent(int key);		///< DIK_*����擾
	SunJoystick* GetJoystick(int index);	///< �[��list::operaotr[]
	static BOOL CALLBACK EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext );
};
extern CSunInput SunInput;
