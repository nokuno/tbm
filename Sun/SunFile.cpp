// SunFile.cpp: SunFile �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunFile.h"
#include "SunExeption.h"
#include "SunUtility.h"
/**
	@brief �t�@�C���ǂݍ���	

	static�֐�
*/
int ReadFile(char* pFileName,void* pBuffer,DWORD dwSize)
{
	SunFileBase file;
	if( file.OpenFileRead(pFileName) == 0)
		return 0;
	return file.ReadFile(pBuffer,dwSize);
}
/**
	@brief �t�@�C����������

	static�֐�
*/
int WriteFile(char* pFileName,void* pBuffer,DWORD dwSize)
{
	SunFileBase file;
	if( file.OpenFileWrite(pFileName) == 0)
		return 0;
	return file.WriteFile(pBuffer,dwSize);
}
/**
	@brief �t�@�C���T�C�Y�擾

	static�֐�
*/
DWORD GetFileSize(char* pFileName)
{
	SunFileBase file;
	if( file.OpenFileRead(pFileName) == 0)
		return 0;
	return file.GetFileSize();
}
/**
	@class SunFileBase
*/
SunFileBase::SunFileBase()
{
	m_hFile = INVALID_HANDLE_VALUE;
}

SunFileBase::~SunFileBase()
{
	this->CloseFile();
}
/**
	@brief �t�@�C����ǂݏ����\�ɂ��ĊJ���܂��B

	�t�@�C�������݂��Ȃ��Ƃ��̂ݍ쐬���܂��B
*/
int SunFileBase::OpenFileReadWrite(char* pFileName)
{
	m_hFile = ::CreateFile(
		pFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	return (m_hFile!=INVALID_HANDLE_VALUE);
}

/**
	@brief �t�@�C����ǂݍ��݉\�ɂ��ĊJ���܂��B

	�t�@�C�������݂��Ȃ��Ǝ��s���܂��B
*/
int SunFileBase::OpenFileRead(char* pFileName)
{
	m_hFile = ::CreateFile(
		pFileName, GENERIC_READ,
		FILE_SHARE_READ,
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	return (m_hFile!=INVALID_HANDLE_VALUE);
}
/**
	@brief �t�@�C�����������݉\�ɂ��ĊJ���܂��B

	��Ƀt�@�C�����쐬���܂��B
	�t�@�C�������łɑ��݂��Ă���Ə㏑�����܂��B
*/
int SunFileBase::OpenFileWrite(char* pFileName)
{
	m_hFile = ::CreateFile(
		pFileName, GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL );
	return (m_hFile!=INVALID_HANDLE_VALUE);
}
/**
	@brief �t�@�C������܂��B
*/
int SunFileBase::CloseFile()
{
	if(m_hFile != INVALID_HANDLE_VALUE)
		::CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	return 1;
}
/**
	@brief �t�@�C���T�C�Y���擾���܂��B
*/
DWORD SunFileBase::GetFileSize()
{
	return ::GetFileSize(m_hFile,NULL);
}
/**
	@brief �t�@�C����ǂ݂܂��B
*/
DWORD SunFileBase::ReadFile(void* pBuffer,DWORD dwSize)
{
	if ( m_hFile == INVALID_HANDLE_VALUE )
		return 0;
	DWORD dwReadBytes;
	::ReadFile( m_hFile, pBuffer, dwSize, &dwReadBytes, NULL);
	return dwReadBytes;
}
/**
	@brief �t�@�C���ɏ����܂��B
*/
DWORD SunFileBase::WriteFile(void* pBuffer,DWORD dwSize)
{
	if ( m_hFile == INVALID_HANDLE_VALUE )
		return 0;
	DWORD dwWriteBytes;
	::WriteFile( m_hFile, pBuffer, dwSize, &dwWriteBytes, NULL);
	return dwWriteBytes;
}
/**
	�V�[�N�i�t�@�C���|�C���^�̈ړ��j
*/
DWORD SunFileBase::SeekFile(LONG lDistanceToMove,DWORD dwMoveMethod)
{
	return ::SetFilePointer(m_hFile,lDistanceToMove,NULL,dwMoveMethod);
}
/**
	@brief	�t�@�C���f�[�^�w�b�_

	�t�@�C�����e�Ɋւ���f�[�^���i�[���Ă���w�b�_
*/
struct FILE_DATA
{
	char FileName[64];			///< �t�@�C����
	unsigned long Address;		///< �t�@�C���J�n�ʒu
	unsigned long Size;			///< �t�@�C���T�C�Y
};

/**
	@brief	�o�C�i���f�[�^�w�b�_

	�o�C�i���f�[�^�Ɋւ���f�[�^���i�[���Ă���w�b�_
*/
struct BINARY_HEADER
{
	char Guid[4];				///< 'P' 'A' 'C' 'K'
	unsigned long FileCount;	///< �t�@�C����
};

/**
	@class SunFile
*/
// �\�z/����

SunFile::SunFile()
{
	m_dwStart = 0;
	m_dwSize = 0;
	m_strPack[0] = '\0';
	m_strFile[0] = '\0';
}

SunFile::~SunFile()
{
}

/**
	�w��t�@�C���̃t�@�C���n���h�����擾���܂��B<BR>
	�擾�����t�@�C���n���h���͓ǂݍ��݁E�������݂��ł��܂��B

	@param pFile	[in] �t�@�C����
*/
int SunFile::OpenFile( char *pFile )
{
	if(pFile == NULL)
		return 0;
	this->CloseFile();
	// �t�@�C���I�[�v��
	if( this->OpenFileRead( pFile ) == 0)
		return 0;
	// �p�b�N�t�@�C���ł͂Ȃ��̂ŃA�h���X�͂O
	m_dwStart = 0;

	// �t�@�C���T�C�Y�擾
	m_dwSize = this->GetFileSize();

	return 1;
}

/**
	�p�b�N�t�@�C�����̎w��t�@�C���̃t�@�C���n���h�����擾���܂��B<BR>
	�擾�����t�@�C���n���h���͓ǂݍ��݁E�������݂��ł��܂��B

	@param pFile	[in] �t�@�C����
	@param pPack	[in] �p�b�N�t�@�C����

*/
SunFile::OpenFileFromPack( char *pFile, char *pPack )
{
	BINARY_HEADER BinHead;	//�t�@�C���w�b�_�\����
	FILE_DATA FileData;		//�t�@�C���f�[�^�\����
	unsigned long i;
	//-----------------------------------------------------------
	// �t�@�C���I�[�v��
	//-----------------------------------------------------------
	if( this->OpenFile(pPack) == 0)
		return 0;

	//-----------------------------------------------------------
	// �w�b�_�ǂݍ���
	//-----------------------------------------------------------
	this->ReadFile(&BinHead, sizeof(BINARY_HEADER) );

	//-----------------------------------------------------------
	// �w�b�_�`�F�b�N
	//-----------------------------------------------------------
	if ( BinHead.Guid[0] != 'P' ) return 0;
	if ( BinHead.Guid[1] != 'A' ) return 0;
	if ( BinHead.Guid[2] != 'C' ) return 0;
	if ( BinHead.Guid[3] != 'K' ) return 0;

	//-----------------------------------------------------------
	// �Y���t�@�C������
	//-----------------------------------------------------------
	for ( i = 0; i < BinHead.FileCount; i++ )
	{
		// �t�@�C���f�[�^�ǂݍ���
		this->ReadFile(&FileData, sizeof(FILE_DATA) );
		// �t�@�C�����`�F�b�N�i�啶���E����������ʂ��܂���j
		if ( !stricmp( FileData.FileName, pFile ) )
		{
			// �Y���A�h���X�܂ňړ�
			this->SeekFile( FileData.Address, FILE_BEGIN );

			// �t�@�C���f�[�^
			m_dwStart = FileData.Address;
			m_dwSize = FileData.Size;

			return 1;
		}
	}

	return 0;
}
/**
	�f�B���N�g�����̃t�@�C�����w�肵�āA���݂��Ȃ�������f�B���N�g�����Ɠ������O�̃p�b�N�t�@�C������T���܂��B
	�擾�����t�@�C���n���h���͓ǂݍ��݁E�������݂��ł��܂��B
	
	@param pFile	[in] �t�@�C����
	@param pPack	[in] �p�b�N�t�@�C����

*/
int SunFile::OpenFileFromDirectory( char *pFile)
{
	//�t���p�X�������肵�āE�E�i���j
	//����ADirectMusic�̃R�[���o�b�N�֐��̊֌W�ŕK�v�Ȃ�ł�
	string current = GetCurrent();
	if(strstr(pFile,current.c_str())!=NULL)
	{
		//���΃p�X��n��
		return this->OpenFileFromDirectory( &pFile[strlen(current.c_str())+1]);
	}
	//���ʂ̃t�@�C��������
	if( this->OpenFile(pFile) == 0)
	{
		char* ptr;
		//�f�B���N�g�����ƃt�@�C�����̋���
		{
			ptr = strchr(pFile,'\\');
			if(ptr == NULL )
				ptr = strchr(pFile,'/');
			if(ptr == NULL || *ptr == '\0')	
				return 0;
		}
		//�f�B���N�g������؂�o��
		{
			size_t size = abs(ptr - pFile);
			if(size > 255)
				return 0;
			strncpy(m_strPack,pFile,size);
			strcpy(&m_strPack[size],".bin");
		}
		//�t�@�C������؂�o��
		{
			ptr++;
			if(ptr == NULL || *ptr == '\0')	
				return 0;
			strcpy(m_strFile,ptr);
		}
		if( this->OpenFileFromPack(m_strFile,m_strPack) == 0)
			return 0;
	}
	else {
		strcpy( m_strFile, pFile );
	}
	return 1;
}
/**
	�I�[�v�������t�@�C���ւ̓ǂݍ��ݏ������s���܂��B
	�o�b�t�@�T�C�Y�ȏ�̓ǂݍ��݂��s�����Ƃ����ꍇ�A
	�ǂݍ��݂͍s���܂���B

	@param Size		[in] �ǂݍ��ݍ��ރf�[�^�T�C�Y
	@param pData	[in] �ǂݍ��ރf�[�^�̃|�C���^
*/
int SunFile::FileRead(void *pData, DWORD dwSize)
{
	if(m_hFile == INVALID_HANDLE_VALUE)
		return 0;

	// �t�@�C���̌��݈ʒu���擾
	unsigned long Pos = SeekFile( 0, FILE_CURRENT );

	// �I�[�o�[�`�F�b�N
	if ( Pos + dwSize > m_dwStart + m_dwSize )
		return 0;

	// �t�@�C���ǂݍ���
	if( this->ReadFile( pData, dwSize ) == 0)
		return 0;

	return 1;
}

/**
	@class SunFileEx
*/
// �\�z/����

SunFileEx::SunFileEx()
{
	m_pBuffer = NULL;
}

SunFileEx::~SunFileEx()
{
	this->Free();
}
/**
	���
*/
int SunFileEx::Free()
{
	if(m_pBuffer != NULL)
		free(m_pBuffer);
	m_pBuffer = NULL;
	return 1;
}
/**
	�ǂݍ���
*/
int SunFileEx::Read()
{
	this->Free();
	m_pBuffer = malloc(m_dwSize);
	if(this->ReadFile(this->m_pBuffer,this->m_dwSize) == 0)
		return 0;;
	return 1;
}