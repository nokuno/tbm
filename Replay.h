#pragma once

/**
	@brief ���v���C1����
*/
struct SunReplayData
{
	LONGLONG flame;		//�t���[���i���ԁj
	unsigned char inp;		//���͔ԍ�
	unsigned char change;	//0�F�������A1�F������
	unsigned char dammy1;
	unsigned char dammy2;
	unsigned int dammy3;
	SunReplayData(){
		dammy1 = 0xff;
		dammy2 = 0xff;
		dammy3 = 0xffffffff;
	}
};
/**
	@brief ���v���C�t�@�C���w�b�_
*/
struct SunReplayHeader
{
	char guid[4];		///< 'r' 'p' 'l' 'y'
	unsigned int seed;	//�����_���V�[�h
	unsigned char mode;
	unsigned char diff;
	unsigned char stage;
	unsigned char dammy;
	unsigned int dammy2;
	SunReplayHeader(){
		dammy = 0xff;
		dammy2 = 0xffffffff;
	}
};
/**
	@brief ���v���C�N���X
*/
class CSunReplay
{
public:
	LONGLONG m_count;		//�t���[���J�E���g
	unsigned int m_seed;	//�����_���V�[�h
	int m_mode;				//�Đ�/�L�^���[�h
	int	m_old[INP_MAX];		///<�P�t���O�̏��
	int	m_state[INP_MAX];	///<���݂̏��
	SunReplayHeader m_header;		//�w�b�_�[
	list<SunReplayData> m_lstData;	//�f�[�^
	int m_pause;			//�|�[�Y���t���O
public:
	CSunReplay();
	virtual ~CSunReplay();
	void Record();
	int Run();
	void Save( char* file );
	void Save();
	void Play( char* file  );
	void Exit();
	int GetInputState(int inp);	///< ���̏u�Ԃ̓��͏��
	int GetInputEvent(int inp);	///< �����ꂽ�u�Ԃ����o
	int GetInputHanasi(int inp);///< ���������o
};
#define REPLAY_NONE		0
#define REPLAY_RECORD	1
#define REPLAY_PLAY		2

extern CSunReplay SunReplay;

//���̏u�Ԃ̓��͏��
int GetInputState(int inp);
//�����ꂽ�u�Ԃ����o
int GetInputEvent(int inp);
//�����ɔ���
int GetInputHanasi(int inp);
//����{�^��
int GetInputDecide();
//�L�����Z���{�^��
int GetInputCancel();
//�㉺�L�[
int GetInputUD();
//���E�L�[
int GetInputLR();
//���E�L�[���
int GetInputLRState();
//�|�[�Y�{�^��
int GetInputPause();
