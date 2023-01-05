// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2023.
// Header file, external dependency file.
// Saitek/Logitech x52 pro HOTAS.
// Linux and Visual Studio Code with gcc compiler.

// REQUIREMENTS
// A.	Make sure you have installed SDL or SDL2 API https://wiki.libsdl.org/SDL2/Installation
//      This provides all the necessary headers like SDL.h and its relevant dependency files.
//      You can install via sudo apt:
//          -> sudo apt-get install libsdl2-dev
//
//      Or manually:
//          -> git clone https://github.com/libsdl-org/SDL.git -b SDL2
//          -> cd SDL
//          -> mkdir build
//          -> cd build
//          -> ../configure
//          -> make
//          -> sudo make install

// B.   Make sure you have installed gcc C++ compiler in your system / Visual Studio Code.

// C.	Necessary files to be in your folder (where your .slx present, or the referenced path):
//			1. x52p_linux64.h the header file
//			3. x52p_linux64.cpp the function definition file
//			4. Your .cpp impelementation file: as an example this file.

// HOW TO COMPILE IN VISUAL STUDIO CODE USING GCC
// If you start fresh or tasks.json in .vscode directory is not present. Do the following:
//  1. Ctrl+Shitf+D -> Run and Debug.
//     Make sure when you run/debug, choose gcc compiler.
//     You may have errors regarding "-fdiagnostics-color=always" and unreferenced SDL_Init etc. Abort.
//  2. Now the tasks.json file exists. Open it. In the "args:" field, delete the "-fdiagnostics-color=always",
//     and add the following linkers and necessary arguments: (dont forget to put comma when you add these)
//          "-lSDL2",
//          "-lstdc++",
//          "-std=gnu++11"          
// ---------------------------------------------------------------------------------------------------------- //

#ifndef X52P_LINUX64_H
#define X52P_LINUX64_H

#include <SDL2/SDL.h>   // SDL2 API header, if SDL, change to <SDL.h>
#include <stdio.h>      // For std I/O namespace
#include <iostream>     // For std I/O stream
#include <assert.h>     // For assert

SDL_Window* window = NULL;		// Get the instance of the window, null is fine
SDL_Renderer* renderer = NULL;	// Get the instance of the renderer, null is fine

// Global variable, the device count
int deviceCount; 

// The maximum number of connected devices is four
// Every time an SDL object is made, put it in the pointer of pointers.
SDL_Joystick** x52p_devs[4];     // Pointer to pointers of the joystick input interface, name it x52p_devs

// Define the structs for multiple joysticks
// For capabilities, manually made
struct JoyCaps
{
    int NumAxes;
    int NumButtons;
    int NumHats;
};

// For states, manually made. The size will be allocated based on capabilities in the implementation
struct JoyStates
{
    Sint16* axes;
    Uint8*  buttons;
    Uint8*  hats;
};

// Define the required values for normalization of the axes
const double thrs = 32767.0f, thrsZ = 65535.0f;
const int deadzone = 3500;  // Deadzone to prevent jumps due to light touch at the origins

class x52p_linux64 {
public:                     // Acces specifier, public = can be accessed and modified outside the class
    // Class constructors!
    x52p_linux64();         // Default constructor
    x52p_linux64(int id);   // Consctructor, accepts arguments, will be defined outside the class via Class::Class( args )

    // Class methods for SDL
    SDL_Joystick* InitDev();
    JoyCaps GetCaps();
    JoyStates GetState();
    int GetButtonNum();
    void UnacqDev();

    // Class methods for state normalization
    double XJoy();
    double YJoy();
    double ZJoy();
    double RXJoy();
    double RYJoy();
    double RZJoy();
    double slid();
    double throtmouseX();
    double throtmouseY();
    int povdeg();

    // Class methods for buttons
    int IsButtonPressed(int button_id);

private:
    // Class important variables! In private for safety! Comment out the above //private: for debugging!
    SDL_Joystick* thejoys;      // SDL_Joystick structure of SDL API
    JoyCaps caps;           
    JoyStates state;
    int joystick_id; 
};

#endif