/**
	@file stdafx.cpp
	@brief �W���C���N���[�h�t�@�C�����܂ރ\�[�X �t�@�C��
	@bug ��̃o�O�FFPS��30�ɗ������ށA������������A�ċN������܂Œ���Ȃ�

	Sun.pch ���������v���R���p�C���σw�b�_�[
	stdafx.obj ���������v���R���p�C���σ^�C�v���
*/
#include "stdafx.h"

/**
	@mainpage �r�������C�u����
	<pre>
	<h2>DirectX9.0���g������ɂQ�c�Q�[���������C�u�����ł��B</h2>

    ���܂���J��O��ɍ���Ă��܂���B
    �������g�����߁i�����i�|������邽�߁j�ɍ���Ă��܂��B
    �Q�c�̊i���Q�[�������̂ɂ͌����Ă�Ǝv���܂��B

	<h3>���W���[��</h3>
		
		�r�����͂������̃��W���[���ō\������Ă��܂��B
		���W���[���͍ė��p�\�ȒP�ʂƎv���Ă��������B
		�Ƃ͂����Astdafx��SunInclude�͕K�v�ɉ����ď���������K�v�������ł���
		�E���[�e�B���e�B
			�v����ɃR���\�[���ł��g������́B
			�ق��̃��W���[���͂��ׂĂ��̃��W���[���Ɉˑ�����B
			SunUtility,SunConfig,SunLog,SunTask,SunExeption,SunFile������ɂ�����B
		�EDirect3D�A�v���P�[�V����
			Sun,Sun3D,SunSurface,SunTexture,SunImage,SunSpriteBase,SunSprite,SunSpriteEx
			������Ɋ܂܂�܂��B
			
		�E���͊֌W
		�E���֌W
		
    <h3>�N���X�����F</h3>
		���ۂɎg���N���X�̂݉�����܂��B
		SunSurface
			�T�[�t�F�[�X�N���X�B�w�i�ȂǓ��߂��K�v�Ȃ����̂�`��B
		SunTexture
			�e�N�X�`���N���X�B
		SunSpiteBase
			SunTexture�ɑΉ�����X�v���C�g�N���X�B
		SunImage
			�����e�N�X�`���N���X�B����P�̂ŕ`�悷��Ƃ��A������SunSpriteEx���g���B
		SunSprite
			SunImage�ɑΉ�����X�v���C�g�N���X�B
		SunSpriteEx
			SunSprite�Ƀ��C���[�ɂ��`�揇���@�\��t���B
   <h3> �ϐ������F</h3>
		�����͊e�N���X�ɂ��P�̃O���[�o���ϐ��ł��B
        CSun Sun;
			�A�v���P�[�V�����S�̂̂͂Ȃ��B
        CSunLog SunLog;
			���O�t�@�C���ilog.txt)�֌W�B
        CSunConfig2 SunConfig;
			�R���t�B�O�t�@�C���iconfig.ini)�֌W�B
        CSun3D Sun3D;
			�R�c�֌W�B�����������o���Ă��������B
	</pre>
*/