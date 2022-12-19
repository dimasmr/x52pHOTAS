# x52proHOTAS
Logitech/Saitek x52 pro H.O.T.A.S. joystick interface (C++, Simulink), using DirectX API (DirectInput and Direct Output).
This requires DirectX.
Developed on the basis of the samples shown in https://github.com/walbourn/directx-sdk-samples/tree/main/DirectInput/Joystick

Make sure you have installed DirectX SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812. This SDK providess the necessary headers like dinput.h and libraries like dinput8.lib, dxguid.lib.

Download the DirectOutput API (.lib, .h, and .dll) https://1drv.ms/u/s!Aq8Kg0iqxkdrgfxXZBc5CL-a9-SAaQ. See https://forums.frontier.co.uk/threads/how-to-use-x52-pro-sdk-making-us-of-the-mfd-and-leds.428813/. Put the DirectOutput.lib, DirectOutput.h, and DirectOutput.dll in your folder.

Necessary files to be in your folder (where your .slx present, or the referenced path):
  1. DirectOutput.lib, DirectOutput.h, and DirectOutput.dll (put the DirectOuput.dll in the folder where your .exe presents)
  2. x52p_ctrl.h the header file
  3. x52p_ctrl.cpp the function definition file: this file
  4. Your .cpp impelementation file (put it in Source Files)

HOW TO COMPILE IN VISUAL STUDIO 
Include the x52p_ctrl.h in the header files (project tree) and in your folder.
When something goes wrong, you can manually set everything in the project properties.
1. In the C/C++ -> General -> Additional Include Directories, add the "Include" folder of the DirectX SDK.
   (Find the folder in your DirectX SDK installation directory).
2. In the Linker -> General -> Additional Library Directories, add the "Lib/x64" (if win64) of the
   DirectX SDK (find it in your DirectX SDK installation directory).
3. In the Linker -> Input -> Additional Dependencies, write DirectOutput.lib; dinput8.lib; dxguid.lib, add.

HOW TO COMPILE (FOR MATLAB SIMULINK)
1. You need C++ compiler available in your system. 
2. Be sure to set the compiler appropriately for C++, use "mex -setup" and choose accordingly. 
3. Compile in Matlab with "mex x52p_ctrl_SFun_wInput.cpp". 
4. This gives you a mexw64 file (Matlab executable file) in the folder, which is why the DirectOutput.dll should be in the same place with the executable file.

Open the Simulink file x52pro_HOTAS.slx and see more.
You can read more detailed information in each of the files here.
