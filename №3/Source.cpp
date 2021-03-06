#pragma comment(lib, "PowrProf.lib")
#include <Windows.h>
#include <WinBase.h>
#include <iostream>
#include <powrprof.h>
#include <thread>   
#include "conio.h"
using namespace std;

int input;

void getinfo() {

	SYSTEM_POWER_STATUS powerStatus;
	char ac[2][8] = { "Offline", "Online" };
	char saver[2][8] = { "is off", "on" };

	while (input != 2) {

		system("cls");

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
	}
}


int main(){
	
	thread log(getinfo);

	while (input != 2) {

		if (input = _getch()) {

			input -= '0';

			switch (input) {
			
			case(0):
				SetSuspendState(FALSE, FALSE, FALSE);
				break;

			case(1):
				SetSuspendState(TRUE, FALSE, FALSE);
				break;

			}
		}
	}

	log.join();

	return 0;
}