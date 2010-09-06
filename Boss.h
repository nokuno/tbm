#pragma once

/*
  �{�X����
*/
struct CBossBase : public CEnemy
{
	int count;
	double x,y;
	double hp_max;
	double hp;
	double deffence;	//�h��́i�_���[�W�W���j
	int spell;	//�X�y���t���O
	int time;	//��������
	int dirty;	//0:�X�y�J�擾�A1:���s
	int flag;	//���Ԃ��i�ނ��ǂ����̃t���O
	SunFont fntSpell;
	SunFont fntBoss;
	CBack* pBack;	//�w�i�i�ۑ��p�j
	CTalk* pTalk;	//��b

	SunMicroThreadEx	mt;
	SunMicroThreadEx	mt2;
	SunMicroThreadEx	mt3;

	CBossBase();
	virtual ~CBossBase();
	virtual int Run();
	virtual int Draw();
	virtual int Draw2();		///< ���C�t�Q�[�W��
	virtual int Draw3();		///< ��b�n
	virtual int Draw4();		///< ���ڃG�t�F�N�g��p
	virtual int SpellCard();	///< �X�y���J�[�h�J�n
	virtual int SpellEnd();		///< �X�y���J�[�h�I��
	int Damage( double damage );	///< �_���[�W�����炤
	int Death();
	int Hantei();		///< ���@�Ƃ̓����蔻��
	void SetHP( double hp );
	void SetTime( int time );
	int IsEnd();

	double GetX(){return x + GetW()/2;}
	double GetY(){return y + GetH()/2;}
	double GetSize(){return (GetW()+GetH())/4;}
	virtual double GetW(){return 64;}
	virtual double GetH(){return 64;}
	virtual int GetItem();	///< �A�C�e���̎��
	virtual void Appear();
	void AddItem(int item,int num);
	void Suspend(double m);
};
extern CBossBase *g_pBoss;
