#pragma once

//���ׂĂ̒e���A�C�e���ɕς���
int TamaToItem();
//�t���[�Y
int TamaFreeze();
//�����_���ɓ���
int TamaRandom(double v);
//�ړ��ĊJ
int TamaResume(double v);

//��{�N���X
struct CTama : public SunObject
{
	double x,y;
	double v,a;
	int type,color;
	virtual int Run() = 0;
	virtual int Draw() = 0;
	virtual int Item() = 0;		//�A�C�e���ɕς��
	virtual int Freeze() = 0;	//����
	virtual CTama *Copy() = 0;
};
extern SunList<CTama> g_lTama;
//���ʃN���X�i���������^���j
struct CTamaNormal : public CTama
{
	int graze;
	int out;
	int count;
	CTamaNormal();
	virtual int Run();
	virtual int Run2();
	virtual int Draw();
	virtual CTama *Copy();
	virtual int Item();
	virtual int Freeze();
	double GetX();
	double GetY();
	int Vanish();
	double GetVX();
	double GetVY();
	void SetVelocityXY(double vx,double vy);
};
//�����x�^��
struct CTamaIncrease : public CTamaNormal
{
	int end;
	double inc;
	CTamaIncrease();
	virtual int Run();
	virtual CTama *Copy();
	void SetIncrease( double sv,double ev,int end );
};
//�܂�e
//������������~��������ς��čĊJ
struct CTamaTurn : public CTamaNormal
{
	int stop_count;
	int stop_time;
	double turn;
	double v2;
	CTamaTurn();
	virtual int Run();
	virtual CTama *Copy();
};
//���@�_���܂�e
//������������~�����@�_��
struct CTamaTurnToPlayer : public CTamaNormal
{
	int stop_count;
	int stop_time;
	double turn;
	double v2;
	CTamaTurnToPlayer();
	virtual int Run();
	virtual CTama *Copy();
};
//�u�e
struct CTamaHotaru : public CTamaNormal
{
	double s,sa;
	CTamaHotaru();
	virtual int Run();
	virtual CTama *Copy();
};
//�܂�e�Q�i�p�x��Ύw��j
struct CTamaTurn2 : public CTamaNormal
{
	int turn_count;
	double turn;
	double v2;
	CTamaTurn2();
	virtual int Run();
	virtual CTama *Copy();
};
//�����e
struct CTamaBurst : public CTamaNormal
{
	int burst_count;
	int t,m,n;
	double sv,ev;
	CTamaBurst();
	virtual int Run();
	virtual CTama *Copy();
};

//���J�f�e
struct CTamaMukade : public CTamaNormal
{
	int burst_start;
	int burst_interval;
	int burst_end;
	int t;
	CTamaMukade();
	virtual int Run();
	virtual CTama *Copy();
};
//���J�f�e�i���j
struct CTamaMukadeLeg : public CTamaNormal
{
	int t,tt;
	double xx,yy,s;
	CTamaMukadeLeg();
	virtual int Run();
	virtual CTama *Copy();
};

//�u���e
struct CTamaSet : public CTamaNormal
{
	int t;
	double c;
	int set_interval;
	CTamaSet();
	virtual int Run();
	virtual CTama *Copy();
};

//�t���[�Y�e
struct CTamaFreeze : public CTamaNormal
{
	int r;
	int t;
	int c;
	double vv;
	virtual int Run();
	virtual CTama *Copy();
};

//�����e2
struct CTamaBurst2 : public CTamaNormal
{
	int burst_start;
	int burst_end;
	int burst_interval;
	int m,n;
	double w;
	double sv,ev;
	double offset;
	virtual int Run();
	virtual CTama *Copy();
};

//���e
struct CTamaFrost : public CTamaNormal
{
	int mode;
	int freeze;
	double frost;
	CTamaFrost();
	virtual ~CTamaFrost();
	virtual int Run();
	virtual CTama *Copy();
	void Stop();
	static list<CTamaFrost*> lTamaFrost;
};

//�����e
struct CTamaCristal : public CTamaNormal
{
	int mode;
	CTamaCristal();
	virtual ~CTamaCristal();
	virtual int Run();
	virtual CTama *Copy();
	void Stop();
	static list<CTamaCristal*> lTamaCristal;
};

//�s���|�C���g�e
struct CTamaPinPoint : public CTamaNormal
{
	int mode;
	int t;
	int c;
	double vv;
	virtual int Run();
	virtual CTama *Copy();
};

//�{�C�h�e
struct CTamaBoid : public CTamaNormal
{
	double w1,w2,w3;
	int loop;
	double vmax;
	CTamaBoid();
	virtual ~CTamaBoid();
	virtual int Run();
	virtual CTama *Copy();
	static list<CTamaBoid*> lTamaBoid;
	static double gx1,gy1;
	static double gx2,gy2;
	static double gx3,gy3;
	static void UpdateGoal();
};

//���ɐ}
struct CTamaTaikyoku : public CTamaNormal
{
	int t1,t2;
	double aa;
	CTamaTaikyoku();
	virtual int Run();
	virtual CTama *Copy();
};

//�^�C�t�[��
struct CTamaTyphoon : public CTamaNormal
{
	CTamaTyphoon();
	virtual ~CTamaTyphoon();
	virtual int Run();
	virtual CTama *Copy();
	static list<CTamaTyphoon*> lTamaTyphoon;
};
//�n���P�[��
struct CTamaHurricane : public CTamaNormal
{
	int t;
	double c;
	CTamaHurricane();
	virtual int Run();
	virtual CTama *Copy();
};

//�c�i�~
struct CTamaTsunami : public CTamaNormal
{
	int end;
	double ax;
	double ay;
	CTamaTsunami();
	virtual int Run();
	virtual CTama *Copy();
};

//���˒e
struct CTamaReflect : public CTamaNormal
{
	int num;
	CTamaReflect();
	virtual int Run();
	virtual CTama *Copy();
};
