#pragma comment(lib, "PowrProf.lib")
#include <Windows.h>
#include <WinBase.h>
#include <iostream>
#include <powrprof.h>
#include <thread>   
#include "conio.h"
using namespace std;

int a;

void getinfo() {

	SYSTEM_POWER_STATUS powerStatus;
	char ac[2][8] = { "Offline", "Online" };
	char saver[2][8] = { "is off", "on" };

	while (a != 2) {

		cout << "Press 0 to Sleep" << endl;
		cout << "Press 1 to Hibernate" << endl;
		cout << "Press 2 to Exit" << endl << endl;

		GetSystemPowerStatus(&powerStatus);
		cout << "Battery percentage: " << (int)powerStatus.BatteryLifePercent << endl;

		if (powerStatus.BatteryLifeTime != -1) {

			cout << "Battery life time: " << powerStatus.BatteryLifeTime / 60 / 60 << " h " << powerStatus.BatteryLifeTime / 60 % 60 << " min" << endl;
		}

		cout << "Battery saver " << saver[powerStatus.SystemStatusFlag] << endl;
		cout << "AC line status: " << ac[powerStatus.ACLineStatus] << endl;

		Sleep(1000);

		system("cls");
	}
}


int main(){
	
	thread log(getinfo);

	while (1) {

		if (a = _getch()) {

			a -= '0';
			if (a == 2)
				break;

			SetSuspendState((bool)a, FALSE, FALSE);
		}
	}

	log.join();

	return 0;
}