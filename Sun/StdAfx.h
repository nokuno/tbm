/**
	@file StdAfx.h
	@brief �v���R���p�C���w�b�_

	�W���̃V�X�e�� �C���N���[�h �t�@�C���A
	�܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
	�v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B

	@todo �v���O�����ŕK�v�ȃw�b�_�[�Q�Ƃ�ǉ����Ă��������B
**/
#pragma once

#define WIN32_LEAN_AND_MEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�
#define STRICT

//�v�h�m�R�Q�`�o�h
#include <windows.h>
#include <windowsx.h>
#include <wingdi.h>
#include <winnls32.h>
#include <process.h>
#include <mmsystem.h>
//DirectX8
#define DIRECTINPUT_VERSION 0x0800
#include <d3d9.h>
#include <d3dx9.h>

#include <dinput.h>
#include <dsound.h>
#include <dmusici.h>

//�W�����C�u����
#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <mbstring.h>
#include <tchar.h>

//�r�s�k
#include <list>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
using namespace std;
//�a�n�n�r�s
//#include <boost/shared_ptr.hpp> 
//using namespace boost;
//�`�s�k�E�v�s�k
#include <atlbase.h>
/*
#define WINVER		0x0400
#define _WIN32_IE	0x0400
#define _RICHEDIT_VER	0x0100
#include <atlbase.h>
#include <atlapp.h>
extern CAppModule _Module;
#include <atlwin.h>
*/
//���������[�N�`�F�b�N�p
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define new  ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//���C�u�����̖����I�����N
//#pragma comment(lib,"d3d9")
#pragma comment(lib,"d3dx9")
#pragma comment(lib,"dxguid")
#pragma comment(lib,"winmm")
#pragma comment(lib,"dinput8")
#pragma comment(lib,"dsound")

//double->float�ϊ��x�����O��
#pragma warning( disable : 4305 )
