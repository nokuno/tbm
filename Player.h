#pragma once

#define MARISA_WIDTH	48
#define MARISA_HEIGHT	48
#define MARISA_CLIP		10

struct CMarisa
{
	SunTexture	imgMarisa;
	SunTexture	imgHakka;
	SunTexture	imgAtari;
	SunFont		font;
	SunEffectLaser2	effect;
	SunEffectLaser2	laser1;
	SunEffectLaser	laser2;
	SunEffectLaser	laser3;
	SunSoundBuffer	sndLaser;
	SunSoundBuffer	sndShot;
	SunSoundBuffer	sndDamage;
	SunSoundBuffer	sndGraze;
	double x,y;
	int count;
	int	state;	///< 0:�ʏ�A1:�{�����A2:��炢�{����t���A3:�o����
	int atack;	///< 0:�U�����Ȃ��A1:�V���b�g
	int slow;	///< 0:�����A1:�ᑬ
	int muteki;	///< 0:�ʏ�A0�ȏ�F���G
	int dcount;	///< �ėp�_�E���J�E���^
	int kurai;	///< ��炢�{����t���ԁi��炢�{�����g�����тɌ����Ă����j
	int renda;	///< �A�ŃJ�E���^�i�{�^��2�p�j
	int forbit;	///< �V���b�g�E�{���֎~
	double rx,ry;	///< �}�X�^�[�X�p�[�N�p�����_���ϐ�
	double si;		///< �}�X�^�[�X�p�[�N�p�T�C���l
	int max_kurai;	///< ��炢�{���̎�t����
	int dec_kurai;	///< ��炢�{���̎�t���Ԍ�����

	CMarisa();
	virtual ~CMarisa();
	int Run();
	int Move();
	int Attack();
	int Bomb();
	int Draw();
	int Draw2();

	double GetMasterHantei(int x,int y);	///< �}�X�^�[�X�p�[�N�̍U������
	int GetMasterHantei2(int x,int y);		///< �}�X�^�[�X�p�[�N�̒e��������
	int GetLaserHantei(RECT *pRect);		///< ���[�U�[�̍U������
	double GetLaserDamage(){return 1;}
	double GetSize();		///< �e�Ƃ̓����蔻��p
	double GetGrazeSize();	///< �O���C�Y�̔���T�C�Y
	int GetMuteki();	///< ���G���ǂ���
	void Damage();		///< �G�t�F�N�g�Ƃ�
	void Damage2();		///< ���ۂ̃_���[�W�i��炢�{���p�j
	void Stop();
	double GetX();
	double GetY();
	void GetItem(int type);
	int Graze();		///< �O���C�Y
	void Talk();		///< ��bON
	void Resume();		///< ��bOFF
	void ResetState();	///< �X�e�[�W�N���A���p
	void ResetPosition();	///<�ʒu������
};

extern CMarisa *g_pPlayer;