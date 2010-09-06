// SunConfig.cpp: CSunConfig �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunUtility.h"
#include "SunApp.h"

CSunConfig2 SunConfig;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CSunConfig::CSunConfig()
{
	m_file		= "config.ini";
	m_section	= "system";
}

CSunConfig::~CSunConfig()
{

}

/*-----------------------------------------------------------------------------
	�Œ�i�V�̎��R�ȃv���t�@�C���ǂݏ���
-----------------------------------------------------------------------------*/
//������ǂݍ���
string	CSunConfig::GetString(const char *file,const char *section, const char *key)
{
	m_file		= file;
	m_section	= section;
	return this->GetString(key);
}
//�����ǂݍ���
int	CSunConfig::GetInt(const char *file,const char *section, const char *key)
{
	m_file		= file;
	m_section	= section;
	return this->GetInt(key);
}
//�����ǂݍ���
double	CSunConfig::GetDouble(const char *file,const char *section, const char *key)
{
	m_file		= file;
	m_section	= section;
	return this->GetDouble(key);
}
//�����񏑂�����
int	CSunConfig::SetString(const char *file,const char *section, const char *key,const char *str)
{
	m_file		= file;
	m_section	= section;
	return this->SetString(key,str);
}
//������������
int	CSunConfig::SetInt(const char *file,const char *section, const char *key,int value)
{
	m_file		= file;
	m_section	= section;
	return this->SetInt(key,value);
}
//������������
int	CSunConfig::SetDouble(const char *file,const char *section, const char *key,double value)
{
	m_file		= file;
	m_section	= section;
	return this->SetDouble(key,value);
}

/*-----------------------------------------------------------------------------
	�t�@�C���Œ�v���t�@�C���ǂݏ���
	�����o���g���Ĉ����ȗ�
-----------------------------------------------------------------------------*/
string	CSunConfig::GetString(const char *section, const char *key)
{
	m_section = section;
	return this->GetString(key);
}
int		CSunConfig::GetInt(const char *section, const char *key)
{
	m_section = section;
	return this->GetInt(key);
}
double	CSunConfig::GetDouble(const char *section, const char *key)
{
	m_section = section;
	return this->GetDouble(key);
}
int		CSunConfig::SetString(const char *section, const char *key,const char *str)
{
	m_section = section;
	return this->SetString(key,str);
}
int		CSunConfig::SetInt(const char *section, const char *key,int value)
{
	m_section = section;
	return this->SetInt(key,value);
}
int		CSunConfig::SetDouble(const char *section, const char *key,double value)
{
	m_section = section;
	return this->SetDouble(key,value);
}
/*-----------------------------------------------------------------------------
	�Z�N�V�����Œ�v���t�@�C���ǂݏ���
	�����o���g���Ĉ����ȗ�
-----------------------------------------------------------------------------*/
//������ǂݍ��݁i�f�t�H���g�l��"DEFAULT")
string	CSunConfig::GetString(const char *key)
{
	string m_filepath = GetCurrent() + "\\" + m_file;
	char buffer[512];
	GetPrivateProfileString(m_section.data(), key, "DEFAULT", buffer, 512, m_filepath.data());
	return (string)buffer;
}
//�����ǂݍ��݁i�f�t�H���g�l��-100)
int	CSunConfig::GetInt(const char *key)
{
	string m_filepath = GetCurrent() + "\\" + m_file;
	return GetPrivateProfileInt(m_section.data(), key, -100, m_filepath.data());
}
//�����ǂݍ��݁i�f�t�H���g�l��-100)
double	CSunConfig::GetDouble(const char *key)
{
	string m_filepath = GetCurrent() + "\\" + m_file;
	char buffer[512];
	GetPrivateProfileString(m_section.data(), key, "DEFAULT", buffer, 512, m_filepath.data());
	double ret;
	if((string)buffer == "DEFAULT")
		ret = -100;
	else
		ret = atof(buffer);
	return ret;
}
//�����񏑂�����
int	CSunConfig::SetString(const char *key,const char *str)
{
	string m_filepath = GetCurrent() + "\\" + m_file;
	WritePrivateProfileString(m_section.data(), key, str, m_filepath.data());
	return 1;
}
//������������
int	CSunConfig::SetInt(const char *key,int value)
{
	string m_filepath = GetCurrent() + "\\" + m_file;
	char buffer[256];
	itoa(value,buffer,10);
	WritePrivateProfileString(m_section.data(), key, buffer, m_filepath.data());
	return 1;
}
//������������
int	CSunConfig::SetDouble(const char *key,double value)
{
	string m_filepath = GetCurrent() + "\\" + m_file;
	char buffer[256];
	sprintf(buffer,"%f",value);
	WritePrivateProfileString(m_section.data(), key, buffer, m_filepath.data());
	return 1;
}


//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CSunConfig2::CSunConfig2()
{
	m_version = 0;
}

CSunConfig2::~CSunConfig2()
{
	this->Write();
}

//�o�[�W�����ǂݍ���
int	CSunConfig2::Read()
{
	m_section = "system";
	m_version = this->GetDouble(m_file.data(),m_section.data(),"version");
	SunLog.LogOut("�R���t�B�O�t�@�C�����\n");
	SunLog.LogOut("	filename:[%s] version:[%g]\n",m_file.data(),m_version);
	//�R���t�B�O�Ȃ�
	if(m_version == 0)
	{
		SunLog.LogOut("�R���t�B�O�t�@�C��������܂���B�f�t�H���g�ŏ��������܂��B\n");
		return 0;
	}
	//�o�[�W�������Â��i���O�o�������j
	else if(m_version < SunApp.GetVersion())
	{
		SunLog.LogOut("�R���t�B�O�t�@�C���̃o�[�W�������Â��ł��B\n");
	}
	return 1;
}
//�o�[�W������������
int	CSunConfig2::Write()
{
	m_section = "system";
	m_version = SunApp.GetVersion();	//�ŐV�łɍX�V
	this->CSunConfig::SetDouble(m_file.data(),m_section.data(),"version",m_version);
	return 1;
}
//�Z�N�V�����Œ�
string	CSunConfig2::GetString2(const char *key,const char *def)
{
	string ret;
	if(m_version == 0)
	{
		ret = def;
	}
	else
	{
		ret = CSunConfig::GetString(key);
		if(ret == "DEFAULT")
			ret = def;
	}
	this->SetString(key,ret.data());
	SunLog.LogOut("	section[%s] key[%s]=[%s]\n",m_section.data(),key,ret.data());
	return ret;
}
int		CSunConfig2::GetInt2(const char *key,const int def)
{
	int ret;
	if(m_version == 0)
	{
		ret = def;
	}
	else
	{
		ret = CSunConfig::GetInt(key);
		if(ret == -100)
			ret = def;
	}
	SunLog.LogOut("	section[%s] key[%s]=[%d]\n",m_section.data(),key,ret);
	this->SetInt(key,ret);
	return ret;
}
//�t�@�C���Œ�
string	CSunConfig2::GetString2(const char *section, const char *key,const char *def)
{
	m_section = section;
	return this->GetString2(key,def);
}
int		CSunConfig2::GetInt2(const char *section, const char *key,const int def)
{
	m_section = section;
	return this->GetInt2(key,def);
}
//���S���R
string	CSunConfig2::GetString2(const char *file,const char *section, const char *key,const char *def)
{
	m_section = section;
	m_file = file;
	return this->GetString2(key,def);
}
int		CSunConfig2::GetInt2(const char *file,const char *section, const char *key,const int def)
{
	m_file = file;
	m_section = section;
	return this->GetInt2(key,def);
}
/**
	�I�v�V�����N���X
*/
SunOption::SunOption()
{
	m_old=0;
	m_now=0;
}
SunOption::~SunOption()
{
}
int SunOption::Load(char* section,char* key,int def)
{
	m_section=section;
	m_key=key;
	m_old = SunConfig.GetInt2(m_section.c_str(),m_key.c_str(),def);
	m_now = m_old;
	return m_old;
}
int SunOption::Save()
{
	if( m_old == m_now )
		return 0;
	SunConfig.SetInt(m_section.c_str(),m_key.c_str(),m_now);
	m_old = m_now;
	return 1;
}