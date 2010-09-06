#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//�X�e�[�W1
CStage1 *g_pStage1 = NULL;

CStage1::CStage1() :
imgRumia("img/rumia.png"),
imgYuki("img/yuki.png"),
imgBack1("img/rumia_back1.png"),
imgBack2("img/rumia_back2.png"),
imgCutin("img/rumia_cutin.png")
{
	g_pStage1 = this;
	g_pBack		= new CBackStage1();

	TSunFunctionCallback<CStage1> fn;
	fn.SetFunction(this,&CStage1::EnemyApear);
	mt.Start(&fn,0x8000);
}
CStage1::~CStage1(){
	g_pStage1 = NULL;
}
int CStage1::Run(){
	return CStage::Run();
}
int CStage1::EnemyApear()
{

	g_pSystem->spell_num = 0;
	PlayBGM("tbm_hosuki");

	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("Stage1",stageFontColor1,320,210,2,0));
	g_lEffect3.Add( new CEffectFont("��ɕ����ꂽ���z��",stageFontColor2,320,230,2,0));
	mt.Suspend(30);
	g_lEffect3.Add( new CEffectFont("��C�����[�W�����~�X�g",bgmFontColor,380,420) );
	mt.Suspend(60);

	//�J�n
	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_TOP + 40) );
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_TOP + 80) );
		mt.Suspend(30);
	}

	mt.Suspend(60);
	for( int i = 0; i < 4; ++i ) {
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_BOTTOM - 40) );
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_BOTTOM - 80) );
		mt.Suspend(30);
	}
	mt.Suspend(60);
	for( int i = 0; i < 2; ++i ) {
		g_lEnemy.Add( new CEnemyFairy2(GAME_RIGHT + 24,GAME_TOP + 80) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy2(GAME_RIGHT + 24,GAME_BOTTOM - 80) );
		mt.Suspend(60);
	}

	mt.Suspend(60);
	{
		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_TOP + 80) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_BOTTOM - 80) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_TOP + 160) );
		mt.Suspend(60);
		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_BOTTOM - 160) );
		mt.Suspend(60);
	}
	mt.Suspend(240);
	{
		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_CENTER_Y + 40) );
		mt.Suspend(10);
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_TOP + 40) );
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_TOP + 80) );
		mt.Suspend(30);
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_TOP + 40) );
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_TOP + 80) );
		mt.Suspend(120);

		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_CENTER_Y - 40) );
		mt.Suspend(10);
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_BOTTOM - 80) );
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_BOTTOM - 120) );
		mt.Suspend(30);
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_BOTTOM - 80) );
		g_lEnemy.Add( new CEnemyFairy1(GAME_RIGHT + 24,GAME_BOTTOM - 120) );
		mt.Suspend(120);

		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_CENTER_Y - 160 ));
		mt.Suspend(20);
		g_lEnemy.Add( new CEnemyFairy2(GAME_RIGHT + 24,GAME_TOP + 120 ));
		mt.Suspend(20);
		g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_CENTER_Y + 160 ));
		mt.Suspend(20);
		g_lEnemy.Add( new CEnemyFairy2(GAME_RIGHT + 24,GAME_BOTTOM - 120 ));
	}
	mt.Suspend(500);
	TamaToItem();
	mt.Suspend(60);

	//���{�X
	g_lEnemy.Add( new CBossRumiaMiddle() );
	while(1) {
		if( g_lEnemy.IsEmpty() )
			break;
		mt.Suspend(1);
	}
	mt.Suspend(240);

	//�㔼
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 40,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 120,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}
	mt.Suspend(60);

	g_lEnemy.Add( new CEnemyFairy4(GAME_RIGHT + 24,GAME_CENTER_Y + 120,-1));
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 40,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 40,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}
	g_lEnemy.Add( new CEnemyFairy4(GAME_RIGHT + 24,GAME_CENTER_Y - 120,1));
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 120,GAME_TOP - 16,1));
		mt.Suspend(10);
	}
	for( int i = 0; i < 8; ++i ) {
		g_lEnemy.Add( new CEnemyKedama1(GAME_RIGHT - 120,GAME_BOTTOM + 16,-1));
		mt.Suspend(10);
	}

	mt.Suspend(240);
	g_lEnemy.Add( new CEnemyFairy4(GAME_RIGHT + 24,GAME_CENTER_Y - 40,1));
	mt.Suspend(30);
	g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_CENTER_Y + 120 ));
	mt.Suspend(30);
	g_lEnemy.Add( new CEnemyFairy4(GAME_RIGHT + 24,GAME_CENTER_Y + 40,1));
	mt.Suspend(30);
	g_lEnemy.Add( new CEnemyFairy3(GAME_RIGHT + 24,GAME_CENTER_Y - 120 ));
	mt.Suspend(30);

	g_lEnemy.Add( new CEnemyFairy4(GAME_RIGHT + 24,GAME_CENTER_Y - 160,1));
	mt.Suspend(30);
	g_lEnemy.Add( new CEnemyFairy2(GAME_RIGHT + 24,GAME_TOP + 40 ));
	mt.Suspend(30);
	g_lEnemy.Add( new CEnemyFairy4(GAME_RIGHT + 24,GAME_CENTER_Y + 160,-1));
	mt.Suspend(30);
	g_lEnemy.Add( new CEnemyFairy2(GAME_RIGHT + 24,GAME_BOTTOM - 40 ));

	mt.Suspend(500);
	TamaToItem();
	mt.Suspend(60);

	//�{�X
	g_lEnemy.Add( new CBossRumia() );
	while(1) {
		if( g_lEnemy.IsEmpty() )
			break;
		mt.Suspend(1);
	}
	mt.Suspend(100);

	return 1;
}
//-------------------�d��1---------------------------------
CEnemyFairy1::CEnemyFairy1(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 20;
	this->v = 2;
	this->s = 180;
	this->color = 0;
}
int CEnemyFairy1::Run()
{
	if( count > 120 ) {
		if( this->y + GetH()/2 > GAME_TOP + GAME_HEIGHT/2 ) {
			if( s > 90 ) {
				s -= 4.5;
			}
		}
		else {
			if( s < 270 ) {
				s += 4.5;
			}
		}
	}
	if( GetDifficult() == DIFF_LUNATIC ) {
		if( count % 40 == 0 ) {
			Kodama(GetX(),GetY());
		}
	}
	x += v * Cos(s);
	y += v * Sin(s);
	return CEnemyBase::Run();
}
//-------------------�d��2---------------------------------
CEnemyFairy2::CEnemyFairy2(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 100;
	this->color = 1;
	if( this->y + GetH()/2 > GAME_TOP + GAME_HEIGHT/2 ) {
		c = 0;
	}
	else {
		c = 1;
	}
}
int CEnemyFairy2::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 120 ) {
	}
	else{
		if( c == 0 ) {
			y -= v;
		}
		else {
			y += v;
		}
		if( count == 120 || count == 180) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 1;

			CDanmakuToPlayerLine d(&tama);
			d.w= 0;
			d.n= 1;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.m = 12.0;
				d.sv = 7.0;
				d.ev = -0.5;
			}else {
				d.m = 2 + 2 * GetDifficult();
				d.sv = 2.0 + 0.5 * GetDifficult();
				d.ev = 2.0 - 0.5 * GetDifficult();
			}

			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	return CEnemyBase::Run();
}
//�A�C�e��
int CEnemyFairy2::GetItem(){
	return ITEM_POWER2;
}
//-------------------�d��3---------------------------------
CEnemyFairy3::CEnemyFairy3(double x,double y)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 120;
	this->color = 2;
}
int CEnemyFairy3::Run()
{
	const v = 2;
	if( count < 60 ) {
		x -= v;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 3;

			CDanmakuNonDirectionalLine d(&tama);
			d.offset = rand(0, 360);
			d.sv = 2;
			d.ev = 1;
			d.w = 360;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 64;
				d.m = 4;
			}else {
				d.n = 8 * ( GetDifficult() + 1 );
				d.m = 1 + GetDifficult();
			}
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	else{
		x += v;
	}
	return CEnemyBase::Run();
}
//�A�C�e��
int CEnemyFairy3::GetItem(){
	return ITEM_SCORE;
}
//-------------------�d��4---------------------------------
CEnemyFairy4::CEnemyFairy4(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 140;
	this->color = 4;
	this->c = c;
}
int CEnemyFairy4::Run()
{
	if( count < 60 ) {
		x -= 2;
	}
	else if( count < 240 ) {
		if( count == 120 ) {
			CTamaNormal tama;
			tama.x = GetX();
			tama.y = GetY();
			tama.type = 13;
			tama.color = 2;

			CDanmakuNonDirectionalLine d(&tama);
			double angle = Atan2( g_pPlayer->GetX() - GetX(), g_pPlayer->GetY() - GetY());
			d.offset = angle;
			d.sv = 3;
			d.ev = 2;
			d.w = 30;
			if( GetDifficult() == DIFF_LUNATIC) {
				d.n = 10;
				d.m = 4;
			}else {
				d.n = 2 * ( GetDifficult() + 1 );
				d.m = 1 + GetDifficult();
			}
			g_pMaterial->sndFire.Play(0);
			d.Fire();
		}
	}
	else {
		y += c * 2;
	}
	return CEnemyBase::Run();
}
//�A�C�e��
int CEnemyFairy4::GetItem(){
	return ITEM_POWER2;
}

//-------------------�ы�1---------------------------------
CEnemyKedama1::CEnemyKedama1(double x,double y,int c)
{
	this->x = x - GetW()/2;
	this->y = y - GetH()/2;
	this->hp = 10;
	this->color = 0;
	this->c = c;
}
int CEnemyKedama1::Run()
{
	this->y += c * 5;
	return CEnemyBase::Run();
}
//�ł��Ԃ��e
int CEnemyKedama1::Death() {
	if( GetDifficult() >= DIFF_HARD ) {
		Kodama(GetX(),GetY());
	}
	return CEnemyBase::Death();
}
