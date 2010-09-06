// SunTask.h: SunTask �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "SunUtility.h"

class SunLayerTask;
/**
	@brief �^�X�N

	�^�X�N�̃C���^�[�t�F�[�X�N���X�ł��B
	�֐��I�u�W�F�N�g�Ƃ��ċ@�\���܂��B
	�Ƃ������A�ėp��fanctor�Ƃ����Ă����ł��B
*/
class SunTask  
{
public:
	SunTask();
	virtual ~SunTask();
	virtual int operator()() = 0;		///<�֐��I�u�W�F�N�g�Ƃ��ċ@�\
};
/**
	@brief �^�X�N���X�g

	�^�X�N�̃��X�g�ł��B
	���ꎩ�̂��^�X�N�ł��B
	���ۂɎg���Ƃ��́A�V�[���N���X�̃����o�Ƃ��Ē�`����Ηǂ��ł��傤�B
	�������A�`��^�X�N�����͑S�̂łP�ł��B
*/
class SunTaskList : public SunTask
{
private:
	list<SunTask*> m_lstTask;	///<�^�X�N���X�g
public:
	SunTaskList();
	virtual ~SunTaskList();
	virtual int operator()();
	int operator+=(SunTask* pTask);	///<�^�X�N�ǉ�
	int Clear();
	int Delete();
};
typedef list<SunTask*>::iterator SunTaskListItr;
/**
	@brief �^�X�N�z��

	�^�X�N�̂u�����������z��ł��B
	���X�g�ƈႤ�̂́A�u�����_���A�N�Z�X�ł��邱�Ɓv�Ɓu��{�I�ɃT�C�Y�����܂��Ă��邱�Ɓv
	�g�����Ȃ������B�B�B
*/
class SunTaskArray : public SunTask
{
private:
	vector<SunTask*> m_vecTask;	///<�^�X�N�z��
public:
	SunTaskArray();
	virtual  ~SunTaskArray();
	virtual int operator()();
	SunTask*& operator[](int i);
	int operator+=(SunTask* pTask);	///<�^�X�N�ǉ�
};
typedef vector<SunTask*>::iterator SunTaskArrayItr;
/**
	@brief ���C���[�^�X�N

	���C���[����������A�u�^�X�N���X�g�̔z��v�ł��B
	�`��̏��������ւ���܂����A�`��Ɍ��炸���������߂�K�v�̂���^�X�N�W��
	�ɑ΂��Ďg�����Ƃ��ł���Ǝv���܂��B
*/
class SunLayerTask : public SunTask
{
private:
	vector<SunTaskList> m_vecTask;	///<�^�X�N�z��
public:
	SunLayerTask();
	virtual ~SunLayerTask();
	virtual int operator()();
	SunTaskList& operator[](int i);
	int Size();
	int Resize(int size);
	int Clear();
	int Delete();
};
typedef vector<SunTaskList>::iterator SunLayerTaskItr;

