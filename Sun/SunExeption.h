// SunExeption.h: SunExeption �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

/**
	@brief ��O�N���X

	��O�N���X�ł��B
	���O�t�@�C�����o�͂��A���b�Z�[�W�{�b�N�X��\�����܂��B
*/
class SunExeption
{
private:
	string m_str;	///<�G���[������
public:
	SunExeption(const char* fmt,...);
	virtual ~SunExeption();
	int operator()();
};

