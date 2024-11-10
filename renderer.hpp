
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#ifndef RENDERH
#define RENDERH
#include "int2_t.h"
#include <iostream>
#include <vector>
#include <thread>
#include "terminal.h"
#include "geometry.hpp"


//so I don't have to keep calculating the minor axis
typedef struct screenDims_struct{
	int width;
	int height;
	int scale;
}screenDims;

int minAxis(int2 screen);

class ValueMap {
	public:
	//outer vector is rows, inner columns
	std::vector<std::vector<float>> matrix;
	screenDims dims;
	ValueMap(int width,int height);
};

int2 convert_pos(vec2 pos,screenDims screen);
vec2 revert_pos(int2 pos, screenDims screen);

typedef struct colorbuffer_struct{
	float value;
	float dist;
}colorBuffer;


class Object {
	public:
	virtual colorBuffer getColorBuffer(Ray const& ray)const = 0;
	virtual colorBuffer getColorBufferLM(Ray const& ray,int levelsRemaining)const = 0; //limited recursion
};

//typedef std::vector<void*>(0) Scene;
#define Scene std::vector<Object*> 

void printMap(ValueMap* map);
void printAscii(ValueMap* map);

colorBuffer raycastScene(Ray const& ray,Scene const& scene,float screenDistance,float levels);

class Camera {
	public:
	Vec3D pitchVector; //the names mean they are the AXIS of the rotation
	Vec3D yawVector;
	Vec3D rollVector;
	Vec3D support;
	Scene& scene;
	ValueMap** map;
	float screenDistance;
	float fov;
	Camera(Scene& scen,ValueMap** mp);
	void changePitch(float angle);
	void changeYaw(float angle);
	void changeRoll(float angle);
	void render();
	void renderThread(int i);
	void renderMultiThread();
};

#endif
