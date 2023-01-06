// ---------------------------------------------------------------------------------------------------------- //
// Osaka University: Dimas M. Rachman, 2023.
// Sample of implementation. Requires x52p_linux64.h header and x52p_linux64.cpp functions definitions files.
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
//			2. x52p_linux64.cpp the function definition file
//			3. Your .cpp impelementation file: as an example this file.

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

#include "x52p_linux64.cpp"     // Functions definitions

// Variable type declaration
double locaxis[9];
int povaim;
int buttons[39];

int main() {
    InitSDL();  // Initialize the SDL API interface
    x52p_linux64 controller(0); // Instantiate an object/instance with ID as argument

    int button_num = controller.GetButtonNum(); // Get the button number

    while (1) {
        controller.GetState();  // Get device's state

        locaxis[0] = controller.XJoy(); // Axes
        locaxis[1] = controller.YJoy();
        locaxis[2] = controller.ZJoy();
        locaxis[3] = controller.RXJoy();
        locaxis[4] = controller.RYJoy();
        locaxis[5] = controller.RZJoy();

        locaxis[6] = controller.slid(); // Slider

        locaxis[7] = controller.throtmouseX(); // Throttle mouse
        locaxis[8] = controller.throtmouseY();

        povaim = controller.povdeg(); // Jostyick PovAim

        printf("X: %4f", locaxis[0]);
        printf("Y: %4f", locaxis[1]);
        printf("Z: %4f", locaxis[2]);
        printf("RX: %4f", locaxis[3]);
        printf("RY: %4f", locaxis[4]);
        printf("RZ: %4f", locaxis[5]);
        printf("Sld: %4f", locaxis[6]);
        printf("ThMouseX: %4f", locaxis[7]);
        printf("ThMouseY: %4f", locaxis[8]);
        printf("PovJoy: %1d", povaim);
        
        for (int i = 0; i < button_num; ++i) {
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
					break;
				case 3:
					printf("Joy button %1d (B). ", i + 1);
					break;
				case 4:
					printf("Joy button %1d (C). ", i + 1);
					break;
				case 5:
					printf("Joy button %1d (pink fire). ", i + 1);
					break;
				case 6:
					printf("Throttle button %1d (D). ", i + 1);
					break;
				case 7:
					printf("Throttle button %1d (E). ", i + 1);
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
                case 14:
                    printf("Joy button %1d (index full fire). ", i + 1);
                    break;  
                case 15:
                    printf("Throttle button %1d (mouse click). ", i + 1);
                    break;   
                case 16:
                    printf("Throttle roller %1d (outside). ", i + 1);
                    break;  
                case 17:
                    printf("Throttle roller %1d (inside). ", i + 1);
                    break;  
                case 18:
                    printf("Throttle roller %1d. ", i + 1);
                    break;                
				case 19:
					printf("Joy aim button %1d (up). ", i + 1);
					break;
				case 20:
					printf("Joy aim button %1d (right). ", i + 1);
					break;
				case 21:
					printf("Joy aim button %1d (down). ", i + 1);
					break;
				case 22:
					printf("Joy aim button %1d (left). ", i + 1);
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
                case 27:
                    printf("Joy mode switch %d (1st). ", i + 1);
                    break;
                case 28:
                    printf("Joy mode switch %d (2nd). ", i + 1);
                    break;    
                case 29:
                    printf("Joy mode switch %d (3rd). ", i + 1);
                    break;
                case 30:
                    printf("Throttle button %1d (i). ", i + 1);
                    break;    
				case 31:
					printf("Display button %1d (FUNCTION). ", i + 1);
					break;
                case 32:
                    printf("Display button %1d (START/STOP/up). ", i + 1);
                    break;   
                case 33:
                    printf("Display button %1d (RESET/down). ", i + 1);
                    break;     
                case 34:
                    printf("Display button %1d (FUNCTION, Pg. up). ", i + 1);
                    break;      
                case 35:
                    printf("Display button %1d (FUNCTION, Pg. down). ", i + 1);
                    break;   
                case 36:
                    printf("Display button %1d (MFD-SELECT, up). ", i + 1);
                    break;      
                case 37:
                    printf("Display button %1d (MFD-SELECT, down). ", i + 1);
                    break;    
                case 38:
                    printf("Display button %1d (MFD-SELECT). ", i + 1);
                    break;              
				}
				//printf("Button:%1d ", i);	// Check simultaneously, comment out the above whole switch
			}
		}
        printf("\n");
    }
    controller.UnacqDev(); // Close device and free memory

}