#include <iostream>
#include <Windows.h>
#include <string>
#pragma once

#define EDITINDEX 10
#define SENDBUTTONINDEX 9

using namespace std;

//Don't touch, must be global :<
int _curidx;
HWND _toret;
int _idxtosearch;
class MorphTTS {
private: 
	HWND TTSWindow;
	HWND editbox;
	HWND sendbutton;


	HWND GetChildHandle(int index) {
		if (TTSWindow == NULL)
			return NULL;
		_curidx = 0;
		_toret = NULL;
		_idxtosearch = index;
		auto cb = [](HWND hwnd, LPARAM lparam) -> BOOL CALLBACK {

			#if _DEBUG
				char clsName[256];
				GetClassNameA(hwnd, clsName, 256);
				cout << "[" << _curidx << "] hwnd_Child = " << hwnd << " with name [" << clsName << "]" << endl;
			#endif

			if (_curidx == _idxtosearch)
				_toret = hwnd;
			_curidx++;
			return TRUE;
		};
		EnumChildWindows(TTSWindow, cb, 0);
		return _toret;
	}
public:
	MorphTTS() {
		TTSWindow = NULL;
		while (TTSWindow == NULL) {

			#if _DEBUG
				cout << "Looking for window..." << endl;
			#endif

			TTSWindow = FindWindow(NULL, "Text-To-VoIP");
			if (TTSWindow == NULL)
				Sleep(1000);
		}

		#if _DEBUG
			cout << "Window handle found!" << endl;
		#endif
		
		editbox = GetChildHandle(EDITINDEX);
		sendbutton = GetChildHandle(SENDBUTTONINDEX);
	}

	void SayText(std::string str) {
		SendMessage(editbox, WM_SETTEXT, NULL, (LPARAM)str.c_str());
		SendMessage(sendbutton, BM_CLICK, NULL, NULL);
	}
};