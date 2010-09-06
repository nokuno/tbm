#include "stdafx.h"
#include "SunMicroThread.h"

void	SunMicroThread::Start(SunFunctionCallback *fn,int nStackSize)
{
	m_bEnd = false;
	m_aStack.resize(nStackSize);
	m_fn = fn;
	m_register_esp = &m_aStack[0]+nStackSize;

	void *fInnerCallBack = (void*)&SunMicroThread::InnerCallBack;
	//	VC++6�̃C�����C���A�Z���u����static�ȃ����o�֐���call��
	//	�����Ă��Ȃ�����(VC++7�ł͉\)

	_asm {
		mov		eax,[this]
		mov		ecx,fInnerCallBack

		push	offset Next	//	�߂�l��push

		push	ebx
		push	ebp
		push	esi
		push	edi

		xchg	[eax].m_register_esp , esp

		//	bp��؂�ւ��Ă���̂ŁAthis���̂��s���ɂȂ�̂�
		//	�����ȍ~�A�����o�ɃA�N�Z�X����o���Ȃ�
		push	eax	//	InnerCallBack�̑�P�p�����[�^�Ƃ��ēn��
//			call	InnerCallBack
//	VC++6�ł̓I�[�o�[���[�h����Ă��Ȃ��O���[�o���֐������C�����C���A�Z���u������Ăяo���Ȃ�(;�L�D`)
		call	ecx

	//	�����ɓ��B�����Ƃ������Ƃ́A���̊֐��̎��s���I�������Ƃ������Ƃ��Ӗ�����̂�
	//	�X�^�b�N�����̏�Ԃɖ߂��Ă��K�v������

		//	this���ǂ�����Ċl������̂�..����肾���AInnerCallBack���߂�l�Ƃ���this��
		//	�Ԃ��Ă���Ηǂ�
		xchg	[eax].m_register_esp , esp
		mov		[eax].m_bEnd,1	//	bEnd_ = true;

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

//			pop		eax	//	�ŏ��ɐς񂾃��x����
//			jmp		eax
		ret
Next:
	}
}

void	SunMicroThread::Suspend()	///	���s���̏����𒆒f����
{
	m_bSuspended = true;

	SwitchThread();
}

#pragma warning(disable:4731)	//	ebp�ύX�������炢�Ŷ����������Ȃ�(߄t�)��٧
void	SunMicroThread::SwitchThread(){
	_asm {
		//	���݂̉��z�X�^�b�N�ɐς�
		push	offset Next
		//	resume�̂��Ƃ����ɖ߂��Ă���K�v������̂�

		push	ebx
		push	ebp
		push	esi
		push	edi

		mov		eax,[this]
		xchg	esp,[eax].m_register_esp

		pop		edi
		pop		esi
		pop		ebp
		pop		ebx

//			pop		eax
//			jmp		eax
		ret
Next:;
	}
}

SunMicroThread* SunMicroThread::InnerCallBack(SunMicroThread*p){
	//	fn_�ɃR�[���o�b�N����
	p->m_fn->Call();	//	fn_�ɃR�[���o�b�N����
	return p;
}	

void	SunMicroThread::Resume()		///	���f�����������ĊJ����
{
	if (!IsSuspended()) return ;	//	���f�i��`��j��Ų!!

	m_bSuspended = false;

	SwitchThread();
}

void	SunMicroThread::Suspend(int n){
	while (n-->0){
		Suspend();
	}
}

void	SunMicroThread::Resume(int n){
	while (n-->0){
		Resume();
	}
}
