#include "Sun/StdAfx.h"
#include "SunInclude.h"
#include "Main.h"

CRecord *g_pRecord = NULL;
char *spell_name[] = {
	"�镄�u�i�C�g�o�[�h�v",
	"�ŕ��u�_�[�N�}�^�[�v",
	"�����u���[�����C�g���C�v",
	"�u���u�Q���W�{�^���v",
	"�����u�i�i�z�V�e���g�E�v",
	"�����u�`���E�Z���J�}�L���v",
	"峕��u�g�r�Y���J�f�v",
	"峕��u�I�j�O���v",
	"�^���u�R�^�P�A�^�b�N�v",
	"�╄�u�Z���V�A�X�E�[���v",
	"�X���u�A�C�X�\�[�h�v",
	"�����u�t���X�g�{�b�N�X�v",
	"�ԕ��u�t���[�Y���t�����[�v",
	"�ᕄ�u�R�b�z�X�m�[�t���[�N�v",
	"�����u�_�Ќ��E�v",
	"�����u�z���]�i���X�g���C�N�v",
	"�c���u���@�[�e�B�J���u���C�N�v",
	"���ځu���~���[�}�v",
	"�ѕ��u�^�钆�̃}�X�^�[�X�p�[�N�v",
	"�����u�~�X�e���[�T�[�N���v",
	"�A�z�u���ɕω��v",
	"�O���u�������̋��P�v",
	"�l���u���Ր����v",
	"�܍s�u�j�[�����v",
	"�Z䊁u�U�ڕ�́v",
	"���_�u������D�v",
	"���T�u���V���v",
	"�͕��u��̒��̊^�v",
	"�����u�z���C�g�A���o���v",
	"�����u�A�u�\�����[�g�E�[���v",
	"�Ж�u�n���P�[���E�J�g���[�i�v",
	"�Ж�u�c�i�~�p�j�b�N�v",
	"�Ж�u�A�[�X�N�G�C�N�v",
	"�l�Ёu�I�]���z�[���v",
	"�l�Ёu�O���[�o�����[�~���O�v",
	"�l�Ёu�j���[�N���A�E�B���^�[�v"
};

CRecord::CRecord(){
	::ZeroMemory(&data,sizeof(data));
	Load();
}
CRecord::~CRecord(){
	Save();
}
void CRecord::Save(){
	WriteFile( "score.dat", &data, sizeof(data));
}
void CRecord::Load(){
	ReadFile( "score.dat", &data, sizeof(data));
}
