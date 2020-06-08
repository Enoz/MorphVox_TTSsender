#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include "MorphTTS.h";

// '/' key
#define VK_RECORD 191

using namespace std;
MorphTTS tts;
bool recording = false;
string sendbuff;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION) {
		switch (wParam) {
			case WM_KEYDOWN:
				#if _DEBUG
					cout << "Key: " << p->vkCode << endl;
				#endif
				if (p->vkCode == VK_RECORD && recording == false) {
					sendbuff = std::string("");
					recording = true;
				}
				if (recording) {
					switch (p->vkCode) {
						case VK_RECORD:
							break;
						default:
							sendbuff += (char)p->vkCode;
					}
				}
				break;
			case WM_SYSKEYDOWN:
				break;
			case WM_KEYUP:
				if (p->vkCode == VK_RECORD && recording == true) {
					recording = false;
					#if _DEBUG
						cout << "Sending " << sendbuff << endl;
					#endif _DEBUG
					tts.SayText(sendbuff);
				}
					
				break;
			case WM_SYSKEYUP:
				break;
				
		}
}
	return (recording ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam));
}


int main() {


	//tts.SayText("Heydod");
	// Install the low-level keyboard & mouse hooks
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);

	// Keep this app running until we're told to stop
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {    //this while loop keeps the hook
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hhkLowLevelKybd);
	
	return 0;


}