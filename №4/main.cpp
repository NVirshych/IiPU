#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "SetupAPI.lib")
#include <windows.h>
#include <stdio.h>
#include "locale.h"
#include <setupapi.h>
#include <devguid.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;


void getCamInfo() {

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


string makeName() {

    time_t now = time(NULL);
    tm* ltm = new tm;
    localtime_s(ltm, &now);
    stringstream stream;
    stream << ltm->tm_year + 1900 << '_' << ltm->tm_mon + 1 << '_'
        << ltm->tm_mday << '_' << ltm->tm_hour << '_'
        << ltm->tm_min << '_' << ltm->tm_sec;
    return stream.str();
}

void recordVideo(VideoCapture vcap) {

    Mat frame;

    int frame_width = vcap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = vcap.get(CAP_PROP_FRAME_HEIGHT);
    VideoWriter video("video" + makeName() + ".avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(frame_width, frame_height), true);

    for (int i = 0; i < 90; i++) {

        vcap >> frame;
        video.write(frame);
        Sleep(33);
    }
}

LRESULT CALLBACK LLKeyProc(int nCode, WPARAM wParam, LPARAM lParam) {

    VideoCapture vcap(0);
    Mat frame;

    if (!vcap.isOpened()) {

        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    DWORD keyCode = ((LPKBDLLHOOKSTRUCT)lParam)->vkCode;

    if (nCode == HC_ACTION) {

        switch (keyCode) {

        case(0x1b):
            PostQuitMessage(0);
            break;

        case(32):
            vcap >> frame;
            imwrite("photo_" + makeName() + ".png", frame);
            break;

        case(162):
            ShowWindow(GetConsoleWindow(), SW_HIDE);
            break;

        case(164):
            ShowWindow(GetConsoleWindow(), SW_SHOW);
            break;

        case(161):
            recordVideo(vcap);
            break;

        default:
            break;
        }
    }

    vcap.release();
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


int main() {

    setlocale(LC_ALL, "rus");
    getCamInfo();

    HHOOK hook = SetWindowsHookEx(WH_KEYBOARD_LL, LLKeyProc, GetModuleHandle(NULL), 0);

    if (!hook) {
        cout << "Error : " << GetLastError() << endl;
        return 0;
    }

    while (WaitMessage()) {

        MSG msg = { 0 };
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                UnhookWindowsHookEx(hook);
                return 0;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}