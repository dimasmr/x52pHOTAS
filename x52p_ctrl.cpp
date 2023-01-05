// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// Functions definitions file. Requires x52p_ctrl.h header!
// This file is not a main file (implementation), you need to make your own implementation file!
// Nevertheless, a sample is provided, namely TESTWORKX52P.cpp.
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
//			2. x52p_ctrl.h the header file
//			3. x52p_ctrl.cpp the function definition file: this file
//			4. Your .cpp impelementation file (put it in Source Files)

// HOW TO COMPILE IN VISUAL STUDIO
// Include the x52p_ctrl.h in the header files (project tree) and in your folder.
// When something goes wrong, you can manually set everything in the project properties.
//	1. In the C/C++ -> General -> Additional Include Directories, add the "Include" folder of the DirectX SDK.
//	   (Find the folder in your DirectX SDK installation directory).
//  2. In the Linker -> General -> Additional Library Directories, add the "Lib/x64" (if win64) of the
//	   DirectX SDK (find it in your DirectX SDK installation directory).
//	3. In the Linker -> Input -> Additional Dependencies, write DirectOutput.lib; dinput8.lib; dxguid.lib, add.
// ---------------------------------------------------------------------------------------------------------- //


#include "x52p_ctrl.h"				// External dependency file, header file

x52p_ctrl::x52p_ctrl() {			// To instantiate a Class object, default
	joystick_id = 0;	// Default set joystick ID as 0
	InitDev();			// Initialize DirectInput
	GetCaps();			// Get caps
	DirectOutputInit();	// Initialize DirectOutput
}

x52p_ctrl::x52p_ctrl(int id) {		// To instantiate a Class object
	joystick_id = id;	// Set manual joystick ID 
	InitDev();			// Initialize DirectInput
	GetCaps();			// Get caps
	DirectOutputInit();	// Initialize DirectOutput
}

// Callback for EnumDevices method (function in a Class) https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416622(v=vs.85)
BOOL CALLBACK DirectEnumCB(LPCDIDEVICEINSTANCE instance, LPVOID context) {
	Joysticks* joysticks = (Joysticks*)context; // Assign a pointer for something with Joysticks struct pointer, name it joysticks

	IDirectInputDevice8* x52pd;		// Create a pointer for the interface of selected device, name it x52pd

	// Method to create and initialize an instance of a device, obtain a device interface 
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417803(v=vs.85)
	joysticks->x52p_inps->CreateDevice(instance->guidInstance, &x52pd, NULL); // Put it in the pointer of x52p_inps (input interface)

	// Method to set coop level, BG access: device can be acquired at any time
	// NONEXCLUSIVE: access to device does not interefere with others who are accessing the same device
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417921(v=vs.85)
	x52pd->SetCooperativeLevel(GetActiveWindow(), DISCL_BACKGROUND | DISCL_EXCLUSIVE); // Access selected device interface via pointer

	// Method to set the format to a joystick (not keyboard etc.) https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417925(v=vs.85)
	x52pd->SetDataFormat(&c_dfDIJoystick2); // Access selected device interface via pointer

	// Method to gain access to the device, AFTER SetDataFormat
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417818(v=vs.85)
	x52pd->Acquire(); // Access selected device interface via pointer

	// Update deviceCount by accessing the elements of a struct via pointer 
	joysticks->deviceCount += 1;

	// Since we do assume there will be many devices, then realloc the pointer to pointer (device interface)
	joysticks->x52p_devs = (IDirectInputDevice8**)realloc(joysticks->x52p_devs, joysticks->deviceCount * sizeof(IDirectInputDevice8*));

	// Assign the pointer to pointer of the device with that of the selected device interface
	joysticks->x52p_devs[joysticks->deviceCount - 1] = x52pd;

	// To continue enumeration, if you want to stop use DIENUM_STOP
	// return DIENUM_CONTINUE;
	// ONLY ONE DEVICE
	return DIENUM_CONTINUE;
}

// Initialize and create device, do it just once
Joysticks x52p_ctrl::InitDev() {
	thejoys = { 0 }; // initialization of the struct, namely joysticks with Joysticks struct

	// Creates a DirectInput object https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416756(v=vs.85)
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&thejoys.x52p_inps, 0);

	// Arrow operator -> allows access elements in a struct via pointer that points to a struct.
	// Similar to dot but dot access elements in a struct directly.
	// Enumerat all devices https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417804(v=vs.85)
	thejoys.x52p_inps->EnumDevices(DI8DEVCLASS_GAMECTRL, DirectEnumCB, (void*)&thejoys, DIEDFL_ALLDEVICES);

	return thejoys;
}

// Get device capabilities
DIDEVCAPS x52p_ctrl::GetCaps() {
	// Describes the DirectInput devices capabiliteis, DIDEVCAPS is a structure
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416607(v=vs.85)
	caps = { sizeof(DIDEVCAPS) }; // Allocate memory

	// Get the capabilities of the device object
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417892(v=vs.85)
	thejoys.x52p_devs[joystick_id]->GetCapabilities(&caps); // Put it in the memory address of caps with DIDEVCAPS struct

	return caps;
}

// Get button number from caps
int x52p_ctrl::GetButtonNum() {
	return caps.dwButtons;
}

// Get the state from the device, do it every step
DIJOYSTATE2 x52p_ctrl::GetState() {
	// Method to get the state of the device
	// Puts the state in the memory addres of state with DIJOYSTATE struct returns a DI_OK
	// Must create, set cooperative level, data format, and acquire, in that order
	// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee417897(v=vs.85)
	// ONLY ONE DEVICE
	ZeroMemory(&state, sizeof(DIJOYSTATE2));    //Set Memory to 0
	thejoys.x52p_devs[joystick_id]->GetDeviceState(sizeof(state), &state);

	return state;
}

// Check device exists or not
int x52p_ctrl::IsDevConnected() {
	HRESULT hr = thejoys.x52p_devs[joystick_id]->GetDeviceState(sizeof(state), &state);
	if (hr == DI_OK) {
		return 1;
	}
	else {
		return 0;
	}
}

// Get joystick ID
int x52p_ctrl::GetDevID() {
	return joystick_id;
}

// Unacquire the device
void x52p_ctrl::UnacqDev() {
	HRESULT hr;
	hr = thejoys.x52p_devs[joystick_id]->Unacquire();
	//if (hr == DI_OK) {
	//	std::cout << "Unacquiring success";
	//}
}

////////////////////////////  AXES ///////////////////////////////////////
double x52p_ctrl::XJoy() {
	if (state.lX > thrs + deadzone || state.lX < thrs - deadzone) {
		return (state.lX - thrs) / thrs;
	}
	else {
		return 0.0f;
	}
}

double x52p_ctrl::YJoy() {
	if (state.lY > thrs + deadzone || state.lY < thrs - deadzone) {
		return((thrs - state.lY) / thrs);
	}
	else {
		return 0.0f;
	}
}

double x52p_ctrl::ZJoy() {
	// Modified correctly, Jan 2023
	double tmpZ = thrsZ - state.lZ; // Set to 0 to thrsZ
	
	if (tmpZ > deadzone) {
		return(tmpZ / thrsZ);
	}
	else {
		return 0.0f;
	}
}

double x52p_ctrl::RXJoy() {
	return(state.lRx / thrsZ);
}

double x52p_ctrl::RYJoy() {
	return(state.lRy / thrsZ);
}

double x52p_ctrl::RZJoy() {
	if (state.lRz > thrs + deadzone || state.lRz < thrs - deadzone) {
		return((state.lRz - thrs) / thrs);
	}
	else {
		return 0.0f;
	}
}

//////////////////////////// SLIDERS //////////////////////////////////////
double x52p_ctrl::slid() {
	return state.rglSlider[0] / thrsZ;	// Slider on the throttle/clutch
}

//////////////////////////// AIMPOV ///////////////////////////////////////
int x52p_ctrl::povdeg() {
	int pov = state.rgdwPOV[0] / 100;	// Return the aim at every 45 deg.
	if (state.rgdwPOV[0] == -1) {
		pov = -1000;	// When not used, it gives -1000.
	}
	return pov;
}

//////////////////////////// BUTTONS //////////////////////////////////////
int x52p_ctrl::IsButtonPressed(int button_id) {
	if (state.rgbButtons[button_id]) {
		return 1;	// If button pressed returns 1 as integer
	}
	else {
		return 0;	
	}
}


//////////////////////////// DIRECTOUTPUT /////////////////////////////////
// Callback to register the devices
void __stdcall DirectOutput_Device_Callback(void* hDevice, bool bAdded, void* pvContext) {
	if (bAdded) {
		DOdevs.push_back(hDevice);
	}
	else {
	}
}

// Callback to enumerate the devices
void __stdcall DirectOutput_Enumerate_Callback(void* hDevice, void* pvContext) {
	DOdevs.push_back(hDevice);
}

// Method to initialize the DirectOutput
void x52p_ctrl::DirectOutputInit() {
	DirectOutput_Initialize(name);	// Initialize the DirectOutput
	DirectOutput_RegisterDeviceCallback(*DirectOutput_Device_Callback, nullptr);	// Register the devices
	DirectOutput_Enumerate(*DirectOutput_Enumerate_Callback, nullptr);				// Enumerate the devices

	DirectOutput_AddPage(DOdevs[joystick_id], dwPage, pageDebugName, FLAG_SET_AS_ACTIVE);	// AddPage for the device, activate

	SetAllLEDGreen();	// Set all LEDS on
	SetAllLEDOff();		// Set most of the LEDs off

	// Set welcome text
	text = L"MAKI ATSUO";
	length = wcslen(text);
	SetMDFText(text, length, 0);
}

// Method to put text in the MDF DirectOutput, id is the id, use 0 for one device. This is predefined
void x52p_ctrl::SetMDFTextAuto(int autoflg, int VecTflg) {
	if (autoflg == 1) {
		text = L"AUTO MODE";
		length = wcslen(text);
		DirectOutput_SetString(DOdevs[joystick_id], dwPage, 2, length, text);
		SetLEDPressGreen(17);	// Set LED on the clutch to green
	}
	else {
		text = L"MANUAL MODE";
		length = wcslen(text);
		DirectOutput_SetString(DOdevs[joystick_id], dwPage, 2, length, text);
		DirectOutput_SetLed(DOdevs[joystick_id], dwPage, 17, 0); // Turn of the LED on the clutch
	}

	if (VecTflg == 1) {
		text = L"VECTWIN ON";
		length = wcslen(text);
		DirectOutput_SetString(DOdevs[joystick_id], dwPage, 1, length, text);
	}
	else {
		text = L"PARALLEL ON";
		length = wcslen(text);
		DirectOutput_SetString(DOdevs[joystick_id], dwPage, 1, length, text);
	}
}

// Method to put text in the MDF DirectOutput, id is the id, use 0 for one device. This is free
void x52p_ctrl::SetMDFText(const wchar_t* text, DWORD length, DWORD pos) {
	DirectOutput_SetString(DOdevs[joystick_id], dwPage, pos, length, text);
}

// Method to put text in the landing page
void x52p_ctrl::SetMDFLanding() {
	text = L"ATSUO MAKI";
	length = wcslen(text);
	SetMDFText(text, length, 0);
}

// Method to change LED in the DirectOuput: green, red, and yellow only, and off
void x52p_ctrl::SetLEDPressYellow(DWORD b_id) {
	DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id, 1);
	DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id + 1, 1);
}

void x52p_ctrl::SetLEDPressRed(DWORD b_id) {
	DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id, 1);
}

void x52p_ctrl::SetLEDPressGreen(DWORD b_id) {
	DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id + 1, 1);
}

void x52p_ctrl::SetLEDOff(DWORD b_id) {
	if (b_id == 0) {
		//DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id, 1);
		return;
	}
	else if (b_id > 16 && b_id < 20) {
		return;
	}
	else {
		DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id, 0);
		DirectOutput_SetLed(DOdevs[joystick_id], dwPage, b_id + 1, 0);
	}
		
}

void x52p_ctrl::SetAllLEDGreen() {
	int i;
	for (i = 1; i < 19; ++i) {
		if (i % 2 == 0 && i != 0) {
			DirectOutput_SetLed(DOdevs[joystick_id], dwPage, i, 1); // All except the fire button and throttle
		}
	}
}

void x52p_ctrl::SetAllLEDOff() {
	int i;
	for (i = 0; i < 19; ++i) {
		if (i % 2 == 0 && i != 0) {
			DirectOutput_SetLed(DOdevs[joystick_id], dwPage, i, 0); // All except the fire button and throttle
		}
	}
}

// Method to stop the DirectOutput
void x52p_ctrl::DirectOutputStop() {
	DirectOutput_Deinitialize();
}