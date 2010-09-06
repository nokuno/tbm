
#pragma once

/**
	@file Sun.h
	CSunApp�N���X�̐錾�B
	���ۂɂ͗B��̃O���[�o���ϐ�Sun��p���܂��B
*/

//���C�����[�v�E�ċN�����[�v�t���O
enum eLoopFlag
{
	LOOP_MAINONLY,				///<���C�����[�v
	LOOP_FINISH,				///<�I��
	LOOP_RESTART,				///<�ċN��
};
/**
	@class CSunApp
	@brief	�A�v���P�[�V�����Ǘ��N���X

	�A�v���P�[�V�����̊Ǘ���S������B
	��̓I�ɂ́u�E�B���h�E�Ǘ��v�u�e�o�r�Ǘ��v�u���O�t�@�C���Ǘ��v
	
	�R���t�B�O�ύX�ɂ��ċN���̃C���[�W�Ƃ��Ă͂���Ȋ���
	if(!Config())
		Sun.ReStart();
*/
class CSunApp
{
public:
//�f�[�^�����o�i�������邢�̂�public�j
//���̂����X�V�K�{�����o��
//m_appname,m_appver,m_hInst�̎O�Ƃ���B�i���Ƃő�����\������j
//�܂��A�R���t�B�O�ɂ��X�V�����o�͂��܂̂Ƃ���
//m_bWindow,m_fps�ł���B
	//�X�V�K�{�����o
	string		m_appname;	///<�A�v���P�[�V������
	double		m_appver;	///<�o�[�W�������
	HINSTANCE	m_hInst;	///<�C���X�^���X�n���h��
	//�A�v���P�[�V�����֌W
	HANDLE	m_hMutex;	///<�~���[�e�b�N�X�n���h��
	HACCEL	m_hAccel;	///<�L�[�{�[�h�A�N�Z�����[�^
	int		m_width;	///<��
	int		m_height;	///<����
	int		m_bAllowDouble;	///<�Q�d�N�����������H
	int		m_vsync;		
	eLoopFlag	m_finish;	///<�I���t���O�i�O�F���C�����[�v�A�P�F���[�v�I���A�Q�F�ċN���j
	//�E�B���h�E�֌W
	int		m_window;		///<�E�B���h�E���[�h�t���O(0:window,1:fullscreen)
	HWND	m_hWnd;			///<�E�B���h�E�n���h��
	bool	m_bActive;		///<�A�N�e�B�u�t���O
	RECT	m_rcClient;		///<�N���C�A���g�̈�
	RECT	m_rcWindow;		///<�E�B���h�E�̈�
	bool	m_bSmoothing;	///<�A���`�G�C���A�X�t���O
	STICKYKEYS	m_stickykeys;	///<�Œ�L�[���\����
	//�e�o�r�^�^�C�}�[�֌W
	int		m_fps;		///<�e�o�r�i�ݒ�l�j
	double	m_obfps;	///<�e�o�r�i���b�ϑ��l�j
	int		m_oldtime;	///<�i�����p�j�P�t���O��timeGetTime�̒l
	int		m_time;		///<�i�ϑ��p�j�P�b�O��timeGetTime�̒l
	int		m_count;	///<�i�ϑ��p�j�P�b�����X�V�̂��߂̃t���[���J�E���g
	int		m_fpsshow;	///<�i�\���p�j�X�V�t���O
	int		m_timer;	///<�^�C�}�[�̎�ށi0�FtimeGetTime�A1:QueryPerformanceCounter)
	UINT	m_wTimerRes;	///<�^�C�}�[�̕���\
	LARGE_INTEGER m_pfreq;	///<QueryPerformanceFrequency
	LARGE_INTEGER m_ptime;	///<QueryPerformanceCounter

public:
//��{�@�\
	CSunApp();					///<�R���X�g���N�^�B�����o�̏������B
	virtual ~CSunApp();		///<�f�X�g���N�^�B������Exit()���ĂԁB�O���[�o���������Ȃ��B
	int Init();				///<���낢�돉�����i������InitLog�Ȃ�)�B�O������f�[�^�����o��K�v�Ȃ����X�V���Ă���Ă�
	int InitWindow();		///<�E�B���h�E�������BInit()�̂��ƃR���t�B�O�����������Ă���Ă�
	int Exit();				///<�G���[�\����Mutex���
	int ReInit();			///<�ď������B���̂Ƃ��X�V�K�{�f�[�^�����o�͑O��̂܂܎c��
	int WaitTime();			///<Message() + SyncFPS() + ObserveFPS()
	int Message();			///<���b�Z�[�W�����i�����g�p�j
	int Finish();			///<�A�v���P�[�V�����I��
	int ReStart();			///<�A�v���P�[�V�����ċN��
	int GetRestart();		///<�ċN���t���O�������Ă��邩�H
	//�e�o�r�^�^�C�}�[�֌W
	int InitTimer();
	int SyncFPS();		///<�e�o�r����(timeGetTime�g�p�j
	int SyncFPS2();		///<�e�o�r����(QueryPerformanceCounter�g�p)
	int ObserveFPS();	///<�e�o�r�ϑ�
	int ShowFPS();		///<�e�o�r�\���i�^�C�g���o�[�g�p�j
	string GetFPSString();
	//���O�E�f�o�b�K�o�͂ȂǕ\���֘A
	int MsgBox(const char *fmt,...);	///<printf�����b�Z�[�W�{�b�N�X
	int SetCaption(const char *fmt,...);///<printf���E�B���h�E�L���v�V����
	int Output(const char* fmt,...);	///<printf���f�o�b�K�o��
	int MoveMouse(int x,int y);			///<�}�E�X�J�[�\���ړ�
	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );	///<�E�B���h�E�v���V�[�W��
//�Z�b�g�E�Q�b�g�n
	void SetAppname(char *appname)	{ m_appname	= appname;}
	void SetVersion(double appver)	{ m_appver	= appver;}
	void SetInst(HINSTANCE hInst)	{ m_hInst	= hInst;}
	double	GetVersion(){return m_appver;}
	HWND GetWindow(){return m_hWnd;}
	HINSTANCE GetInst(){return m_hInst;}
	void SetVsync(int vsync){ m_vsync = vsync; }
//�����g�p�֐�
	int InitMember();			///<�����o�ϐ�������
	int DumpMessage();			///<���b�Z�[�W��S�Ď̂Ă�
	int CheckMutex();			///<�Q�d�N���h�~�p
	int GetAppInformation();	///<�A�v�����i�r���������o�j�o��
	int LoadConfig();			///<�R���t�B�O�ǂݍ���
};

extern CSunApp SunApp;