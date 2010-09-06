#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CSunReplay SunReplay;

//////////////////////////////////////////////////////////////////////
// ���v���C�N���X
//////////////////////////////////////////////////////////////////////
CSunReplay::CSunReplay(){
	m_mode = REPLAY_NONE;
	m_count = 0;
	m_seed = 0;
	::ZeroMemory(m_old,sizeof(m_state));
	::ZeroMemory(m_state,sizeof(m_state));
}
CSunReplay::~CSunReplay(){
	m_lstData.clear();
}
//�L�^�J�n
void CSunReplay::Record(){
	Exit();
	m_mode = REPLAY_RECORD;
	m_seed = (unsigned)time( NULL );
	SunRand.SetSeed( m_seed );

	m_header.guid[0] = 'r';
	m_header.guid[1] = 'p';
	m_header.guid[2] = 'l';
	m_header.guid[3] = 'y';
	m_header.seed = m_seed;
	m_header.mode = g_pGame->mode;
	m_header.diff = g_pGame->diff;
	m_header.stage = g_pGame->stage;
}
//���s�i�L�^�E�Đ����ʁj
int CSunReplay::Run(){
	switch( m_mode ) {
	case REPLAY_RECORD:
		for( int i = 0; i < INP_MAX; i++ ) {
			SunReplayData data;
			data.flame = m_count;
			data.inp = i;
			m_old[i] = m_state[i];
			m_state[i] = SunInput.GetInputState( i );
			if( g_pTitle->replay_config ) {
				if( !m_old[i] && m_state[i] ) {
					data.change = 1;
					m_lstData.push_back(data);
				}
				else if( m_old[i] && !m_state[i] ) {
					data.change = 0;
					m_lstData.push_back(data);
				}
			}
		}
		break;
	case REPLAY_PLAY:
		for( int i = 0; i < INP_MAX; i++ ) {
			m_old[i] = m_state[i];
		}
		while( !m_lstData.empty() ) {
			SunReplayData data = m_lstData.front();
			if( data.inp > INP_MAX )
				SunLog.LogOut( "���v���C�f�[�^�̓��͂����Ă��܂�\n");
			if( data.flame < m_count )
				SunLog.LogOut( "���v���C�f�[�^�̎��n�񂪉��Ă��܂�\n");

			if( data.flame == m_count) {
				if( data.change == 1 ) {
					if( m_state[ data.inp ] == 1 ) {
						SunLog.LogOut( "���v���C�̃f�[�^������������\n" );
					}
					m_state[ data.inp ] = 1;
					m_lstData.pop_front();
				}
				else if( data.change == 0 ) {
					if( m_state[ data.inp ] == 0 ) {
						SunLog.LogOut( "���v���C�̃f�[�^������������\n" );
					}
					m_state[ data.inp ] = 0;
					m_lstData.pop_front();
				}
				else
					SunLog.LogOut( "���v���C�̃f�[�^������������\n" );
			}
			else
				break;
		}
		break;
	case REPLAY_NONE:
		for( int i = 0; i < INP_MAX; i++ ) {
			m_old[i] = m_state[i];
			m_state[i] = SunInput.GetInputState( i );
		}
		return 1;
	}
	m_count ++;
	return 1;
}
//�L�^��ۑ�
void CSunReplay::Save( char* file ) {
	if( g_pTitle->replay_config ) {
		CreateDirectory( "replay" );

		SunFileBase f;
		if( f.OpenFileWrite( file ) == 0)
			SunLog.LogOut( "�t�@�C���������ݎ��s\n" );	
		
		//�w�b�_��������
		if( f.WriteFile( (void*)&m_header, sizeof(SunReplayHeader)) != sizeof(SunReplayHeader))
			SunLog.LogOut( "�t�@�C���������ݎ��s\n" );	

		//�f�[�^��������
		while( !m_lstData.empty() ) {
			if( f.WriteFile( (void*)&m_lstData.front(), sizeof(SunReplayData)) != sizeof(SunReplayData) )
				SunLog.LogOut( "�t�@�C���������ݎ��s\n" );	
			m_lstData.pop_front();
		}
		f.CloseFile();
		Exit();
	}
}
//�Z�[�u
void CSunReplay::Save(){
	if( g_pTitle->replay_config ) {
		::ResetCurrent();
		::CreateDirectory("replay");
		WIN32_FIND_DATA FindFileData;
		HANDLE hFind;
		char filename[MAX_PATH];
		char kakutyosi[] = ".rpy";
		for( int i = 0; i < 100; ++i ) {
			sprintf( filename, "replay/tbm%02d%s", i, kakutyosi );
			hFind = FindFirstFile( filename, &FindFileData );
			if( hFind == INVALID_HANDLE_VALUE ) {
				this->Save( filename );
				break;
			}
		}
	}
}
//���[�h���čĐ�
void CSunReplay::Play( char* file  ) {
	Exit();

	SunFileBase f;
	if( f.OpenFileRead( file ) == 0)
		SunLog.LogOut( "�t�@�C���ǂݍ��ݎ��s\n" );	

	SunReplayHeader header;
	if( f.ReadFile( (void*)&header, sizeof( SunReplayHeader )) != sizeof( SunReplayHeader ))
		SunLog.LogOut( "�t�@�C���ǂݍ��ݎ��s\n" );	
	if( header.guid[0] != 'r' ) return;
	if( header.guid[1] != 'p' ) return;
	if( header.guid[2] != 'l' ) return;
	if( header.guid[3] != 'y' ) return;
	m_seed = header.seed;

	g_pGame->diff = header.diff;
	g_pGame->stage = header.stage;

	SunReplayData data;
	while( f.ReadFile( (void*)&data, sizeof(SunReplayData)) == sizeof(SunReplayData)) {
		m_lstData.push_back(data);
	}
	f.CloseFile();

	m_mode = REPLAY_PLAY;
	SunRand.SetSeed( m_seed );
}
void CSunReplay::Exit() {
	m_mode = REPLAY_NONE;
	m_count = 0;
	m_lstData.clear();
	::ZeroMemory(m_state,sizeof(m_state));
	::ZeroMemory(m_old,sizeof(m_state));
}
int CSunReplay::GetInputState(int inp)	///< ���̏u�Ԃ̓��͏��
{
	return m_state[inp];
}
int CSunReplay::GetInputEvent(int inp)	///< �����ꂽ�u�Ԃ����o
{
	return !m_old[inp] && m_state[inp];
}
int CSunReplay::GetInputHanasi(int inp)	///< ���������o
{
	return m_old[inp] && !m_state[inp];
}

/**
	�⏕�֐�
*/
int GetInputState(int inp){
	return SunReplay.GetInputState( inp );
}
int GetInputEvent(int inp){
	return SunReplay.GetInputEvent( inp );
}
int GetInputHanasi(int inp){
	return SunReplay.GetInputHanasi( inp );
}

int GetInputDecide(){
	return SunInput.GetInputEvent( INP_B0 ) ||
		SunInput.GetInputEvent( INP_B2 );
}
int GetInputCancel(){
	return SunInput.GetInputEvent( INP_B1 );
}
int GetInputUD(){
	return SunInput.GetInputEvent( INP_DOWN ) - SunInput.GetInputEvent( INP_UP );
}
int GetInputLR(){
	return SunInput.GetInputEvent(INP_RIGHT) - SunInput.GetInputEvent(INP_LEFT);
}
int GetInputLRState(){
	return SunInput.GetInputState(INP_RIGHT) - SunInput.GetInputState(INP_LEFT);
}
int GetInputPause(){
	return SunInput.GetInputEvent(INP_B4);
}