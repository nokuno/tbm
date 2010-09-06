#pragma once


//�ė��p�\�Ȓe���������[�`���W

//�ȈՂȒe���֐�
//���@�_�����e
int Kodama( double x, double y );


//�e���C���^�[�t�F�[�X
struct CDanmaku
{
	virtual int Fire() = 0;
};

//��{�N���X
struct CDanmakuBase : public CDanmaku
{
	CTama *pTama;
	virtual int Fire() = 0;
};

//���@�_��nWay�e
struct CDanmakuToPlayer : public CDanmakuBase
{
	int n;
	double w;		//�㉺�ɊJ���p�x
	CDanmakuToPlayer(CTama *pTama);
	virtual int Fire();
};
//���@�_�������_���e
struct CDanmakuToPlayerRandom : public CDanmakuBase
{
	int n;
	double w,offset;
	double sv,ev;
	CDanmakuToPlayerRandom(CTama *pTama);
	virtual int Fire();
};

//���@�_��nWay m��e
struct CDanmakuToPlayerLine : public CDanmakuBase
{
	int n, m;
	double offset;
	double w;
	double sv, ev;	//�J�n���x�A�I�����x
	CDanmakuToPlayerLine(CTama *pTama);
	virtual int Fire();
};

//�S���ʒe
struct CDanmakuNonDirectional : public CDanmakuBase
{
	int n;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	CDanmakuNonDirectional(CTama *pTama);
	virtual int Fire();
};
//�S����m��e
struct CDanmakuNonDirectionalLine : public CDanmakuBase
{
	int n, m;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	double sv, ev;	//�J�n���x�A�I�����x
	CDanmakuNonDirectionalLine(CTama *pTama);
	virtual int Fire();
};
//�Ă�Ƃ���
struct CDanmakuTentou : public CDanmaku
{
	int n, m;
	double offset;	//����
	double sv, ev;	//�J�n���x�A�I�����x
	CTamaNormal *pTama;
	CDanmakuTentou(CTamaNormal *pTama);
	virtual int Fire();
};
//�N���̑�
struct CDanmakuSpyder : public CDanmakuBase
{
	int n, m;
	double offset;	//����
	double sv, ev;	//�J�n���x�A�I�����x
	CDanmakuSpyder(CTama *pTama);
	virtual int Fire();
};

//�������~�e
struct CDanmakuCircle : public CDanmakuBase
{
	int n;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	double aa,vv;	//���x�̃I�t�Z�b�g
	CDanmakuCircle(CTama *pTama);
	virtual int Fire();
};
//���@�_���~�e
struct CDanmakuCircleToPlayer : public CDanmakuBase
{
	int n;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	double aa,vv;	//���x�̃I�t�Z�b�g
	CDanmakuCircleToPlayer(CTama *pTama);
	virtual int Fire();
};
//�S���ʉ~�e
struct CDanmakuCircleNonDirectional : public CDanmakuBase
{
	int n,m;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	double aa,vv;	//���x�̃I�t�Z�b�g
	CDanmakuCircleNonDirectional(CTama *pTama);
	virtual int Fire();
};
//n�sm��~�e
struct CDanmakuCircleLine : public CDanmakuBase
{
	int n,m;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	double aa,vv;	//���x�̃I�t�Z�b�g
	double sv, ev;	//�J�n���x�A�I�����x
	CDanmakuCircleLine(CTama *pTama);
	virtual int Fire();
};

//����
struct CDanmakuSegment : public CDanmakuBase
{
	int n;
	double a1,a2,v1,v2;
	CDanmakuSegment(CTama *pTama);
	virtual int Fire();
	void Set( double t );	//���x�Œe���w��i�����̍l�����s�v�ɂȂ�j
};

//������
struct CDanmakuSegmentLine : public CDanmakuBase
{
	int n,m;
	double a1,a2,v1,v2,ev;
	CDanmakuSegmentLine(CTama *pTama);
	virtual int Fire();
	void Set( double t );	//���x�Œe���w��i�����̍l�����s�v�ɂȂ�j
};

//���p�`
struct CDanmakuPolygons : public CDanmakuBase
{
	int n,m;
	double sv,ev;
	double offset;
	double t;
	CDanmakuPolygons(CTama *pTama);
	virtual int Fire();
};

//���^��e
struct CDanmakuHorizon : public CDanmakuBase
{
	int n,m;
	double w;
	double offset;	//�p�x�̃I�t�Z�b�g
	CDanmakuHorizon(CTama *pTama);
	virtual int Fire();
};
