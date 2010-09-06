/**
	@file SunUtility.cpp
	@brief ���[�e�B���e�B
*/
#include "stdafx.h"
#include "SunUtility.h"

/**
	@brief �J�����g�f�B���N�g��

	�J�����g���擾���܂��B
	���s�t�@�C���̈ʒu�ɃJ�����g��ݒ肵�A�ݒ肵���J�����g��Ԃ��܂��B
*/
string GetCurrent()
{
	char buffer[MAX_PATH];
	::GetModuleFileName( NULL, buffer, MAX_PATH );
	char* p = strrchr(buffer,'\\');
	if(p != NULL)
		p[0] = '\0';
	::SetCurrentDirectory(buffer);
	return (string)buffer;
}

/**
	@brief �J�����g�f�B���N�g��

	�J�����g�����s�C���[�W�̈ʒu�ɐݒ肵�܂��B
*/
void ResetCurrent()
{
	char buffer[MAX_PATH];
	::GetModuleFileName( NULL, buffer, MAX_PATH );
	char* p = strrchr(buffer,'\\');
	if(p != NULL)
		p[0] = '\0';
	::SetCurrentDirectory(buffer);
}
/*
	�⏕�֐�
*/
DWORD   GetMouseParam(int nXY,bool bX)
{
	int		nSize;

	if(bX == true)
		nSize = ::GetSystemMetrics(SM_CXSCREEN);
	else
		nSize = ::GetSystemMetrics(SM_CYSCREEN);

	return  nXY * (65535 / nSize);
}

/*
	�}�E�X�J�[�\���̈ʒu��ݒ肵�܂��B
*/
void	MoveMouse(int nX,int nY)
{
	nX = GetMouseParam(nX,true);
	nY = GetMouseParam(nY,false);

	::mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE,nX,nY,NULL,NULL);
}

//��`���m�̓����蔻��
int GetHantei(RECT *r1,RECT *r2)
{
	if( r1->left > r2->right ||
		r1->right < r2->left ||
		r1->top > r2->bottom ||
		r1->bottom < r2->top ) {
		return 0;
	}
	return 1;
}
//�t�H���_�쐬�i�X�i�b�v�V���b�g�E���v���C�p�j
int CreateDirectory( char* name )
{
	int result = CreateDirectory(name,NULL);
	SetFileAttributes( name, FILE_ATTRIBUTE_NORMAL );
	return result;
}
int pown( int x, int n ){
	int result = 1;
	for( int i = 0; i < n; ++ i ) {
		result *= x;
	}
	return result;
}
