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
	
		printf("VendorID = %d\n", static_cast<int>(*data + deviceDesc->VendorIdOffset));
		printf("Model = %s\n", data + deviceDesc->ProductIdOffset);
		printf("Serial = %s\n", data + deviceDesc->SerialNumberOffset);
		printf("Firmware revision = %s\n", data + deviceDesc->ProductRevisionOffset);

	}
	else {
	
		cout << "Error: " << GetLastError() << endl;
	}

	return 0;
}