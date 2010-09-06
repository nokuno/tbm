#pragma once

//���[�U�[�C���^�[�t�F�[�X
//���͕��ʂ̒e�Ɠ����B
//�`�揇����e�����ɂ��邽�߂����ɕ������B
struct CLaser : public SunObject
{
	virtual int Run() = 0;
	virtual int Draw() = 0;
	virtual int Item() = 0;
};
extern SunList<CLaser> g_lLaser;

//�l�p�����[�U�[
struct CLaserSquare : public CLaser
{
	int count;
	int start_count;
	int end_count;
	int vanish_count;
	int graze_count;
	double iw;
	double ow;
	double a; //�p�x
	double w; //�p���x
	SunEffectLineLaser laser;
	CLaserSquare();
	virtual int Run();
	virtual int Draw();
	virtual int Item();
};