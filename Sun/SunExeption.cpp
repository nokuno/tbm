// SunExeption.cpp: SunExeption �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunExeption.h"
#include "SunLog.h"

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SunExeption::SunExeption(const char* fmt,...)
{
	char str[512];	//���߂�����
	va_list arg;
	va_start(arg,fmt);
	vsprintf(str, fmt, arg);
	va_end(arg);

	m_str = str;
}

SunExeption::~SunExeption()
{

}

int SunExeption::operator ()()
{
	SunLog.LogOut(m_str.c_str());
	::MessageBox(NULL,m_str.c_str(),"�G���[",MB_OK | MB_ICONERROR);
	return 1;
}