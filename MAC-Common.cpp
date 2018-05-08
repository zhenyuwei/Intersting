#include "stdafx.h"
#include <Windows.h>
#include <IPHlpApi.h>

#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Ws2_32.lib")

/*
根据IP地址获取网卡适配器设备名称
OUT AdapterName:适配器名称缓冲区 IN IP:IP地址(网络字节序)
返回值:TRUE 成功获取
*/
BOOL GetAdapterByIp(PCHAR AdapterName, ULONG IP)
{
	ULONG ulAdapterInfoSize = sizeof(IP_ADAPTER_INFO);
	IP_ADAPTER_INFO *pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
	IP_ADAPTER_INFO *pAdapterInfoEnum = NULL;

	if (GetAdaptersInfo(pAdapterInfo, &ulAdapterInfoSize) == ERROR_BUFFER_OVERFLOW)
	{	/* not enough ulAdapterInfoSize */
		delete[] pAdapterInfo;
		pAdapterInfo = NULL;
		pAdapterInfo = (IP_ADAPTER_INFO*)new char[ulAdapterInfoSize];
		if (NULL == pAdapterInfo)
		{
			printf("new IP_ADAPTER_INFO Error!\n");
			return FALSE;
		}
	}

	pAdapterInfoEnum = pAdapterInfo;
	if (GetAdaptersInfo(pAdapterInfoEnum, &ulAdapterInfoSize) == ERROR_SUCCESS){
		do{        /* 遍历所有适配器 */
			if (pAdapterInfoEnum->Type == MIB_IF_TYPE_ETHERNET){
				/* 判断是否为以太网接口 */
				/* pAdapterInfoEnum->Description 是适配器描述 */
				/* pAdapterInfoEnum->AdapterName 是适配器名称 */
				/* pAdapterInfoEnum->Address        是Mac地址 */
				if (inet_addr(pAdapterInfoEnum->IpAddressList.IpAddress.String) == IP){
					printf("%s \nIP: %s \nGATEIP: %s\n", pAdapterInfoEnum->AdapterName,
						pAdapterInfoEnum->IpAddressList.IpAddress.String, pAdapterInfoEnum->GatewayList.IpAddress.String);
					printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", pAdapterInfoEnum->Address[0], pAdapterInfoEnum->Address[1],
						pAdapterInfoEnum->Address[2], pAdapterInfoEnum->Address[3], pAdapterInfoEnum->Address[4], pAdapterInfoEnum->Address[5]);
					break;
				}
			}
			pAdapterInfoEnum = pAdapterInfoEnum->Next;
		} while (pAdapterInfoEnum);
	}
	delete[]pAdapterInfo;
	return FALSE;
}

void test()
{
	char szName[MAX_PATH] = { 0 };
	char szIP[MAX_PATH] = { 0 };
	int err = gethostname(szName, MAX_PATH);

	if (err == SOCKET_ERROR)
	{
		printf("gethostname fail %08x\n", WSAGetLastError());
		return;
	}

	hostent *pHostent = gethostbyname(szName);
	if (NULL == pHostent)
	{
		printf("gethostbyname fail %08x\n", WSAGetLastError());
		return;
	}
	printf("Name: %s\n", szName);
	char *ip = inet_ntoa(*(struct in_addr *)*pHostent->h_addr_list);
	printf("IP:%s\n", ip);

	ULONG ulAdapterIp = *(ULONG*)pHostent->h_addr_list[0];

	GetAdapterByIp(NULL, ulAdapterIp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	if (0 == WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		test();
		WSACleanup();
	}	

	system("pause");
	return 0;
}
