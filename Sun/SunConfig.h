/**
	@file SunConfig.h
	@brief CSunConfig �N���X�̃C���^�[�t�F�C�X
*/

#pragma once

/**
	@class CSunConfig
	@brief �R���t�B�O�N���X

	�R���t�B�O���[�e�B���e�B�B�R���t�B�O���̂��̂ł͂Ȃ��B
	�t�@�C������Z�N�V�������������o���g���ďȗ�����
*/
class CSunConfig  
{
protected:
//�����o�ϐ��i�v���p�e�B�j
	string	m_file;		//�R���t�B�O�t�@�C����
	string	m_section;	//�R���t�B�O�Z�N�V������
public:
//�\�z�Ə���
	CSunConfig();
	virtual ~CSunConfig();
//�Z�b�g
	void SetFile(char *file){m_file = file;}
	void SetSection(char *section){m_section = section;}
//�����o�֐�
//�Z�N�V�����Œ�
	string	GetString( const char *key);
	int		GetInt(const char *key);
	double	GetDouble(const char *key);
	int		SetString(const char *key,const char *str);
	int		SetInt(const char *key,int value);
	int		SetDouble(const char *key,double value);
//�t�@�C���Œ�
	string	GetString(const char *section, const char *key);
	int		GetInt(const char *section, const char *key);
	double	GetDouble(const char *section, const char *key);
	int		SetString(const char *section, const char *key,const char *str);
	int		SetInt(const char *section, const char *key,int value);
	int		SetDouble(const char *section, const char *key,double value);
//���S���R
	string	GetString(const char *file,const char *section, const char *key);
	int		GetInt(const char *file,const char *section, const char *key);
	double	GetDouble(const char *file,const char *section, const char *key);
	int		SetString(const char *file,const char *section, const char *key,const char *str);
	int		SetInt(const char *file,const char *section, const char *key,int value);
	int		SetDouble(const char *file,const char *section, const char *key,double value);
};
/**
	���p�I�ȃR���t�B�O���[�e�B���e�B
	�w�肵���L�[�����݂��Ȃ��ꍇ�A�ǂݍ��݊֐��Ńf�t�H���g�l���������ށB
*/
class CSunConfig2 : public CSunConfig
{
protected:
	double	m_version;	//�R���t�B�O�t�@�C���̃o�[�W����
public:
//�\�z�Ə���
	CSunConfig2();
	virtual ~CSunConfig2();
//�Q�b�g
	double	GetVersion(){return m_version;}
//�ǂݍ��݁E��������
	int		Read();		//�o�[�W��������ǂݎ��
	int		Write();	//�o�[�W���������������ށE���ʂ̓f�X�g���N�^���Ă�ł����
//�Z�N�V�����Œ�
	string	GetString2(const char *key,const char *def);
	int		GetInt2(const char *key,const int def);
//�t�@�C���Œ�
	string	GetString2(const char *section, const char *key,const char *def);
	int		GetInt2(const char *section, const char *key,const int def);
//���S���R
	string	GetString2(const char *file,const char *section, const char *key,const char *def);
	int		GetInt2(const char *file,const char *section, const char *key,const int def);
};
extern CSunConfig2 SunConfig;

/**
	�I�v�V�����N���X

	�P��int�^�̃I�v�V�����������B�R���t�B�O��ʗp�B
	�ǂݍ��ݎ��̒l��ۑ����Ă����A�X�V����Ă��邩�ǂ������擾�ł���B
*/
class SunOption
{
public:
	int m_old;	//�����l
	int m_now;	//�ݒ�l
	string m_section;	//
	string m_key;		//
public:
	SunOption();
	~SunOption();
	int Load(char* section,char* key,int def);	//���[�h
	int Save();							//�Z�[�u�i���[�h�������Ɓj
	int GetUpdate(){return m_old!=m_now;}
};