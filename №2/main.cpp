#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <string>
#include <ntddscsi.h>

using namespace std;

void getMemInfo(_ULARGE_INTEGER& clusters, _ULARGE_INTEGER& freeClusters, _ULARGE_INTEGER& busyClusters, int driveNumber)
{
	_ULARGE_INTEGER diskSpace = { 0 }, freeSpace = { 0 };
	string mountPath("\\\\.\\ :"), volumePath(" :\\");
	for (char volumeLetter = 'A'; volumeLetter <= 'Z'; volumeLetter++)
	{
		mountPath[4] = volumeLetter;
		HANDLE logicalDiskHandle = CreateFile(const_cast<char*>(mountPath.c_str()), GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (logicalDiskHandle == INVALID_HANDLE_VALUE) continue;

		volumePath[0] = volumeLetter;

		STORAGE_DEVICE_NUMBER devNum = { 0 };
		DeviceIoControl(logicalDiskHandle, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &devNum, sizeof(devNum), NULL, NULL);
		if (devNum.DeviceNumber != driveNumber) continue;

		GetDiskFreeSpaceExA(const_cast<char*>(volumePath.c_str()), 0, &diskSpace, &freeSpace);
		clusters.QuadPart += diskSpace.QuadPart;
		freeClusters.QuadPart += freeSpace.QuadPart;
		CloseHandle(logicalDiskHandle);
	}
	busyClusters.QuadPart = clusters.QuadPart - freeClusters.QuadPart;
}

int main() {
	HANDLE hDevice;
	DWORD BytesReturned;
	LPOVERLAPPED lpOverlapped = NULL;
	_ULARGE_INTEGER freeClusters = { 0 }, clusters = { 0 }, busyClusters = { 0 };
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"), NULL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

	STORAGE_DEVICE_NUMBER devNum = { 0 };

	DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &devNum, sizeof(devNum), NULL, NULL);
	unsigned long int logicalDrivesCount = GetLogicalDrives();
	//==================================================================================================================================

	STORAGE_PROPERTY_QUERY query;
	ZeroMemory(&query, sizeof(query));
	query.PropertyId = StorageDeviceProperty;
	query.QueryType = PropertyStandardQuery;

	char data[1024];

	STORAGE_DEVICE_DESCRIPTOR* deviceDesc;
	deviceDesc = (STORAGE_DEVICE_DESCRIPTOR*)&data;


	DWORD dwIoControlCode = IOCTL_STORAGE_QUERY_PROPERTY;
	LPVOID lpInBuffer = &query;
	DWORD nInBufferSize = sizeof(query);
	LPVOID lpOutBuffer = &data;
	DWORD nOutBufferSize = sizeof(data);


	//                     OK                                                                                  OK              OK
	if (DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, &BytesReturned, lpOverlapped)) {

		printf("VendorID = %d\n", static_cast<int>(*data + deviceDesc->VendorIdOffset));
		printf("Model = %s\n", data + deviceDesc->ProductIdOffset);
		printf("Serial = %s\n", data + deviceDesc->SerialNumberOffset);
		printf("Firmware revision = %s\n", data + deviceDesc->ProductRevisionOffset);
		const char* interfaceTypes[] = { "Unknown", "SCSI", "ATAPI", "ATA", "1394", "SSA", "Fibre", "USB", "RAID", "ISCSI", "SAS", "SATA", "SD", "MMC", "Virtual", "FileBackedVirtual",
		"Spaces", "Nvme", "SCM", "Ufs", "Max", "MaxReserved" };
		printf("Interface type: %s\n", interfaceTypes[deviceDesc->BusType]);
		getMemInfo(clusters, freeClusters, busyClusters, 1);
		printf("Size = %lld bytes\nFree space = %lld bytes\nBusy space = %lld bytes\n", (long long int)clusters.QuadPart, freeClusters.QuadPart, busyClusters.QuadPart);
	}
	else {

		cout << "Error: " << GetLastError() << endl;
	}
	return 0;
}