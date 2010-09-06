// SunTask.cpp: SunTask �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunTask.h"

/**
	@brief �^�X�N
*/
SunTask::SunTask()
{
}
SunTask::~SunTask()
{
}
/**
	@brief �^�X�N���X�g
*/
SunTaskList::SunTaskList()
{
}
SunTaskList::~SunTaskList()
{

}

int SunTaskList::operator ()()
{
	SunTaskListItr i;
	for(i=m_lstTask.begin();i!=m_lstTask.end();++i)
	{
		//�m�t�k�k�`�F�b�N
		if(*i != NULL)
		{
			**i;
			if((**i)() == 0)	//�^�X�N���s
			{
				delete(*i);
				return 0;
			}
		}
	}
	return 1;
}
int SunTaskList::Clear()
{
	this->m_lstTask.clear();
	return 1;
}
//�S�폜
int SunTaskList::Delete()
{
	SunTaskListItr i;
	for(i=m_lstTask.begin();i!=m_lstTask.end();++i)
	{
		//�m�t�k�k�`�F�b�N
		if(*i != NULL)
		{
			delete(*i);
		}
	}
	this->Clear();
	return 1;
}
//�^�X�N�ǉ�
int SunTaskList::operator+=(SunTask* pTask)
{
	this->m_lstTask.push_back(pTask);
	return 1;
}
/**
	@brief �^�X�N�z��
*/
SunTaskArray::SunTaskArray()
{

}

SunTaskArray::~SunTaskArray()
{

}
int SunTaskArray::operator ()()
{
	SunTaskArrayItr i;
	for(i=m_vecTask.begin();i!=m_vecTask.end();++i)
	{
		//�m�t�k�k�`�F�b�N
		if(*i != NULL)
		{
			if((**i)() == 0)	//�^�X�N���s
			{
				delete(*i);
				return 0;
			}
		}
	}
	return 1;
}
//�����_���A�N�Z�X
SunTask*& SunTaskArray::operator[](int i)
{
	return this->m_vecTask[i];
}
//�^�X�N�ǉ�
int SunTaskArray::operator+=(SunTask* pTask)
{
	this->m_vecTask.push_back(pTask);
	return 1;
}
/**
	@brief ���C���[�^�X�N
*/
SunLayerTask::SunLayerTask()
{

}

SunLayerTask::~SunLayerTask()
{

}
int SunLayerTask::operator ()()
{
	for(int i=0; i<this->m_vecTask.size(); ++i)
	{
		//�|�C���^�ł͂Ȃ��̂łm�t�k�k�`�F�b�N�s�v
		{
			if(m_vecTask[i]() == 0)	//�^�X�N���X�g���s
				return 0;
		}
	}
	return 1;
}

//�����_���A�N�Z�X
SunTaskList& SunLayerTask::operator[](int i)
{
	if(i<0 || i>= m_vecTask.size())
	{
		::MessageBox(NULL,"�G���[","�z��̃A�N�Z�X�͈͈ᔽ",MB_OK|MB_ICONERROR);
		throw;
	}
	return this->m_vecTask[i];
}

//�T�C�Y�擾
int SunLayerTask::Size()
{
	return this->m_vecTask.size();
}
//�T�C�Y�ݒ�
int SunLayerTask::Resize(int size)
{
	this->m_vecTask.resize(size);
	return 1;
}
//�N���A
int SunLayerTask::Clear()
{
	this->m_vecTask.clear();
	return 1;
}
//�S�폜
int SunLayerTask::Delete()
{
	for(int i=0; i<m_vecTask.size(); ++i)
	{
		//�|�C���^�ł͂Ȃ��̂łm�t�k�k�`�F�b�N�s�v
		{
			m_vecTask[i].Delete();
		}
	}
	return 1;
}
