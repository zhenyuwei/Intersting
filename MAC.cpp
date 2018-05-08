/* Get MAC From Windows OS
 * you can use some ways:
   1. GetAdaptersInfo functions
   2. ipconfig/all command and use pipe to get it
   3. Windows SNMP(you computer must install SNPM protocol)
   4. Windows Netbios API(you computer must install Netbios service)
   5. COM GUID API(have some problems, sometimes get MAC is hex random data)

 * how to solve virtual and cloud server machines to getting real MAC
 * this 5 ways cannot sovle this problem, you can use the following method.
   read register data:HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards
   Description and ServiceName data

 * use vs 2013 test ok. use vc you must change some functions,like _stprintf_s/_tcscpy_s/_tcscat_s
 */

#include "stdafx.h"
#include <Windows.h>

#define OID_802_3_PERMANENT_ADDRESS	0x01010101
#define OID_802_3_CURRENT_ADDRESS	0x01010102
#define IOCTL_NDIS_QUERY_GLOBAL_STATS	0x00170002

int MACInfo(IN OUT TCHAR *szMAC, IN int nSize)
{
	int nError = -1;
	if (NULL == szMAC || nSize < 18)	return nError;

	HKEY hKey = NULL, hTmp = NULL;
	TCHAR szKey[MAX_PATH], szBuffer[MAX_PATH], szServiceName[MAX_PATH], szFileName[MAX_PATH];
	DWORD dwRet, dwType, cbData, cName;	 
	
	memset(szFileName, 0, sizeof(szFileName));
	memset(szServiceName, 0, sizeof(szServiceName));
	cbData = 0;
	dwRet = 0;
	cName = _countof(szBuffer);
	
	if (ERROR_SUCCESS != RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\"), &hKey))
	{
		return nError;
	}

	for (int i = 0; ERROR_SUCCESS == RegEnumKeyEx(hKey, i, szBuffer, &cName, NULL, NULL, NULL, NULL); ++i, cName = _countof(szBuffer))
	{
		_tcscpy_s(szKey, MAX_PATH, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\"));
		_tcscat_s(szKey, MAX_PATH, szBuffer);
		
		/* Open register data */
		if (RegOpenKey(HKEY_LOCAL_MACHINE, szKey, &hTmp) != ERROR_SUCCESS)
		{
			continue;
		}

		dwType = REG_SZ;
		cbData = MAX_PATH*sizeof(TCHAR);
		if (RegQueryValueEx(hTmp, _T("ServiceName"), NULL, &dwType, (LPBYTE)szServiceName, &cbData) == ERROR_SUCCESS){
			/* RegQueryValueEx(hTmp,_T("Description"),NULL,&dwType,(LPBYTE)szDescription,&cbData) */

			RegCloseKey(hTmp); /* close query handle */

			_tcscpy_s(szFileName, MAX_PATH, _T("\\\\.\\"));
			_tcscat_s(szFileName, MAX_PATH, szServiceName);
			HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				DWORD dwInBuff = OID_802_3_PERMANENT_ADDRESS;
				BYTE outBuff[MAX_PATH];
				dwRet = DeviceIoControl(hFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwInBuff, sizeof(dwInBuff), outBuff, sizeof(outBuff), &cbData, NULL);

				CloseHandle(hFile); /* close file handle */
				hFile = INVALID_HANDLE_VALUE;

				if (dwRet)
				{
					_stprintf_s(szMAC, nSize / sizeof(TCHAR), _T("%02X:%02X:%02X:%02X:%02X:%02X"), outBuff[0], outBuff[1], outBuff[2], outBuff[3], outBuff[4], outBuff[5]);
					nError = 0;
					/* break; */
				}
			}
		}
		else
		{
			RegCloseKey(hTmp); /* close query handle */
		}

	}///end

	if (hKey != NULL)
	{
		RegCloseKey(hKey);
	}

	return nError;
}


int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szMAC[MAX_PATH] = { 0 };
	if (MACInfo(szMAC, MAX_PATH) == 0){
		_tprintf(_T("MAC: %s\n"), szMAC);
	}
	else{
		_tprintf(_T("get MAC failed\n"));
	}
	return 0;
}
