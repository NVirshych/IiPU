#include <Windows.h>
#include <WinBase.h>
#include <iostream>
using namespace std;


int main()
{
	SYSTEM_POWER_STATUS powerStatus;
	while (1)
	{
		GetSystemPowerStatus(&powerStatus);
		std::cout << "Battery Life Percent: " << (int)powerStatus.BatteryLifePercent << endl;
		std::cout << "AC Line Status: " << (bool)powerStatus.ACLineStatus << endl;
		std::cout << "System Status Flag: " << (bool)powerStatus.SystemStatusFlag << endl;
		Sleep(1000);
		system("cls");
	}
}