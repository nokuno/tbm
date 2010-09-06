// SunObject.h: SunObject �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once

/**
	@brief �I�u�W�F�N�g�C���^�[�t�F�[�X

	���s�ƕ`��𕪗����܂��B
*/
class SunObject
{
public:
	SunObject(){};
	virtual ~SunObject(){};
	virtual int Run() = 0;	///<���s
	virtual int Draw() = 0;	///<�`��
};
/**
	
	@breaf ���X�g�e���v���[�g

	stl::list���A�I�u�W�F�N�g�̃|�C���^��������悤�Ɋg���������̂ł��B
*/
template<class T>
class SunList : public list<T*>
{
public:
	SunList();
	SunList(int size);
	~SunList();
	int Add(T* pObj);
	void DeleteAll();
	bool IsAllNull();
	bool IsEmpty();
	void EraseNull();
	void Resize(int size);
	
	int Run();
	int Draw();
public:
	int GetSize(){return list<T*>::size();}
};
/*-----------------------------------------------------------------------------
	��������SunList�֌W�̎���
-----------------------------------------------------------------------------*/
template <class T>
SunList<T>::SunList()
{
}
template <class T>
SunList<T>::SunList(int size)
:list<T>(size)
{
}
template <class T>
SunList<T>::~SunList()
{
	DeleteAll();
}
template <class T>
int SunList<T>::Add(T* pObj)
{
	list<T*>::push_back( pObj );
	return 1;
}
template <class T>
void SunList<T>::DeleteAll()
{
	list<T*>::iterator itr;
	for( itr = list<T*>::begin(); itr != list<T*>::end(); itr++ ) {
		SAFE_DELETE(*itr);
	}
	EraseNull();
}
template <class T>
bool SunList<T>::IsAllNull()
{
	list<T*>::iterator itr;
	for( itr = list<T*>::begin(); itr != list<T*>::end(); itr++ ) {
		if( *itr == NULL ) {
			return 0;
		}
	}
	return 1;
}
template <class T>
bool SunList<T>::IsEmpty()
{
	return list<T*>::empty();
}
template <class T>
void SunList<T>::EraseNull()
{
	list<T*>::iterator itr;
	list<T*>::iterator next;
	for( itr = list<T*>::begin(); itr != list<T*>::end();  ) {
		next = itr;
		next++;
		if( *itr == NULL ) {
			list<T*>::erase( itr );
		}
		itr = next;
	}
}
template <class T>
int SunList<T>::Run()
{
	list<T*>::iterator itr;
	for( itr = list<T*>::begin(); itr != list<T*>::end(); itr++ ) {
		if( *itr != NULL ) {
			//���s
			if(! (*itr)->Run()) {
				//0��Ԃ�����ADELETE����
				SAFE_DELETE( *itr );
			}
		}
	}
	EraseNull();
	return 1;
}
template <class T>
int SunList<T>::Draw()
{
	list<T*>::iterator itr;
	for( itr = list<T*>::begin(); itr != list<T*>::end(); itr++ ) {
		if( *itr != NULL ){
			//���s
			if(! (*itr)->Draw()) {
				//0��Ԃ�����ADELETE����
				SAFE_DELETE( *itr );
			}
		}
	}
	return 1;
}
template <class T>
void SunList<T>::Resize(int size)
{
	list<T*>::resize(size);
}