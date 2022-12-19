// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// Header file, external dependency file.
// Saitek/Logitech x52 pro HOTAS.

// REQUIREMENTS
// A.	Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812.
//		This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

// B.	Download the DirectOutput API (.lib, .h, and .dll) https://1drv.ms/u/s!Aq8Kg0iqxkdrgfxXZBc5CL-a9-SAaQ.
//		See https://forums.frontier.co.uk/threads/how-to-use-x52-pro-sdk-making-us-of-the-mfd-and-leds.428813/.
//		Put the DirectOutput.lib, DirectOutput.h, and DirectOutput.dll in your folder.

// C.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. DirectOutput.lib, DirectOutput.h, and DirectOutput.dll
//			   (Put the DirectOuput.dll in the folder where your .exe presents)
//			2. x52p_ctrl.h the header file: this file
//			3. x52p_ctrl.cpp the function definition file
// ---------------------------------------------------------------------------------------------------------- //


#ifndef X52P_H
#define X52P_H

#define DIRECTINPUT_VERSION 0x0800	// DirectX version, MUST!
#include <dinput.h>		// DirectInput API header
#include <stdio.h>		// For std I/O namespace
#include <iostream>		// For std I/0 stream
#include <Windows.h>	// For ZeroMemory function
#include <vector>		// For vector class
extern "C" {
#include "DirectOutput.h"	// For DirectOuput API header, pure C so use extern C
}

#pragma comment(lib, "dinput8.lib")	// The necessary libraries for DirectInput API
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectOutput.lib")	// For DirectOuput

HINSTANCE hInstance = GetModuleHandle(NULL); // Instance of the window, null is fine

// Define the struct for multiple joysticks
struct Joysticks
{
	unsigned int deviceCount;
	IDirectInputDevice8** x52p_devs;	// Pointer to the interface of device, name it x52p_devs
	IDirectInput8* x52p_inps;			// Pointer to the interface of input, name the pointer as x52p_inps
};

// Pointer to DirectOuput interface of the device
// vector encapsulates dynamic size arrays https://en.cppreference.com/w/cpp/container/vector
std::vector<void*> DOdevs;	// Pointer-to-pointer / pointers vector
	
// For MDF
const wchar_t* text;	// Wide character pointer
DWORD length;

// Define the required values for normalization of the axes
const double thrs = 32767.0f, thrsZ = 65535.0f;
const int deadzone = 3500;	// Deadzone to prevent jumps due to light touch at the origins

class x52p_ctrl {			
public:						// Access specifier, public = can be accessed and modified outside the class
	// Class constructors!
	x52p_ctrl();			// Default constructor 
	x52p_ctrl(int id);		// Constructor, accepts arguments, will be defined outside the class via Class::Class( args )
	
	// Class methods for DirectInput!
	Joysticks InitDev();	// Methods (functions belong to a class, defined outside the class via void Class::Class( args ) { }
	DIDEVCAPS GetCaps();
	DIJOYSTATE2 GetState();	
	int GetButtonNum();
	void UnacqDev();

	// Class methods for DirectOutput!
	void DirectOutputInit();
	void DirectOutputStop();
	void SetMDFTextAuto(int autoflg, int VecTflg);
	void SetMDFText(const wchar_t* text, DWORD length, DWORD pos);
	void SetMDFLanding();
	void SetLEDPressYellow(DWORD butt_id);
	void SetLEDPressRed(DWORD butt_id);
	void SetLEDPressGreen(DWORD butt_id);
	void SetLEDOff(DWORD b_id);
	void SetAllLEDGreen();
	void SetAllLEDOff();

	// Class methods for state normalization!
	double XJoy();	// void has no return type, double returns a double type, bool returns binary 1 or 0 (T/F)
	double YJoy();
	double ZJoy();
	double RXJoy();
	double RYJoy();
	double RZJoy();
	double slid();
	int povdeg();

	// Class methods for buttons
	int IsButtonPressed(int button_id);

	// Class methods for debugging ID and device
	int IsDevConnected();
	int GetDevID();

private:
	// Class important variables! In private for safety! Comment out the above //private: for debugging!
	DIDEVCAPS caps;			// DIDEVCAPS structure of DirectInput https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416607(v=vs.85)
	DIJOYSTATE2 state;		// DJOYSTATE structure of DirectInput https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416627(v=vs.85)
	Joysticks thejoys;
	int joystick_id; 
	int button_num;

	DWORD dwPage = 1;
	const wchar_t* name = L"X52P_App";			// Any name of the App, necessary
	const wchar_t* pageDebugName = L"TestPage";	// Any page for debug, not necessary
	// The L is a wchar_t literal, wide character, 16-bits storage
};

// DirectOuput LED IDs
//DWORD LED_FIRE = 0;
//DWORD LED_FIRE_A_RED = 1;
//DWORD LED_FIRE_A_GREEN = 2;
//DWORD LED_FIRE_B_RED = 3;
//DWORD LED_FIRE_B_GREEN = 4;
//DWORD LED_FIRE_D_RED = 5;
//DWORD LED_FIRE_D_GREEN = 6;
//DWORD LED_FIRE_E_RED = 7;
//DWORD LED_FIRE_E_GREEN = 8;
//DWORD LED_TOGGLE_1_2_RED = 9;
//DWORD LED_TOGGLE_1_2_GREEN = 10;
//DWORD LED_TOGGLE_3_4_RED = 11;
//DWORD LED_TOGGLE_3_4_GREEN = 12;
//DWORD LED_TOGGLE_5_6_RED = 13;
//DWORD LED_TOGGLE_5_6_GREEN = 14;
//DWORD LED_POV_2_RED = 15;
//DWORD LED_POV_2_GREEN = 16;
//DWORD LED_CLUTCH_RED = 17;
//DWORD LED_CLUTCH_GREEN = 18;
//DWORD LED_THROTTLE = 19;

#endif