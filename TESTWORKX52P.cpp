// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// Sample of implementation. Requires x52p_ctrl.h header and x52p_ctrl.cpp functions definitions files.
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
//			3. x52p_ctrl.cpp the function definition file
//			4. Your .cpp impelementation file: as an example this file (put it in Source Files).

// HOW TO COMPILE IN VISUAL STUDIO
// Include the x52p_ctrl.h in the header (project tree) and in your folder.
// When something goes wrong, you can manually set everything in the project properties.
//	1. In the C/C++ -> General -> Additional Include Directories, add the "Include" folder of the DirectX SDK.
//	   (Find the folder in your DirectX SDK installation directory).
//  2. In the Linker -> General -> Additional Library Directories, add the "Lib/x64" (if win64) of the
//	   DirectX SDK (find it in your DirectX SDK installation directory).
//	3. In the Linker -> Input -> Additional Dependencies, write DirectOutput.lib; dinput8.lib; dxguid.lib, add.
// ---------------------------------------------------------------------------------------------------------- //

#include "x52p_ctrl.h"				// External dependency file, header file
#include "x52p_ctrl.cpp"			// Functions definitions

//////////////////////////// TESTS HERE ///////////////////////////////////
// Variable type declaration
int button_id;
double axes[6];
double buttons[39];
double slider;
double povaim;

// Main implementation
int main() {
	x52p_ctrl controller(0);	// Instantiate and object/instance with ID as argument
	//controller.InitDev();

	while (1) {
		//bool resp = controller.IsDevConnected();	// Debugging section
		//std::cout << resp;
		//sint chkID = controller.GetDevID();
		//std::cout << chkID;
		//std::cout << controller.thejoys.x52p_devs;	// Should persist
		
		controller.GetState();	// Get device's state
		controller.GetCaps();	// Get device's capabilities
		int button_num = controller.GetButtonNum();	// Get the button number
		
		axes[0] = controller.XJoy();	// Axes
		axes[1] = controller.YJoy();
		axes[2] = controller.ZJoy();
		axes[3] = controller.RXJoy();
		axes[4] = controller.RYJoy();
		axes[5] = controller.RZJoy();

		slider = controller.slid();		// Slider has noise 0.003967345693141f
		povaim = controller.povdeg();	// PovAim

		//DWORD x;
		//std::cin >> x;
		//DirectOutput_SetLed(DOdevs[0], 1, x, 0); // For finding button LED id
		
		//controller.SetMDFLanding(); // Set landing of the MFD, test

		for (int i = 0; i < button_num; ++i) {
			controller.SetLEDOff(i);
			buttons[i] = controller.IsButtonPressed(i);	// Buttons
			if (buttons[i] == 1) {
				switch (i) {
				case 0:
					printf("Joy button %1d (index fire). ", i + 1);
					break;
				case 1:
					printf("Joy button %1d (safe fire). ", i + 1);
					break;
				case 2:
					printf("Joy button %1d (A). ", i + 1);
					controller.SetLEDPressYellow(1);
					break;
				case 3:
					printf("Joy button %1d (B). ", i + 1);
					controller.SetLEDPressRed(3);
					break;
				case 4:
					printf("Joy button %1d (C). ", i + 1);
					break;
				case 5:
					printf("Joy button %1d (pink fire). ", i + 1);
					break;
				case 6:
					printf("Throttle button %1d (D). ", i + 1);
					controller.SetLEDPressYellow(5);
					break;
				case 7:
					printf("Throttle button %1d (E). ", i + 1);
					controller.SetLEDPressRed(7);
					break;
				case 8:
					printf("Joy toggle %1d (T1/left up). ", i + 1);
					break;
				case 9:
					printf("Joy toggle %1d (T2/left down). ", i + 1);
					break;
				case 10:
					printf("Joy toggle %1d (T3/mid up). ", i + 1);
					break;
				case 11:
					printf("Joy toggle %1d (T4/mid down). ", i + 1);
					break;
				case 12:
					printf("Joy toggle %1d (T5/right up). ", i + 1);
					break;
				case 13:
					printf("Joy toggle %1d (T6/right down). ", i + 1);
					break;
				case 19:
					printf("Joy aim button %1d (up). ", i + 1);
					controller.SetLEDPressYellow(15);
					break;
				case 20:
					printf("Joy aim button %1d (right). ", i + 1);
					controller.SetLEDPressYellow(15);
					break;
				case 21:
					printf("Joy aim button %1d (down). ", i + 1);
					controller.SetLEDPressRed(15);
					break;
				case 22:
					printf("Joy aim button %1d (left). ", i + 1);
					controller.SetLEDPressYellow(15);
					break;
				case 23:
					printf("Throttle aim button %1d (inside). ", i + 1);
					break;
				case 24:
					printf("Throttle aim button %1d (right). ", i + 1);
					break;
				case 25:
					printf("Throttle aim button %1d (outside). ", i + 1);
					break;
				case 26:
					printf("Throttle aim button %1d (left). ", i + 1);
					break;
				case 31:
					printf("Display button %1d (FUNCTION). ", i + 1);
					break;
				}
				//printf("Button:%1d ", i);	// Check simultaneously, comment out the above whole switch
			}
		}
		printf("\n ");

		printf("X:%4f ", axes[0]);	// Print on screen
		printf("Y:%4f ", axes[1]);
		printf("Z:%4f ", axes[2]);
		printf("RX:%4f ", axes[3]);
		printf("RY:%4f ", axes[4]);
		printf("RZ:%4f ", axes[5]);

		printf("Sld:%4f ", slider);
		printf("AimDeg:%4f ", povaim);
		
		// controller.SetMDFTextAuto(0, 1); // Test MDF
		
	}
	controller.UnacqDev();	// Unacquire device
	Sleep(15);
}