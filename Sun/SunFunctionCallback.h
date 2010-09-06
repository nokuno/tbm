#pragma once

class SunFunctionCallback {
public:
	virtual int Call() {return 0;}

	virtual ~SunFunctionCallback(){}
};

//	�����o�֐��|�C���^�̃T�C�Y�̓N���X�ˑ��Ȃ̂ŁA�ėp�^�̂悤�Ȃ��̂�
//	���݂��Ȃ��B�����ŁA���̂悤�Ȃ��Ƃ����Ȃ��Ă͂Ȃ�Ȃ��Ȃ�B
template <class T>
class TSunFunctionCallback : public SunFunctionCallback {
public:
	TSunFunctionCallback(void){
		m_pThis = NULL;
		m_pFunc = NULL;
	}

	virtual int Call() {
		if (m_pThis) return (m_pThis->*m_pFunc)();
		return 0;
	}
	virtual void SetFunction(T* pThis,int (T::*pFunc)()){
		m_pThis = pThis;
		m_pFunc = pFunc;
	}
	virtual void GetFunction(T*&pThis,int (T::*&pFunc)()){
		pThis = m_pThis;
		pFunc = m_pFunc;
	}
private:
	T* m_pThis;
	int (T::*m_pFunc)();
};