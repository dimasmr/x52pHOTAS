// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2022.
// C MEX S-Function for Simulink with two inputs and LED/MDF modification.
// Saitek/Logitech x52 pro HOTAS.

// REQUIREMENTS
// A.	Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812.
//		This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

// B.	Download the DirectOutput API (.lib, .h, and .dll) https://1drv.ms/u/s!Aq8Kg0iqxkdrgfxXZBc5CL-a9-SAaQ.
//		See https://forums.frontier.co.uk/threads/how-to-use-x52-pro-sdk-making-us-of-the-mfd-and-leds.428813/.
//		Put the DirectOutput.lib, DirectOutput.h, and DirectOutput.dll in your folder.

// C.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. DirectOutput.lib, DirectOutput.h, and DirectOutput.dll
//			2. x52p_ctrl.h the header file
//			3. x52p_ctrl.cpp the function definition file
//			4. This file

// HOW TO COMPILE (FOR MATLAB SIMULINK)
// You need C++ compiler available in your system.
// Be sure to set the compiler appropriately for C++, use "mex -setup" and choose accordingly.
// Compile in Matlab with "mex x52p_ctrl_SFun_wInput.cpp".
// This gives you a mexw64 file (Matlab executable file) in the folder, which is why the DirectOutput.dll
//		should be in the same place with the executable file.
// ---------------------------------------------------------------------------------------------------------- //


#define S_FUNCTION_NAME  x52p_ctrl_SFun_wInput		// define S-Function Name
#define S_FUNCTION_LEVEL 2							// define S-Function Level

#include "simstruc.h"		// For Simulink S-Function
#include "x52p_ctrl.cpp"	// Functions definitions

// Check parameters, hmm still needs checking
#define MDL_CHECK_PARAMETERS
#if defined (MDL_CHECK_PARAMETERS) && defined(MATLAB_MEX_FILE)

static void mdlCheckParameters(SimStruct* S) {
	size_t NrParameters = mxGetNumberOfElements(ssGetSFcnParam(S, 0)); // Take number of element in the SFun param
	real_T* joyid_param = (real_T*)mxGetPr(ssGetSFcnParam(S, 0)); // Take pointer of the SFun param, must be double, name it joystick_id

	const char* msg = NULL; // allocate pointer for message of char with null/zeros

	if (NrParameters != 1) { // If people enters array, check!
		msg = "Put only one joystick ID, not an array.";
	}
	else { // If people joystick ID not 0, put error!
		if (joyid_param[0] != 0) {
			msg = "PLEASE USE ID 0, SORRY! WILL FIX THIS WHEN IT IS POSSIBLE";
		}
	}
	if (msg != NULL) { // Give error if and break Simulink if either message is coming
		ssSetErrorStatus(S, msg);
		return;
	}
}
#endif 

// Initialize the S-Function block parameter
static void mdlInitializeSizes(SimStruct* S) {
	ssSetNumSFcnParams(S, 1);	// Expect a parameter: joystick ID, default is 0 !!!
	if (ssGetNumSFcnParams(S) == ssGetSFcnParamsCount(S)) {
		mdlCheckParameters(S);	// Run the check, if msg is coming, then break Simulink
		if (ssGetErrorStatus(S) != NULL) {
			return;
		}
	}
	else { // If the number of parameters does not match, break Simulink
		return;
	}

	ssSetNumContStates(S, 0);	// No continuous states
	ssSetNumDiscStates(S, 0);	// No discrete states

	if (!ssSetNumInputPorts(S, 2)) { // One input ports (Auto and VecTwin)
		return; // Break Simulink if there are input ports
	}

	ssSetInputPortWidth(S, 0, 1); // 1st port: Auto 
	ssSetInputPortWidth(S, 1, 1); // 2nd port: VecTwin
	ssSetInputPortDirectFeedThrough(S, 0, 1);	// To make sure that the input is available!
	ssSetInputPortDirectFeedThrough(S, 1, 1);	// To make sure that the input is available!

	if (!ssSetNumOutputPorts(S, 4)) { // Four outputs: axes, slider, pov, button, flags
		return; // Break Simulink if the number of output ports is not correct
	}

	ssSetOutputPortWidth(S, 0, 6);	// 1st port: Six axes X, Y, Z, and RX, RY, RZ
	ssSetOutputPortWidth(S, 1, 1);	// 2nd port: One slider
	ssSetOutputPortWidth(S, 2, 1);	// 3rd port: One PovAim
	ssSetOutputPortWidth(S, 3, 39);	// 4th port: Thirty nine buttons

	ssSetNumSampleTimes(S, -1);	// If sample time is inherited, use -1
	ssSetNumPWork(S, 1);		// Set pointers for persistent objects!
	ssSetNumModes(S, 0);
	ssSetNumNonsampledZCs(S, 0);

	ssSetOperatingPointCompliance(S, USE_DEFAULT_OPERATING_POINT); // Misc for save/restore the Simulink
}

// Initialize the S-Function block parameter: sample time
static void mdlInitializeSampleTimes(SimStruct* S) {
	ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);	// Set to inherit the simulink
	ssSetOffsetTime(S, 0, 0.0);	// No offset
}

// Callback to initialize initial state just once! Make persist the DirectInput object
#define MDL_START 
#if defined(MDL_START) 
static void mdlStart(SimStruct* S) {
	// Get the S-Function parameter, here is the joystick ID
	real_T* joyid_param = (real_T*)mxGetPr(ssGetSFcnParam(S, 0));
	// int joyid_chk = int(joyid_param[0]); // For debugging
	// std::cout << joyid_chk;

	// Check and give error to make sure
	if (joyid_param[0] != 0) {
		ssSetErrorStatus(S, "PLEASE USE ID 0, SORRY! WILL FIX THIS WHEN IT IS POSSIBLE");
	}

	// Use PWork (pointer that points to a vector of pointers)
	// to make DirectInput object persist (and DirectOutput)
	// Create the object x52p_ctrl with joystick_id, default is 0!!
	void** PWork = ssGetPWork(S);
	PWork[0] = (void*) new x52p_ctrl(int(joyid_param[0]));	// allocate memory with new
}
#endif

// Update the output: the state of the Joystick
static void mdlOutputs(SimStruct* S, int_T tid) {
	// Get the output of SFun to be used in Simulink, everything is double
	// Convert to the appropriate data type to a pointer
	real_T* axes = (real_T*)ssGetOutputPortRealSignal(S, 0);
	real_T* slider = (real_T*)ssGetOutputPortRealSignal(S, 1);
	real_T* povaim = (real_T*)ssGetOutputPortRealSignal(S, 2);
	real_T* buttons = (real_T*)ssGetOutputPortRealSignal(S, 3);

	// Get the input of SFun to be used here, all double
	InputRealPtrsType auto_ptr = ssGetInputPortRealSignalPtrs(S, 0);
	InputRealPtrsType VT_ptr = ssGetInputPortRealSignalPtrs(S, 1);

	// Take the pointer to the persistent DirectInput object from the pointer vectors, name it c.
	x52p_ctrl* c = (x52p_ctrl*)ssGetPWork(S)[0];

	// Update the state by calling the method in the object
	c->GetState();

	// DEBUGGING, MAKE PUBLIC ALL CLASS VARIABLES
	//std::cout << c->thejoys.x52p_devs;	// Print out the DirecInput object, should persist each time step
	//std::cout << c->joystick_id;			// For debuggin please always make public the class variables in the x52p_ctrl.h
	//std::cout << &c->state.lX << &c->state;
	//bool rsp = c->IsDevConnected();
	//int chkID = c->GetDevID();
	//std::cout << rps << chkID;
	//std::cout << DOdevs[0];
	//std::cout << *VT_ptr[0];

	// Get number of buttons by calling the method in the object
	int butt_no = c->GetButtonNum();

	// Normalize the values
	axes[0] = c->XJoy();	// Axes
	axes[1] = c->YJoy();
	axes[2] = c->ZJoy();
	axes[3] = c->RXJoy();
	axes[4] = c->RYJoy();
	axes[5] = c->RZJoy();

	slider[0] = c->slid();		// Slider has noise 0.003967345693141f
	povaim[0] = c->povdeg();	// POV Aim

	// Take the button states, if true, it returns 1
	for (int i = 0; i < 39; ++i) {
		c->SetLEDOff(i);	// Set Led OFF when not pressed
		buttons[i] = c->IsButtonPressed(i);
		if (buttons[i] == 1) {
			switch (i) {
			case 2:
				c->SetLEDPressYellow(1);
				break;
			case 3:
				c->SetLEDPressRed(3);
				break;
			case 6:
				c->SetLEDPressYellow(5);
				break;
			case 7:
				c->SetLEDPressRed(7);
				break;
			case 19:
				c->SetLEDPressYellow(15);
				break;
			case 20:
				c->SetLEDPressYellow(15);
				break;
			case 21:
				c->SetLEDPressRed(15);
				break;
			case 22:
				c->SetLEDPressYellow(15);
				break;
			}
			//printf("Buttons: %d, ", i);
		}
	}

	// Print text to MDF
	c->SetMDFTextAuto(int( *auto_ptr[0]), int(*VT_ptr[0]));


}

// Unacquire the DirectInput objct and free the memory that we allocate to make persistent object
static void mdlTerminate(SimStruct* S) {
	x52p_ctrl* c = (x52p_ctrl*)ssGetPWork(S)[0];	// Take the pointer to persistent object
	c->UnacqDev();			// Unacquire
	c->DirectOutputStop();	// Stop DirectOutput API
	delete c;		// Free memory
}

#ifdef  MATLAB_MEX_FILE    // Is this file being compiled as a MEX-file?
#include "simulink.c"      // MEX-file interface mechanism
#else
#include "cg_sfun.h"       // Code generation registration function 
#endif