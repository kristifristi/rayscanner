
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

int main(){
	int2 size = getScreenSize();
	ValueMap screenBuffer(size.x/2,size.y);
	Scene VPO;
	VPO.push_back(new Floor(0,5));
	VPO.push_back(new MatBall(3,1,5,3));
	VPO.push_back(new MirrorBall(10,10,10,9,VPO));
	VPO.push_back(new MirrorBall(10,25,10,6,VPO));
	VPO.push_back(new MirrorBall(3,17,5,1,VPO));
	ValueMap* bufRef = &screenBuffer;
	Camera c1(VPO,&bufRef);
	c1.support.y = 5;
	//c1.screenDistance = 1.0;
	setCanonical(false);
	while(1){
		c1.render();
		printAscii(bufRef);
		char input = getc(stdin);
		switch(input){
			case 'w':
				c1.support = c1.support.add(c1.rollVector.mul(0.1));
				break;
			case 's':
				c1.support = c1.support.add(c1.rollVector.mul(-0.1));
				break;
			case 'd':
				c1.support = c1.support.add(c1.pitchVector.mul(0.1));
				break;
			case 'a':
				c1.support = c1.support.add(c1.pitchVector.mul(-0.1));
				break;
			case 'q':
				c1.support.z += 0.1;
				break;
			case 'e':
				c1.support.z -= 0.1;
				break;
			case 'i':
				c1.changePitch(M_PI / -50.0);
				break;
			case 'k':
				c1.changePitch(M_PI / 50.0);
				break;
			case 'j':
				c1.changeYaw(M_PI / -50.0);
				break;
			case 'l':
				c1.changeYaw(M_PI / 50.0);
				break;
			case 'u':
				c1.changeRoll(M_PI / 50.0);
				break;
			case 'o':
				c1.changeRoll(M_PI / -50.0);
				break;
			case 'p':
				c1.rollVector = Vec3D(1,0,0);
				c1.pitchVector = Vec3D(0,1,0);
				c1.yawVector = Vec3D(0,0,1);
			case 'r':
				c1.fov -= 0.1;
				break;
			case 'f':
				c1.fov += 0.1;
				break;
		}
	}
}

