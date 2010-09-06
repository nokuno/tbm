/**
	@file SunFile.h
	@brief SunFile �N���X�̃C���^�[�t�F�C�X
*/

#pragma once

// ������SunFileBase���g���֐��Q
DWORD GetFileSize(char* pFileName);
int ReadFile(char* pFileName,void* pBuffer,DWORD dwSize);
int WriteFile(char* pFileName,void* pBuffer,DWORD dwSize);
/**
	@brief �t�@�C���N���X��b

	�Ȃɂ��Ɩ��ʂ̑����r�c�j�̃t�@�C���֐���P���Ƀ��b�v�������́B
	�{���͂l�e�b���v�s�k����Y��������������Ă����΂����񂾂��ǁB�B
	
	����𒼐ڎg���p�r�́A�X�R�A�t�@�C���⃊�v���C�t�@�C���i�������݂��K�v�Ȃ��́j���炢�B
*/
class SunFileBase
{
protected:
	HANDLE	m_hFile;	
public:
	SunFileBase();
	virtual ~SunFileBase();
	int OpenFileReadWrite(char* pFileName);
	int OpenFileRead(char* pFileName);
	int OpenFileWrite(char* pFileName);
	int CloseFile();
	DWORD GetFileSize();
	DWORD ReadFile(void* pBuffer,DWORD dwSize);
	DWORD WriteFile(void* pBuffer,DWORD dwSize);
	DWORD SeekFile(LONG lDistanceToMove,DWORD dwMoveMethod);
};
/**
	@brief �t�@�C���N���X

	�o�C�i���t�@�C���̓ǂݍ��݂��s���N���X�ł��B
	�������݂͂ł��܂���B
	�k�o�`�b�j�ɂ��p�b�N�t�@�C�����ǂ߂܂��B
	�i���ڃt�@�C�����Ȃ���΃p�b�N�t�@�C������ǂݍ��ށj
	�k�y�r�r���k�ɂ��Ή��E�E���Ȃ������B�i��
	�Ȃ��A���N���X�̃����o�ɊO������A�N�Z�X���Ă͂����܂���B
*/
class SunFile  : public SunFileBase
{
protected:
	DWORD	m_dwStart;		///< �t�@�C���J�n�ʒu
	DWORD	m_dwSize;		///< �t�@�C���I���ʒu
	char	m_strPack[MAX_PATH];
	char	m_strFile[MAX_PATH];
public:
	SunFile();
	virtual ~SunFile();
	int OpenFile( char *pFile );						///< �t�@�C�����J��
	int OpenFileFromPack( char *pFile, char *pPack );	///< �p�b�N�t�@�C������J��
	int OpenFileFromDirectory( char *pFile);			///< �f�B���N�g������J��
	int FileRead(void *pData, DWORD dwSize);			///< �t�@�C���ǂݍ���
	int GetSize(){return (int)m_dwSize;}				///< �T�C�Y�擾
	HANDLE GetHandle(){return m_hFile;}	///<�X�g���[�~���O�p�ɃV�[�N�ς݃t�@�C���n���h���擾
	DWORD GetStart(){return m_dwStart;}	///<�X�g���[�~���O�p
	char	*GetPack(){return m_strPack;}
	char	*GetFile(){return m_strFile;}
};
/**
	@brief �t�@�C���f�[�^�N���X

	SunFile�N���X�̓t�@�C���̓ǂݍ��ݐ�̃o�b�t�@�͎��삵�ēn���K�v������A
	���̂Ƃ���void�|�C���^���g���K�v������댯�ł��B
	���̃N���X�̓t�@�C���̓Ǎ���ł���f�[�^�܂ŊǗ����܂��B
	�ǂݍ��݂̓t�@�C���S�́i�p�b�N�t�@�C���Ȃ�ړI�̃t�@�C���̂݁j�ł��B
	�������������݂͂ł��܂���B
	�Ȃ��A���N���X�̃����o�ɊO������A�N�Z�X���Ă͂����܂���B
*/
class SunFileEx : protected SunFile
{
protected:
	void* m_pBuffer;	///<�t�@�C���ǂݍ��݃o�b�t�@
public:
	SunFileEx();
	virtual ~SunFileEx();
	int Free();								///<�o�b�t�@���
	int Read();								///<�S�ǂݍ���
	void* GetBuffer(){return m_pBuffer;}	///<�o�b�t�@�|�C���^�擾
	int GetSize(){return (int)m_dwSize;}	///< �T�C�Y�擾
	int Open(char* pFile){return OpenFileFromDirectory(pFile);}
};