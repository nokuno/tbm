/**
	@file SunLog.h
	@brief CSunLog �N���X�̃C���^�[�t�F�C�X
*/

#pragma once

/**
	@class CSunLog
	@brief ���O�t�@�C���o�̓N���X

	���O�o�̓��[�e�B���e�B�Ƃ��Ă̋@�\�ƁA
	�b�o�t���Ȃǂ̋�̓I�ȃ��O�o�͂������o�Ȃ��łł���͈͂ŃJ�o�[���܂��B
	���Ƃ��Ƃr�����N���X�ɓ����Ă��̂ł���
	���ꂵ�������̂ŕ������܂�����
	���̃N���X�͂ق��̂ǂ̃N���X�ɂ��ˑ����܂���(SunUtility�ɂ�)�B
*/
class CSunLog  
{
private:
	string	m_logfilename;	//���O�t�@�C����
public:
//��{�@�\
	CSunLog();
	CSunLog(const char* file);
	virtual ~CSunLog();
	int LogStart();						//stderr�̃��_�C���N�g
	int LogOut(const char *fmt,...);	//printf�����O�t�@�C���o��
	int LogCaps(int i,char *text);
	int operator()(const char *fmt,...);
//�����g�p�֐�
//�ȉ��̏�񃍃O�֌W�̊֐��͂قڂk�������̃R�s�y
	int GetAllInformation();
	int GetCPUInformation();
	int GetOSInformation();
	int GetMemoryInformation();
	int GetSystemInformation();
};

extern CSunLog		SunLog;
