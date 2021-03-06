//http://www.glfw.org/docs/latest/input_guide.html
#include "inputmanager.h"
#include "debug.h"
#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>//transforming mouse input into angle 

//to remove 

InputManager::InputManager(Interface* interface, Camera* camera, Gpu* gpu){
	oldXrotation=vec3(0.0f,0.0f,-1.0f);
	oldMouseX= 0.0;
	oldMouseY=0.0;

	left=false;
	right=false;
	up=false;
	down=false;
	this->gpu=gpu;
	this->camera=camera;
	this->interface=interface;
}
void InputManager::upPress(){
	up=true;
	down=false;
}

void InputManager::upRelease(){
	up=false;
}

void InputManager::downPress(){
	down=true;
	up=false;
}

void InputManager::downRelease(){
	down=false;
}

void InputManager::leftPress(){
	left=true;
	right=false;
}

void InputManager::leftRelease(){
	left=false;
}

void InputManager::rightPress(){
	right=true;
	left=false;
}

void InputManager::rightRelease(){
	right=false;
}

bool InputManager::updateOrientation(){
	double xMouseD, yMouseD;
	interface->getMousePosition(&xMouseD, &yMouseD);	
	float xMouse= (float) xMouseD;
	float yMouse=(float) yMouseD;

	if (xMouse==oldMouseX && yMouse	== oldMouseY){//same situation than last time 
		return false;
	}
	vec3 initialDir=vec3(0.0f,0.0f,-1.0f);//TODO put up and right global, use more pointer, initial dir should be linked to the direction entered in the main
	float speed = camera->getRotationSpeed(); 
	if (xMouse!=oldMouseX){//moved horizontally
		vec3 up= vec3(0.0f,1.0f,0.0f);
		oldXrotation=rotate(initialDir, xMouse*-speed, up); //dont ask why the -
		camera->setCrossProduct(cross(up, oldXrotation));
		//now X rotation and crossProduct are updated
	}
	//need to reorient in all cases	
	
	camera->orient(rotate(oldXrotation, yMouse*-speed, camera->getCrossProduct()));
	gpu->transferLookAtMatrix(camera->getLookAtMatrix());	
	oldMouseX=xMouse;
	oldMouseY=yMouse;
#ifdef DEBUG
	printf("\n===============================================================\nMouse Postion info: \n\nX= %f\nY=%f\nSpeed: %f \n",xMouse,yMouse,speed );
	camera->debug();
#endif
	return true;
}
bool InputManager::updateAcceleration(){	
	camera->stop();
	if (up){
		camera->accelerateForward();
		return true;

	}

	else if (down){
		camera->accelerateBackward();
		return true;

	}

	if (left){
		camera->accelerateLeft();
		return true;

	}
	else if(right){
		camera->accelerateRight();
		return true;
	}
	return false;
}

void InputManager::escapePressed(){
	interface->closeWindow();
}

void InputManager::update(){
	if (updateOrientation()){
		gpu->setLookAtMatrix(camera->getLookAtMatrix());	
		updateAcceleration();
	}
	else if(updateAcceleration()){//avoiding lazy evaluation
		gpu->setLookAtMatrix(camera->getLookAtMatrix());	
	}
}


