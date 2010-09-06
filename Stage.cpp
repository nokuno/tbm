#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//�X�e�[�W
CStage *g_pStage = NULL;

CStage::CStage(){
	count = 0;
}
CStage::~CStage(){
	StopBGM();
	SAFE_DELETE( g_pBack );
}
int CStage::Run(){
	if( !mt.IsEnd() ) {
		mt.Resume();
		count ++;
		if( mt.IsEnd() ){
			g_pSystem->state = SYSTEM_RESULT;
			g_pGame->stage ++;
			return 1;
		}
	}
	return 1;
}
int CStage::Draw(){
	return 1;
}
void CStage::Stop(){
	PauseBGM();
}
void CStage::Resume(){
	ResumeBGM();
}

SunList<CEnemy> g_lEnemy;

//-------------------�U�R����,�U�R�͂����Ȃ�---------------------------------

CEnemyBase::CEnemyBase()
{
	count = 0;
	x = y = 0;
}
CEnemyBase::~CEnemyBase(){
}
//���s
int CEnemyBase::Run(){
	//�ړ�
	if( x < GAME_LEFT - GetW() ||
		x > GAME_RIGHT ||
		y < GAME_TOP - GetH() ||
		y > GAME_BOTTOM ) {
		return 0;
	}
	return Run2();
}
//���s�i��ʊO����Ȃ��j
int CEnemyBase::Run2(){
	count++;
	//�V���b�g�Ƃ̓����蔻��
	RECT myRect = {x,y,x+GetW(),y+GetH()};
	list<CShot*>::iterator itr;
	for( itr = g_lShot.begin(); itr != g_lShot.end(); itr++ ) {
		if( *itr != NULL ) {
			RECT shotRect;
			(*itr)->GetHantei(&shotRect);
			if( GetHantei( &shotRect, &myRect )) {
				double damage = (*itr)->GetDamage();
				SAFE_DELETE( *itr );
				if( !Damage( damage ) ) {
					return 0;
				}
			}
		}
	}
	//���[�U�[�Ƃ̓����蔻��
	RECT laserRect;
	if( g_pPlayer->GetLaserHantei( &laserRect ) ) {
		if( GetHantei( &laserRect, &myRect )) {
			if( !Damage( g_pPlayer->GetLaserDamage()) ) {
				return 0;
			}
		}
	}
	//�}�X�^�[�X�p�[�N�Ƃ̓����蔻��
	double damage = g_pPlayer->GetMasterHantei(x+GetW()/2,y+GetH()/2);
	if( damage > 0 ) {
		if( !Damage( damage ) ) {
			return 0;
		}
	}
	//���@�Ƃ̔���
	if( !Hantei() ) {
		return 0;
	}
	return 1;
}
//�_���[�W
int CEnemyBase::Damage( double damage )
{
	hp -= damage;
	//�_���[�W�G�t�F�N�g�ESE������
	if( hp <= 0 ) {
		//���S�G�t�F�N�g�ESE������
		return Death();
	}
	else {
		g_pMaterial->sndHit.Play(0);
	}
	return 1;
}
//���ڂ��
int CEnemyBase::Death() {
	g_lEffect.Add( new CEffectRing( GetX(), GetY() ));
	if( GetItem() != ITEM_NONE )
		g_lItem.Add( new CItem( GetX(), GetY(), GetItem() ));
	g_pMaterial->sndExplode.Play(0);
	return 0;
}
//���@�Ƃ̓����蔻��
int CEnemyBase::Hantei()
{
	if( pow2( g_pPlayer->GetX() - GetX() ) + pow2( g_pPlayer->GetY() - GetY())
		< pow2 ( g_pPlayer->GetSize() + GetSize() )) {
		if( !g_pPlayer->GetMuteki() ) {
			g_pPlayer->Damage();
			return Damage(10);
		}
		return Damage(1);
	}
	return 1;
}
//-------------------�d������---------------------------------
//�`��
int CEnemyFairyBase::Draw()
{
	int index = (count/7) % 4;
	int c = color % 5;
	RECT rect={index*GetW(),c*GetH(),(index+1)*GetW(),(c+1)*GetH()};
	if( color <= 4 )
		g_pMaterial->imgFairy.DrawRect(x,y,rect);
	else if( color <= 8 )
		g_pMaterial->imgFairy2.DrawRect(x,y,rect);
	return 1;
}
//�A�C�e��
int CEnemyFairyBase::GetItem(){
	int type = irand(1,2);
	return type;
}


//-------------------�ыʋ���---------------------------------
//�`��
int CEnemyKedamaBase::Draw()
{
	RECT rect={color*32,0,(color+1)*32,32};
	double angle = (double)count / 5;
	g_pMaterial->imgKedama.DrawRect(x,y,rect,alpha(255),1,angle);
	return 1;
}
//�A�C�e��
int CEnemyKedamaBase::GetItem(){
	return irand(1,2);
}
//-------------------�t�[�h����---------------------------------
//�`��
int CEnemyHoodBase::Draw()
{
	int index = (count/7) % 2;
	int c = color % 5;
	RECT rect={index*GetW(),c*GetH(),(index+1)*GetW(),(c+1)*GetH()};
	g_pMaterial->imgHood.DrawRect(x,y,rect);
	//g_pMaterial->imgHood.Draw(x,y);
	return 1;
}
//�A�C�e��
int CEnemyHoodBase::GetItem(){
	return irand(1,2);
}

