#include <Windows.h>
#include <Lm.h>
#include "stdafx.h"
#include "Environment.h"
#include "KernelObject.h"
#include "Log.h"

#pragma comment(lib, "netapi32.lib")

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void Sleep(DWORD dwMilliSec)
	{
		::Sleep(dwMilliSec);
	}

	//////////////////////////////////////////////////////////////////////////
	ECODE GetLastError(void)
	{
		return ::GetLastError();
	}

	//////////////////////////////////////////////////////////////////////////
	void SetLastError(ECODE nCode)
	{
		::SetLastError(nCode);
	}

	//////////////////////////////////////////////////////////////////////////
	void GetLocalTime(ST_SYSTEMTIME* pOutTime)
	{
		SYSTEMTIME stTime;
		::GetLocalTime(&stTime);
		memcpy(pOutTime, &stTime, sizeof(SYSTEMTIME));
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetLocalTime(const ST_SYSTEMTIME* pInTime)
	{
		return FALSE != ::SetLocalTime((const SYSTEMTIME*)pInTime);
	}

	//////////////////////////////////////////////////////////////////////////
	void GetSystemTime(ST_SYSTEMTIME* pOutTime)
	{
		SYSTEMTIME stTime;
		::GetSystemTime(&stTime);
		memcpy(pOutTime, &stTime, sizeof(SYSTEMTIME));
	}

	//////////////////////////////////////////////////////////////////////////
	bool SetSystemTime(const ST_SYSTEMTIME* pInTime)
	{
		return FALSE != ::SetSystemTime((const SYSTEMTIME*)pInTime);
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE GetCurrentThread(void)
	{
		return ::GetCurrentThread();
	}

	//////////////////////////////////////////////////////////////////////////
	HANDLE GetCurrentProcess(void)
	{
		return ::GetCurrentProcess();
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD GetCurrentThreadId(void)
	{
		return ::GetCurrentThreadId();
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD GetCurrentProcessId(void)
	{
		return ::GetCurrentProcessId();
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD GetTickCount(void)
	{
		return ::GetTickCount();
	}

	//////////////////////////////////////////////////////////////////////////
	struct _ST_GET_OS_TYPE_DATA
	{
		E_OS_TYPE	nOSType					;
		WORD	dwMajorVersion				;
		WORD	dwMinorVersion				;
		bool	bCheckProductType			;
		bool	bIsProductTypeWorkStation	;
		bool	bCheckSystemMetrics			;
		bool	bIsSystemMetrixServerR2		;
	};

	//////////////////////////////////////////////////////////////////////////
	const _ST_GET_OS_TYPE_DATA g_cOSTypeData[] = 
	{
		{	OS_TYPE_WINDOWS_10				, 10, 0, true, true, false, false	},
		{	OS_TYPE_WINDOWS_SERVER_2016		, 10, 0, true, false, false, false	},
		{	OS_TYPE_WINDOWS_8_1				,  6, 3, true, true, false, false	},
		{	OS_TYPE_WINDOWS_SERVER_2012_R2	,  6, 3, true, false, false, false	},
		{	OS_TYPE_WINDOWS_8				,  6, 2, true, true, false, false	},
		{	OS_TYPE_WINDOWS_SERVER_2012		,  6, 2, true, false, false, false	},
		{	OS_TYPE_WINDOWS_7				,  6, 1, true, true, false, false	},
		{	OS_TYPE_WINDOWS_SERVER_2008_R2	,  6, 1, true, false, false, false	},
		{	OS_TYPE_WINDOWS_SERVER_2008		,  6, 0, true, true, false, false	},
		{	OS_TYPE_WINDOWS_VISTA			,  6, 0, true, false, false, false	},
		{	OS_TYPE_WINDOWS_SERVER_2003_R2	,  5, 2, false, false, true, true	},
		{	OS_TYPE_WINDOWS_SERVER_2003		,  5, 2, false, false, true, false	},
		{	OS_TYPE_WINDOWS_XP				,  5, 1, false, false, false, false	},
		{	OS_TYPE_WINDOWS_2000			,  5, 0, false, false, false, false	},
	};
	size_t g_tOSTypeDataCount = sizeof(g_cOSTypeData) / sizeof(g_cOSTypeData[0]);

	//////////////////////////////////////////////////////////////////////////
	// ref. from: http://iamskylover.tistory.com/75
	BOOL GetWindowsVersion(OSVERSIONINFOEX& outInfo)
	{
		static DWORD dwMajorCache = 0, dwMinorCache = 0;
		if (0 != dwMajorCache)
		{
			outInfo.dwMajorVersion = dwMajorCache;
			outInfo.dwMinorVersion = dwMinorCache;
			return TRUE;
		}

		LPWKSTA_INFO_100 pBuf = NULL;
		if (NERR_Success != ::NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf))
			return FALSE;

		outInfo.dwMajorVersion = dwMajorCache = pBuf->wki100_ver_major;
		outInfo.dwMinorVersion = dwMinorCache = pBuf->wki100_ver_minor;
		NetApiBufferFree(pBuf);

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	void* GetProcAddress(HANDLE hModule, LPCSTR pszProcName)
	{
		return ::GetProcAddress((HMODULE)hModule, pszProcName);
	}

	//////////////////////////////////////////////////////////////////////////
	bool FreeLibrary(HANDLE hModule)
	{
		return FALSE != ::FreeLibrary((HMODULE)hModule);
	}

	//////////////////////////////////////////////////////////////////////////
	void UnixTimeToFileTime(UINT64 tUnixTime, FILETIME* pft)
	{
		LONGLONG ll;
		// 64 bit value
		ll = Int32x32To64(tUnixTime, 10000000) + 116444736000000000L; 
		pft->dwLowDateTime = (DWORD) ll;   
		pft->dwHighDateTime = (DWORD)(ll >> 32);
	}

	//////////////////////////////////////////////////////////////////////////
	UINT64 UnixTimeFrom(const FILETIME& ft)
	{  
		LONGLONG llFileTime = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;  
		return (time_t)((llFileTime - 116444736000000000L) / 10000000L);
	}
}

