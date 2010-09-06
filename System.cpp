#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

#define RESULT_COUNT 60
CSystem *g_pSystem = NULL;

CSystem::CSystem() :
	imgSystem1("img/system1.png"),
	imgSystem2("img/system2.png"),
	imgMaho("img/maho.png"),
	imgMoji("img/moji.png"),
	imgMoji2("img/moji2.png"),
	imgLife("img/life.png"),
	imgBomb("img/bomb.png"),
	sndPause("snd/pause.wav")
{
	sndPause.SetVolume(-1000);
	count = 0;
	life = 2;
	bomb = 3;
	power = 0;
	graze = 0;
	score = 0;
	hiscore = g_pRecord->data.score[GetDifficult()].score;
	spellcard = 0;
	rank = 0;
	bullet = 0;
	item = 0;

	state = SYSTEM_NORMAL;
	pause_select = 0;
	gameover_select = 0;
	replay_select	= 1;
	fullpower_count = 0;
	spellbonus_count = 0;
	spellbonus = 0;
	continue_dirty = 0;
	continue_rest = 3; //�R���e�j���[��
	result_count = 0;
	result_mode = 0;
	extend_count = 0;
	extend_interval = 3000000;
	extend_score = extend_interval;
	resume_count = 0;
	spell_num = 0;
	draw_system = 1;

	if( g_pGame->stage == 2 )
		power = 900;
	else if( g_pGame->stage > 2 )
		power = 1000;
}
CSystem:: ~CSystem()
{
}
//���s
int CSystem::Run()
{
	switch(state) {
	case SYSTEM_NORMAL://�ʏ�
		count++;
		if( GetInputPause() && (g_pBoss==NULL||g_pBoss->pTalk==NULL) ) {
			sndPause.Play(0);
			state = SYSTEM_PAUSE;
			g_pGame->Stop();
		}
		if( fullpower_count > 0 )
			fullpower_count--;
		if( spellbonus_count > 0 )
			spellbonus_count--;
		//���v���C����
		SunReplay.Run();
		break;
	case SYSTEM_PAUSE://�|�[�Y��
		{
			int old = pause_select;
			int maxpause = ( g_pTitle->title_select == 3 ) ? 2 : 3;
			pause_select += GetInputUD();
			pause_select = mod(pause_select,maxpause);
			if( pause_select != old )
				g_pTitle->sndSelect.Play(0);
			if(GetInputDecide()) {
				g_pTitle->sndDecide.Play(0);
				switch(pause_select) {
				case 0://�Q�[�����ĊJ����
					ResumeGame();
					break;
				case 1://�^�C�g���ɖ߂�
					ReturnTitle();
					return 0;
				case 2://�͂��߂����蒼��
					Restart();
					break;
				}
			}
		}
		if( GetInputPause() ) {
			ResumeGame();
		}
		break;
	case SYSTEM_GAMEOVER://�Q�[���I�[�o�[
		{
			int old = gameover_select;
			gameover_select += GetInputUD();
			gameover_select = mod(gameover_select,2);
			if( gameover_select != old )
				g_pTitle->sndSelect.Play(0);
			if(GetInputDecide()) {
				g_pTitle->sndDecide.Play(0);
				switch(gameover_select) {
				case 0://�R�C�������������
					life = 2;
					bomb = 3;
					continue_dirty = 1;
					continue_rest--;
					score = 0;
					graze = 0;
					//�Q�[���ĊJ
					this->ResumeGame();
					//�e�����ׂăA�C�e����
					TamaToItem();
					//�G�t�F�N�g
					g_lEffect2.Add( new CEffectDamage( g_pPlayer->GetX(),g_pPlayer->GetY() ));
					//�ʒu������
					g_pPlayer->ResetPosition();
					break;
				case 1://�l����������߂�
					if( g_pGame->mode == 0 || g_pGame->mode == 2 ) {
						g_pSystem->SaveReplay();
					}
					else {
						g_pTitle->Resume();
						return 0;
					}
					break;
				}
			}
		}
		break;
	case SYSTEM_REPLAY://���v���C�ۑ�
		if( continue_dirty == 0 ) {
			int old = replay_select;
			replay_select += GetInputUD();
			replay_select = mod(replay_select,2);
			if( replay_select != old )
				g_pTitle->sndSelect.Play(0);
			if(GetInputDecide()) {
				g_pTitle->sndDecide.Play(0);
				switch(replay_select) {
				case 0://�������I
					SunReplay.Save();
					break;
				case 1://����
					break;
				}
				SunReplay.Exit();
				ReturnTitle();
				return 0;
			}
		}
		else if(GetInputDecide()) {
			g_pTitle->sndDecide.Play(0);
			SunReplay.Exit();
			ReturnTitle();
		}
		break;
	case SYSTEM_RESULT://�X�e�[�W���U���g
		count++;
		switch( result_mode ) {
		case 0:
			result_count ++;
			//���v���C����
			SunReplay.Run();
			if( result_count == RESULT_COUNT ) {
				result_count = 0;
				result_mode ++;
				g_pGame->Stop();
			}
			break;
		case 1:
			result_count ++;
			if( GetInputDecide() || (g_pGame->mode == 3 && result_count > 180)) {
				result_count = 0;
				result_mode ++;
				g_pTitle->sndDecide.Play(0);

				g_lShot.DeleteAll();
				g_lLaser.DeleteAll();
				g_lTama.DeleteAll();
				g_lItem.DeleteAll();
				g_lEnemy.DeleteAll();
				g_lEffect.DeleteAll();
				g_lEffect2.DeleteAll();
				g_lEffect3.DeleteAll();
				SAFE_DELETE( g_pStage );
				g_pPlayer->ResetState();
				g_pGame->stop = 0;
				AddScore( (1+life*4+bomb)*g_pGame->stage*(1+GetDifficult())*10000 );
				if( (g_pGame->mode == 0 || (g_pGame->mode == 3&&SunReplay.m_header.mode==0)) && g_pGame->stage <= STAGE_NUM ) {
					g_pGame->CreateStage();
				}
				else if( g_pGame->mode == 2 || (g_pGame->mode==0&&g_pGame->stage > STAGE_NUM) ) {
					SaveReplay();
				}
				else{
					g_pTitle->Resume();
					return 0;
				}
			}
			break;
		case 2:
			result_count ++;
			//���v���C����
			SunReplay.Run();
			if( result_count == RESULT_COUNT ) {
				result_count = 0;
				result_mode = 0;
				state = SYSTEM_NORMAL;
				g_pGame->Resume();
			}
			break;
		}
		break;
	case SYSTEM_RESUME: //�Q�[���ɕ��A��
		resume_count--;
		if( resume_count == 0 ) {
			g_pGame->Resume();
		}
		break;
	}
	return 1;
}
//�`��
int CSystem::Draw()
{
	int i;
	RECT rect;

	//�X�e�[�W���U���g
	if( state == SYSTEM_RESULT ){
		switch( result_mode ) {
		case 0:
			DrawResult(200,100*(RESULT_COUNT-result_count),alpha(255));
			break;
		case 1:
			DrawResult(200,0,alpha(255));
			break;
		case 2:
			DrawResult(200,100*result_count,alpha(255));
			break;
		}
	}

	//�V�X�e���w�i
	if( draw_system ) {
		//��
		SetRect(&rect,0,0,256,48);
		imgSystem1.DrawRect(0,0,rect);

		SetRect(&rect,0,48,256,96);
		imgSystem1.DrawRect(256,0,rect);
		
		SetRect(&rect,0,96,128,144);
		imgSystem1.DrawRect(512,0,rect);

		//��
		SetRect(&rect,0,144,256,176);
		imgSystem1.DrawRect(0,448,rect);

		SetRect(&rect,0,176,256,208);
		imgSystem1.DrawRect(256,448,rect);

		SetRect(&rect,128,96,256,144);
		imgSystem1.DrawRect(512,448,rect);

		//��
		SetRect(&rect,0,0,24,256);
		imgSystem2.DrawRect(0,48,rect);

		SetRect(&rect,24,0,48,144);
		imgSystem2.DrawRect(0,304,rect);

		//�E
		SetRect(&rect,48,0,88,256);
		imgSystem2.DrawRect(600,48,rect);

		SetRect(&rect,88,0,128,144);
		imgSystem2.DrawRect(600,304,rect);

		//���C�t
		for(i=0;i<life;++i){
			imgLife.Draw(93+i*24,7,alpha(255),1,(double)count/36*2);
		}
		//�{��
		for(i=0;i<bomb;++i){
			imgBomb.Draw(93+i*24,23,alpha(255),1.0+0.1*sin((double)count/36*10));
		}

		//���@�w
		Sun3D.SetBlendingType(BLEND_ADD);
		imgMaho.Draw(414,440,alpha(255),0.8,(double)count/36);
		Sun3D.SetBlendingType(BLEND_NORMAL);

		//��Փx
		SetRect(&rect,0,16+16*g_pGame->diff,88,32+16*g_pGame->diff);
		imgMoji.DrawRect(50,454,rect);

		//���l
		DrawNumber(480,7,score,8);
		DrawNumber(480,23,hiscore,8);
		if( power < 1000 ) {
			DrawNumber(340,7,power,3);
		}
		else {
			SetRect(&rect,208,0,248,16);
			imgMoji.DrawRect(340,7,rect);
		}
		DrawNumber(340,23,graze,3);

		//FPS
		DrawNumber(360,454,SunApp.m_obfps,2);

		//�e��
		DrawNumber(240,454,g_lTama.GetSize(),3);
		if( g_lTama.GetSize() > bullet )
			bullet = g_lTama.GetSize();

		//�t���p���[
		if( fullpower_count > 0 ) {
			SetRect(&rect,0,72,190,96);
			imgMoji2.DrawRect(240,80,rect);
		}
		//�X�y���J�[�h�{�[�i�X
		if( spellbonus_count > 0 ) {
			SetRect(&rect,0,96,120,120);
			imgMoji2.DrawRect(180,100,rect);
			if( spellbonus > 0 ) {
				SetRect(&rect,0,120,72,144);
				imgMoji2.DrawRect(320,100,rect,alpha(255));
				DrawNumber( 550,100,spellbonus,0,xrgb(255,255,255));
			}
			else {
				SetRect(&rect,72,120,144,144);
				imgMoji2.DrawRect(320,100,rect,alpha(255));
			}
		}
	}
	switch(state) {
	case SYSTEM_PAUSE:	//�|�[�Y��
		{
			SunEffectRect effect;
			SetRect(&effect.m_rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
			effect.m_color = argb(200,0,0,0);
			effect.Draw();

			SetRect(&rect,0,128,96,160);
			imgMoji.DrawRect(250,130,rect);

			SetRect(&rect,0,160,216,191);
			imgMoji.DrawRect(200,200,rect,alpha(128+127*(pause_select==0)));

			SetRect(&rect,0,192,168,224);
			imgMoji.DrawRect(220,250,rect,alpha(128+127*(pause_select==1)));

			//���v���C���[�h�ł́u�ŏ������蒼���v�͂Ȃ�
			if( g_pTitle->title_select != 3 ) {
				SetRect(&rect,0,224,254,255);
				imgMoji.DrawRect(200,300,rect,alpha(128+127*(pause_select==2)));
			}
		}
		break;
	case SYSTEM_GAMEOVER://�Q�[���I�[�o�[
		{
			SunEffectRect effect;
			SetRect(&effect.m_rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
			effect.m_color = argb(200,0,0,0);
			effect.Draw();

			SetRect(&rect,0,0,256,24);
			imgMoji2.DrawRect(200,130,rect);

			SetRect(&rect,0,24,216,48);
			imgMoji2.DrawRect(220,200,rect,alpha(128+127*(gameover_select==0)));

			SetRect(&rect,0,48,200,72);
			imgMoji2.DrawRect(220,250,rect,alpha(128+127*(gameover_select==1)));

			SetRect(&rect,144,120,224,144);
			imgMoji2.DrawRect(420,300,rect);
			DrawNumber(420+40,303,continue_rest,1);
		}
		break;
	case SYSTEM_REPLAY://���v���C�ۑ�
		{
			SunEffectRect effect;
			SetRect(&effect.m_rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
			effect.m_color = argb(200,0,0,0);
			effect.Draw();

			SetRect(&rect,0,144,256,168);
			imgMoji2.DrawRect(200,130,rect);

			if( continue_dirty == 0 ) {
				SetRect(&rect,120,96,200,120);
				imgMoji2.DrawRect(220,200,rect,alpha(128+127*(replay_select==0)));

				SetRect(&rect,200,96,256,120);
				imgMoji2.DrawRect(220,250,rect,alpha(128+127*(replay_select==1)));
			}else{
				SetRect(&rect,0,168,256,216);
				imgMoji2.DrawRect(220,200,rect);
			}
		}
		break;
	case SYSTEM_RESUME://�Q�[���ĊJ��
		{
			SunEffectRect effect;
			SetRect(&effect.m_rect,GAME_LEFT,GAME_TOP,GAME_RIGHT,GAME_BOTTOM);
			effect.m_color = argb(200,0,0,0);
			effect.Draw();
		}
		break;
	}
	return 1;
}
//���U���g�`��
void CSystem::DrawResult(int x,int y,D3DCOLOR color)
{
	int xx = 300;
	RECT rect;
	SetRect(&rect,0,0,512,480);
	g_pMaterial->imgResult.DrawRect(x-200,y,rect);
	SetRect(&rect,0,0,128,480);
	g_pMaterial->imgCutin.DrawRect(x-200+512,y,rect);
}
//�H�����
int CSystem::Damage() {
	if( life == 0 ) {
		if( !GameOver() )
			return 0;
	}
	else {
		life --;
		bomb = 3;
		//�e�����ׂăA�C�e����
		TamaToItem();
		//�G�t�F�N�g
		g_lEffect2.Add( new CEffectDamage( g_pPlayer->GetX(),g_pPlayer->GetY() ));
		//�ʒu������
		g_pPlayer->ResetPosition();
	}
	return 1;
}
//�Q�[���I�[�o�[
int CSystem::GameOver() {
	if( g_pTitle->title_select == 3 ) { //���v���C���͑I�����Ȃ�
		ReturnTitle();
		return 0;
	}
	g_pGame->Stop();
	if( continue_rest > 0 ) {
		state = SYSTEM_GAMEOVER;
	}
	else {
		if( g_pGame->mode == 0 || g_pGame->mode == 2 )
			g_pSystem->SaveReplay();
		else {
			g_pTitle->Resume();
			return 0;
		}
	}
	return 1;
}
//�X�R�A���Z
int CSystem::AddScore(LONGLONG s) {
	LONGLONG old = score;
	LONGLONG mul = 1;
	score += s * mul;
	if( score >= extend_score && old < extend_score) {
		extend_count += 1;
		extend_score += extend_interval * extend_count;
		g_pMaterial->sndLife.Play(0);
		AddLife(1);
	}
	return 1;
}
//�p���[���Z
int CSystem::AddPower(int p){
	if( power == 1000 )
		return 0;
	else
		power += p;
	if( power >= 1000 ) { //�t���p���[
		power = 1000;
		g_pMaterial->sndPower.Play(0);
		TamaToItem();
		ItemSuiyose();
		fullpower_count = 120;
	}
	return 1;
}
//���C�t���Z
int CSystem::AddLife(int l){
	if( life < 7 )
		life += l;
	return 1;
}
//�{�����Z
int CSystem::AddBomb(int b){
	if( bomb < 7 )
	bomb += b;
	return 1;
}
//�O���C�Y���Z�i�ő�l�Ȃ��j
int CSystem::AddGraze(int g){
	graze += g;
	return 1;
}
//���l�`�� n:���l k:����
int CSystem::DrawNumber(int x,int y,LONGLONG n,int k,D3DCOLOR color,int scale){
	RECT rect;
	int kk = max( k, (int)log10( n ) + 1 );
	for(int i=0;i<kk;++i){
		int index = (int)(n/pow(10,kk-1-i)) % 10;
		SetRect(&rect,index*16,0,(index+1)*16,16);
		imgMoji.DrawRect(x+16*(i-(kk-k))*scale,y,rect,color,scale);
	}
	return 1;
}
//�^�C�g���ɖ߂�
void CSystem::ReturnTitle(){
	g_pTitle->Resume();
}
//�Q�[�����ŏ������蒼��
void CSystem::Restart(){
	g_pTitle->StartGame();
}
//���v���C�ۑ����[�h��
//���łɃX�R�A�ۑ�
void CSystem::SaveReplay(){
	//�v���N�e�B�X�̓X�R�A�Ȃ��A�A���X�y���J�[�h�L�^�͗L��
	if( g_pGame->mode == 0 ) {
		if( score > hiscore ) {
			g_pRecord->data.score[GetDifficult()].score = this->score;
		}
	}
	g_pRecord->Save();
	g_pGame->Stop();
	state = SYSTEM_REPLAY;
	replay_select = 1;
	SAFE_DELETE( g_pPlayer );
}
//�Q�[���ĊJ
void CSystem::ResumeGame() {
	state = SYSTEM_RESUME;
	resume_count = 4;
}