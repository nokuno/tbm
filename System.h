#pragma once

#define SYSTEM_NORMAL 0
#define SYSTEM_PAUSE 1
#define SYSTEM_GAMEOVER 2
#define SYSTEM_REPLAY 3
#define SYSTEM_RESULT 4
#define SYSTEM_RESUME 5

struct CSystem : SunObject
{
	SunTexture	imgSystem1;
	SunTexture	imgSystem2;
	SunTexture	imgMaho;
	SunTexture	imgMoji;
	SunTexture	imgMoji2;
	SunTexture	imgLife;
	SunTexture	imgBomb;
	SunSoundBuffer sndPause;
	int	count;	//�J�E���g
	int life;	//�c�@
	int bomb;	//�c�{��
	int power;	//�p���[
	int graze;	//�O���C�Y
	LONGLONG score;		//�X�R�A
	LONGLONG hiscore;	//�n�C�X�R�A
	int spellcard;		//�X�y���J�[�h
	double rank;			//�{��
	int bullet;				//�ő�e��
	int item;				//�A�C�e���������

	int state;				//SYSTEM_**
	int pause_select;		//�|�[�Y���j���[
	int gameover_select;	//�Q�[���I�[�o�[���j���[
	int replay_select;		//���v���C���j���[
	int fullpower_count;	//�t���p���[�\��
	int spellbonus_count;	//�X�y���{�[�i�X�\��
	int spellbonus;			//0�Ȃ�SPELLCARD FAILED
	int continue_dirty;		//�R���e�B�j���[�������H
	int continue_rest;		//�c��R���e�B�j���[��
	int result_mode;		//���U���g�\��
	int result_count;		//���U���g�\��
	int extend_count;		//�G�N�X�e���h��
	int extend_interval;	//�G�N�X�e���h�Ԋu
	int extend_score;		//���̃G�N�X�e���h�X�R�A
	int resume_count;		//�ĊJ�J�E���g
	int spell_num;			//���݂̃X�y���ԍ�
	int draw_system;		//�V�X�e���`�悷��H

	CSystem();
	virtual ~CSystem();
	virtual int Run();
	virtual int Draw();
	int Damage();
	int GameOver();
	int AddScore(LONGLONG s);
	int AddPower(int p);
	int AddLife(int l);
	int AddBomb(int b);
	int AddGraze(int g);
	int DrawNumber(int x,int y,LONGLONG n,int k,D3DCOLOR color = alpha(255),int scale = 1);
	void ReturnTitle();
	void Restart();
	void SaveReplay();
	void DrawResult(int x,int y,D3DCOLOR color);
	void ResumeGame();
};

extern CSystem *g_pSystem;
