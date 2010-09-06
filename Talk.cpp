#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

//-------------------�⏕�֐�---------------------------------
int Next(){
	if( GetInputEvent( INP_B0 ))
		return 1;
	if( GetInputEvent( INP_B2 ))
		return 1;
	if( GetInputState( INP_B6 ))
		return 1;
	return 0;
}
void DrawMarisa(int x,int y,int face,int manfu,D3DCOLOR color){
	if(g_pTitle->otaku && face >= 0) {
		g_pMaterial->imgMarisaBody.DrawClip(x,y,color);
		if( face > 0 ) {
			int fx = face % 3;
			int fy = face / 3;
			RECT rect = {72*fx,64*fy,72*(fx+1),64*(fy+1)};
			g_pMaterial->imgMarisaFace.DrawRectClip(x+288,y+88,rect,color);
		}
	}
}
//-------------------��b����---------------------------------
CTalk::CTalk(){
	text = "";
	chara = 0;
	face = 0;
	manfu = -1;
}
int CTalk::IsEnd(){
	return mt.IsEnd();
}
int CTalk::IsSuspended(){
	return mt.IsSuspended();
}
void CTalk::Resume(){
	mt.Resume();
}
void CTalk::Talk(){
	if( g_pTitle->talk ) {
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		do{
			mt.Suspend();
			int mae = (chara != 0 && chara != 2);
			D3DCOLOR color = mono(128+127*(!mae));
			int cx = - mae * 20;
			int cy = mae * 20;
			DrawMarisa(-200+cx,60+cy,face,manfu,color);
			RECT rect;
			switch( manfu ) {
			case 0://��
				SetRect(&rect,0,0,48,64);
				break;
			case 1://��
				SetRect(&rect,48,0,128,64);
				break;
			case 2://��
				SetRect(&rect,128,0,160,64);
				break;
			case 3://�{
				SetRect(&rect,160,0,208,64);
				break;
			default:
				break;
			}
			if( manfu != -1 )
				g_pMaterial->imgManfu.DrawRectClip(40+cx,90+cy,rect,color);
			if( g_pTitle->otaku )
				DrawEnemy();
			flame.Draw();

			D3DCOLOR c1 = (chara==1) ? ENEMY_TALK_COLOR : MARISA_TALK_COLOR;
			D3DCOLOR c2 = ENEMY_TALK_COLOR;
			font.DrawFormat(100,370,c1);
			font2.DrawFormat(100,400,c2);
			fntEnemy.DrawFormat(500-16*fntEnemy.m_str.size()/2,340,ENEMY_NAME_COLOR);
		}while( !Next() );
	}
}
//-------------------���[�~�A---------------------------------
int CTalkRumia1::Start(){
	TSunFunctionCallback<CTalkRumia1> fn;
	fn.SetFunction(this,&CTalkRumia1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkRumia1::Thread(){

	chara = 0;
	face = 0;
	font.CreateList("�������������������邺");
	fntEnemy.CreateList("");
	Talk();

	chara = 1;
	font.CreateList("���[�Ȃ̂��[");
	fntEnemy.CreateList("���ł̗d�� ���[�~�A",16);
	Talk();

	StopBGM();

	chara = 0;
	font.CreateList("���[�~�A����Ȃ����A�����炢���񂾁H\n�S�R�C�t���Ȃ��������B");
	Talk();

	chara = 1;
	font.CreateList("��������������ǁA\n�Ђ���Ƃ��Ē��ځH");
	Talk();

	chara = 0;
	face = 2;
	manfu = 1;
	font.CreateList("�Ђ���Ƃ��Ȃ��Ă����ڂ����B");
	Talk();

	chara = 1;
	font.CreateList("����͎��̂��������ǁA\n�����͎̂��̂�������Ȃ���");
	Talk();

	chara = 0;
	face = 1;
	manfu = 1;
	font.CreateList("���������������\n����������ƃ}�V�Ȃ񂾂���");
	Talk();

	chara = 1;
	font.CreateList("�����͂��₾�[�A\n�������������ɂȂ�[");
	Talk();

	chara = 0;
	face = 2;
	manfu = 2;
	font.CreateList("���Ƃ��A\n������Ȃ��̂��H");
	Talk();

	chara = 1;
	font.CreateList("�ށ[�A\n���̃��{����������΁c");
	Talk();

	chara = 0;
	face = 1;
	manfu = -1;
	font.CreateList("�����Ƃ������{����\n�v���[���g���Ă�낤���H");
	Talk();

	PlayBGM("tbm_rumia");
	g_lEffect3.Add( new CEffectFont("��E�B���^�[�N�[���[",bgmFontColor,380,420) );
	return 1;
}
void CTalkRumia1::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	g_pStage1->imgCutin.DrawClip(270+(chara != 1)*20,120+(chara != 1)*20,c);
}
//-------------------���[�~�A2---------------------------------
int CTalkRumia2::Start(){
	TSunFunctionCallback<CTalkRumia2> fn;
	fn.SetFunction(this,&CTalkRumia2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkRumia2::Thread(){
	chara = 0;
	face = 1;
	font.CreateList("��������܂�7��ނ��邪�A�ǂꂪ�����H");
	font2.CreateList("");
	Talk();

	return 1;
}
void CTalkRumia2::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	g_pStage1->imgCutin.DrawClip(270+(chara != 1)*20,120+(chara != 1)*20,c);
}
//-------------------���O��---------------------------------
int CTalkWriggle1::Start(){
	TSunFunctionCallback<CTalkWriggle1> fn;
	fn.SetFunction(this,&CTalkWriggle1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkWriggle1::Thread(){

	chara = 0;
	face = 0;
	enemy_face = 0;
	font.CreateList("���[\n����͒���������");
	font2.CreateList("");
	fntEnemy.CreateList("");
	Talk();

	chara = 1;
	font.CreateList("����́A�������邩���I");
	fntEnemy.CreateList("峎t ���O��",16);
	Talk();

	StopBGM();

	chara = 0;
	font.CreateList("��̒��͌��̂���Ƃ���ɏW�܂�B\n�����Ă���ȂɋP���Ă������H");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("�U�铔�����񂽂́B\n�����W�܂�̂͌�����Ȃ��Ď��O�������B");
	Talk();

	chara = 0;
	face = 2;
	manfu = 1;
	font.CreateList("�ǂ����̌ӎU�L���d���ƈꏏ�ɂ��Ȃ��ł���B\n�E���܂̂ق������͂����B");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�E���܂͌���");
	Talk();

	chara = 0;
	face = 1;
	manfu = 1;
	font.CreateList("���O���̂ق����������A�����̑�G��");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("�Ƃ���ŁA�N�������H");
	Talk();

	chara = 0;
	face = 2;
	manfu = 2;
	font.CreateList("���͕��ʂ̖��@�g���A���J���������B\n�����ɂ͊o�����܂��B");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("�Ȃɂ�A�������Ēɂ��ڂɑ��킳�ꂽ\n���肮�炢�o���Ă�����");
	Talk();

	chara = 0;
	face = 1;
	manfu = -1;
	font.CreateList("�挊���@�����ȁH\n������ɂ��ڂɑ��킹�Ă�낤");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("峂ɕ�΂͕K�v�Ȃ��B");
	Talk();

	chara = 1;
	font.CreateList("���R�ɐ��܂�A���R�Ɋ҂�峂̗́A\n�Ƃ��Ɩ���킹�Ă������I");
	Talk();

	chara = 0;
	face = 0;
	manfu = -1;
	font.CreateList("����܂��A���܂�������Ȃ��Ȃ�");
	Talk();

	PlayBGM("tbm_wriggle");
	g_lEffect3.Add( new CEffectFont("���K�N�\���T���",bgmFontColor,380,420) );
	return 1;
}
void CTalkWriggle1::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	switch( enemy_face ) {
	case 0:
		g_pStage2->imgCutin.DrawClip(360+(chara != 1)*20,100+(chara != 1)*20,c);
		break;
	case 1:
		SetRect(&rect,0,0,256,438);
		g_pStage2->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,256,0,512,438);
		g_pStage2->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}
//-------------------���O��2---------------------------------
int CTalkWriggle2::Start(){
	TSunFunctionCallback<CTalkWriggle2> fn;
	fn.SetFunction(this,&CTalkWriggle2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkWriggle2::Thread(){

	chara = 0;
	face = 0;
	manfu = -1;
	font.CreateList("�E���܂��g���߂���΋�Ɍ����J���A\n���O���𑝂₷�B");
	font2.CreateList("");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("�w�Z�ŏK��Ȃ��������H\n�܂��K���Ă��Y�ꂽ���A���F�������ȁB");
	Talk();

	return 1;
}
void CTalkWriggle2::DrawEnemy(){
	D3DCOLOR c = mono(128+127*(chara==1));
	g_pStage2->imgCutin.DrawClip(360+(chara != 1)*20,100+(chara != 1)*20,c);
}

//-------------------�`���m1---------------------------------
int CTalkChirno1::Start(){
	TSunFunctionCallback<CTalkChirno1> fn;
	fn.SetFunction(this,&CTalkChirno1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno1::Thread(){

	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("����񂾂�H\n�o�Ă�����B");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 3;
	font.CreateList("�ȁA�Ȃ�ŕ��������̂�c�c");
	fntEnemy.CreateList("�X�� �`���m",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("�d���������̂ɂ́A�O�G�ꂪ����񂾂��A\n�m��Ȃ��̂��H");
	Talk();

	return 1;
}
void CTalkChirno1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,300,512);
	g_pStage3->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,128);
		g_pStage3->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,128,512,256);
		g_pStage3->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,256,512,256+128);
		g_pStage3->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}

//-------------------�`���m2---------------------------------
int CTalkChirno2::Start(){
	TSunFunctionCallback<CTalkChirno2> fn;
	fn.SetFunction(this,&CTalkChirno2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno2::Thread(){

	chara = 0;
	face = 3;
	enemy_face = -1;
	font.CreateList("�ށA�N���I");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 2;
	font.CreateList("����������������Ȃ��I");
	fntEnemy.CreateList("�X�� �`���m",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("�������������H");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�ӂ�A\n���̊����œ������������Ȃ����񂶂�Ȃ��H");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("�Ƃ���ł��̊����́c�c");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("�ӂ��ӂ���");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("���O�ȊO�̒N�̎d�Ƃ��H");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�ނ�");
	Talk();

	chara = 0;
	face = 2;
	font.CreateList("���āA�m��Ȃ���������\n�����������܂��͉ĈȊO�o�v�֎~�Ȃ񂾂�");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("��K�}���~�����Ă��鍡�����A\n�������􂷂�`�����X�Ȃ̂�I");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("�܂�������B");
	Talk();
	face = 1;
	font.CreateList("�R�~�R�́H");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�H�I");
	Talk();

	StopBGM();
	PlayBGM("tbm_otenba");
	g_lEffect3.Add( new CEffectFont("��o�J�ƃo�J�̋��E",bgmFontColor,380,420) );

	return 1;
}
//-------------------�`���m3---------------------------------
int CTalkChirno3::Start(){
	TSunFunctionCallback<CTalkChirno3> fn;
	fn.SetFunction(this,&CTalkChirno3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno3::Thread(){

	chara = 0;
	face = 4;
	enemy_face = 2;
	font.CreateList("�Ȃ񂾁A���O�̎d�Ƃ���Ȃ��̂��c�c");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�ŏ����番�����Ă��ł���I");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("�͂��͂���");
	Talk();

	return 1;
}

//-------------------�~�X�e�B�A1---------------------------------
int CTalkMystia1::Start(){
	TSunFunctionCallback<CTalkMystia1> fn;
	fn.SetFunction(this,&CTalkMystia1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkMystia1::Thread(){
	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("���[������ȁA���̏ꏊ��");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 2;
	font.CreateList("����`����`�����`�̎��`��");
	fntEnemy.CreateList("�݂݂����̖� �~�X�e�B�A",16);
	Talk();

	chara = 0;
	face = 4;
	font.CreateList("�Ȃ񂾁A�鐝�̑���");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("���C�ȏ�́`�����ȁ`��");
	Talk();

	chara = 0;
	face = 3;
	manfu = 3;
	font.CreateList("�̂��ȁI");
	Talk();

	return 1;
}
void CTalkMystia1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,256+32,512);
	g_pStage4->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,128);
		g_pStage4->imgTalk.DrawRectClip(32+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,128,512,256);
		g_pStage4->imgTalk.DrawRectClip(32+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,256,512,384);
		g_pStage4->imgTalk.DrawRectClip(32+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}
//-------------------�~�X�e�B�A2---------------------------------
int CTalkMystia2::Start(){
	TSunFunctionCallback<CTalkMystia2> fn;
	fn.SetFunction(this,&CTalkMystia2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkMystia2::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 2;
	font.CreateList("�˂�");
	fntEnemy.CreateList("�݂݂����̖� �~�X�e�B�A",16);
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("��H");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("���Ȃ��A�l�ԁH");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("������A����������");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("������͐l�Ԃ�������Ă��Ăق������̂�");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("�����l�Ԃ��B");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("�����̂ɐl�ԁH");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("�����l�Ԃ���");
	Talk();
	font.CreateList("�Ƃ����Ă�����H�ׂ�C�Ȃ�A\n��Q�͕⏞���Ȃ�����");
	Talk();

	chara = 1;
	enemy_face = 2;
	font.CreateList("�����l�Ԃ͂ǂ�������������̂�����H");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("�����ƁA�ڂ̑O�̒���������������������@�́c");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�ڂ��J���Ă��^���Âɂ��Ă��I");
	Talk();

	StopBGM();
	PlayBGM("tbm_mystia");
	g_lEffect3.Add( new CEffectFont("������т���",bgmFontColor,380,420) );

	return 1;
}
//-------------------�~�X�e�B�A3---------------------------------
int CTalkMystia3::Start(){
	TSunFunctionCallback<CTalkMystia3> fn;
	fn.SetFunction(this,&CTalkMystia3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkMystia3::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("���̃J���X�Ɍ���ꂽ�ʂ�A\n�߂Â��Ȃ���Ηǂ�������");
	Talk();

	return 1;
}
//-------------------��1---------------------------------
int CTalkChen1::Start(){
	TSunFunctionCallback<CTalkChen1> fn;
	fn.SetFunction(this,&CTalkChen1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChen1::Thread(){
	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		mt.Suspend();
	}
	return 1;
}
void CTalkChen1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,320,512);
	g_pStage5->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-96,0,512,64);
		g_pStage5->imgTalk.DrawRectClip(360+64+(chara != 1)*20,100+64+(chara != 1)*20,rect,c);
		break;
	}
}
//-------------------��2---------------------------------
int CTalkChen2::Start(){
	TSunFunctionCallback<CTalkChen2> fn;
	fn.SetFunction(this,&CTalkChen2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChen2::Thread(){
	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("���[�����́A���ʂɒg�����ȁB\n�����Ŋۂ��Ȃ肽����");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("����`�I\n����ȂƂ���ŉ������Ă���́I");
	fntEnemy.CreateList("�u���b�N�L���b�g ��",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("���ʂɖ\��Ă��邾������");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("���̂����łʂ��ʂ����ז��������͏d����B\n�����S����");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("�����͕��ʂɈ������Ƃ������ȁA\n���l�тɗV��ł���");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�Ɛl���N���A�����Ȃ��́H");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("���ʂɂ��̕K�v�͖����ȁB\n�ڂ̑O�̉��L��ގ����邾����");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("���̊i�D�ŉA�z�t���C������H");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("������A���ʂɖ����n���^�[��");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�l�Ԃ̂����ɁI");
	Talk();

	PlayBGM("tbm_chen");
	return 1;
}
//-------------------��3---------------------------------
int CTalkChen3::Start(){
	TSunFunctionCallback<CTalkChen3> fn;
	fn.SetFunction(this,&CTalkChen3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChen3::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("��̉��Ȃ́H");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("���́A���ʂ���");
	Talk();
	return 1;
}

//-------------------���e�B1---------------------------------
int CTalkLety1::Start(){
	TSunFunctionCallback<CTalkLety1> fn;
	fn.SetFunction(this,&CTalkLety1::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkLety1::Thread(){
	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		mt.Suspend();
	}
	return 1;
}
void CTalkLety1::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	int mae = (chara != 1 && chara != 2);
	D3DCOLOR c = mono(128+127*(!mae));
	RECT rect;
	SetRect(&rect,0,0,256,512);
	g_pStage6->imgTalk.DrawRectClip(360+mae*20,100+mae*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,96);
		g_pStage6->imgTalk.DrawRectClip(64+360+mae*20,64+100+mae*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,96,512,96*2);
		g_pStage6->imgTalk.DrawRectClip(64+360+mae*20,64+100+mae*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,96*2,512,96*3);
		g_pStage6->imgTalk.DrawRectClip(64+360+mae*20,64+100+mae*20,rect,c);
		break;
	}
}
//-------------------���e�B2---------------------------------
int CTalkLety2::Start(){
	TSunFunctionCallback<CTalkLety2> fn;
	fn.SetFunction(this,&CTalkLety2::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkLety2::Thread(){
	chara = 0;
	face = 0;
	enemy_face = -1;
	font.CreateList("�����A���낻�덕���ɂ����Ŋ肨����");
	Talk();

	for( int i = 0; i < 46; i++ ) {
		g_pBoss->Appear();
		SunEffectRect flame;
		flame.m_color = argb(127,0,0,0);
		SetRect(&flame.m_rect,GAME_LEFT+60,350,GAME_RIGHT-60,430);
		D3DCOLOR color = mono(128+127*(chara==0));
		int cx = - (chara != 0) * 20;
		int cy = (chara != 0) * 20;
		DrawMarisa(-200+cx,60+cy,face,manfu,color);
		flame.Draw();
		if( !GetInputState( INP_B6 ) )
			mt.Suspend();
	}

	chara = 1;
	enemy_face = 0;
	font.CreateList("����A���Ȃ��͂��̔������P���I�[������\n����������́H");
	fntEnemy.CreateList("�~�̗��Ƃ��q ���e�B",16);
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("���������ċP����񂾂��B\n�m��Ȃ��̂��H");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�P���̂͂��Ȃ�����Ȃ��āA\n���Ȃ��̖��@�ł��傤�H");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("��Ȃ��Ȃ��B\n����Ȃ����炱��Ȋ����~�ɂ����܂��񂾂��H");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("���͂����ʂ�~���^�񂾂����B\n�ޏ����o�Ă��Ȃ��̂������̏؋���");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("���͖��@�g�������A\n���������̂����@�Ȃ�ė[�ёO��");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("���Ȃ��B�l�Ԃ́A�����΂���ߎ���ɂȂ��Ă���B\n�����͗d�������K������ǂ��H");
	Talk();

	chara = 0;
	face = 0;
	font.CreateList("�d���͑ގ�������̂���");
	Talk();

	chara = 1;
	enemy_face = 1;
	font.CreateList("�d���͎��R���i��c�c");
	Talk();

	chara = 1;
	enemy_face = 0;
	font.CreateList("���R�Ɉˑ�����l�Ԃ́A\n���R�̂����ؕԂ���H�炤���ƂɂȂ�");
	Talk();

	chara = 2;
	enemy_face = 1;
	face = 1;
	font.CreateList("���̖��@��H��킹�Ă�낤�A�~�̗d��");
	font2.CreateList("�����~�𖡂�킹�Ă������A���@�g��");
	Talk();

	PlayBGM("tbm_lety");
	return 1;
}
//-------------------���e�B3---------------------------------
int CTalkLety3::Start(){
	TSunFunctionCallback<CTalkLety3> fn;
	fn.SetFunction(this,&CTalkLety3::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkLety3::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("");
	Talk();
	return 1;
}

//-------------------�G���f�B���O---------------------------------

CEnding::CEnding(){
	Init();
}
int CEnding::Init(){
	for( int i = 0; i < SERIF_NUM; i++){
		serif[i].c = NARRATION_COLOR;
		serif[i].x = 100;
		serif[i].y = 320+30*i;
		serif[i].font.Create("");
	}
	return 1;
}
void CEnding::Talk(){
	if( g_pTitle->talk ) {
		do{
			mt.Suspend();
			imgBack.Draw(0,0);
			for( int i = 0; i < SERIF_NUM; i++){
				serif[i].font.DrawFlame(serif[i].x,serif[i].y,serif[i].c);
			}
		}while( !Next() );
	}
}
int CEnding::Start(){
	TSunFunctionCallback<CEnding> fn;
	fn.SetFunction(this,&CEnding::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
void CEnding::DrawEnemy(){}
int CEnding::Thread(){
	PlayBGM("tbm_end");

//	g_pSystem->continue_dirty = 1;
	if( g_pSystem->continue_dirty == 0 ) {
		imgBack.LoadTexture("img/ending01.png");

		serif[0].font.Create("����_�ЁB");
		Talk();
		serif[1].font.Create("�ꎞ�͌��z�����𖄂ߐs������Ƃ��Ă������Ⴊ�~�݁A");
		Talk();
		serif[2].font.Create("��������Ă����Y�N��������I����āA");
		Talk();
		serif[3].font.Create("�����������������̂悤�ɐ������}���Ă悤�Ƃ��Ă����B");
		Talk();
		Init();
		serif[0].font.Create("�얲�u�ǂ��������̐����񂵂�����A����H�v");
		Talk();
		Init();

		imgBack.LoadTexture("img/ending02.png");

		serif[0].font.Create("�������u�ڂ��������Č���ꂽ��łȁA");
		Talk();
		serif[1].font.Create("      �r���ō������Ɋ���Ď؂�Ă����B");
		Talk();
		serif[2].font.Create("      �������ʂ܂ł̊ԂȁB");
		Talk();
		serif[3].font.Create("      �����͎���蒷�������邾�낤���v");
		Talk();
		Init();
		serif[0].font.Create("�얲�u���񂽂̏ꍇ�A");
		Talk();
		serif[1].font.Create("      �O���邩�󂷂�����ق��������������ǂˁv");
		Talk();
		Init();

		serif[0].font.Create("�얲�u�ŁA�����́H�v");
		Talk();

		imgBack.LoadTexture("img/ending03.png");
		
		serif[1].font.Create("�������u����̍��������v");
		Talk();
		serif[2].font.Create("�얲�u�������āc�c�����������́H�v");
		Talk();
		serif[3].font.Create("���e�B�u�����Ȃ���v");
		Talk();
		Init();

		serif[0].font.Create("�������u�܁A�����������Ƃ��B");
		Talk();
		serif[1].font.Create("      ���ƁA���낻�뎞�Ԃ��ȁv");
		Talk();
		serif[3].font.Create("�w�V�N�����܂��Ă��߂łƂ��I�x");
		Talk();
		Init();

		serif[0].font.Create("���z���̓~�͌������B");
		Talk();
		serif[1].font.Create("���z���ɐ��ޏ����B�͓K�x�ɉ^�������Ȃ����");
		Talk();
		serif[2].font.Create("�̂�����t���Ă��܂��̂�������Ȃ��B");
		Talk();
		if( GetDifficult() != DIFF_LUNATIC )
			serif[3].font.Create("Good End                  �N���A�[���߂łƂ��I");
		else
			serif[3].font.Create("Good End                  ���񂽂͂������I");
		Talk();
		Init();
	}else{
		imgBack.LoadTexture("img/ending10.png");
		serif[0].font.Create("���@�̐X�ɂāB");
		Talk();
		serif[1].font.Create("�������u���[���A��ꂽ���B");
		Talk();
		serif[2].font.Create("      ��̂Ȃ񂾁A���̊����́H");
		Talk();
		serif[3].font.Create("      �܂�Ő^�~�̂悤����Ȃ����v");
		Talk();
		Init();

		serif[0].font.Create("�͂����茾���Đ^�~�ł���B");
		Talk();
		serif[1].font.Create("���z���̓~�͌������̂��B");
		Talk();

		serif[3].font.Create("Bad End          �R���e�B�j���[�����ɃN���A��ڎw�����I");
		Talk();
		Init();
	}
	imgBack.LoadTexture("img/thend.png");
	if( g_pTitle->talk ) {
		for(int i = 0; i < 255; i++ ){
			mt.Suspend();
			imgBack.Draw(0,0,mono(i));
		}
	}
	Talk();
	if( g_pGame->mode == 0 || g_pGame->mode == 2 )
		g_pSystem->SaveReplay();
	else {
		g_pTitle->Resume();
		return 0;
	}
	return 1;
}

//-------------------�`���m4---------------------------------
int CTalkChirno4::Start(){
	TSunFunctionCallback<CTalkChirno4> fn;
	fn.SetFunction(this,&CTalkChirno4::Thread);
	mt.Start(&fn,0x8000);
	return 1;
}
int CTalkChirno4::Thread(){
	chara = 1;
	face = 0;
	enemy_face = 1;
	font.CreateList("�����͒ʂ��Ȃ���I");
	Talk();

	chara = 0;
	face = 1;
	font.CreateList("�܂������̂��H\n�����Ԃ�k�܂ŗ����Ǝv�������A�ĊO�߂��񂾂�");
	Talk();

	return 1;
}
void CTalkChirno4::DrawEnemy(){
	if( enemy_face < 0 )
		return;
	D3DCOLOR c = mono(128+127*(chara==1));
	RECT rect;
	SetRect(&rect,0,0,300,512);
	g_pChirno2->imgTalk.DrawRectClip(360+(chara != 1)*20,100+(chara != 1)*20,rect,c);

	switch( enemy_face ) {
	case 1:
		SetRect(&rect,512-128,0,512,128);
		g_pChirno2->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 2:
		SetRect(&rect,512-128,128,512,256);
		g_pChirno2->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	case 3:
		SetRect(&rect,512-128,256,512,256+128);
		g_pChirno2->imgTalk.DrawRectClip(64+360+(chara != 1)*20,100+(chara != 1)*20,rect,c);
		break;
	}
}