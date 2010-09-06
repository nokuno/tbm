#pragma once

#include "SunFunctionCallback.h"

class SunMicroThread {
public:

	void	Start(SunFunctionCallback *fn,int nStackSize=0x8000);
	void	Suspend();		///	���s���̏����𒆒f����
	void	Suspend(int n);	/// ���s���̏�����n�񒆒f����BSuspend��n��Ăяo���ƍl����Ηǂ�
	void	Resume();		///	���f�����������ĊJ����
	void	Resume(int n);	///	���f�����������ĊJ����

	///	�T�X�y���h�����H
	bool	IsSuspended() {
		return m_bSuspended;
	}

	///	�}�C�N���X���b�h�̎��s�͏I���������H
	bool	IsEnd() {
		return m_bEnd;
	}

	SunMicroThread(){
		m_bSuspended = false;
		m_bEnd = false;
	}

protected:
	vector<BYTE>	m_aStack;	//	�X�^�b�N
	SunFunctionCallback* m_fn; // �R�[���o�b�N���function

	bool			m_bSuspended;	//	�}�C�N���X���b�h�̃T�X�y���h
	bool			m_bEnd;			//	�}�C�N���X���b�h�̏I���t���O
	BYTE*			m_register_esp;			//	�L�^���Ă���esp

	void	SwitchThread();
	static SunMicroThread*	InnerCallBack(SunMicroThread*p);
};

class SunMicroThreadEx : public SunMicroThread{
public:
	double m_t1;
	double m_t2;
	int m_stop;
	SunMicroThreadEx() {
		m_t1 = 0;
		m_t2 = 0;
		m_stop = 0;
	}
	void	Start(SunFunctionCallback *fn,int nStackSize=0x8000) {
		m_t1 = 0;
		m_t2 = 0;
		m_stop = 0;
		SunMicroThread::Start(fn,nStackSize);
	}
	void Suspend( double t ) {
		double sum = m_t1 + t;
		int	num = (int)sum;	//�����_�ȉ��؂�̂�
		m_t1 = sum - num;	//�؂�̂ĕ���
		SunMicroThread::Suspend( num );
	}
	void Resume( double t ) {
		if( m_stop ) return;
		double sum = m_t2 + t;
		int	num = (int)sum;	//�����_�ȉ��؂�̂�
		m_t2 = sum - num;	//�؂�̂ĕ���
		SunMicroThread::Resume( num );
	}
	void Suspend() {
		m_t1 = 0;
		SunMicroThread::Suspend();
	}
	void Resume() {
		if( m_stop ) return;
		m_t2 = 0;
		SunMicroThread::Resume();
	}
	void Stop() { m_stop = 1; }
	int IsStop() { return m_stop; }
};