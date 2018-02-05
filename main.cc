
#include <stdlib.h>
#include <stdio.h>


#include "camera.h"
#include "interface.h"
#include "inputmanager.h"
#include "Shader.h"
#include "gpu.h"

//to move
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp> //cross plateform cast matrix to gpu


int main(void)
{	
	//initialisation des objets
	int height=480;
	int width=640;
	float moveSpeed=0.05f;
	float rotationSpeed=0.0005f;	
	float* initialPosition= (float*) malloc(3*sizeof(float));
	initialPosition[0]=0.0f;
	initialPosition[1]=0.0f;
	initialPosition[2]=-1.0f;
	float fov=0.6f;
	float nearLimit=0.1f;
	float farLimit=10000.0f;

	Interface* interface= new Interface(width, height);	
	Camera* camera=new Camera(interface->getRatio(), initialPosition, moveSpeed, rotationSpeed, fov, nearLimit, farLimit);	

	//TODO le main a un drawer, et pas un gpu
	Gpu* gpu=new Gpu();
	Interface::inputManager=new InputManager(interface, camera, gpu);	
	gpu->cameraMode();//activating camera mode once for all (never udapted yet)
	//sending settings of camera to GPU once for all (never udapted yet)	
	gpu->transferProjectionMatrix(camera->getProjectionMatrix(interface->getRatio()));

	while (!interface->windowShouldClose())
	{

		//update block (move all moveable accordingly to acceleration) 
		
		camera->move();

		//compute users inputs
		Interface::inputManager->update();

		//TODO  VBO/VAO ?


		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		/*glMatrixMode(GL_PROJECTION);
		  glLoadIdentity();
		  glOrtho(-interface->getRatio(), interface->getRatio(), -1.f, 1.f, 1.f, -1.f);
		  glMatrixMode(GL_MODELVIEW);
		  glLoadIdentity();
		  glRotatef((float) glfwGetTime() * 50.f, 0.f, 1.f, 1.f);
		  */
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		//TODO we should try to turn this busy wait into a lazy one see http://www.glfw.org/docs/latest/input_guide.html glfwWaitEvent()
		interface->refreshBuffer();
		interface->threatEvents();//store input for later computation


	}
	interface->quit();
	delete(interface);
	delete(camera);
	exit(EXIT_SUCCESS);
}


