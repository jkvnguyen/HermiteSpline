////////////////////////////////////////////////////
// // Template code for  CS 174C
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <shared/defs.h>

#include "shared/opengl.h"

#include <string.h>
#include <util/util.h>
#include <GLModel/GLModel.h>
#include "anim.h"
#include "animTcl.h"
#include "myScene.h"
#include "SampleParticle.h"
#include "Hermite.h"
#include "SampleGravitySimulator.h"
#include <util/jama/tnt_stopwatch.h>
#include <util/jama/jama_lu.h>
#include <util/Picker.h>

// register a sample variable with the shell.
// Available types are:
// - TCL_LINK_INT 
// - TCL_LINK_FLOAT

int g_testVariable = 10;

SETVAR myScriptVariables[] = {
	"testVariable", TCL_LINK_INT, (char *) &g_testVariable,
	"",0,(char *) NULL
};


//---------------------------------------------------------------------------------
//			Hooks that are called at appropriate places within anim.cpp
//---------------------------------------------------------------------------------

// start or end interaction
void myMouse(int button, int state, int x, int y)
{

	// let the global resource manager know about the new state of the mouse 
	// button
	GlobalResourceManager::use()->setMouseButtonInfo( button, state );

	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{

		BaseSystem* sampleSystemRetrieval =
			GlobalResourceManager::use()->getSystem("hermite");

		Hermite *hermite = (Hermite*)sampleSystemRetrieval; 

		Vector temp; 
		pickFromXYPlane(temp, x, y);

		hermite->add(temp[0], temp[1], temp[2], 0, 0, 0);
		hermite->computeCR();
		
		animTcl::OutputMessage(
			"Point x: %f y: %f addded.", temp[0], temp[1]);
	}
	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{
		//animTcl::OutputMessage(
			//"My mouse received a mouse button release event\n") ;
	}
}	// myMouse

// interaction (mouse motion)
void myMotion(int x, int y)
{

	GLMouseButtonInfo updatedMouseButtonInfo = 
		GlobalResourceManager::use()->getMouseButtonInfo();

	if( updatedMouseButtonInfo.button == GLUT_LEFT_BUTTON )
	{
		//animTcl::OutputMessage(
		//	"My mouse motion callback received a mousemotion event\n") ;
	}

}	// myMotion


void MakeScene(void)
{

	/* 
	
	This is where you instantiate all objects, systems, and simulators and 
	register them with the global resource manager

	*/


}	// MakeScene

// OpenGL initialization
void myOpenGLInit(void)
{
	animTcl::OutputMessage("Initialization routine was called.");

}	// myOpenGLInit

void myIdleCB(void)
{
	
	return;

}	// myIdleCB

void myKey(unsigned char key, int x, int y)
{
	 animTcl::OutputMessage("Updated display.");
	return;

}	// myKey

static int startPart1(ClientData clientData, Tcl_Interp *interp, int argc, myCONST_SPEC char **argv)
{

	animTcl::OutputMessage("Part 1 started.");

	bool success;

	// instantiate hermite iff "part1" is entered
	Hermite* hermite = new Hermite("hermite");

	// make sure it was registered successfully
	success = GlobalResourceManager::use()->addSystem(hermite, true);

	//gets warning if part1 entered twice (ignore to continue if entered twice)
	assert(success);

	return TCL_OK;

}	// testGlobalCommand

static int startPart2(ClientData clientData, Tcl_Interp *interp, int argc, myCONST_SPEC char **argv)
{

	animTcl::OutputMessage("Part 2: I have no idea.");
	return TCL_OK;

}

void mySetScriptCommands(Tcl_Interp *interp)
{

	// here you can register additional generic (they do not belsong to any object) 
	// commands with the shell

	Tcl_CreateCommand(interp, "part1", startPart1, (ClientData) NULL,
					  (Tcl_CmdDeleteProc *)	NULL);

	Tcl_CreateCommand(interp, "part2", startPart2, (ClientData)NULL,
		(Tcl_CmdDeleteProc *)NULL);
}	// mySetScriptCommands
