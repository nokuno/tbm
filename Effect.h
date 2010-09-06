#pragma once

//�G�t�F�N�g�̃C���^�[�t�F�[�X
struct CEffect : public SunObject
{
	CEffect(){};
	virtual ~CEffect(){};
	virtual int Run() = 0;
	virtual int Draw() = 0;
};

extern SunList<CEffect> g_lEffect;	//���ʃ��C���[�i���Z�j
extern SunList<CEffect> g_lEffect2;	//��ʃ��C���[�i���Z�j
extern SunList<CEffect> g_lEffect3;	//��ʃ��C���[�i�ʏ�j

//��ʑS�̂łȂ��G�t�F�N�g�̊�b�N���X
struct CEffectBase : public CEffect
{
	int count;
	CEffectBase();
	virtual int Run();
	virtual int Draw() = 0;
};

//�����O�G�t�F�N�g�i�U�R��|�����Ƃ��j
struct CEffectRing : public CEffectBase
{
	SunEffectRing effect;
	CEffectRing(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//�_���[�W�G�t�F�N�g
struct CEffectDamage : public CEffectBase
{
	SunEffectRing effect;
	CEffectDamage(double x,double y);
	virtual int Run();
	virtual int Draw();
};


//�J�b�g�C���G�t�F�N�g
struct CEffectCutin : public CEffectBase
{
	SunTexture	*pTex;
	RECT		rect;
	double		x,y;
	CEffectCutin(SunTexture *p,RECT *pRect,double x,double y);
	virtual int Run();
	virtual int Draw();
};

//�t�H���g�G�t�F�N�g
struct CEffectFont : public CEffectBase
{
	int c1,c2,c3;
	double x,y;
	double vx,vy;
	SunFont	font;
	D3DCOLOR color;
	int a;
	CEffectFont(char *text,D3DCOLOR color,double x,double y,double vx=-2,double vy=0,int c1=30,int c2=120,int c3=30);
	virtual int Run();
	virtual int Draw();
};

//�O���C�Y�G�t�F�N�g
struct CEffectGraze : public CEffectBase
{
	double x,y;
	double a,v;
	CEffectGraze(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//�X�^�[�G�t�F�N�g
struct CEffectStar : public CEffectBase
{
	double x,y;
	double a,v;
	CEffectStar(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//�q�b�g�G�t�F�N�g
struct CEffectHit : public CEffectBase
{
	double x,y;
	double a,v;
	CEffectHit(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//�z���b�N�G�t�F�N�g
struct CEffectHolic : public CEffectBase
{
	double x,y;
	CEffectHolic(double x,double y);
	virtual int Run();
	virtual int Draw();
};

//�{�X�j��G�t�F�N�g
struct CEffectBreak : public CEffectBase
{
	double x,y;
	CEffectBreak(double x,double y);
	virtual int Run();
	virtual int Draw();
};
//���ڃG�t�F�N�g
struct CEffectTorime : public CEffectBase
{
	double x,y;
	CEffectTorime();
	virtual int Run();
	virtual int Draw();
};
