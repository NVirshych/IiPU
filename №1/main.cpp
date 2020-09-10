#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "SetupAPI.lib")
#include <windows.h>
#include <stdio.h>
#include "locale.h"
#include <setupapi.h>
#include <devguid.h>
#define INITGUID

int main() {

	setlocale(LC_ALL, "ru_RU.UTF-8");

	int deviceIndex = 1;
	char tmp[2][9] = { "VendorID", "DeviceId" };
	DWORD error;

	TCHAR szDesc[200];
	TCHAR* str;
	const wchar_t* delim = L"_";

	SP_DEVINFO_DATA deviceInfoData;
	ZeroMemory(&deviceInfoData, sizeof(SP_DEVINFO_DATA));
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	HDEVINFO deviceInfoSet;
	deviceInfoSet = SetupDiGetClassDevs(NULL, TEXT("PCI"), NULL, DIGCF_ALLCLASSES);

	while (SetupDiEnumDeviceInfo(deviceInfoSet, 0, &deviceInfoData)) {

		printf("%2d. ", deviceIndex++);

		if (SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_DEVICEDESC, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL)) {

			printf("%-80ls\t", szDesc);
		}
		else{

			error = GetLastError();
			printf("ERROR - %-72lu\t", error);
		}

		if (SetupDiGetDeviceRegistryPropertyW(deviceInfoSet, &deviceInfoData, SPDRP_HARDWAREID, NULL, (BYTE*)szDesc, sizeof(szDesc), NULL)) {

			str = _wcstok(szDesc, delim);
			for (int i = 0; i < 2; i++) {

				str = _wcstok(NULL, delim);
				printf("%s:%.4ls\t", tmp[i], str);
			}
		}
		else {

			error = GetLastError();
			printf("ERROR - %lu", error);
		}

		printf("\n\n");
		SetupDiDeleteDeviceInfo(deviceInfoSet, &deviceInfoData);
	}

	SetupDiDestroyDeviceInfoList(deviceInfoSet);

	return 0;
}