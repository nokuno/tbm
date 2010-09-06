#pragma once

//�X�e�[�W5
struct CStage5 : public CStage
{
	SunTexture imgChen;
	SunTexture imgBack1;
	SunTexture imgBack3;
	SunTexture imgTalk;
	CStage5();
	virtual ~CStage5();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage5 *g_pStage5;
//�d��18�i�s��e�j
struct CEnemyFairy18 : public CEnemyHoodBase
{
	int t;
	CEnemyFairy18(double x,double y,int t);
	virtual int Run();
	virtual int GetItem();
};
//�d��19�i�s��e�j
struct CEnemyFairy19 : public CEnemyHoodBase
{
	CEnemyFairy19(double x,double y);
	virtual int Run();
	virtual int GetItem();
};
//�d��20�i�d�C�j
struct CEnemyFairy20 : public CEnemyFairyBase
{
	double v,t;
	CEnemyFairy20(double x,double y,double t);
	virtual int Run();
	virtual int GetItem();
};
//�d��21�i�����܂��e�j
struct CEnemyFairy21 : public CEnemyFairyBase
{
	int t;
	CEnemyFairy21(double x,double y,int t);
	virtual int Run();
	virtual int GetItem();
};
//�d��22�i���g�p�j
struct CEnemyFairy22 : public CEnemyFairyBase
{
	double c,t;
	CEnemyFairy22(double x,double y,double c,double t);
	virtual int Run();
	virtual int GetItem();
};

//�d��23�i�����Ԃ��j
struct CEnemyFairy23 : public CEnemyFairyBase
{
	double c,t;
	CEnemyFairy23(double x,double y,double c,double t);
	virtual int Run();
	virtual int GetItem();
};

//�ы�5�i�����܂��j
struct CEnemyKedama5 : public CEnemyKedamaBase
{
	double c,t;
	CEnemyKedama5(double x,double y,double c,double t);
	virtual int Run();
	virtual int GetItem();
};
//�����_
struct CEnemy7God : public CEnemyHoodBase
{
	int c;
	CEnemy7God(double x,double y,int c);
	virtual int Run();
	virtual int GetItem();
};