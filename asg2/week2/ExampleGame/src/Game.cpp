//------------------------------------------------------------------------
// Game
//
// Created:	2012/12/06
// Author:	Carel Boers
//	
// This class serves as a base Game class. Derive from this class and
// override the pure virtuals to setup the Init/Update/Render hooks for
// the main game logic.
//------------------------------------------------------------------------

#define GLFW_INCLUDE_GL3
#define GLFW_NO_GLU
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GL/glfw.h>
#include "W_Common.h"
#include <glm/glm.hpp>

#include "Game.h"

using namespace Common;

//------------------------------------------------------------------------------
// Method:    Game
// Parameter: void
// Returns:   
// 
// Constructor
//------------------------------------------------------------------------------
Game::Game()
{
}

//------------------------------------------------------------------------------
// Method:    ~Game
// Parameter: void
// Returns:   
// 
// Destructor.
//------------------------------------------------------------------------------
Game::~Game()
{
}

//------------------------------------------------------------------------------
// Method:    Run
// Returns:   int
// 
// Starts (and runs) the main game loop. Does not return until application
// shuts down.
//------------------------------------------------------------------------------
int Game::Run(const char* p_strAppName, int p_iWindowWidth, int p_iWindowHeight)
{
	int width, height, x;
	double t;

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return EXIT_FAILURE;
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( p_iWindowWidth, p_iWindowHeight, 0,0,0,0, 24,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
		return EXIT_FAILURE;
	}

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// Leave this here! On windows machines in Lab, glewInit is generating some mysterious, but apparently
	// harmless, error - and we need it gone.
	glGetError();

	glfwSetWindowTitle( p_strAppName );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Enable vertical sync (on cards that support it)
	glfwSwapInterval( 1 );

	// Call Init() on game subclass.
	bool bContinue = this->Init();
	glEnable(GL_DEPTH_TEST);
	while (bContinue
			&& glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS
			&& glfwGetWindowParam(GLFW_OPENED))
	{
		t = glfwGetTime();
		glfwGetMousePos( &x, NULL );

		// Get window size (may be different than the requested size)
		glfwGetWindowSize( &width, &height );

		// Special case: avoid division by zero below
		height = height > 0 ? height : 1;

		glViewport( 0, 0, width, height );

		// Clear color buffer to black
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// Update and render game
		bContinue = this->Update((float)t - m_fFrameTime);
		this->Render();

		// Swap buffers
		glfwSwapBuffers();

		m_fFrameTime = t;
	}

	// Shutdown app
	this->Shutdown();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	exit( EXIT_SUCCESS );
}
