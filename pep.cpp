
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#include "renderer.hpp"
#include "terminal.h"
#include <iostream>
#include "objectRegistry.hpp"
#include <thread>
#include <chrono>


void controller(Camera* c1){
	float controlScale = 1.0;
	while(1){
	char input = getc(stdin);
	switch(input){
		case 'w':
			c1->support = c1->support.add(c1->rollVector.mul(0.1 * controlScale));
			break;
		case 's':
			c1->support = c1->support.add(c1->rollVector.mul(-0.1* controlScale));
			break;
		case 'd':
			c1->support = c1->support.add(c1->pitchVector.mul(0.1* controlScale));
			break;
		case 'a':
			c1->support = c1->support.add(c1->pitchVector.mul(-0.1* controlScale));
			break;
		case 'q':
			c1->support.z += 0.1* controlScale;
			break;
		case 'e':
			c1->support.z -= 0.1* controlScale;
			break;
		case 'i':
			c1->changePitch(M_PI / -50.0* controlScale);
			break;
		case 'k':
			c1->changePitch(M_PI / 50.0* controlScale);
			break;
		case 'j':
			c1->changeYaw(M_PI / -50.0* controlScale);
			break;
		case 'l':
			c1->changeYaw(M_PI / 50.0* controlScale);
			break;
		case 'u':
			c1->changeRoll(M_PI / 50.0* controlScale);
			break;
		case 'o':
			c1->changeRoll(M_PI / -50.0* controlScale);
			break;
		case 'p':
			c1->rollVector = Vec3D(1,0,0);
			c1->pitchVector = Vec3D(0,1,0);
			c1->yawVector = Vec3D(0,0,1);
		case 'r':
			c1->fov -= 0.1* controlScale;
			break;
		case 'f':
			c1->fov += 0.1* controlScale;
			break;
		case 'z':
			controlScale /= 2.0;
			break;
		case 'x':
			controlScale *= 2.0;
			break;
		}
	}
}

typedef std::chrono::duration<int,std::milli> milliseconds_t;

int main(){
	int2 size = getScreenSize();
	ValueMap screenBuffer(size.x/2,size.y);
	Scene VPO;
	TexturePlane* tester = new TexturePlane(Vec3D(10,0,3),0,M_PI_4,0,"textures/meow.ppm","textures/meowMask.ppm",3);
	
	VPO.push_back(new MirrorBall(20,0,3,5,VPO));
	VPO.push_back(tester);
	VPO.push_back(new MatBall(5,0,0,1));
	VPO.push_back(new MatBall(0,5,0,2));
	VPO.push_back(new MatBall(0,0,5,4));
	VPO.push_back(new Floor(0,5));
	VPO.push_back(new TexturePlane(Vec3D(-5,-5,2),0,-M_PI_4,0,"textures/nick.ppm","textures/nickMask.ppm",1));
	VPO.push_back(new TexturePlane(Vec3D(20,20,10),0,0,0,"textures/meow.ppm","textures/meowMask.ppm",1));
	VPO.push_back(new TexturePlane(Vec3D(20,20,10),M_PI_2,0,0,"textures/meow.ppm","textures/meowMask.ppm",1));
	VPO.push_back(new TexturePlane(Vec3D(20,20,10),M_PI,0,0,"textures/meow.ppm","textures/meowMask.ppm",1));
	VPO.push_back(new TexturePlane(Vec3D(20,20,10),M_PI * 1.5,0,0,"textures/meow.ppm","textures/meowMask.ppm",1));
	VPO.push_back(new TexturePlane(Vec3D(0,5,5),0,0,M_PI * 1.1,"textures/meow.ppm","textures/meowMask.ppm",3));
	VPO.push_back(new TexturePlane(Vec3D(-20,0,2.1),0,0,0,"textures/wizard.ppm","textures/wizardMask.ppm",2));
	VPO.push_back(new MirrorBall(-20,0,3.7,0.5,VPO));
	TexturePlane* halo = new TexturePlane(Vec3D(-20,-0.2,4),M_PI_4,M_PI_4,0,"textures/halo.ppm","textures/haloMask.ppm",1);
	VPO.push_back(halo);
	ValueMap* bufRef = &screenBuffer;
	Camera c1(VPO,&bufRef);
	c1.support.y = 5;
	//c1.screenDistance = 1.0;
	setCanonical(false);
	std::thread controls(controller,&c1);
	while(1){
		halo->changeRoll(0.1);
		c1.renderMultiThread();
		std::this_thread::sleep_for(milliseconds_t(50));
		printMap(bufRef);
	}
}

