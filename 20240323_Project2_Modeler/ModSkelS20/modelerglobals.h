#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum ModelControls
{ 
	LOD, XPOS, YPOS, ZPOS,
	WING_A, WING_B, WING_C, WING_D, WING_E, 
	MOTION_SET,
	LIGHT0_X, LIGHT0_Y, LIGHT0_Z, LIGHT0_INTENSITY, 
	LIGHT1_X, LIGHT1_Y, LIGHT1_Z, LIGHT1_INTENSITY, WIDGETS, 
	INVERSEKINEMATICSX, INVERSEKINEMATICSY, INVERSEKINEMATICSZ, 
	DISPLAY_LSYSTEM, LSYSTEM_OBJECT_TYPE, LSYSTEM_INITIAL_ANGLE, LSYSTEM_LENGTH_FACTOR, LSYSTEM_INCREMENT_ANGLE, LSYSTEM_ITERATION,
	NUMCONTROLS
};

// Colors
#define COLOR_RED		1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE		0.0f, 0.0f, 1.0f
#define COLOR_WHITE		1.0f, 1.0f, 1.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif