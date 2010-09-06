#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CMarisa *g_pPlayer = NULL;

const int laser_volume = -1000;
const int max_bomb = 250;
const int max_shot = 20;
const int max_cutin = 60;
const int speed_fast = 5;
const int speed_slow = 2;
const int max_renda = 10;
const int suiyose_x = 400;
const int restart_time = 60;

CMarisa::CMarisa():
	imgMarisa("img/marisa.png"),
	imgHakka("img/hakka.png"),
	imgAtari("img/atari.png"),
	font("�����u�}�X�^�[�X�p�[�N�v"),
	sndLaser("snd/noise.wav"),
	sndShot("snd/shot.wav"),
	sndDamage("snd/damage.wav"),
	sndGraze("snd/graze.wav")
{
		
	SunSprite::SetOffset(0,0);
	sndLaser.SetVolume( laser_volume );
	sndShot.SetVolume( -2000 );
	sndDamage.SetVolume( -500 );
	sndGraze.SetVolume( 0 );
	
	max_kurai = 30 - 8 * GetDifficult();
	dec_kurai = 3;

	x=40;
	y=240-32;
	count=0;

	state = 0;
	atack = 0;
	slow = 0;
	muteki = 0;
	dcount = 0;
	kurai = max_kurai;
	renda = 0;
	forbit = 1;

	laser1.m_outColor = 0;
	laser1.m_lastx = GAME_RIGHT;
	laser2.m_lastx = GAME_RIGHT;
	laser3.m_lastx = GAME_RIGHT;
	laser3.m_num = 30;

}
CMarisa::~CMarisa()
{
	SunSprite::SetOffset(0,0);
}
//�ړ��E�V���b�g�E�{������
int CMarisa::Run()
{
	//��Ԃŏꍇ�킯
	switch( state ) {
	case 0: //�ʏ���
		//�ړ�
		this->Move();

		if( forbit == 0 ) {
			//�U��
			this->Attack();

			//�{��
			this->Bomb();
		}
		else if( count == 10 )
			forbit = 0;

		break;
	case 1: //�{����
		//�ړ�
		this->Move();

		//�����_���ɉ�ʂ�h�炷
		rx = rand(-5,5);
		ry = rand(-5,5);
		SunSprite::SetOffset(rx,ry);

		//�G�t�F�N�g�������蔻��p�ϐ�
		si = sin( PI * (max_bomb - dcount - 1) / max_bomb );

		//���ʉ��̉��ʂ�h�炷
		sndLaser.SetVolume(laser_volume + 1000*si);

		//�G�t�F�N�g�N���X�̃����o�͂����Őݒ肵�Ȃ��ƃ��v���C�������
		{
			double lx = GetX() + rx;
			double ly = GetY() + ry;
			laser3.m_x = lx;
			laser3.m_y = ly;
			laser3.m_a = 25.0*si;
			laser3.m_color = mono(255*si);

			laser1.m_x = lx;
			laser1.m_y = ly;
			laser1.m_ina = 0;
			laser1.m_outa = 20;
			laser1.m_inColor = alpha(200*si);

			laser2.m_x = lx;
			laser2.m_y = ly;
			laser2.m_a = 23;
			laser2.m_color = alpha(60*si);
		}

		if( dcount == 0 ) { //�I��
			state = 0;
			slow = 0;
			atack = 0;
			dcount = 0;
			sndLaser.Stop();
			SunSprite::SetOffset(0,0);
		}
		break;
	case 2: //��炢�{����t��
		//��炢�{��
		if( !this->Bomb() ){
			kurai -= dec_kurai;
		}

		//���Ԑ؂�
		if( dcount == 0 ) {
			this->Damage2();
		}
		break;
	case 3: //���X�^�[�g��
		if( dcount == 0 ) {
			dcount = 0;
			state = 0;
			muteki = 240;
		}
		break;
	}
	if( dcount > 0 )
		dcount --;
	if( muteki > 0 )
		muteki --;
	count++;

	return 1;
}
//�ړ�����
int CMarisa::Move()
{
	double v = 0;
	if( state == 1 ) //�{��
		v = 1.0;
	else if( slow == 0) {//����
		v = speed_fast;
		if( GetInputState(INP_B3) )
			slow = 1;
		if( GetInputState(INP_B0) )
			renda ++;
		else
			renda = 0;
		if( renda == max_renda ) {
			slow = 1;
			renda = 0;
		}
	}
	else {//�ᑬ
		v = speed_slow;
		if( !GetInputState(INP_B3) && !GetInputState(INP_B0) ){
			slow = 0;
			renda = 0;
		}
	}
	//�΂߈ړ�
	if((GetInputState(INP_RIGHT) || GetInputState(INP_LEFT))
		&& ( GetInputState(INP_DOWN) || GetInputState(INP_UP))) {
		v = v/sqrt(2);
	}
	double px = x;
	double py = y;
	x += v * (GetInputState(INP_RIGHT) - GetInputState(INP_LEFT));
	y += v * (GetInputState(INP_DOWN) - GetInputState(INP_UP));

	//��ʊO�̂Ƃ��͖߂�
	if( GetX() < GAME_LEFT + MARISA_CLIP ||
		GetX() > GAME_RIGHT - MARISA_CLIP )
		x = px;
	if( GetY() < GAME_TOP + MARISA_CLIP ||
		GetY() > GAME_BOTTOM - MARISA_CLIP )
		y = py;

	//��ʉE�ŃA�C�e�����
	if( GetX() > suiyose_x || state == 1)
		ItemSuiyose();
	return 1;
}
//�U������
int CMarisa::Attack()
{
	//�U����Ԃŏꍇ�킯
	switch( atack ) {
	case 0: //�Ȃ�
		//�V���b�g
		if( GetInputState(INP_B0) || GetInputState(INP_B2)) {
			atack = 1;
			dcount = 0;
		}
		break;
	case 1: //�V���b�g
		if( GetInputHanasi(INP_B0) || GetInputHanasi(INP_B2)) {
			dcount = max_shot;
		}
		if( dcount == 1 ) {
			atack = 0;
			dcount = 0;
		}
		//�T�u�V���b�g
		int c = 8 - ( g_pSystem->life == 0 ) - ( g_pSystem->bomb == 0 );
		if( count % c == 2 ) {
			const double m = 8 + 10 * g_pSystem->power / 1000;
			sndShot.Play(0);
			for( int i=0; i<m; ++i) {
				for( int j = 0; j < 2; ++j ) {
					double dy = j ? -1 : 1;
					double da = (i*2-m+1);
					g_lShot.Add(new CSubShot( GetX(), GetY()+8.0*dy+4.0*da, 2.0*da));
				}
			}
		}
		//���C���V���b�g
		if( slow == 0 ) {
			if( count%3 == 1 ) {
				g_lShot.Add(new CMainShot(GetX(),GetY()));
			}
		}
		break;
	}
	return 1;
}
//�{��
int CMarisa::Bomb()
{
	//�{������
	if( GetInputEvent(INP_B1) && g_pSystem->bomb > 0){
		state = 1;
		muteki = 0;
		dcount = max_bomb;
		g_pSystem->bomb--;
		sndLaser.Play(DSBPLAY_LOOPING);
		return 0;
	}
	return 1;
}
//�`�����
int CMarisa::Draw()
{
	if( state == 0 ) {
		int a = 0;
		//�ᑬ�G�t�F�N�g
		if( slow == 1 ) {
			Sun3D.SetBlendingType(BLEND_BRILLIANT);
			imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1,(double)count/30);
			imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1,(double)count/30);
			a = 200;
			//���[�U�[
			if( atack == 1) {
				Sun3D.SetBlendingType(BLEND_ADD);
				SunEffectLineLaser line;
				line.m_sx = GetX();
				line.m_sy = GetY();
				line.m_ex = GAME_RIGHT;
				line.m_ey = GetY();
				line.m_iw = 0;
				line.m_ow = 10;
				line.m_inColor = argb( a, 255, 255, 255 );
				line.m_outColor = argb( a/2, 0, 100, 255 );
				line.Draw();
			}
			Sun3D.SetBlendingType(BLEND_NORMAL);
		}
	}
	if( state != 3 ) {
		//������
		if( g_pLety==NULL || g_pLety->freeze == 0 ) {
			int index = (count/7) % 4;
			D3DCOLOR color = COLOR_WHITE;
			if( muteki % 10 > 5  )
				color = alpha(125);
			RECT rect={index*64,0,(index+1)*64,64};
			imgMarisa.DrawRect(x,y,rect,color);
			if( slow == 1 || state == 1) {
				Sun3D.SetBlendingType(BLEND_NORMAL);
				imgAtari.Draw( GetX() - 4, GetY() - 4 );
			}
		} else {
			g_pMaterial->imgMarisaFreeze.Draw(x,y);
		}
	}
	return 1;
}
//�`��2�i�}�X�^�[�X�p�[�N�E��炢�{���G�t�F�N�g�j
int CMarisa::Draw2()
{
	//�}�X�^�[�X�p�[�N
	if( state == 1 ){
		/*//�Ó]
		SunEffectRect effect;
		SetRect(&effect.m_rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
		effect.m_color = argb(128,0,0,0);
		effect.Draw();*/

		//���k
		Sun3D.SetBlendingType(BLEND_ADD);
		laser1.Draw();

		//���k
		laser2.Draw();

		//�J�b�g�C��
		if( dcount > max_bomb - max_cutin ) {
			RECT rect;
			int c = max_bomb - dcount;
			SetRect(&rect,128+8,0,512-8,384);
			g_pMaterial->imgCutin.DrawRect(320-184,120*(max_cutin-c)/max_cutin,rect,alpha(255*sin(PI*(c-1)/max_cutin)));
			SetRect(&rect,128+8,384,320,512);
			g_pMaterial->imgCutin.DrawRect(20,140+120*(max_cutin-c)/max_cutin,rect,alpha(255*sin(PI*(c-1)/max_cutin)));
			SetRect(&rect,320,384,512-8,512);
			g_pMaterial->imgCutin.DrawRect(640-40-184,140+120*(max_cutin-c)/max_cutin,rect,alpha(255*sin(PI*(c-1)/max_cutin)));
		}
		//�O�k
		Sun3D.SetBlendingType(BLEND_BRILLIANT);
		laser3.Draw();

		//���T�F
		imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1.0+1.0*si,(double)count/10);
		imgHakka.Draw(GetX()-64,GetY()-64,alpha(255),1.0+1.0*si,(double)count/10);
		Sun3D.SetBlendingType(BLEND_NORMAL);
		//�t�H���g
		font.DrawFormat(55,55,xrgb(250,30,140));
	}
	if( state == 2 ) {
		g_pMaterial->imgHolic.Draw(GetX()-256,GetY()-256,alpha(255),1.0*dcount/max_kurai);
	}
	return 1;
}
//�}�X�^�[�X�p�[�N�̓����蔻��i�߂�l�F�_���[�W�j
double CMarisa::GetMasterHantei(int x,int y)
{
	double damage = 0;
	//�{�����ł͂Ȃ�
	if( state != 1) {
		return 0;
	}
/*	//�œ��k
	if( x - laser1.m_x >
		(laser1.m_lastx - laser1.m_x)/pow2(15*laser1.m_outa)*pow2(y-laser1.m_y)){
		damage += si*3.0;
	}
*/	//�ŊO�k
	if( x - laser3.m_x >
		(laser3.m_lastx - laser3.m_x)/pow2(15*laser3.m_a)*pow2(y-laser3.m_y)){
		damage += 3;
	}
	//���T�~
	if( pow2( x - GetX() ) + pow2( y - GetY() ) < pow2( (1+si)*64 ) ) {
		damage += 1;
	}
	return damage;
}
//�}�X�^�[�X�p�[�N�̒e��������
int CMarisa::GetMasterHantei2(int x,int y)
{
	//�{�����ł͂Ȃ�
	if( state != 1) {
		return 0;
	}
	//�ŊO�k
	if( x - laser3.m_x >
		(laser3.m_lastx - laser3.m_x)/pow2(15*laser3.m_a)*pow2(y-laser3.m_y)){
		return 1;
	}
	//���T�~
	if( pow2( x - GetX() ) + pow2( y - GetY() ) < pow2( (1+si)*64 ) ) {
		return 1;
	}
	return 0;
}
//���G����
int CMarisa::GetMuteki(){
	if( state ==0 && muteki == 0 )
		return 0;
	return 1;
}
//���[�U�[�̓����蔻��
int CMarisa::GetLaserHantei(RECT *pRect){
	if( state == 0 && slow == 1 && atack == 1) {
		SetRect(pRect,GetX(),GetY()-10,GAME_RIGHT,GetY()+10);
		return 1;
	}
	return 0;
}
//�_���[�W
void CMarisa::Damage(){
	//���ʉ�
	sndDamage.Play(0);
	//��������(��炢�{����t���ցj
	dcount = kurai;
	state = 2;
}
//�^�_���[�W�i��炢�{�����s�j
void CMarisa::Damage2() {
	//�_���[�W
	g_pSystem->Damage();
	if( g_pBoss != NULL )
		g_pBoss->dirty = 1;
	//���������i���X�^�[�g�ցj
	dcount = restart_time;
	state = 3;
	kurai = max_kurai;
	atack=0;
	slow=0;
}
void CMarisa::Stop(){
	SunSprite::SetOffset(0,0);
	sndLaser.SetVolume(DSBVOLUME_MIN);
}
double CMarisa::GetX(){
	return x + 44;
}
double CMarisa::GetY(){
	return y + 40;
}
void CMarisa::GetItem(int type){
	double bairitu = 1.0 + (x>suiyose_x?suiyose_x:x) /suiyose_x;
	switch(type){
	case 0://��
		g_pSystem->AddScore(SCORE_ITEMTAMA);
		break;
	case 1://�p���[�A�b�v
		g_pSystem->AddPower(10);
		break;
	case 2://�_
		g_pSystem->AddScore(SCORE_ITEM1*bairitu);
		g_pSystem->item ++;
		break;
	case 3://�p���[�A�b�v(��j
		g_pSystem->AddPower(30);
		break;
	case 4://�{��
		g_pSystem->AddBomb(1);
		break;
	case 5://�t���p���[
		g_pSystem->AddPower(1000);
		break;
	case 6://�PUP
		g_pMaterial->sndLife.Play(0);
		g_pSystem->AddLife(1);
		break;
	case ITEM_SCORE2: //�X�R�A�i��j
		g_pSystem->AddScore(SCORE_ITEM2*bairitu);
		g_pSystem->item ++;
		break;
	case ITEM_SCORE0: //�X�R�A�i���j
		g_pSystem->AddScore(SCORE_ITEM0);
		g_pSystem->item ++;
		break;
	}
}

//�O���C�Y
int CMarisa::Graze(){
	sndGraze.Play(0);
	g_pSystem->AddGraze(1);
	g_pSystem->AddScore(SCORE_GRAZE);
	return 1;
}
//�����蔻��̃T�C�Y
double CMarisa::GetSize(){
	return 2.0;
}
//�O���C�Y�̃T�C�Y
double CMarisa::GetGrazeSize() {
	return 30.0;
}
//��bON
void CMarisa::Talk(){
	forbit = 1;
	atack = 0;
}
//��bOFF
void CMarisa::Resume(){
	forbit = 0;
}
//���Z�b�g
void CMarisa::ResetState(){
	ResetPosition();

	count=0;
	state = 0;
	atack = 0;
	slow = 0;
	muteki = 0;
	dcount = 0;
	kurai = max_kurai;
	renda = 0;
	forbit = 0;
	sndLaser.Stop();
	SunSprite::SetOffset(0,0);
}
//�ʒu������
void CMarisa::ResetPosition(){
	x=40;
	y=240-32;
}