/**
	@file ThkgmMain.cpp
	@brief ���C���t�@�C��
*/
#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"
#include "Title.h"

inline int SunInit();
inline int SunExit();

CScene *g_pScene = NULL;

/**
	@brief ���C���֐�

	WinMain�ł��B�����ɒ��ڃ��[�v�������Ă����܂��B


*/
int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int nCmdShow )
{
	//�A�v���P�[�V�����������Z�b�g
	SunApp.SetInst(hInst);
	SunApp.SetAppname("�Ƃт܂肳");	//�A�v����
	SunApp.SetVersion(1.00); 			//�o�[�W����
	SunInput.UseJoystick(1);
	SunInput.UseKeyboard(1);
	SunInput.UseMouse(0);
	SunRand2.Randomize();

	//��O�����������ꍇ�A�O���[�o���ϐ���������ďI�����܂��B
	//��O��SunTask�̃C���^�[�t�F�[�X�ł��B
	try{
		//���C�u����������
		if(!SunInit())
			return 0;

		//���[�U�[�I�u�W�F�N�g������
		g_pTitle = new CTitle();
		g_pScene = g_pTitle;
		SunSoundBuffer sndCamera("snd/camera.wav");

		//���C�����[�v
		while(1) {
	
			//FPS�ϑ�
			if(!SunApp.ObserveFPS())
				break;
			
			//�V�[���J�n
			if(!Sun3D.BeginScene())
				break;

			//�`��
			if(Sun3D.GetUpdate()) {
				if(!g_pScene->Draw())
					break;
			}
			//���s���`��
			if(!g_pScene->Process())
				break;

			//�X�N���[���V���b�g
			if(SunInput.GetInputEvent( INP_B5 )) {
				if( g_pTitle == NULL || g_pTitle->mode != 7 ) {
					sndCamera.Play(0);
					SaveSnapShot();
				}
			}

			//�V�[���I��
			if(!Sun3D.EndScene())
				break;

			//����
			if(!g_pScene->Run())
				break;

			//���{��
			for( int i = 0; i < 10; i++ ) {
				if( SunInput.GetKeyState( DIK_F1+i ) & 0x80 )
					SunApp.m_fps = 60 * (i+1);
			}

			//�`��Ԋu
			for( int i = 0; i < 10; i++ ) {
				if( SunInput.GetKeyState( DIK_1+i ) & 0x80 ) {
					Sun3D.m_skip = 2;
					Sun3D.m_interval = i+1;
				}
			}

			//�v���[���e�[�V����
			if(!Sun3D.Present())
				break;

			//�t���[������
			if(!SunApp.WaitTime())
				break;

			//���̓f�o�C�X�X�V
			if(!SunInput.UpdateInput())
				break;
		}

		sndCamera.Release();
		SAFE_DELETE(g_pGame);
		SAFE_DELETE(g_pTitle);
		SunExit();

		//�ċN��
		if( SunApp.GetRestart() ) {
			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			ZeroMemory(&si,sizeof(si));
			si.cb=sizeof(si);
			char buffer[MAX_PATH];
			::GetModuleFileName( NULL, buffer, MAX_PATH );
			CreateProcess(NULL,(LPTSTR)buffer,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi);
		}
	}
	catch(SunExeption exeption){
		exeption();
		return 1;
	}
	return 0;
}

//�������⏕�֐�
inline int SunInit()
{
	//�J�����g�̐ݒ�
	ResetCurrent();

	//���O�t�@�C���̏�����
	SunLog.LogStart();

	//���O�t�@�C���o��
	SunLog.GetAllInformation();

	//�R���t�B�O�t�@�C���̃o�[�W�����m�F
	//�J�����g�f�B���N�g���ɍ��E����邱�Ƃɒ���
	SunConfig.Read();

	//SunApp������
	if(!SunApp.Init())
		return 0;

	//Sun3D������
	if(!Sun3D.Init())
		return 0;

	//SunInput������
	if(!SunInput.ReInit())
		return 0;

	//SunSound������
	SunSound.Init();

	return 1;
}
//�I���֐�
inline int SunExit()
{
	SunSound.Release();
	SunInput.Release();
	Sun3D.Exit();
	SunApp.Exit();
	return 1;
}