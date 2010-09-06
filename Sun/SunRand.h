/**
	@file SunRand.h
	@brief �[�����������N���X

    Mersenne Twister�@�ɂ��[�������B
	�N���X�ɂ������ŁA�����̗����n��������܂��B
	�ق�YandSDK3��CRand�ł��B
*/

#pragma once

#define MAX_RAND (0xffffffff)

class CSunRand {
public:
	DWORD	Get();			//	�����̎擾
	DWORD	Get(DWORD n) {	//	0�`n-1�̗����̎擾
		if (n==0) { 
			return 0;
		}
		return Get() % n;
	}

	void	SetSeed(DWORD dwSeed);
	void	Randomize()	{SetSeed(::GetTickCount());}

	CSunRand() { m_nMti = 624+1; }
	CSunRand(DWORD dwSeed) { SetSeed(dwSeed); }
	virtual ~CSunRand() {}
protected:
	DWORD	m_dwMt[624];	// the array for the state vector
	int		m_nMti;			// initialization counter
};

extern CSunRand SunRand;	// ���v���C�ɉe������ړI
extern CSunRand SunRand2;	// ���v���C���֌W�i�G�t�F�N�g�p�j

// �⏕�֐�
inline double rand(double min,double max){return min + (max-min) * SunRand.Get() / MAX_RAND;}
inline int irand(int min,int max){return min + SunRand.Get(max-min+1);}
inline double rand2(double min,double max){return min + (max-min) * SunRand2.Get() / MAX_RAND;}
inline int irand2(int min,int max){return min + SunRand2.Get(max-min+1);}
