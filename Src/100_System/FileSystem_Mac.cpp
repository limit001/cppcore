#include "stdafx.h"
#include "Information.h"
#include "System_Mac.h"
#include "FileSystem.h"
#include "Log.h"
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

namespace core
{
	//////////////////////////////////////////////////////////////////////////
	void FindClose(HANDLE hFindFile)
	{
		_ST_FINDFIRSTFILE* pHandle = (_ST_FINDFIRSTFILE*)hFindFile;
		::closedir(pHandle->pDir);
		delete pHandle;
	}
}
