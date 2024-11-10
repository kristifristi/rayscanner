/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#include "geometry.hpp"
#include "renderer.hpp"
#include "ppmReader.hpp"

class MatBall : public Object{
	public:
		colorBuffer getColorBuffer(Ray const& ray)const override;
		colorBuffer getColorBufferLM(Ray const& ray,int levelsRemaining)const override;
		MatBall(float x,float y,float z,float r);
		Sphere ball;
};


class Floor : public Object{
	public:
		colorBuffer getColorBuffer(Ray const& ray)const override;
		colorBuffer getColorBufferLM(Ray const& ray,int levelsRemaining)const override;
		Floor(float height,float squareSize);
		float height;
		float squareSize;
};

class MirrorBall : public Object{
	public:
		colorBuffer getColorBuffer(Ray const& ray)const override;
		colorBuffer getColorBufferLM(Ray const& ray,int levelsRemaining)const override;
		MirrorBall(float x,float y,float z,float r,Scene& scene);
		Sphere ball;
		Scene& environment;
};

class TexturePlane : public Object{
	public:
		//rotation in radians
		//rotated in this order: yaw,pitch,roll
		TexturePlane(Vec3D const&pos,float pitch,float yaw,float roll,std::string image,std::string transparency,float scale);
		colorBuffer getColorBuffer(Ray const& ray)const override;
		colorBuffer getColorBufferLM(Ray const& ray,int levelsRemaining)const override;
		Plane plane;
		Vec3D pitchVector;
		Vec3D yawVector;
		ValueMap* texture;
		ValueMap* mask;
		//higher scale makes image bigger
		float scale;
		void changePitch(float angle);
		void changeYaw(float angle);
		void changeRoll(float angle);
};

