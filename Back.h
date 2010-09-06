#pragma once

//�w�i�C���^�[�t�F�[�X
struct CBack : public SunObject
{
	virtual int Run() = 0;
	virtual int Draw() = 0;
};
extern CBack *g_pBack;

//�X�e�[�W�w�i(3D)����
struct CBackStageBase : public CBack
{
	int	count;
	CBackStageBase();
};

//�X�e�[�W1
struct CBackStage1 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunMicroThreadEx	mt;
	CBackStage1();
	virtual int Run();
	virtual int Draw();
	int Thread();
};

//���[�~�A�X�y��
struct CBackRumia : public CBack
{
	int	count;
	CBackRumia();
	virtual int Run();
	virtual int Draw();
};


//�X�e�[�W2
struct CBackStage2 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgMori2;
	SunTexture	imgTree;
	SunTexture	imgGrass;

	SunMicroThreadEx	mt;
	CBackStage2();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//���O���X�y��
struct CBackWriggle : public CBack
{
	int	count;
	CBackWriggle();
	virtual int Run();
	virtual int Draw();
};

//�X�e�[�W3
struct CBackStage3 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgYuki;

	SunMicroThreadEx	mt;
	CBackStage3();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//�`���m�X�y��
struct CBackChirno : public CBack
{
	int	count;
	CBackChirno();
	virtual int Run();
	virtual int Draw();
};

//�X�e�[�W4
struct CBackStage4 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgBack;
	SunTexture	imgMokume;

	CBackStage4();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//�~�X�e�B�A�X�y��
struct CBackMystia : public CBack
{
	int	count;
	CBackMystia();
	virtual int Run();
	virtual int Draw();
};
//�X�e�[�W5
struct CBackStage5 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgBack;
	SunTexture	imgSyoji;

	CBackStage5();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//��X�y��
struct CBackChen : public CBack
{
	int	count;
	CBackChen();
	virtual int Run();
	virtual int Draw();
};

//�X�e�[�W6
struct CBackStage6 : public CBackStageBase
{
	double x,y,z;
	double ax,ay,az;
	SunTexture	imgYuki;
	SunTexture	imgHyoga;

	CBackStage6();
	virtual int Run();
	virtual int Draw();
	int Thread();
};
//���e�B�X�y��
struct CBackLety : public CBack
{
	int	count;
	CBackLety();
	virtual int Run();
	virtual int Draw();
};

//�`���m�X�y��2
struct CBackChirno2 : public CBack
{
	int	count;
	CBackChirno2();
	virtual int Run();
	virtual int Draw();
};