#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <iostream>
#include <string>
using namespace std;

void getMemInfo(_ULARGE_INTEGER& clusters, _ULARGE_INTEGER& freeClusters, _ULARGE_INTEGER& busyClusters)
{
	_ULARGE_INTEGER diskSpace = { 0 }, freeSpace = { 0 };
	string mountPath("\\\\.\\ :"), volumePath(" :\\");
	for (char volumeLetter = 'A'; volumeLetter <= 'D'; volumeLetter++)
	{	
		mountPath[4] = volumeLetter;
		HANDLE logicalDiskHandle = CreateFile(const_cast<char*>(mountPath.c_str()), GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		if (logicalDiskHandle == INVALID_HANDLE_VALUE)
			continue;
		volumePath[0] = volumeLetter;
		GetDiskFreeSpaceExA(const_cast<char*>(volumePath.c_str()), 0, &diskSpace, &freeSpace);
		clusters.QuadPart += diskSpace.QuadPart;
		freeClusters.QuadPart += freeSpace.QuadPart;
	}
	busyClusters.QuadPart = clusters.QuadPart - freeClusters.QuadPart;
}

int main() {
	HANDLE hDevice;																
	DWORD BytesReturned;										
	LPOVERLAPPED lpOverlapped = NULL;							
	_ULARGE_INTEGER freeClusters = { 0 }, clusters = { 0 }, busyClusters = { 0 };
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive1"), NULL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

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
		getMemInfo(clusters, freeClusters, busyClusters);
		printf("Size = %lld bytes\nFree space = %lld bytes\nBusy space = %lld bytes\n", (long long int)clusters.QuadPart, freeClusters.QuadPart, busyClusters.QuadPart);
	}
	else {
	
		cout << "Error: " << GetLastError() << endl;
	}

	dwIoControlCode = IOCTL_STORAGE_FIRMWARE_GET_INFO;
	STORAGE_HW_FIRMWARE_INFO_QUERY quer;
	ZeroMemory(&quer, sizeof(quer));
	quer.Version = sizeof(quer);

	lpInBuffer = &quer;
	nInBufferSize = sizeof(quer);

	STORAGE_HW_FIRMWARE_INFO buf;
	ZeroMemory(&buf, sizeof(buf));
	buf.Version = sizeof(buf);

	lpOutBuffer = &buf;
	nOutBufferSize = sizeof(buf);

	//                     OK                                                                                  OK              OK
	if (DeviceIoControl(hDevice, dwIoControlCode, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, &BytesReturned, lpOverlapped)) {

		printf("Firmware revision = %s\n", buf.Slot[0].Revision);

	}
	else {

		cout << "Error: " << GetLastError() << endl;
	}

	return 0;
}