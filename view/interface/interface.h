#ifndef INTERFACE
#define INTERFACE
#include <GL/glew.h>
#include "inputmanager.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>//error control
#include <stdio.h>

class InputManager; //forward declaration

class Interface{
	public:	
		static InputManager* inputManager;//lib-unrelated callback wrapper 
		/*
		 * Parameter : size of the window, responsible for creation of the window and opengl extensions loading
		 * */
		Interface(int width, int height);
		/*retrieve total deplacement of the mous since the creation of 
		 * the window 
		 * X: + = right
		 * Y: + = up
		 * */
		void getMousePosition(double* receiveX, double* receiveY);
		
		float getRatio();
		/*
		 * Close the window
		 * */
		void closeWindow();

		/*
		 * return true if closeWindow had been called
		 * */
		bool windowShouldClose();

		/**
		 * update the window
		 * */
		void refreshBuffer();
		
		/**
		 * Threat receved event 
		 * */	
		void threatEvents();
	//TODO should be in destructor??	
		void quit();
	private:
		GLFWwindow* window;	
		float ratio;
};
#endif
