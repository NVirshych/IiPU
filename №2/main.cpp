#include <Windows.h>
#include <iostream>

using namespace std;

int main() {

	HANDLE hDevice;																
	DWORD BytesReturned;										
	LPOVERLAPPED lpOverlapped = NULL;							

	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive0"), NULL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);

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
	
		printf("VendorID = %s\n", data + deviceDesc->VendorIdOffset);
		printf("Model = %s\n", data + deviceDesc->ProductIdOffset);
		printf("Serial = %s\n", data + deviceDesc->SerialNumberOffset);
	
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