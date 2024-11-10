
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#ifndef GEOMH
#define GEOMH
#include <iostream>
#include <cmath>
#include <string>

class Vec3D {
	public:
	float x, y, z;
	void show(std::string label)const;
	Vec3D(float x, float y, float z);
	Vec3D neg()const;
	Vec3D add(Vec3D const &other)const;
	Vec3D sub(Vec3D const &other)const;
	Vec3D mul(float scalar)const;
	Vec3D div(float scalar)const;
	float len()const;
	Vec3D unit()const;
	float dot(Vec3D const &other)const;
	Vec3D cross(Vec3D const &other)const;
	bool isFinite()const;
	Vec3D rotate(Vec3D const &axis,float angle)const;
	Vec3D rotFast(Vec3D const &axis,float angle)const;
	Vec3D reflect(Vec3D const& normal)const;
	Vec3D project(Vec3D const& onto)const;
	float projectionLen(Vec3D const& onto)const;
};

void showFloat(std::string label,float scalar);

void newLine();

void vec3UnitTest();

class Ray{
	public:
		Ray(Vec3D const &pos,Vec3D const &dir);
		Ray(float x,float y,float z,float a,float b,float c);
		Ray();
		Vec3D support;
		Vec3D direction;
		void show(std::string label)const;
};

class Sphere{
	public:
		Sphere(float x,float y,float z,float r);
		Sphere(Vec3D const &pos,float r);
		Vec3D center;
		float radius;
		void show(std::string label)const;
		float distFromRay(Ray const &ray)const;
		bool hit(Ray const &ray)const;
		Vec3D hitPoint(Ray const &ray)const;
};

void SphereRayTest();


class Plane{
	public:
		Plane(Vec3D const &pos,Vec3D const &normal);
		Vec3D support;
		Vec3D normal;
		Vec3D hitPoint(Ray const &ray)const;
};

void PlaneRayTest();

#endif
