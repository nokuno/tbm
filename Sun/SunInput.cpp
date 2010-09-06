// SunInput.cpp: SunInput �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SunUtility.h"
#include "SunFile.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunExeption.h"
#include "SunInput.h"
#include "SunApp.h"

CSunInput SunInput;

//////////////////////////////////////////////////////////////////////
// �L�[�{�[�h
//////////////////////////////////////////////////////////////////////

SunKeyboard::SunKeyboard()
{
	m_pdidKeyboard = NULL;
	this->ClearInput();
	m_button[0] = DIK_C;
	m_button[1] = DIK_X;
	m_button[2] = DIK_Z;
	m_button[3] = DIK_LSHIFT;
	m_button[4] = DIK_A;
	m_button[5] = DIK_S;
	m_button[6] = DIK_LCONTROL;
	m_button[7] = DIK_D;
}

SunKeyboard::~SunKeyboard()
{
	this->Release();
}
int SunKeyboard::Release()
{
	if(m_pdidKeyboard != NULL)
	{
		m_pdidKeyboard->Unacquire();
		SAFE_RELEASE(m_pdidKeyboard);
	}
	return 1;
}
//!�X�V
int SunKeyboard::UpdateInput()
{
	HRESULT hr;
	//�擾�O�ɂP�t���O�̏�ԕۑ�
	for(int i=0;i<256;++i)
		m_bKeyStateOld[i] = m_bKeyState[i];
	//�L�[�{�[�h
	if( FAILED( hr = m_pdidKeyboard->GetDeviceState(256,(LPVOID)&m_bKeyState)))
	{
		if( hr == DIERR_INPUTLOST ) 
		{
			if(FAILED(hr = m_pdidKeyboard->Acquire()))
				throw SunExeption("�L�[�{�[�h�̏�Ԏ擾�Ɏ��s���܂���\n");
		}
		else
			throw SunExeption("�L�[�{�[�h�̏�Ԏ擾�Ɏ��s���܂���\n");
	}
	//�擾
	if(FAILED(m_pdidKeyboard->Acquire()))
	{
		throw SunExeption("�L�[�{�[�h�f�o�C�X�̎擾�Ɏ��s���܂���\n");
		return 0;
	}
	return 1;
}
//!��Ԏ擾
int SunKeyboard::GetInputState(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return (m_bKeyState[DIK_RIGHT] & 0x80) !=0;
		break;
	case INP_LEFT:
		return (m_bKeyState[DIK_LEFT] & 0x80) !=0;
		break;
	case INP_DOWN:
		return (m_bKeyState[DIK_DOWN] & 0x80) !=0;
		break;
	case INP_UP:
		return (m_bKeyState[DIK_UP] & 0x80) !=0;
		break;
	default:
		//�{�^���̏ꍇ
		if(inp>INP_MAX)
			throw SunExeption("�{�^���̍ő吔�𒴂��Ă��܂�\n");
		return (m_bKeyState[m_button[inp-4]] & 0x80) !=0;
		break;
	}
	return 0;
}
//!�ω��擾
int SunKeyboard::GetInputEvent(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return (m_bKeyState[DIK_RIGHT] & 0x80) !=0 && (m_bKeyStateOld[DIK_RIGHT] & 0x80) ==0;
		break;
	case INP_LEFT:
		return (m_bKeyState[DIK_LEFT] & 0x80) !=0 && (m_bKeyStateOld[DIK_LEFT] & 0x80) ==0;
		break;
	case INP_DOWN:
		return (m_bKeyState[DIK_DOWN] & 0x80) !=0 && (m_bKeyStateOld[DIK_DOWN] & 0x80) ==0;
		break;
	case INP_UP:
		return (m_bKeyState[DIK_UP] & 0x80) !=0 && (m_bKeyStateOld[DIK_UP] & 0x80) ==0;
		break;
	default:
		//�{�^���̏ꍇ
		if(inp>INP_MAX)
			throw SunExeption("�{�^���̍ő吔�𒴂��Ă��܂�\n");
		return (m_bKeyState[m_button[inp-4]] & 0x80) !=0 && (m_bKeyStateOld[m_button[inp-4]] & 0x80) ==0;
		break;
	}
	return 0;
}
//!�N���A
int SunKeyboard::ClearInput()
{
	::ZeroMemory(m_bKeyState,sizeof(m_bKeyState));
	for(int i=0;i<numof(m_bKeyStateOld);++i)
		m_bKeyStateOld[i] = 0x80;
	return 1;
}
//!������
int SunKeyboard::Init(LPDIRECTINPUT8 pDI)
{
	// �f�o�C�X���擾����B
	if(FAILED(pDI->CreateDevice(GUID_SysKeyboard,&m_pdidKeyboard, NULL)))
		throw SunExeption("�L�[�{�[�h�̏������Ɏ��s���܂����B\n");
	// �f�[�^�`�����Z�b�g����B
	m_pdidKeyboard->SetDataFormat(&c_dfDIKeyboard);
	// �������x����ݒ肷��B
	if(FAILED(m_pdidKeyboard->SetCooperativeLevel(SunApp.m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		throw SunExeption("�L�[�{�[�h�ɋ������x����ݒ�ł��܂���\n");
	//�A�N�Z�X�����擾����B
	if(FAILED(m_pdidKeyboard->Acquire()))
		throw SunExeption("�L�[�{�[�h�A�N�Z�X���̎擾�Ɏ��s���܂����B\n");
	//�{�^���R���t�B�O��ǂݍ���
	char section[MAX_PATH];
	sprintf(section,"Keyboard");
	for(int i=0;i<INP_MAX-4;++i)
	{
		char key[MAX_PATH];
		sprintf(key,"Button%d",i);
		m_button[i] = SunConfig.GetInt2(section,key,m_button[i]);
	}
	return 1;
}
//!���f�[�^�擾
int SunKeyboard::GetButtonData()
{
	for(int i=0;i<255;++i)
	{
		if(i!=DIK_ESCAPE && i!=DIK_UP && i!=DIK_DOWN && i!=DIK_LEFT && i!=DIK_RIGHT)
			if(m_bKeyState[i] & 0x80)
				return i;
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////
//�}�E�X
//////////////////////////////////////////////////////////////////////

SunMouse::SunMouse()
{
	m_pdidMouse = NULL;
	::ZeroMemory(&m_dims,sizeof(m_dims));
	::ZeroMemory(&m_dimsOld,sizeof(m_dimsOld));
	m_x=SunApp.m_width/2;
	m_y=SunApp.m_height/2;
	m_z=0;
}
SunMouse::~SunMouse()
{
	this->Release();
}
int SunMouse::Release()
{
	SAFE_RELEASE(m_pdidMouse);
	return 1;
}
int SunMouse::Init(LPDIRECTINPUT8 pdi)
{
	//�}�E�X�J�[�\�����E�B���h�E���Ɉړ�
	{
		RECT rect;
		::GetWindowRect(SunApp.m_hWnd,&rect);
		MoveMouse(rect.left + m_x, rect.right + m_y);
	}
	//�f�o�C�X���擾����B
	if(FAILED(pdi->CreateDevice(GUID_SysMouse, &m_pdidMouse, NULL)))
		throw SunExeption("�}�E�X�̏������Ɏ��s���܂����B\n");
	// �f�[�^�`�����Z�b�g����B
	if(FAILED(m_pdidMouse->SetDataFormat(&c_dfDIMouse2)))
		throw SunExeption("�}�E�X�̐ݒ�Ɏ��s���܂����B\n");
	// �������x����ݒ肷��B
	if(FAILED(m_pdidMouse->SetCooperativeLevel(SunApp.m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		throw SunExeption("�}�E�X�ɋ������x����ݒ�ł��܂���\n");
	//�o�b�t�@�����O�f�[�^�̃T�C�Y
	#define SAMPLE_BUFFER_SIZE  16
	DIPROPDWORD dipdw;
		// the header
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		// the data
		dipdw.dwData            = SAMPLE_BUFFER_SIZE;
	//�v���p�e�B�̐ݒ�
	if(FAILED(m_pdidMouse->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
		throw SunExeption("�}�E�X�v���p�e�B��ݒ�ł��܂���\n");
	//�A�N�Z�X�����擾����B
//	if(FAILED(m_pdidMouse->Acquire()))
//		throw SunExeption("�}�E�X�̃A�N�Z�X���̎擾�Ɏ��s���܂����B\n");
	return 1;
}
//!�X�V
int SunMouse::UpdateInput()
{
	HRESULT hr;
	//�P�t���O�̒l��ۑ�
	m_dimsOld = m_dims;
    // Get the input's device state, and put the state in dims
    ZeroMemory( &m_dims, sizeof(m_dims) );
    hr = m_pdidMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_dims );
    if( FAILED(hr) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = m_pdidMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = m_pdidMouse->Acquire();

        // Update the dialog text 
        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
		{
			SunLog("Mouse is unaquired.\n");
		}
    }
//	SunApp.Output("x:%d y:%d\n",m_x,m_y);
	m_x += 1.5*m_dims.lX;
	m_y += 1.5*m_dims.lY;
	m_z += 1.5*m_dims.lZ;
	m_x = max(0,min(SunApp.m_width,m_x));
	m_y = max(0,min(SunApp.m_height,m_y));
	if(m_x < 0 || m_x > SunApp.m_width || m_y < 0 || m_y > SunApp.m_height)
	{
		SunApp.MoveMouse(m_x, m_y);
	}
	return 1;
}
//////////////////////////////////////////////////////////////////////
// �W���C�X�e�B�b�N
//////////////////////////////////////////////////////////////////////

SunJoystick::SunJoystick()
{
	m_pdidJoystick = NULL;
	this->ClearInput();
	m_name[0]='\0';
	for(int i=0;i<INP_MAX-4;++i)
		m_button[i] = i;
	m_asobi = 0;
}

SunJoystick::~SunJoystick()
{
	this->Release();
}
int SunJoystick::Release()
{
	if(m_pdidJoystick != NULL)
	{
		m_pdidJoystick->Unacquire();
		SAFE_RELEASE(m_pdidJoystick);
	}
	return 1;
}
//!������
int SunJoystick::Init(CSunInput* pInput,const DIDEVICEINSTANCE* pdidInstance)
{
	SunLog("JoyStick [ %d ]\n",pInput->m_lstpJoystick.size());
	SunLog("	Instance[ %s ]\n",&pdidInstance->tszInstanceName);
	SunLog("	Product[ %s ]\n",&pdidInstance->tszProductName);
	//�f�o�C�X����ۑ�
	strcpy(m_name,pdidInstance->tszProductName);
	//�W���C�X�e�B�b�N�ԍ���ۑ�
	this->m_number = pInput->m_lstpJoystick.size();
	// �f�o�C�X�𐶐�����B
	if(FAILED(pInput->m_pDI->CreateDevice(pdidInstance->guidInstance,&m_pdidJoystick, NULL)))
		throw SunExeption("�W���C�X�e�B�b�N���������ł��܂���\n");
	//�f�[�^�t�H�[�}�b�g
	if(FAILED(m_pdidJoystick->SetDataFormat( &c_dfDIJoystick2 )))
		throw SunExeption("�W���C�X�e�B�b�N���������ł��܂���\n");
	//�������x��
	if(FAILED(m_pdidJoystick->SetCooperativeLevel( SunApp.m_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		throw SunExeption("�W���C�X�e�B�b�N�ɋ������x����ݒ�ł��܂���\n");
	//���͈̔͂̐ݒ�
	if(FAILED(m_pdidJoystick->EnumObjects( SunJoystick::EnumObjectsCallback, (VOID*)this, DIDFT_ALL )))
		throw SunExeption("�W���C�X�e�B�b�N�̏������Ɏ��s���܂����B\n");
	//�{�^���R���t�B�O��ǂݍ���
	char section[MAX_PATH];
	sprintf(section,"Joystick%d",m_number);
	for(int i=0;i<INP_MAX-4;++i)
	{
		char key[MAX_PATH];
		sprintf(key,"Button%d",i);
		m_button[i] = SunConfig.GetInt2(section,key,i);
	}
	m_asobi = SunConfig.GetInt2(section,"asobi",600);
	return 1;
}
//!�X�V
int SunJoystick::UpdateInput()
{
	HRESULT hr;
	//�擾�O�ɂP�t���O�̒l�Ƃ��ĕۑ�
	m_diJoyStateOld = m_diJoyState;
	//�W���C�X�e�B�b�N
	if(m_pdidJoystick)
	{
		if(FAILED(m_pdidJoystick->Acquire()))
			throw SunExeption("�W���C�X�e�B�b�N�f�o�C�X�̃A�N�Z�X���擾���ł��܂���\n");
		if( FAILED( m_pdidJoystick->Poll()))
		{
			hr = m_pdidJoystick->Acquire();
			//for(int i=0; i>10 || hr == DIERR_INPUTLOST ;++i) 
			while( hr == DIERR_INPUTLOST)
				hr = m_pdidJoystick->Acquire();
		}
		if(FAILED(m_pdidJoystick->GetDeviceState( sizeof(DIJOYSTATE2), &m_diJoyState )))
			throw SunExeption("�W���C�X�e�B�b�N�f�o�C�X��Ԃ̎擾�Ɏ��s���܂���\n");
	}
	return 1;
}
//!��Ԏ擾
int SunJoystick::GetInputState(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return m_diJoyState.lX > m_asobi;
		break;
	case INP_LEFT:
		return m_diJoyState.lX < -m_asobi;
		break;
	case INP_DOWN:
		return m_diJoyState.lY > m_asobi;
		break;
	case INP_UP:
		return m_diJoyState.lY < -m_asobi;
		break;
	default:
		//�{�^���̏ꍇ
		if(inp > INP_MAX)
			throw SunExeption("�{�^���̍ő吔�𒴂��Ă��܂�\n");
		return (m_diJoyState.rgbButtons[m_button[inp-4]] & 0x80) !=0;
	}
	return 0;
}
//!�ω��擾
int SunJoystick::GetInputEvent(int inp)
{
	switch(inp)
	{
	case INP_RIGHT:
		return m_diJoyState.lX > m_asobi && m_diJoyStateOld.lX <= m_asobi;
		break;
	case INP_LEFT:
		return m_diJoyState.lX < -m_asobi && m_diJoyStateOld.lX >= -m_asobi;
		break;
	case INP_DOWN:
		return m_diJoyState.lY > m_asobi && m_diJoyStateOld.lY <= m_asobi;
		break;
	case INP_UP:
		return m_diJoyState.lY < -m_asobi && m_diJoyStateOld.lY >= -m_asobi;
		break;
	default:
		//�{�^���̏ꍇ
		if(inp>INP_MAX)
			throw SunExeption("�{�^���̍ő吔�𒴂��Ă��܂�\n");
		return (m_diJoyState.rgbButtons[m_button[inp-4]] & 0x80) !=0 && (m_diJoyStateOld.rgbButtons[m_button[inp-4]] & 0x80) ==0;
		break;
	}
	return 0;
}
//!�N���A
int SunJoystick::ClearInput()
{
	::ZeroMemory(&m_diJoyState,sizeof(m_diJoyState));
	::ZeroMemory(&m_diJoyStateOld,sizeof(m_diJoyStateOld));
	for(int i=0; i<INP_MAX; ++i)
		m_diJoyStateOld.rgbButtons[i] = 0x80;
	return 1;
}
//!���f�[�^�擾
int SunJoystick::GetButtonData()
{
	//rgbButtons�͔z�񐔂��P�Q�W�Ȃ̂ŁE�E�i�΁j
	for(int i=0;i<128;++i)
	{
		if(m_diJoyState.rgbButtons[i])
			return i;
	}
	//�{�^����������Ă��Ȃ����-1��Ԃ��܂��B
	return -1;
}
//�l�͈̔͂̐ݒ�
BOOL CALLBACK SunJoystick::EnumObjectsCallback( const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext )
{
	SunJoystick *pInput = (SunJoystick*)pContext;

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if( pdidoi->dwType & DIDFT_AXIS )
    {
        DIPROPRANGE diprg; 
        diprg.diph.dwSize       = sizeof(DIPROPRANGE); 
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
        diprg.diph.dwHow        = DIPH_BYID; 
        diprg.diph.dwObj        = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin              = -1000; 
        diprg.lMax              = +1000; 
    
        // Set the range for the axis
        if( FAILED(pInput->m_pdidJoystick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) ) 
			throw SunExeption("�W���C�X�e�B�b�N�̏������Ɏ��s���܂����B\n");
         
    }
    return DIENUM_CONTINUE;
}
//////////////////////////////////////////////////////////////////////
// �C���v�b�g����
//////////////////////////////////////////////////////////////////////

CSunInput::CSunInput()
{
	m_pDI = NULL;
	::ZeroMemory(m_state,sizeof(m_state));
	::ZeroMemory(m_device,sizeof(m_device));
	m_usekeyboard = 1;
	m_usemouse = 1;
	m_usejoystick = 1;
}

CSunInput::~CSunInput()
{
	this->Release();
}
int CSunInput::Release()
{
	try
	{
		for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
			SAFE_DELETE(*i)
		m_lstpJoystick.clear();
		m_keyboard.Release();
		m_mouse.Release();
		SAFE_RELEASE(m_pDI);
	}catch(...){MsgBox("DirectInput�̉���ŕs���ȗ�O���܂����B");}
	return 1;
}
//!Input������
int CSunInput::InitInput()
{
	// DirectInput �I�u�W�F�N�g���쐬����B
	if(FAILED(DirectInput8Create(::GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL)))
		throw SunExeption("DirectInput�̏������Ɏ��s���܂����B\n");

	//�L�[�{�[�h������
	if(m_usekeyboard)
		m_keyboard.Init(m_pDI);

	//�}�E�X������
	if(m_usemouse)
		m_mouse.Init(m_pDI);

	//�W���C�X�e�B�b�N��
	if(m_usejoystick)
		if(FAILED(m_pDI->EnumDevices( DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback,(VOID*)this, DIEDFL_ATTACHEDONLY)))
			throw SunExeption("�W���C�X�e�B�b�N�̏������Ɏ��s���܂����B\n");

	//�R���t�B�O
	for(int i=0;i<PLAYER_NUM;++i)
	{
		//�R���t�B�O�̃f�t�H���g�����
		if(i<m_lstpJoystick.size())
			m_device[i] = i;	//�W���C�X�e�B�b�N
		else
			m_device[i] = -1;	//�L�[�{�[�h
		
		//�R���t�B�O�ǂݍ���
		char buffer[16];
		sprintf(buffer,"player%d",i);
		m_device[i] = SunConfig.GetInt2("keyconfig",buffer,m_device[i]);
	}
	return 1;
}
//!�S�f�o�C�X���X�V
int CSunInput::UpdateInput()
{
	//�X�V�O�ɂP�t���O�̒l���擾
	for(int i=0;i<INP_MAX;++i)
		m_state[i] = this->GetInputState(i);
	//�L�[�{�[�h�X�V
	if(m_usekeyboard)
		if(!m_keyboard.UpdateInput())
			return 0;
	//�}�E�X�X�V
	if(m_usemouse)
		if(!m_mouse.UpdateInput())
			return 0;
	//�S�W���C�X�e�B�b�N�X�V
	if(m_usejoystick)
		for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
			if(!(*i)->UpdateInput())
				return 0;
	return 1;
}
//!�S�f�o�C�X�̏�Ԃ���擾
int CSunInput::GetInputState(int inp)
{
	if(m_keyboard.GetInputState(inp))
		return 1;
	for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
		if((*i)->GetInputState(inp))
			return 1;
	return 0;
}
//!�����ꂽ�u�Ԃ��H
int CSunInput::GetInputEvent(int inp)
{
	if(m_state[inp] == 0 && this->GetInputState(inp) == 1)
		return 1;
	return 0;
}
//!����
int CSunInput::GetInputHanasi(int inp)
{
	if(m_state[inp] == 1 && this->GetInputState(inp) == 0)
		return 1;
	return 0;
}
//!�v���C���[�ԍ��̃f�o�C�X����擾
int CSunInput::GetInputState2(int player,int inp)
{
	if(m_device[player] == -1) //�L�[�{�[�h
		return m_keyboard.GetInputState(inp);
	else if(m_device[player] >= 0) //�W���C�X�e�B�b�N
	{
		SunJoystick* pJoystick = GetJoystick(m_device[player]);
		if(pJoystick!=NULL)
			return pJoystick->GetInputState(inp);
	}
	return 0;
}
//!�����ꂽ�u�Ԃ��H
int CSunInput::GetInputEvent2(int player,int inp)
{
	if(m_device[player] == -1) //�L�[�{�[�h
		return m_keyboard.GetInputEvent(inp);
	else if(m_device[player] >= 0) //�W���C�X�e�B�b�N
	{
		SunJoystick* pJoystick = GetJoystick(m_device[player]);
		if(pJoystick!=NULL)
			return pJoystick->GetInputEvent(inp);
	}
	return 0;
}
//!�}�E�X
int CSunInput::GetMouseX()
{
	return m_mouse.m_x;
}
int CSunInput::GetMouseY()
{
	return m_mouse.m_y;
}
int CSunInput::GetMouseZ()
{
	return m_mouse.m_z;
}
void CSunInput::SetMouseXY(int x,int y)
{
	m_mouse.m_x=x;
	m_mouse.m_y=y;
}
//!�S�f�o�C�X���N���A
int CSunInput::ClearInput()
{
	m_keyboard.ClearInput();
	for(SunJoyItr i=m_lstpJoystick.begin();i!=m_lstpJoystick.end();++i)
		(*i)->ClearInput();
	//�����ꂽ�u�ԂɂȂ�Ȃ��悤�A���łɉ�����Ă����ԂƂ݂Ȃ�
	for(int i=0;i<INP_MAX;++i)
		m_state[i] = 1;
	return 1;
}
//!�ċN���\���\�ȏ�����
int CSunInput::ReInit()
{
	if(!this->Release())
		return 0;
	if(!this->InitInput())
		return 0;
	if(!this->UpdateInput())
		return 0;
	return 1;
}
/**
	���f�[�^�擾
	�L�[�R���t�B�O�p
	�{�^����������Ă��Ȃ����-1��Ԃ�
	�����L�[�͖���
*/
int CSunInput::GetButtonData()
{
	for( SunJoyItr i = m_lstpJoystick.begin(); i != m_lstpJoystick.end(); ++i ) {
		int inp = (*i)->GetButtonData();
		if( inp >= 0 )
			return inp;
	}
	return -1;
}
/**
	���f�[�^�擾 �f�o�C�X�w��
	�L�[�R���t�B�O�p
	�{�^����������Ă��Ȃ����-1��Ԃ�
	�����L�[�͖���
*/
int CSunInput::GetButtonData(int device)
{
	if(device == -1) //�L�[�{�[�h
		return m_keyboard.GetButtonData();
	else if(device >= 0) //�W���C�X�e�B�b�N
	{
		SunJoystick* pJoystick = GetJoystick(device);
		if(pJoystick!=NULL)
			return pJoystick->GetButtonData();
	}
	return -1;
}
/**
	���f�[�^�擾 �����v���C���[�Ή�
	�L�[�R���t�B�O�p
	�{�^����������Ă��Ȃ����-1��Ԃ�
	�����L�[�͖���
*/
int CSunInput::GetButtonData2(int player)
{
	if(m_device[player] == -1) //�L�[�{�[�h
		return m_keyboard.GetButtonData();
	else if(m_device[player] >= 0) //�W���C�X�e�B�b�N
	{
		SunJoystick* pJoystick = GetJoystick(m_device[player]);
		if(pJoystick!=NULL)
			return pJoystick->GetButtonData();
	}
	return -1;
}
//DIK����擾
int CSunInput::GetKeyState(int key) {
	return m_keyboard.m_bKeyState[key] & 0x80;
}
int CSunInput::GetKeyEvent(int key) {
	return (m_keyboard.m_bKeyState[key] & 0x80) &&
		 (m_keyboard.m_bKeyStateOld[key] & 0x80);
}

//�[��list::operator[]
SunJoystick* CSunInput::GetJoystick(int index)
{
	int i=0;
	for(SunJoyItr itr=m_lstpJoystick.begin();itr!=m_lstpJoystick.end();++itr)
	{
		if(index == i)
			return *itr;
		++i;
	}
	return NULL;
}
//!Input�R�[���o�b�NStatic�֐�
BOOL CALLBACK CSunInput::EnumJoysticksCallback( const DIDEVICEINSTANCE* pdidInstance, VOID* pContext )
{
	CSunInput *pInput = (CSunInput*)pContext;
	SunJoystick* pJoystick = new SunJoystick;
	//�W���C�X�e�B�b�N������
	if(!pJoystick->Init(pInput, pdidInstance))
	{
		SunLog("�W���C�X�e�B�b�N�̏������Ɏ��s[%s]\n",pJoystick->m_name);
		SAFE_DELETE(pJoystick);
		return DIENUM_CONTINUE;
	}
	//���X�g�ɒǉ�
	pInput->m_lstpJoystick.push_back(pJoystick);
    return DIENUM_CONTINUE;
}
