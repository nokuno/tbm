#pragma once

//�X�e�[�W3
struct CStage3 : public CStage
{
	SunTexture imgChirno;
	SunTexture imgBack1;
	SunTexture imgBack2;
	SunTexture imgCutin;
	SunTexture imgTalk;
	CStage3();
	virtual ~CStage3();
	virtual int Run();
	virtual int EnemyApear();
};
extern CStage3 *g_pStage3;

//�d��9�i�S���ʒe�j
struct CEnemyFairy9 : public CEnemyFairyBase
{
	CEnemyFairy9(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//�d��10�i���@�_���~�e�j
struct CEnemyFairy10 : public CEnemyFairyBase
{
	CEnemyFairy10(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//�d��11�i�S���ʉ~�e�j
struct CEnemyFairy11 : public CEnemyFairyBase
{
	CEnemyFairy11(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//�d��12�i�O��~�e�j
struct CEnemyFairy12 : public CEnemyFairyBase
{
	CEnemyFairy12(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//�d��13�i�㉺�~�e�j
struct CEnemyFairy13 : public CEnemyFairyBase
{
	CEnemyFairy13(double x,double y);
	virtual int Run();
	virtual int GetItem();
};

//�ы�4�i�S���ʑł��Ԃ��j
struct CEnemyKedama4 : public CEnemyKedamaBase
{
	int c;
	CEnemyKedama4(double x,double y,int c);
	virtual int Run();
	virtual int Death();
};