// SunSound.cpp: SunSound �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunLog.h"
#include "SunConfig.h"
#include "SunUtility.h"
#include "SunExeption.h"
#include "SunFile.h"
#include "SunApp.h"
#include "SunSound.h"
#include "Ayame.h"


CSunSound	SunSound;
CSunAyame	SunAyame;

#define BGM_OGG	2
#define BGM_AYM 3

static int bgm_mode = BGM_AYM;

//���b�p�֐�
void PlayBGM(char *pFile){
	char buffer[MAX_PATH];
	switch( bgm_mode ) {
	case BGM_OGG:
		sprintf( buffer, "ogg\\%s.ogg", pFile );
		SunAyame.Play( buffer );
		break;
	case BGM_AYM:
		sprintf( buffer, "bgm\\%s.wav", pFile );
		SunAyame.Play( buffer );
		break;
	}
}
void ResumeBGM(){
//	SunAyame.Resume();
}
void StopBGM(){
	SunAyame.Stop();
}
void PauseBGM(){
//	SunAyame.Pause();
}
/**
	�T�E���h�I�u�W�F�N�g
*/
CSunSound::CSunSound()
{
	CoInitialize(NULL);
	m_pDS = NULL;
	m_pDS8 = NULL;
	m_sevolume = 0;
	m_bgmvolume = 0;
}

CSunSound::~CSunSound()
{
	Release();
	CoUninitialize();
}

void CSunSound::Init()
{
	//DirectSound8�I�u�W�F�N�g���擾
	if(FAILED(DirectSoundCreate8(NULL, &m_pDS8, NULL)))
		throw SunExeption("DirectSound�̏������Ɏ��s���܂����B\n");

	//�������x����ݒ�
	if(FAILED(m_pDS8->SetCooperativeLevel(SunApp.m_hWnd, DSSCL_NORMAL)))
		throw SunExeption("DirectSound�̏������Ɏ��s���܂����B\n");

	//DirectSound�I�u�W�F�N�g���擾
	if(FAILED(m_pDS8->QueryInterface(IID_IDirectSound,(void**)&m_pDS)))
		throw SunExeption("IDirectSound�ւ�QueryInterface�Ɏ��s���܂���\n");

	//�R���t�B�O���[�h
	m_sevolume = SunConfig.GetInt2("config.ini","sound","sevolume",SEVOLUME);
	m_bgmvolume = SunConfig.GetInt2("config.ini","sound","bgmvolume",BGMVOLUME);

	//Ayame�̏�����
	SunAyame.Init( "Ayame.dll" );
}
void CSunSound::Release()
{
	SunAyame.Release();

	SAFE_RELEASE( m_pDS8 );
	SAFE_RELEASE( m_pDS );
}
/**
	SunSoundBuffer�N���X
*/
SunSoundBuffer::SunSoundBuffer()
{
	m_pdsb = NULL;
	m_pDS = SunSound.m_pDS8;
}
SunSoundBuffer::SunSoundBuffer(char* pFile)
{
	m_pdsb = NULL;
	m_pDS = SunSound.m_pDS8;
	LoadFromDirectory(pFile);
}
SunSoundBuffer::~SunSoundBuffer()
{
	Release();
}
int SunSoundBuffer::Release()
{
	SAFE_RELEASE(m_pdsb);
	return 1;
}
//�Đ�
int SunSoundBuffer::Play(DWORD dwFlags)
{
	if( m_pdsb != NULL ) {
		m_pdsb->SetCurrentPosition(0);
		m_pdsb->Play(0,0,dwFlags);
	}
	return 1;
}
//�ĊJ
int SunSoundBuffer::Resume(DWORD dwFlags)
{
	if( m_pdsb != NULL ) {
		m_pdsb->Play(0,0,dwFlags);
	}
	return 1;
}
//��~
int SunSoundBuffer::Stop()
{
	DWORD dwFlags;
	if( m_pdsb != NULL ){
		if(FAILED(m_pdsb->GetStatus(&dwFlags)))
			return 0;
		else
			if(dwFlags & DSBSTATUS_PLAYING)
				m_pdsb->Stop();
	}
	return 1;
}
//�{�����[���ݒ�
int SunSoundBuffer::SetVolume( LONG lVolume )
{
//	LONG volume = lVolume + log10((double)(SunSound.m_sevolume+1)/101.0)*2000;
	LONG volume = lVolume + DSBVOLUME_MIN + ( DSBVOLUME_MAX - DSBVOLUME_MIN ) * SunSound.m_sevolume / 100;
	volume = min( DSBVOLUME_MAX, max( DSBVOLUME_MIN, volume ) );
	if( m_pdsb != NULL ){
		if(FAILED(m_pdsb->SetVolume( volume )))
			return 0;
	}
	return 1;
}
//�p���ݒ�
int SunSoundBuffer::SetPan( LONG lPan )
{
	if( FAILED( m_pdsb->SetPan( lPan )))
		return 0;
	return 1;
}
//�t�@�C�����烍�[�h
int SunSoundBuffer::Load(char* lpszFileName)
{
	//RIFF�\���̃t�@�C����ǂݍ��ނ̂ɕK�v�ȍ\���̂ł��B
	MMCKINFO oya_chunk, ko_chunk;
	
	//wave�t�@�C���̃I�[�v��
	HMMIO hmmio = mmioOpen(lpszFileName, NULL, MMIO_READ|MMIO_ALLOCBUF);
	if(!hmmio) {
		//throw SunExeption("%s�̃��[�h�Ɏ��s���܂���\n",lpszFileName);
		return 0;
	}
	
	//wave�t�@�C�����ǂ������ׂ�
	oya_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if(mmioDescend(hmmio, &oya_chunk, NULL, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return 0;
	}
	
	//fmt�`�����N�ֈړ�����
	ko_chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(mmioDescend(hmmio, &ko_chunk, &oya_chunk, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return 0;
	}
	
	//fmt�`�����N��ǂݎ��܂��B
	//WAVEFORMATEX�\���̂ɕK�v�ȏ�񂪃R�s�[����܂��B
	if(mmioRead(hmmio, (HPSTR)&m_wf, (LONG)ko_chunk.cksize) != (LONG)ko_chunk.cksize) {
		mmioClose(hmmio, 0);
		return 0;
	}
	
	//fmt�T�u�`�����N�̊O���Ɉړ�����
	//�`�����N��������̂ŊO�ɏo�܂��B
	//�����A�Ȃ�ăZ���X�̖��������Ȃ�ł��傤(^^;
	//����������ƃX�}�[�g�ɏo����n�Y���낤�ƌ����Ă�肽���ł��ˁB
	mmioAscend(hmmio, &ko_chunk, 0);
	
	//data�`�����N�Ɉړ�
	ko_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(hmmio, &ko_chunk, &oya_chunk, MMIO_FINDCHUNK)) {
		mmioClose(hmmio, 0);
		return 0;
	}

	//�o�b�t�@�쐬
	this->CreateBuffer( ko_chunk.cksize );

	LPVOID pmem;
	DWORD size;

	//�T�E���h�o�b�t�@�����b�N
	//���b�N�����T�C�Y�ƃo�b�t�@�̐擪�A�h���X���擾�o���܂��B
    if( FAILED( m_pdsb->Lock(0, ko_chunk.cksize, &pmem, &size, NULL, 0, 0))) {
		m_pdsb->Restore();
		if( FAILED( m_pdsb->Lock(0, ko_chunk.cksize, &pmem, &size, NULL, 0, 0))) {
			SunLog.LogOut( "�T�E���h�o�b�t�@�̃��b�N�Ɏ��s\n");
			return 0;
		}
	}
	
	//data�`�����N��ǂݎ��
	//�������摜�t�@�C���Ō����ƈ�ԗ~�����摜�̕����ł��ˁB
	//�ۂ��Ƃ������肢���������ɂ��܂��B
	if(mmioRead(hmmio, (HPSTR)pmem, (LONG)ko_chunk.cksize) != (LONG)ko_chunk.cksize){
		SunLog.LogOut("mmioRead�Ɏ��s\n");
	}
	//�o�b�t�@���A�����b�N
	m_pdsb->Unlock(pmem, size, NULL, 0);
	
	//�E�F�[�u�t�@�C�������
	mmioClose(hmmio, 0);
	
	return 1;
}

//�p�b�N�t�@�C�����烍�[�h
int SunSoundBuffer::LoadFromPack(char* pPack,char* pFile)
{
	unsigned long		DataSize		= 0;

	//=====================================================
	//	WAVE�f�[�^����
	//=====================================================

	FILE *fp = fopen( pPack, "rb" );
	if ( fp == NULL ) 
		throw SunExeption( "%s:�p�b�N�t�@�C���̃I�[�v���Ɏ��s\n", pPack );

	// �p�b�N�t�@�C���̊m�F
	char guid[4] = "";
	guid[0] = (char)fgetc( fp );
	guid[1] = (char)fgetc( fp );
	guid[2] = (char)fgetc( fp );
	guid[3] = (char)fgetc( fp );
	if ( strcmp( guid, "SND" ) )
		throw SunExeption( "%s:�p�b�N�t�@�C���ł͂���܂���\n", pPack );

	// ����ł͒T���܂��傤����
	while ( TRUE )
	{
		char Name[32];

		// �f�[�^�擾
		fread( &Name, sizeof(char), 32, fp );

		// �_���Ȃ�I��
		if ( feof( fp ) ) break;

		// �����H
		if ( strcmp( Name, pFile ) == 0 )
		{
			//�r�o
			fread( &DataSize, sizeof(long), 1, fp );
			break;
		}
		else
		{
			// ��
			long size;
			fread( &size, sizeof(long), 1, fp );
			fseek( fp, size+sizeof(WAVEFORMATEX), SEEK_CUR );
		}
	}

	if ( DataSize == 0 )
		throw SunExeption( "%s: �f�[�^�̎擾�Ɏ��s\n", pFile );

	// WAVE�t�H�[�}�b�g�擾
	fread( &m_wf, sizeof(WAVEFORMATEX), 1, fp );

	//�o�b�t�@�쐬
	this->CreateBuffer( DataSize );

	//��قǓǂݍ���wave�̃f�[�^�������T�E���h�o�b�t�@�ɃR�s�[���܂��B
	LPVOID pmem;
	DWORD size;
	
	//�T�E���h�o�b�t�@�����b�N
	//���b�N�����T�C�Y�ƃo�b�t�@�̐擪�A�h���X���擾�o���܂��B
	m_pdsb->Lock(0, DataSize, &pmem, &size, NULL, 0, 0);
	
	// �T�E���h�f�[�^�ǂݍ���
	fread( pmem, size, 1, fp );
	
	//�o�b�t�@���A�����b�N
	//Draw�̎��Ɠ����ł��B
	m_pdsb->Unlock(pmem, size, NULL, 0);


	return 1;
}
//�f�B���N�g���������͂��̖��O�̃p�b�N�t�@�C�����烍�[�h
int SunSoundBuffer::LoadFromDirectory(char* pFile)
{
	this->Release();
//	if( this->Load( pFile ) == 0 )
	{
		char* ptr;
		char strPack[MAX_PATH];
		char strFile[MAX_PATH];
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
			strncpy(strPack,pFile,size);
			strcpy(&strPack[size],".bin");
		}
		//�t�@�C������؂�o��
		{
			ptr++;
			if(ptr == NULL || *ptr == '\0')	
				return 0;
			strcpy(strFile,ptr);
		}
		if( this->LoadFromPack(strPack,strFile) == 0)
			return 0;
	}
	return 1;
}
//�o�b�t�@�쐬
int SunSoundBuffer::CreateBuffer( DWORD dwSize )
{
	DSBUFFERDESC dsbdesk;

	ZeroMemory(&dsbdesk, sizeof(DSBUFFERDESC));
	dsbdesk.dwSize = sizeof(DSBUFFERDESC);

	//�o�b�t�@�̃t���O���w��
	dsbdesk.dwFlags =
		DSBCAPS_CTRLPAN|
		DSBCAPS_CTRLVOLUME|
		DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_CTRLPOSITIONNOTIFY|
		DSBCAPS_GETCURRENTPOSITION2|
		DSBCAPS_GLOBALFOCUS;

	//�o�b�t�@�̃T�C�Y���w��
	dsbdesk.dwBufferBytes = dwSize;

	//WAVEFORMATEX���Z�b�g
	dsbdesk.lpwfxFormat = &m_wf; 
	
	//�I�t�X�N���[���o�b�t�@�̐���
	LPDIRECTSOUNDBUFFER pdsb = NULL;
	if(FAILED(m_pDS->CreateSoundBuffer(&dsbdesk, &pdsb, NULL))){		
		SunLog.LogOut("CreateSoundBuffer�Ɏ��s���܂���\n");
		return 0;
	}
	if(FAILED(pdsb->QueryInterface(IID_IDirectSoundBuffer8,(void**)&m_pdsb))){
		SunLog.LogOut("IDirectSoundBuffer8�ւ�QueryInterface�Ɏ��s���܂���\n");
		return 0;
	}
	SAFE_RELEASE( pdsb );
	return 1;
}

/**
	Ayame�EOggVorbis�Ǘ��N���X
*/
CSunAyame::CSunAyame()
{
	m_IsInitAyame = false;
	m_hAyame = NULL;
	m_pAyameInitialize = NULL;
	m_pAyameUninitialize = NULL;
	m_pAyameCreateFromFile	= NULL;
	m_pAyameCreateFromFileEx = NULL;
	m_pAyameCreateFromMemory = NULL;
	m_pAyameGetError = NULL;
	m_pAyame = NULL;
}
static void *AyameAlloc( unsigned int Size, const char *pFileName, unsigned int Line ) {
	return malloc( Size );
}

static void AyameFree( void *pData ) {
	free( pData );
}
//DLL�̏�����
int CSunAyame::Init( const char *pDllPath )
{
	if ( m_hAyame != NULL ) return true;

	// DLL�̃��[�h
	m_hAyame = ::LoadLibrary( pDllPath );
	if ( m_hAyame == NULL ) return false;

	// �֐��̃��[�h
	m_pAyameInitialize			= (AYAME_INITIALIZE_PROC)::GetProcAddress( m_hAyame, "Ayame_Initialize" );
	m_pAyameUninitialize		= (AYAME_UNINITIALIZE_PROC)::GetProcAddress( m_hAyame, "Ayame_Uninitialize" );
	m_pAyameCreateFromFile		= (AYAME_CREATE_FROM_FILE_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromFile" );
	m_pAyameCreateFromFileEx	= (AYAME_CREATE_FROM_FILE_EX_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromFileEx" );
	m_pAyameCreateFromMemory	= (AYAME_CREATE_FROM_MEMORY_PROC)::GetProcAddress( m_hAyame, "Ayame_CreateInstanceFromMemory" );
	m_pAyameGetError			= (AYAME_GETERROR_PROC)::GetProcAddress( m_hAyame, "Ayame_GetLastError" );

	// �����ƃ��[�h�ł��Ă�H
	if ( m_pAyameInitialize			== NULL ) return false;
	if ( m_pAyameUninitialize		== NULL ) return false;
	if ( m_pAyameCreateFromFile		== NULL ) return false;
	if ( m_pAyameCreateFromFileEx	== NULL ) return false;
	if ( m_pAyameCreateFromMemory	== NULL ) return false;
	if ( m_pAyameGetError			== NULL ) return false;

	// �h���C�o�[�̏�����
	m_IsInitAyame = m_pAyameInitialize( NULL, (void**)&SunSound.m_pDS, (AYAME_MEMORY_ALLOC_PROC)AyameAlloc, AyameFree );

	// Ogg��Wav���̔���
	SunFile file;
	if( file.OpenFile("bgm.bin") != 0)
		bgm_mode = BGM_AYM;
	else if( file.OpenFile("ogg.bin") != 0)
		bgm_mode = BGM_OGG;
	else if( SetCurrentDirectory( "bgm" ) != 0 )
		bgm_mode = BGM_AYM;
	else if( SetCurrentDirectory( "ogg" ) != 0 )
		bgm_mode = BGM_OGG;
	ResetCurrent();
	file.CloseFile();

	return m_IsInitAyame;
}

int CSunAyame::Release()
{
	// �h���C�o�[�̉��
	if( m_hAyame != NULL ) {
		if( m_pAyame != NULL ) {
			m_pAyame->Stop();
			m_pAyame->Release();
			m_pAyame = NULL;
		}
		if ( m_pAyameUninitialize != NULL ) {
			m_pAyameUninitialize();
		}
		::FreeLibrary( m_hAyame );
		m_hAyame = NULL;
	}
	return 1;
}

//�Đ�
int CSunAyame::Play( char *pFile )
{
	if( m_pAyameCreateFromFileEx == NULL )
		return 0;
	if( m_pAyameCreateFromFile == NULL )
		return 0;
	if( m_pAyame != NULL ) {
		m_pAyame->Stop();
		m_pAyame->Release();
		m_pAyame = NULL;
	}
	SunFile file;
	file.OpenFileFromDirectory( pFile );
	unsigned long Start = file.GetStart();
	unsigned long Size = file.GetSize();
	char strPack[MAX_PATH];
	char strFile[MAX_PATH];
	strcpy( strPack, file.GetPack() );
	strcpy( strFile, file.GetFile() );
	file.CloseFile();

	if( strPack[0] != '\0' )
		m_pAyame = m_pAyameCreateFromFileEx( strPack, Start, Size, false );
	else
		m_pAyame = m_pAyameCreateFromFile( strFile, false );
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->SetVolume( SunSound.m_bgmvolume );
	return m_pAyame->Play(-1,0);
}
//�ꎞ��~
int CSunAyame::Pause()
{
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->Pause();
	return 1;
}
//��~
CSunAyame::Stop()
{
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->Stop();
	return 1;
}
//�ĊJ
int CSunAyame::Resume()
{
	if( m_pAyame == NULL )
		return 0;
	m_pAyame->Play(-1,0);
	return 1;
}
