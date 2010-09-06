/**
	@file CSunApp
	@brief CSunApp �N���X�̃C���v�������e�[�V����
*/
#include "stdafx.h"
#include "resource.h"
#include "SunLog.h"
#include "SunApp.h"
#include "SunConfig.h"
#include "SunUtility.h"
#include "SunExeption.h"
#include "SunSound.h"
//��A�N�e�B�u����BGM��~�̂��߁ASunSound�ˑ��B

CSunApp SunApp;
///////////////////////////////////////////////
// �\�z/����
///////////////////////////////////////////////

CSunApp::CSunApp()
{
	InitMember();
}
/**
	@brief �����o�ϐ�������
*/
int CSunApp::InitMember()
{
	//�X�V�K�{�����o
	this->m_appname	= "Sun";
	this->m_appver	= 1.0;
	this->m_hInst		= NULL;
	//�A�v���P�[�V�����֌W
	this->m_hMutex	= NULL;
	this->m_bActive	= true;
	this->m_width		= 640;
	this->m_height	= 480;
	this->m_bAllowDouble	= 0;
	this->m_vsync	= 1;
	this->m_finish	= LOOP_MAINONLY;
	//�E�B���h�E�֌W
	this->m_hWnd		= NULL;
	this->m_hAccel	= NULL;
	this->m_window	= true;
	::SetRect(&this->m_rcClient,0,0,0,0);
	::SetRect(&this->m_rcWindow,0,0,0,0);
	this->m_bSmoothing = false;
	::ZeroMemory( &m_stickykeys, sizeof( m_stickykeys ));
	//�e�o�r�^�^�C�}�[�֌W
	this->m_fps		= 60;
	this->m_obfps		= 60;
	this->m_oldtime	= 0;
	this->m_time		= 0;
	this->m_count		= 0;
	this->m_fpsshow	= 1;
	this->m_pfreq.QuadPart = 0;
	this->m_ptime.QuadPart = 0;
	this->m_timer = 0;
	return 1;
}
/**
	@brief �f�X�g���N�^
*/
CSunApp::~CSunApp()
{
	this->Exit();
}
/*
	@brief �A�v���P�[�V������������
*/
int CSunApp::Init()
{
	//���������[�N�`�F�b�N
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM�̏�����
	::CoInitialize( NULL );

	//�ċN�����Ɏc���Ă��郁�b�Z�[�W���̂Ă�
	this->DumpMessage();

	//�A�v�����o��
	this->GetAppInformation();

	//�R���t�B�O�ǂݍ���
	this->LoadConfig();

/*	//�Q�d�N���h�~
	if(!this->CheckMutex())
	{
		return 0;
	}
*/
	//�E�B���h�E������
	if(!this->InitWindow())
	{
		return 0;
	}
	//�e�o�r�E�^�C�}�[�֌W
	if(!this->InitTimer())
	{
		return 0;
	}
/*	//�A���`�G�C���A�X����
	::SystemParametersInfo( SPI_GETFONTSMOOTHING, 0, &m_bSmoothing,0);
	if(m_bSmoothing)
		::SystemParametersInfo( SPI_SETFONTSMOOTHING, FALSE, NULL, 0 );
*/
	//�Œ�L�[����
	::SystemParametersInfo( SPI_GETSTICKYKEYS, sizeof( STICKYKEYS ), &m_stickykeys, 0 );
	m_stickykeys.cbSize = sizeof( STICKYKEYS );
	STICKYKEYS stickykeys = m_stickykeys;
	//stickykeys.dwFlags &= ~SKF_STICKYKEYSON;
	//stickykeys.dwFlags &= ~SKF_AVAILABLE;
	//stickykeys.dwFlags &= ~SKF_HOTKEYACTIVE;
	stickykeys.dwFlags &= ~SKF_CONFIRMHOTKEY;
	//stickykeys.dwFlags &= ~SKF_INDICATOR;
	::SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof( STICKYKEYS ), &stickykeys, 0 );
	return 1;
}
/**
	@brief �A�v���P�[�V�������I��
*/
int CSunApp::Exit()
{
	try
	{
		//�^�C�}�[�̉���
		::timeEndPeriod(m_wTimerRes);
		//�~���[�e�b�N�X�̉��
		if ( this->m_hMutex != NULL )
		{
			::CloseHandle( this->m_hMutex );
			this->m_hMutex	= NULL;
		}
		//�E�B���h�E�j��
		::DestroyWindow(this->m_hWnd);
		::UnregisterClass(this->m_appname.data(),this->m_hInst);
		//�b�n�l�����
		::CoUninitialize();
/*		//�A���`�G�C���A�X�ݒ�𕜌�
		if(m_bSmoothing)
			SystemParametersInfo( SPI_SETFONTSMOOTHING, TRUE, NULL, 0 );
*/
		//�Œ�L�[�ݒ�𕜌�
		::SystemParametersInfo( SPI_SETSTICKYKEYS, sizeof( STICKYKEYS ), &m_stickykeys, 0 );

	}catch(...){MsgBox("�A�v���P�[�V�����̉���ŕs���ȗ�O���������܂����B");}
	return 1;
}
/**
	@brief ��d�N���m�F
*/
int CSunApp::CheckMutex()
{
	if(this->m_bAllowDouble == 0)
	{
		if(this->m_hMutex != NULL)	//�Ă΂��̂Q���
			return 1;
		this->m_hMutex = ::CreateMutex( NULL, FALSE, this->m_appname.data() );
		if ( ::WaitForSingleObject( this->m_hMutex, 0 ) != WAIT_OBJECT_0 )
		{
			throw SunExeption("�Q�d�N�����Ȃ��ł��������B\n");
		}
	}
	return 1;
}
/**
	@brief ���b�Z�[�W����
*/
int CSunApp::Message()
{
	if(this->m_finish == LOOP_FINISH || this->m_finish == LOOP_RESTART)
		return 0;

	do
	{
		MSG	   msg;
		while(::PeekMessage(&msg,0,0,0,PM_REMOVE))	//���b�Z�[�W���[�v(���b�Z�[�W���Ȃ��Ȃ�܂Ń��[�v�j
		{
			if(msg.message==WM_QUIT)
			{
				this->m_finish	= LOOP_FINISH;	//�I���t���O�𗧂Ă�
				return 0;			//�I���̂Ƃ��͂O��Ԃ�
			}
			if( 0 == TranslateAccelerator( this->m_hWnd, this->m_hAccel, &msg ) )
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		//�A�N�e�B�u���ǂ������擾
		this->m_bActive = (GetActiveWindow()!=NULL);

		//CPU����΍�
		Sleep(1);
	}
	while( !this->m_bActive);
	return 1;
}
/**
	@brief WaitTime�֐�

	Message() + SyncFPS() + ObserveFPS()
*/
int CSunApp::WaitTime()
{
	//�����X�L�b�v�@�\�̂��߁A���������̗L���ɂ�����炸�E�F�C�g�����܂�
//	if( m_vsync )
//		return this->Message();
//	else
		if( m_timer )
			return this->Message() && this->SyncFPS2();
		else
			return this->Message() && this->SyncFPS();
}
/**
	@brief �^�C�}�[������
*/
int CSunApp::InitTimer()
{
	//TGT�^�C�}�[�֘A
	this->m_oldtime	= (int)::timeGetTime();
//	this->m_time		= (int)::timeGetTime();
	TIMECAPS tc;
 	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
		return 0;
	m_wTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
	::timeBeginPeriod(m_wTimerRes); 

	//QPC�^�C�}�[�֘A
	if( m_timer == 1 )
	{
		::QueryPerformanceFrequency( &m_pfreq );
		if( m_pfreq.QuadPart == 0 )
			m_timer = 0;
		::QueryPerformanceCounter( &m_ptime );
	}
	return 1;
}
/**
	@brief �e�o�r����
*/
int CSunApp::SyncFPS()
{
	if(this->m_finish == LOOP_FINISH || this->m_finish == LOOP_RESTART)
		return 0;
	if(this->m_fps <= 0)
		return 1;

	int time = ::timeGetTime();
	while(true)
    {
		//FPS����i�󃋁[�v���ׂ����H�j
		if( time < this->m_oldtime)
		{//�o�ߎ��ԕϐ����I�[�o�[�t���[
			this->m_oldtime = time;	//���݂̋N������̌o�ߎ��Ԃ��擾
		}

		//�O��̃��[�v��������̌o�ߎ��Ԃ��擾
		double t=fabs(time-this->m_oldtime);	//�O�񃋁[�v����̌o�ߎ���

		int _fps = 1000/this->m_fps;
		//�o�ߎ��Ԃɂ���Ď��s���鏈������
		if( t >= _fps )
		{//�O�񃋁[�v����o�ߎ��Ԃ���茈�ߒl���o�߂��Ă���
			do
			{//��茈�ߎ��ԂɂȂ�܂ŋ󃋁[�v
				t -= _fps;				//�o�ߎ��Ԃ�i�߂�
				this->m_oldtime+=_fps;	//�o�ߎ��Ԃ�i�߂�
			}
			while( t >= _fps );			//���o�ߎ��Ԃ���茈�ߎ��Ԃ��߂���܂Ń��[�v
			break;	//�^�C�����[�v�E�o
		}
		//���b�Z�[�W���P�񏈗�
		if(!this->Message())
			return 0;
	}
	return 1;
}
/**
	@brief �e�o�r����2
*/
int CSunApp::SyncFPS2()
{
	if(this->m_finish == LOOP_FINISH || this->m_finish == LOOP_RESTART)
		return 0;
	if(this->m_fps <= 0)
		return 1;
	while(true)
    {
		LARGE_INTEGER time = {0,0};

		QueryPerformanceCounter( &time );
//		QueryPerformanceFrequency( &m_pfreq );

		//FPS����i�󃋁[�v���ׂ����H�j
		if( time.QuadPart < this->m_ptime.QuadPart)
		{//�o�ߎ��ԕϐ����I�[�o�[�t���[
			this->m_ptime.QuadPart = time.QuadPart;	//���݂̋N������̌o�ߎ��Ԃ��擾
		}

		//�O��̃��[�v��������̌o�ߎ��Ԃ��擾
		LONGLONG t=abs(time.QuadPart-this->m_ptime.QuadPart);	//�O�񃋁[�v����̌o�߃J�E���g

		LONGLONG _fps = m_pfreq.QuadPart/this->m_fps; //���z��1�t���[���̃J�E���g��

		//�o�ߎ��Ԃɂ���Ď��s���鏈������
		if( t >= _fps )
		{//�O�񃋁[�v����o�ߎ��Ԃ���茈�ߒl���o�߂��Ă���
			do
			{//��茈�ߎ��ԂɂȂ�܂ŋ󃋁[�v
				t -= (double)_fps;				//�o�ߎ��Ԃ�i�߂�
				this->m_ptime.QuadPart += _fps;	//�o�ߎ��Ԃ�i�߂�
			}
			while( t >= _fps );			//�o�ߎ��Ԃ���茈�ߎ��Ԃ��߂���܂Ń��[�v
			break;	//�^�C�����[�v�E�o
		}
		//���b�Z�[�W���P�񏈗�
		if(!this->Message())
			return 0;
	}
	return 1;
}
/**
	@brief �e�o�r�ϑ�
*/
int CSunApp::ObserveFPS()
{
	int time = (int)::timeGetTime();
	this->m_count++;

	//�P�t���[����
	if( this->m_time == 0 ) {
		m_time = time;
	}
	//�P�b�����ɍX�V�i�P�b�ȏエ���ɍX�V�j
	if(time - this->m_time >= 1000)
	{
		this->m_obfps		= 1000.0 * this->m_count / (time-this->m_time);	//�e�o�r��double�^�ł��B
		this->m_count		= 0;
		this->m_time		= (int)::timeGetTime();
		this->m_fpsshow	= 1;	//�e�o�r�\���X�V�t���O
	}

	return 1;
}
/**
	@brief �e�o�r�\���i�^�C�g���o�[�g�p�j
*/
int CSunApp::ShowFPS()
{
	if(this->m_fpsshow > 0)
	{
		this->SetCaption("%s fps=%d",this->m_appname.data(),(int)this->m_obfps);
		this->m_fpsshow --;
	}
	return 1;
}
/**
	@brief �e�o�r������擾
*/
string CSunApp::GetFPSString()
{
	char str[32];
	sprintf(str,"%dFPS",(int)this->m_obfps);
	return str;
}
/**
	@brief ���b�Z�[�W�{�b�N�X

	printf�̏����Ń��b�Z�[�W�{�b�N�X�\��
*/
int CSunApp::MsgBox(const char *fmt,...)
{
	if(fmt == NULL)
		return 0;
	char str[512];	//���߂�����
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	SunLog(str,fmt,arg);
	va_end(arg);

	::MessageBox(NULL,str,this->m_appname.data(),MB_ICONERROR);
	return 1;
}
/**
	@brief �E�B���h�E�L���v�V����

	printf�̏����ŃE�B���h�E�L���v�V�����\��
*/
int CSunApp::SetCaption(const char *fmt,...)
{
	//�t���X�N���[����������E�B���h�E�L���v�V�����͕\���ł��Ȃ�
	if(this->m_window == false)
		return 0;
	if(fmt == NULL)
		return 0;
	char str[512];	//���߂�����
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	va_end(arg);

	::SetWindowText(this->m_hWnd,str);
	return 1;
}
/**
	@brief �f�o�b�K�o��

	Ouptput��printf�g��
*/
int CSunApp::Output(const char* fmt,...)
{
	if(fmt == NULL)
		return 0;
	char str[512];	//���߂�����
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	va_end(arg);

	OutputDebugString(str);
	return 1;
}
/**
	@brief �ď�����
*/
int CSunApp::ReInit()
{
	//�C�ӂɂd���������Ă΂Ȃ��Ă��ςނ悤�Ɂi�Q�x�ǂ�ł����܂�Ȃ�
	this->Exit();

	//�܂��v�l�Q�p�t�h�s���c���Ă�\��������̂Ń��b�Z�[�W���̂Ă�
	this->DumpMessage();

	//�I���E�G���[�֌W�̃f�[�^�����o��������
	this->m_finish	= LOOP_MAINONLY;

	//�������֐��͓���
	if(!this->Init())
		return 0;

	return 1;
}
/**
	@brief ���b�Z�[�W���̂Ă�

	�ď������p�֐�
*/
int CSunApp::DumpMessage()
{
    MSG	   msg;
	while(::PeekMessage(&msg,0,0,0,PM_REMOVE))	//���b�Z�[�W���[�v(���b�Z�[�W���Ȃ��Ȃ�܂Ń��[�v�j
	{
	    if( 0 == TranslateAccelerator( this->m_hWnd, this->m_hAccel, &msg ) )
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return 1;
}
/**
	@brief �A�v���P�[�V�����������O�A�E�g
*/
int CSunApp::GetAppInformation()
{
	//�A�v�����ƃo�[�W�������o��
	SunLog.LogOut("%s Ver %g\n",this->m_appname.data(),(double)this->m_appver);

	// �N������
	SYSTEMTIME SystemTime;
	::GetLocalTime( &SystemTime );
	SunLog.LogOut("�N������				[%u�N%u��%u�� %u��%u��%u�b]\n",
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
		SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond );
	return 1;
}
/**
	@brief �R���t�B�O�ǂݍ���
*/
int CSunApp::LoadConfig()
{
	m_window	= SunConfig.GetInt2("system","window",0);
	m_fps		= SunConfig.GetInt2("system","fps",60);
	m_timer		= SunConfig.GetInt2("system","timer",1);
	return 1;
}
/**
	@brief �I���t���O�𗧂Ă�
*/
int CSunApp::Finish()
{
	m_finish = LOOP_FINISH;
	return 1;
}
/**
	@brief �ċN���t���O�𗧂Ă�
*/
int CSunApp::ReStart()
{
	//�I���t���O�������Ă��炻�����D��
	if(m_finish == LOOP_FINISH)
		return 0;
	m_finish = LOOP_RESTART;
	return 1;
}
/**
	@brief �ċN���t���O�������Ă��邩�H
*/
int CSunApp::GetRestart()
{
	if(m_finish == LOOP_RESTART)
	{
		m_finish = LOOP_MAINONLY;
		return 1;
	}
	return 0;
}
/**
	�}�E�X�J�[�\���̈ړ�
*/
int CSunApp::MoveMouse(int x,int y)
{
	RECT rect;
	::GetWindowRect(m_hWnd,&rect);
	MoveMouse(rect.left + x, rect.right + y);
	return 1;
}
/**
	@brief �����g�p�֐�

	�E�B���h�E������
*/
int CSunApp::InitWindow()
{
	//�L�[�{�[�h�A�N�Z�����[�^�[
    this->m_hAccel = ::LoadAccelerators( this->m_hInst, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	//�E�B���h�E�N���X�̓o�^
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(wc);
    wc.lpszClassName = this->m_appname.data();
    wc.lpfnWndProc   = WndProc;
    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance     = this->m_hInst;
    wc.hIcon         = LoadIcon( this->m_hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) );
    wc.hIconSm       = LoadIcon( this->m_hInst, MAKEINTRESOURCE(IDI_MAIN_ICON) );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground= (HBRUSH)GetStockObject( BLACK_BRUSH );
//    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName  = "";
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;

    if( ::RegisterClassEx( &wc ) == 0 )
	{
		throw SunExeption("�E�B���h�E�N���X�̓o�^�Ɏ��s");
	}

	DWORD x = 0;
	DWORD y = 0;
	DWORD w = this->m_width;
	DWORD h = this->m_height;
	DWORD flag = WS_POPUP;

	//�E�B���h�E�T�C�Y���v�Z
	if (this->m_window)
	{//�E�C���h�E���[�h�ł̓E�B���h�E�T�C�Y��������Ƒ傫��
		flag |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

		w +=
			::GetSystemMetrics(SM_CXBORDER)		+
			::GetSystemMetrics(SM_CXEDGE)		+
			::GetSystemMetrics(SM_CXDLGFRAME);

		h +=
			::GetSystemMetrics(SM_CYBORDER)		+
			::GetSystemMetrics(SM_CYEDGE)		+
			::GetSystemMetrics(SM_CYDLGFRAME)	+
			::GetSystemMetrics(SM_CYCAPTION);

		x = ::GetSystemMetrics(SM_CXSCREEN)/2 - w/2;
		y = ::GetSystemMetrics(SM_CYSCREEN)/2 - h/2;
	}

	//�E�B���h�E���쐬�E�\��
    this->m_hWnd = ::CreateWindowEx( 0,
							this->m_appname.data(), 
							this->m_appname.data(),
							flag,
							x, y, w, h,
							NULL,
							NULL,
							this->m_hInst,
							NULL );
    if( this->m_hWnd == NULL )
	{
		throw SunExeption("�E�B���h�E�쐬�Ɏ��s\n");
	}
    ::ShowWindow( this->m_hWnd, 1 );
    ::UpdateWindow( this->m_hWnd );

	//�E�B���h�E�̈�擾
    ::GetWindowRect( this->m_hWnd, &this->m_rcWindow );

	return 1;
}
/**
	@brief WndProc�֐�

	�E�B���h�E�v���V�[�W��
	�E�B���h�E���b�Z�[�W������
*/
LRESULT CALLBACK CSunApp::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	HINSTANCE hInst = SunApp.m_hInst;
    switch (msg)
    {
        case WM_COMMAND:
            switch( LOWORD(wParam) )
            {
				case IDM_SCREENMODE:
					//�e�P
					//�t���������؂�ւ��؂�ւ��p��������
					//�؂�ւ��̓R���t�B�O�݂̂ɂȂ����̂Ŏg��Ȃ�
					return 0L;

                case IDM_EXIT:
					//�d�r�b�i�I���j
            	    ::PostMessage( hWnd, WM_CLOSE, 0, 0 );
                    return 0L;
            }
            break;
		case WM_PAINT:
			//������Sun3D.Present������Ă��o�b�N�o�b�t�@���X�V���ꂸ�ɓ]�������̂�
			//�΂�s���N�̉�ʂ��o�邾��
			break;
        case WM_GETMINMAXINFO:
            {
				//�E�B���h�E�T�C�Y�ύX�������Ȃ��B
                MINMAXINFO* pMinMax = (MINMAXINFO*) lParam;

                DWORD dwFrameWidth    = ::GetSystemMetrics( SM_CXSIZEFRAME );
                DWORD dwFrameHeight   = ::GetSystemMetrics( SM_CYSIZEFRAME );
                DWORD dwCaptionHeight = ::GetSystemMetrics( SM_CYCAPTION );

                pMinMax->ptMinTrackSize.x = (DWORD)SunApp.m_width  + dwFrameWidth * 2;
                pMinMax->ptMinTrackSize.y = (DWORD)SunApp.m_height + dwFrameHeight * 2 + 
                                            dwCaptionHeight;

                pMinMax->ptMaxTrackSize.x = pMinMax->ptMinTrackSize.x;
                pMinMax->ptMaxTrackSize.y = pMinMax->ptMinTrackSize.y;
            }
            return 0L;

        case WM_MOVE:
            return 0L;
        case WM_SIZE:
            break;
        case WM_SETCURSOR:
			//�t���X�N���[�����[�h�ł́A�J�[�\�����B��
            if( !SunApp.m_window )
            {
                SetCursor( NULL );
                return TRUE;
            }
            break;
        case WM_EXITMENULOOP:
			//���j���[���J���Ă�ԁA�Q�[���̐i�s���~�߂�
            SunApp.m_time = (int)::timeGetTime();
            SunApp.m_oldtime = (int)::timeGetTime();
			SunApp.m_count = 0;
			SunApp.m_obfps = 0;
            break;
        case WM_EXITSIZEMOVE:
			//�T�C�Y��ύX���悤�Ƃ��Ă���ԁA�Q�[���̐i�s���Ƃ߂�
            SunApp.m_time = (int)::timeGetTime();
            SunApp.m_oldtime = (int)::timeGetTime();
			SunApp.m_count = 0;
			SunApp.m_obfps = 0;
            break;
		case WM_ACTIVATE:
			//��A�N�e�B�u����BGM��~
			//SunSound�Ɉˑ����Ă�̂͂��������B
			switch( LOWORD( wParam )) {
			case WA_INACTIVE:	//��A�N�e�B�u
				PauseBGM();
				break;
			case WA_ACTIVE:		//�}�E�X�N���b�N�ȊO�ɂ��A�N�e�B�u
			case WA_CLICKACTIVE://�}�E�X�N���b�N�ɂ��A�N�e�B�u
				ResumeBGM();
				break;
			}
			break;
        case WM_SYSCOMMAND:
            switch( wParam ) {
				//�t���X�N���[���ł̓T�C�Y�ύX�E�ő剻�����s�\
                case SC_MOVE:			//�E�B���h�E�ړ�
				case SC_SIZE:			//�E�B���h�E�T�C�Y�ύX
				case SC_MAXIMIZE:		//�E�B���h�E�ő剻
                    if( !SunApp.m_window ) {
                        return TRUE;
					}
					break;
				case SC_MONITORPOWER:	//�ȓd�̓��[�h�}��
				case SC_SCREENSAVE:		//�X�N���[���Z�[�o�[�}��
					return 1;
            }
            break;
        case WM_DESTROY:
            ::PostQuitMessage( 0 );
            return 0L;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}