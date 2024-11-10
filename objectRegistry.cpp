
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#include "objectRegistry.hpp"


MatBall::MatBall(float x,float y,float z,float r):ball(x,y,z,r){}

colorBuffer MatBall::getColorBuffer(Ray const& ray)const{
	if(ball.center.sub(ray.support).dot(ray.direction) < 0.0){
		return {0,-1};
	}
	Vec3D hitPos = ball.hitPoint(ray);
	if(hitPos.isFinite()){
		return {1,hitPos.sub(ray.support).len()};
	}
	return {0,-1};
}

colorBuffer MatBall::getColorBufferLM(Ray const& ray,int levelsRemaining)const{
	return getColorBuffer(ray);
}



Floor::Floor(float height,float squareSize): height(height),squareSize(squareSize) {}

colorBuffer Floor::getColorBuffer(Ray const& ray)const{
	if(ray.direction.z == 0)
		return {0,-1};

	float determinant = (height - ray.support.z) / ray.direction.z;
	if(determinant <= 0)
		return {0,-1};

	Vec3D hitPos = Vec3D(ray.direction.x,ray.direction.y,height).mul(determinant)
		.add(Vec3D(ray.support.x,ray.support.y,height));

	bool pivot = fabs(fmod(hitPos.x,squareSize)) < squareSize/2.0;
	float phase = fabs(fmod(hitPos.y,squareSize));
	bool signx = hitPos.x > 0;
	bool signy = hitPos.y > 0;
	colorBuffer buffer = {0,hitPos.sub(ray.support).len()};
	if(pivot != (signy != signx) ? phase < squareSize/2.0 : phase >= squareSize/2.0)
		buffer.value = 1;
	return buffer;
}

colorBuffer Floor::getColorBufferLM(Ray const& ray,int levelsRemaining)const{
	return getColorBuffer(ray);
}



MirrorBall::MirrorBall(float x,float y,float z,float r,Scene& scene):ball(x,y,z,r), environment(scene){}

colorBuffer MirrorBall::getColorBuffer(Ray const& ray)const {
	return getColorBufferLM(ray,1000);
}

colorBuffer MirrorBall::getColorBufferLM(Ray const& ray,int levelsRemaining)const{
	if(ball.center.sub(ray.support).dot(ray.direction) < 0.0){
		return {0,-1};
	}
	Vec3D hitPos = ball.hitPoint(ray);
	if(hitPos.isFinite()){
		Ray reflector(hitPos,ray.direction.reflect(hitPos.sub(ball.center)));
	colorBuffer color = {0.4,hitPos.sub(ray.support).len()};
		if(levelsRemaining > 1){
		colorBuffer recurse = raycastScene(reflector,environment,0.001,levelsRemaining - 1);
			color.value += recurse.value * 0.6;
		}
		return color;
	} else {
		return {0,-1};
	}
}


void TexturePlane::changePitch(float angle){
	plane.normal = plane.normal.rotate(pitchVector,angle);
	yawVector = yawVector.rotate(pitchVector,angle);
}

void TexturePlane::changeYaw(float angle){
	plane.normal = plane.normal.rotate(yawVector,angle);
	pitchVector = pitchVector.rotate(yawVector,angle);
}

void TexturePlane::changeRoll(float angle){
	pitchVector = pitchVector.rotate(plane.normal,angle);
	yawVector = yawVector.rotate(plane.normal,angle);
}

//rotation in radians
//rotated in this order: yaw,pitch,roll
//if transparency and texture are different, everything breaks
TexturePlane::TexturePlane(Vec3D const&pos,float pitch,float yaw,float roll,std::string image,std::string transparency,float scale):
	plane(pos,Vec3D(1,0,0)), pitchVector(0,1,0),yawVector(0,0,1), scale(scale){
	this->changeYaw(yaw);
	this->changePitch(pitch);
	this->changeRoll(roll);
	texture = readPPM(image);
	mask = readPPM(transparency);
}

colorBuffer TexturePlane::getColorBuffer(Ray const& ray)const {
	if(plane.support.sub(ray.support).dot(ray.direction) < 0.0){
		return {0,-1};
	}
	Vec3D hit = plane.hitPoint(ray);
	if(!hit.isFinite())
		return {0.0,-1.0};
	Vec3D relative = hit.sub(plane.support);
	//dot product gives length along axes because axes are normalised
	vec2 imageHit = {relative.dot(pitchVector)/scale,relative.dot(yawVector)/scale};
	int2 imagePixel = convert_pos(imageHit,texture->dims);
	if(imagePixel.x < 0 || imagePixel.y < 0 || imagePixel.x >= texture->dims.width || imagePixel.y >= texture->dims.height)
		return {0.0,-1.0};
	if(mask->matrix[imagePixel.y][imagePixel.x] < 0.5)
		return {0.0,-1.0};
	return {texture->matrix[imagePixel.y][imagePixel.x],hit.sub(ray.support).len()};
}

colorBuffer TexturePlane::getColorBufferLM(Ray const& ray,int levelsRemaining)const {
	return this->getColorBuffer(ray);
}


