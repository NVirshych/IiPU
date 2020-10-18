#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "SetupAPI.lib")
#include <windows.h>
#include <stdio.h>
#include "locale.h"
#include <setupapi.h>
#include <devguid.h>

void getCamInfo()
{
	SP_DEVINFO_DATA deviceInfoData;
	ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	HDEVINFO deviceInfoSet;
	deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_CAMERA, TEXT("USB"), NULL, DIGCF_PRESENT);
	TCHAR szDesc[200] = { 0 };
	SetupDiEnumDeviceInfo(deviceInfoSet, 0, &deviceInfoData);
	SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL);
	printf("vendor id: %c%c%c%c\n", szDesc[8], szDesc[9], szDesc[10], szDesc[11]);
	printf("prod id: %c%c%c%c\n", szDesc[17], szDesc[18], szDesc[19], szDesc[20]);
	SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL);
	printf("friendly name: %ls\n", szDesc);
	SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_MFG, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL);
	printf("manufacturer: %ls\n", szDesc);
	SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL);
	printf("device description: %ls\n", szDesc);
	SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_SERVICE, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL);
	printf("service name: %ls\n", szDesc);
}


int main()
{
	setlocale(LC_ALL, "rus");
	getCamInfo();
}