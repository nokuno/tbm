#pragma once

//���[�~�A����
struct CBossRumiaBase : public CBossBase
{
	SunEffectRing		circle;

	CBossRumiaBase();
	virtual int Draw();
	virtual int SpellCard();
	int DrawEffect();			//�ŃG�t�F�N�g
	int DontCome();	//�ߕt���h�~���@�_���e
};
//���{�X
struct CBossRumiaMiddle : public CBossRumiaBase
{
	CBossRumiaMiddle();
	virtual int Run();
	virtual int Draw();
	int Thread();
	int NormalAtack1();
};
struct CBossRumia : public CBossRumiaBase
{
	CBossRumia();
	virtual int Run();
	virtual int Draw();
	int Thread();

	int NormalAtack2();
	int NightBird();
	int NormalAtackBase(CTamaNormal *pTama, int j); //�З��^�e��
	int Demarcation();
	int MoonLightRay();
	int NonDirectional();
};
