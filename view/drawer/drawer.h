#ifndef DRAWER 
#define DRAWER
#include "triangle.h"
#include "planet.h"
#include "gpu.h"
/*
 * This class do the link from world to view: it define how the data
 * should be rendered 
 * */
class Drawer{	
	public:
		Drawer(Gpu* gpu);
		void draw(Triangle triangle); 
		void draw(Triangle* triangle); 

		void draw(Planet* planet);

	private:
		Gpu* gpu;
};
#endif

