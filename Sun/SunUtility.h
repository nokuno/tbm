/**
	@file SunUtility.h
	@brief ���[�e�B���e�B�t�@�C��

	�N���X�ɂ���܂ł��Ȃ����[�e�B���e�B�Q�ł��B
	stdafx�ȊO�̂ق��̃t�@�C���ɂ͈ˑ����܂���B
*/

#pragma once

//!�|�C���^�̈��S�ȑ���
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p);	  (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
//!�~����
#define PAI (3.1415926535897932384626)
#define PI	(3.1415926535897932384626)
//!���R�ΐ��̒�
#define EXP (2.7182818284590452353602)
//!for�X�R�[�v�����ߗp
#define for						if ( false ) ; else for
//!�z��̗v�f�������߂�
#define numof(array) (sizeof(array)/sizeof(array[0]))
//!���ߐF
#define COLOR_TRANS D3DCOLOR_ARGB(255,255,0,255)
//!����
#define COLOR_WHITE D3DCOLOR_ARGB(255,255,255,255)

//!2��
inline double	pow2(double x){return x*x;}
//!�ꂪ�Q�̑ΐ�
inline double	log2(double x){return log(x)/log(2);}
//!�Q�̂邢��ȊO�؂�グ
inline int		ceil2(int x){return (int)pow(2.0,ceil(log2(x)));}
//!�Q�̂邢��ȊO�؎̂�
inline int		floor2(int x){return (int)pow(2.0,floor(log2(x)));}
//!����Z�̗]��i%�ƈႤ�͕̂�����n�����Ƃ������ɂȂ��ĕԂ邱�Ɓj
inline int		mod(int a,int b){return a>=0 ? a%b : b+a%b;}
//!�O�p�֐�
inline double Cos(double a){return cos(2.0*PI*a/360);}
inline double Sin(double a){return sin(2.0*PI*a/360);}
inline double Tan(double a){return tan(2.0*PI*a/360);}
inline double Rad2Deg(double r){return r*180/PI;}
inline double Deg2Rad(double d){return d*PI/180;}
inline double Atan2(double x,double y){return Rad2Deg(atan2(y,x));}
//�V�O���C�h�֐�
inline double Sigmoid(double x,double max,double min,double s){return min+(max-min)*exp(x)/(s+exp(x));}
//!�A���t�@��D3DCOLOR
inline D3DCOLOR	argb(BYTE a,BYTE r, BYTE g,BYTE b){return D3DCOLOR_ARGB(a,r,g,b);}
//!�A���t�@�Ȃ�D3DCOLOR
inline D3DCOLOR	xrgb(BYTE r, BYTE g,BYTE b){return D3DCOLOR_XRGB(r,g,b);}
//!�A���t�@�̂�D3DCOLOR
inline D3DCOLOR alpha(BYTE a){return D3DCOLOR_ARGB(a,255,255,255);}
//!���m�N��
inline D3DCOLOR mono(BYTE c){return D3DCOLOR_XRGB(c,c,c);}
//!���b�Z�[�W�{�b�N�X
inline int MsgBox(char* pStr,char* pCaption="�G���["){return ::MessageBox(NULL,pStr,pCaption,MB_OK | MB_ICONERROR);}
//!�J�����g�f�B���N�g��
string GetCurrent();
void ResetCurrent();
//�}�E�X
DWORD   GetMouseParam(int nXY,bool bX);
void	MoveMouse(int nX,int nY);

//��`���m�̓����蔻��
int GetHantei(RECT *r1,RECT *r2);

//�t�H���_�쐬�i�X�i�b�v�V���b�g�E���v���C�p�j
int CreateDirectory( char* name );

//�����ݏ�
int pown( int x, int n );
