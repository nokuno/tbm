// SunLog.cpp: CSunLog �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunLog.h"

CSunLog		SunLog;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

CSunLog::CSunLog()
{
	//���O�֌W
	this->m_logfilename	= "log.txt";
}

CSunLog::CSunLog(const char* file)
{
	this->m_logfilename = file;
	this->LogStart();
}
CSunLog::~CSunLog()
{

}

/**
	�������istderr�̃��_�C���N�g�j
*/
int CSunLog::LogStart()
{
	//���O�t�@�C����stderr�����_�C���N�g
	freopen(this->m_logfilename.data(),"w",stderr);
	return 1;
}
/*
	@brief ���O�o��
	printf�̏�����this->m_logfilename�֏o��
*/
int CSunLog::LogOut(const char *fmt,...)
{
	if(fmt == NULL)
		return 0;
	va_list arg;
	va_start(arg,fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);

	//�����ɏ������݁i�t���b�V���j
	fflush(stderr);
	return 1;
}
/**
	@brief ���O�o��
	����l���P���O���ŏo��
*/
int CSunLog::LogCaps(int i,char *text)
{
	this->LogOut("%s [ %s ]\n",text,i ? "��" : "�~");
	return 1;
}
/**
	@brief �i�j���Z�q
*/
int CSunLog::operator()(const char *fmt,...)
{
	if(fmt == NULL)
		return 0;
	va_list arg;
	va_start(arg,fmt);
	vfprintf(stderr, fmt, arg);
	va_end(arg);
	return 1;
}
/*-----------------------------------------------------------------------------
	���O�o�͏������ƃV�X�e�����o��
-----------------------------------------------------------------------------*/
int CSunLog::GetAllInformation()
{
	// �b�o�t���̎擾
	this->GetCPUInformation();

	// �n�r���̎擾-
	this->GetOSInformation();

	// ���������̎擾
	this->GetMemoryInformation();

	// �V�X�e�����̎擾
	this->GetSystemInformation();
	return 1;
}
/******************************************************************************

  *******     *       **     **      **
        *     ****** *  *    * **   *  *
		*     *       **    *    **  **
        *     *            *      **
  *******      *****      *         **

******************************************************************************/
//=============================================================================
/**	�b�o�t���̎擾
	�b�o�t�̔\�͂��擾���܂�
	�k�������̃R�s�y��
*/
//=============================================================================
int CSunLog::GetCPUInformation()
{

	char CPUName[256]	= "";
	char CPUType[128]	= "";
	long bFPU			= FALSE;
	long bTSC			= FALSE;
	long bCMOV			= FALSE;
	long bFCMOV			= FALSE;
	long bCPUID			= FALSE;
	long bMMX			= FALSE;
	long bMMX2			= FALSE;
	long bSSE			= FALSE;
	long bSSE2			= FALSE;
	long b3DNOW			= FALSE;
	long bE3DNOW		= FALSE;
	long TypeID			= 0;
	long FamilyID		= 0;
	long ModelID		= 0;
	long SteppingID		= 0;
	long CPUClock		= 0;

	__asm
	{
		//-----------------------------------------------------------
		// CPUID���߂̑��݃`�F�b�N
		//-----------------------------------------------------------
		PUSHFD
		POP		EAX
		MOV		EBX,		EAX
		XOR		EAX,		1<<21
		PUSH	EAX
		POPFD
		PUSHFD
		POP		EAX
		CMP		EAX,		EBX
		JE		EXIT				// �Ȃ�
		MOV		bCPUID,		1

		//-----------------------------------------------------------
		// CPUID 0
		//-----------------------------------------------------------
		MOV		EAX,		0
		CPUID

		CMP		EAX,		0
		JE		EXIT				// 0�ł͘b�ɂȂ��

		MOV DWORD PTR [CPUType+0],	EBX
		MOV DWORD PTR [CPUType+8],	ECX
		MOV DWORD PTR [CPUType+4],	EDX

		//-----------------------------------------------------------
		// CPUID 1
		//-----------------------------------------------------------
		MOV		EAX,		1
		CPUID

		//----------------------------------------------
		// EAX���W�X�^�̒��g����
		//----------------------------------------------
		// �X�e�b�s���OID
		MOV		ESI,		EAX
		AND		ESI,		0x0F;		// ����4�o�C�g�ɃX�e�b�s���OID
		MOV		[SteppingID],ESI

		// ���f��ID
		SHR		EAX,		4			// �E�X�e�b�s���OID���ɋl�߂ĥ��
		MOV		ESI,		EAX
		AND		ESI,		0x0F		// �����4�o�C�g��
		MOV		[ModelID],	ESI

		// �t�@�~���[ID
		SHR		EAX,		4			// �܂��l�߂ĥ��
		MOV		ESI,		EAX
		AND		ESI,		0x0F		// ���Ȃ���4�o�C�g
		MOV		[FamilyID],	ESI

		// �^�C�vID
		SHR		EAX,		4			// ������������
		MOV		ESI,		EAX
		AND		ESI,		0x03		// ���x��2�o�C�g
		MOV		[TypeID],	ESI

		//----------------------------------------------
		// EDX���W�X�^�̒��g����
		//----------------------------------------------
		// FPU�i�Ȃ������烄�o�C�Ǝv�����ǂˁO�O�G
		XOR		EAX,		EAX			// �[���N���A
		TEST	EDX,		1<<0		// 1�o�C�g�ڃ`�F�b�N
		SETNZ	AL						// 0�łȂ��Ȃ�AL���W�X�^�֓]��
		MOV		[bFPU],		EAX			// ���ʂ₢����

		// TSC
		TEST	EDX,		1<<4
		SETNZ	AL
		MOV		[bTSC],		EAX

		// �����t�]�����߂���є�r
		XOR		EAX,		EAX
		TEST	EDX,		1<<15
		SETNZ	AL
		MOV		[bCMOV],	EAX

		// MMX����
		XOR		EAX,		EAX
		TEST	EDX,		1<<23
		SETNZ	AL
		MOV		[bMMX],		EAX

		// MMX2 & SSE ����
		XOR		EAX,		EAX
		TEST	EDX,		1<<25
		SETNZ	AL
		MOV		[bMMX2],	EAX
		MOV		[bSSE],		EAX

//////////////////////////////////
//		�ȉ���AMD��CPU���		//
//////////////////////////////////

		//-----------------------------------------------------------
		// CPUID 0x80000000
		//-----------------------------------------------------------
		MOV		EAX,		0x80000000
		CPUID

		CMP		EAX,		0x80000001	// ����CPUID���߂̃T�|�[�g�`�F�b�N
		JB		EXIT

		//-----------------------------------------------------------
		// CPUID 0x80000001
		//-----------------------------------------------------------
		MOV		EAX,		0x80000001
		CPUID

		// MMX2
		XOR		EAX,		EAX
		TEST	EDX,		1<<22
		SETNZ	AL
		MOV		[bMMX2],	EAX

		// Enhansed 3DNow!
		XOR		EAX,		EAX
		TEST	EDX,		1<<30
		SETNZ	AL
		MOV		[bE3DNOW],	EAX

		// Enhansed 3DNow!
		XOR		EAX,		EAX
		TEST	EDX,		1<<31
		SETNZ	AL
		MOV		[b3DNOW],	EAX

		//-----------------------------------------------------------
		// CPUID 0x80000002 - 0x80000004
		//-----------------------------------------------------------
		// �T�|�[�g���Ă邩�H
		MOV		EAX,		0x80000000
		CPUID
		CMP		EAX,		0x80000004
		JB		EXIT

		// 0x80000002
		MOV EAX, 0x80000002
		CPUID

		MOV DWORD PTR [CPUName+ 0],		EAX
		MOV DWORD PTR [CPUName+ 4],		EBX
		MOV DWORD PTR [CPUName+ 8],		ECX
		MOV DWORD PTR [CPUName+12],		EDX

		// 0x80000003
		MOV EAX, 0x80000003
		CPUID

		MOV DWORD PTR [CPUName+16],		EAX
		MOV DWORD PTR [CPUName+20],		EBX
		MOV DWORD PTR [CPUName+24],		ECX
		MOV DWORD PTR [CPUName+28],		EDX

		// 0x80000004
		MOV EAX, 0x80000004
		CPUID

		MOV DWORD PTR [CPUName+32],		EAX
		MOV DWORD PTR [CPUName+36],		EBX
		MOV DWORD PTR [CPUName+40],		ECX
		MOV DWORD PTR [CPUName+44],		EDX
		
	// �I��
	EXIT:
	}


	//------------------------------------------------------------------------
	// CPU�N���b�N�擾
	//------------------------------------------------------------------------
	if ( bTSC )
	{
		__asm
		{
			RDTSC
			MOV		[CPUClock],		EAX
		}

		Sleep( 1000 );

		__asm
		{
			RDTSC
			SUB		EAX,			[CPUClock]
			MOV		[CPUClock],		EAX
		}

		CPUClock /= 1000000;
	}

	//--------------------------------------------------
	// �`�F�b�N�����`
	//--------------------------------------------------

	if ( (strlen(CPUType)>0) && (strlen(CPUName)>0) )
	{
		strcat( CPUName, " - " );
		strcat( CPUName, CPUType );
	}
	else
	{
		strcpy( CPUName, "UnKnown" );
	}

	// ���O
	this->LogOut( "�b�o�t���\n" );
	this->LogOut( "�@�@�@Name [ %s ]\n", CPUName );
	this->LogOut( "�@�@�@Clock [ %uMB ]\n", CPUClock );
	this->LogOut( "�@�@�@Type [ %u ] Family [ %u ] Model [ %u ] Stepping [ %u ]\n", TypeID, FamilyID, ModelID, SteppingID );
	this->LogOut( "�@�@�@FPU %s : CPUID %s : MMX %s\n", (bFPU  )?("��"):("�~"), (bCPUID )?("��"):("�~"), (bMMX )?("��"):("�~") );
	this->LogOut( "�@�@�@MMX2 %s : SSE %s : SSE2 %s\n", (bMMX2 )?("��"):("�~"), (bSSE   )?("��"):("�~"), (bSSE2)?("��"):("�~") );
	this->LogOut( "�@�@�@3DNow %s : E3DNow %s : CMOV %s\n", (b3DNOW)?("��"):("�~"), (bE3DNOW)?("��"):("�~"), (bCMOV)?("��"):("�~") );
	this->LogOut( "�@�@�@FCMOV %s : TSC %s\n", (bFCMOV)?("��"):("�~"), (bTSC   )?("��"):("�~") );

	return 1;
}
//=============================================================================
/**	�n�r�̎擾
	�n�r�̎�ނƃo�[�W�������擾���܂��B
	������k�������̃R�s�y��
*/
//=============================================================================
int CSunLog::GetOSInformation()
{
	OSVERSIONINFOEX OSVer;
	BOOL IsVersionEx;

	char StrType[256]		= "";
	char StrVersion[256]	= "";
	char StrBuild[256]		= "";

	//---------------------------------------------------------------------
	// OS�̃o�[�W�����擾
	//---------------------------------------------------------------------
	memset( &OSVer, 0x00, sizeof(OSVERSIONINFOEX) );
	OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	IsVersionEx = GetVersionEx( (OSVERSIONINFO*)&OSVer );

	// �d�w�n�̊֐��͂��߂Ȃ悤�ŁE�E�E
	if ( !IsVersionEx )
	{
		OSVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if ( !GetVersionEx( (OSVERSIONINFO*)&OSVer ) )
		{
			this->LogOut( "Operation System [ Unknown ]" );
			return 1;
		}
	}

	//---------------------------------------------------------------------
	// �o�[�W����
	//---------------------------------------------------------------------
	sprintf( StrVersion, "Version %d.%d", OSVer.dwMajorVersion, OSVer.dwMinorVersion );

	//---------------------------------------------------------------------
	// ����Ȃn�r���肦��̂��낤���E�E�E
	//---------------------------------------------------------------------
	if ( OSVer.dwPlatformId == VER_PLATFORM_WIN32s )
	{
		sprintf( StrVersion, "Windows 3.1 ( Build %d %s )", OSVer.dwBuildNumber, OSVer.szCSDVersion );
	}
	//---------------------------------------------------------------------
	// �v�����������m�s�n
	//---------------------------------------------------------------------
	else if ( OSVer.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		// �n�r��
		switch ( OSVer.dwMajorVersion )
		{
		case 3:
			strcpy( StrType, "WindowsNT3.5" );
			break;
		case 4:
			strcpy( StrType, "WindowsNT4" );
			break;
		case 5:
			switch ( OSVer.dwMinorVersion )
			{
			case 0:
				strcpy( StrType, "Windows2000" );
				break;
			case 1:
				strcpy( StrType, "WindowsXP" );
				break;
			}
			break;
		}
#ifdef __VISUAL_STUDIO_NET__
		// �n�r�g�����
		if ( IsVersionEx )
		{
			if( OSVer.wProductType == VER_NT_WORKSTATION )
			{
				if ( OSVer.wSuiteMask & VER_SUITE_PERSONAL )
				{
					strcat( StrType, " - Home Edition" );
				}
				else
				{
					strcat( StrType, " - Professional" );
				}
			}
			else
			{
				if ( OSVer.wProductType == VER_NT_SERVER )
				{
					if ( OSVer.wSuiteMask & VER_SUITE_DATACENTER )
					{
						strcat( StrType, " - DataCenter Server" );
					}
					else
					{
						if( OSVer.wSuiteMask & VER_SUITE_ENTERPRISE )
						{
							if ( OSVer.dwMajorVersion == 4 )
							{
								strcat( StrType, " - Advanced Server" );
							}
							else
							{
								strcat( StrType, " - Enterprise Server" );
							}
						}
						else
						{
							if ( OSVer.wSuiteMask == VER_SUITE_BLADE )
							{
								strcat( StrType, " - Web Server" );
							}
							else
							{
								strcat( StrType, " - Server" );
							}
						}
					}
				}
			}
		}
#endif // __VISUAL_STUDIO_NET__
		// �o�[�W����
		sprintf( StrBuild, "Build %d / %s", OSVer.dwBuildNumber, OSVer.szCSDVersion );	
	}
	//---------------------------------------------------------------------
	// �v�����������X���n
	//---------------------------------------------------------------------
	else if ( OSVer.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		if ( OSVer.dwMinorVersion == 0 )
		{
			strcpy( StrType, "Windows95" );
			if( (OSVer.szCSDVersion[1] == 'C') || (OSVer.szCSDVersion[1] == 'B') )
			{
				strcat( StrType, " - OSR2 " );
			}
		}
		else if ( OSVer.dwMinorVersion < 90 )
		{
			strcpy( StrType, "Windows98" );
			if ( OSVer.szCSDVersion[1] == 'A' )
			{
				strcat( StrType, " - SE " );
			}
		}
		else
		{
			strcpy( StrType, "WindowsMe" );
		}

		// �o�[�W����
		sprintf( StrBuild, "( Build %d / %d )", HIWORD( OSVer.dwBuildNumber ), LOWORD( OSVer.dwBuildNumber ) );
	}
	//---------------------------------------------------------------------
	// �킯�킩���
	//---------------------------------------------------------------------
	else
	{
		sprintf( StrType, "Windows ( %d )", OSVer.dwPlatformId );
	}

	// ���O
	this->LogOut( "�n�r���\n" );
	this->LogOut( "�@�@�@Type [ %s ]\n", StrType );
	this->LogOut( "�@�@�@Version [ %s ]\n", StrVersion );
	this->LogOut( "�@�@�@Build [ %s ]\n", StrBuild );
	return 1;
}
//=============================================================================
/**	���������̐ݒ�
	���������̎擾�����܂��B
*/
//=============================================================================
int CSunLog::GetMemoryInformation()
{
	this->LogOut( "���������\n" );

	//----------------------------------------------------------
	// �������e�ʎ擾
	//----------------------------------------------------------
	MEMORYSTATUS MemState = { sizeof(MEMORYSTATUS) };
	::GlobalMemoryStatus( &MemState );

	this->LogOut( "�@�@�@Total [ %.3lfMB ]", MemState.dwTotalPhys / 1024.0 / 1024.0 );
	this->LogOut( "�@�@�@Free [ %.3lfMB ]", MemState.dwAvailPhys / 1024.0 / 1024.0 );
	this->LogOut( "�@�@�@Used [ %u%% ]\n", MemState.dwMemoryLoad );

	this->LogOut( "�@�@�@Total PageFile [ %.3lfMB ]", MemState.dwTotalPageFile / 1024.0 / 1024.0 );
	this->LogOut( "�@�@�@Free PageFile [ %.3lfMB ]\n", MemState.dwAvailPageFile / 1024.0 / 1024.0 );

	this->LogOut( "�@�@�@Total Virtual [ %.3lfMB ]", MemState.dwTotalVirtual / 1024.0 / 1024.0 );
	this->LogOut( "�@�@�@Free Virtual [ %.3lfMB ]\n", MemState.dwAvailVirtual / 1024.0 / 1024.0 );

	return 1;
}
//=============================================================================
/**	�V�X�e�����
	�V�X�e���̏����擾���܂��B
*/
//=============================================================================
int CSunLog::GetSystemInformation()
{
	this->LogOut( "�V�X�e�����\n" );

	//---------------------------------------------------------------------
	// ��ʏ��
	//---------------------------------------------------------------------
	this->LogOut( "�@	Screen Mode" );
	this->LogOut( "�@	[ %u x %u ]\n",
		::GetSystemMetrics( SM_CXSCREEN ),
		::GetSystemMetrics( SM_CYSCREEN ) );

	//---------------------------------------------------------------------
	// �u�[�g���[�h�̎擾
	//---------------------------------------------------------------------
	switch( GetSystemMetrics( SM_CLEANBOOT ) )
	{
	case 0:
		this->LogOut( "	OS Boot Mode" );
		this->LogOut( "	[ Normal Boot ]\n" );
		break;
	case 1:
		this->LogOut( "	OS Boot Mode" );
		this->LogOut( "	[ Safe Mode ]\n" );
		break;
	case 2:
		this->LogOut( "	OS Boot Mode" );
		this->LogOut( "	[ Safe Mode with Network ]\n" );
		break;
	}
	return 1;
}

