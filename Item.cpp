#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

SunList<CItem> g_lItem;

//�S�ẴA�C�e�����z���񂹂�
int ItemSuiyose(){
	list<CItem*>::iterator itr;
	for( itr = g_lItem.begin(); itr != g_lItem.end(); itr++ ) {
		if( *itr != NULL ) {
			(*itr)->suiyose = 1;
		}
	}
	return 1;
}

//�A�C�e���N���X�itype�Ō^�w��j
CItem::CItem(double x,double y,int type){
	this->x = x;
	this->y = y;
	this->type = type;
	this->count = 0;

	//�t���p���[�̂Ƃ��̓p���[�A�b�v�A�C�e�����S�ē��_�A�C�e����
	if( g_pSystem->power >= 1000 ) {
		if( type == ITEM_POWER )
			this->type = ITEM_SCORE;
		else if( type == ITEM_POWER2 || type == ITEM_FULLPOWER )
			this->type = ITEM_SCORE2;
	}

	vx = -1;
	ax = -0.1;

	//���͎����z����
	if( type == ITEM_STAR )
		suiyose = 1;
	else
		suiyose = 0;
}
CItem::~CItem(){
}
int CItem::Run(){
	count++;
	double r = sqrt( pow2( g_pPlayer->GetX() - x ) + pow2( g_pPlayer->GetY() - y ) );
	if( suiyose == 0 || g_pPlayer->state == 3) {
		{
			//���R����
			vx += ax - 0.02 * vx;
			x += vx;
			if( x < GAME_LEFT - 48 )
				return 0;
		}
	}
	else {
		//�z���񂹂���
		double v = 10.0 / sqrt( pow2( g_pPlayer->GetX() - x ) + pow2( g_pPlayer->GetY() - y ) );
		x += v * ( g_pPlayer->GetX() - x );
		y += v * ( g_pPlayer->GetY() - y );
	}
	if( r < g_pPlayer->GetSize() + GetSize() ) {
		g_pMaterial->sndItem.Play(0);
		g_pPlayer->GetItem( type );
		return 0;
	}
	return 1;
}
int CItem::Draw(){
	RECT rect;
	switch(type){
	case ITEM_STAR://��
		SetRect( &rect, 32, 0, 48, 16);
		break;
	case ITEM_POWER://�p���[�A�b�v
		SetRect( &rect, 0, 0, 16, 16);
		break;
	case ITEM_SCORE://�_
		SetRect( &rect, 16, 0, 32, 16);
		break;
	case ITEM_POWER2://�p���[�A�b�v(��j
		SetRect( &rect, 0, 16, 24, 40);
		break;
	case ITEM_BOMB://�{��
		SetRect( &rect, 48, 16, 72, 40);
		break;
	case ITEM_FULLPOWER://�t���p���[
		SetRect( &rect, 72, 16, 96, 40);
		break;
	case ITEM_1UP://�PUP
		SetRect( &rect, 96, 0, 128, 32);
		break;
	case ITEM_SCORE2://�X�R�A�i��j
		SetRect( &rect, 24, 16, 48, 40);
		break;
	case ITEM_SCORE0://�X�R�A�i���j
		SetRect( &rect, 88, 0, 96, 8);
		break;
	}
	double X = x - (rect.right-rect.left)/2;
	double Y = y - (rect.bottom-rect.top)/2;
	g_pMaterial->imgItem.DrawRect(X,Y,rect);
	return 1;
}
double CItem::GetSize() {
	double result;
	switch(type){
	case ITEM_STAR://��
		result = 8;
		break;
	case ITEM_POWER://�p���[�A�b�v
		result = 8;
		break;
	case ITEM_SCORE://�_
		result = 8;
		break;
	case ITEM_POWER2://�p���[�A�b�v(��j
		result = 12;
		break;
	case ITEM_BOMB://�{��
		result = 12;
		break;
	case ITEM_FULLPOWER://�t���p���[
		result = 12;
		break;
	case ITEM_1UP://�PUP
		result = 16;
		break;
	case ITEM_SCORE2://�X�R�A�i��j
		result = 12;
		break;
	case ITEM_SCORE0://�X�R�A�i���j
		result = 4;
		break;
	}

	if( suiyose == 0 )
		result += 32;
	return result;
}