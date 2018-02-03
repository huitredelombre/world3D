
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
	Interface::inputManager=new InputManager(interface, camera);

	//shaders (a passer dans la classe drawer)
	
	Shader shader("view/drawer/gpu/Shaders/geometry.vert", "view/drawer/gpu/Shaders/texture.frag");
	shader.charger();


	while (!interface->windowShouldClose())
	{

		//update block (to put in Mover class (move all object that havent 0 as acceleration and check collision) 

		camera->move();
		Interface::inputManager->update();
		


		//render block(to put in renderer class
		glm::mat4 lookAtMatrix = camera->getLookAtMatrix();	
		glm::mat4 projectionMatrix = camera->getProjectionMatrix(interface->getRatio());
		//shader shit 
		
		/*feeding vertex shader whit projec and look at matrix */	
		GLuint lookAtMatrixVramLocation = glGetUniformLocation(shader.getProgramID(), "lookAtMatrix");//TODO dans la classe drawer optimiser ça (pointeur vrm fixe)
		glUniformMatrix4fv(lookAtMatrixVramLocation , 1, GL_FALSE, glm::value_ptr(lookAtMatrix));

		GLuint projectionMatrixVramLocation = glGetUniformLocation(shader.getProgramID(), "projectionMatrix");//TODO dans la classe drawer optimiser ça (pointeur vrm fixe)
		glUniformMatrix4fv(projectionMatrixVramLocation , 1, GL_FALSE, glm::value_ptr(projectionMatrix));



		glUseProgram(shader.getProgramID());

		//envoi de data. a foutre en VBO

				
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
		//essential stuff, SwapBuffer replace the ancient drawing whit the new, PollEvent process all event received since last poll event and proced them
		//TODO we should try to turn this busy wait into a lazy one see http://www.glfw.org/docs/latest/input_guide.html glfwWaitEvent()
		interface->refreshBuffer();
		interface->threatEvents();


	}
	interface->quit();
	delete(interface);
	delete(camera);
	exit(EXIT_SUCCESS);
}


