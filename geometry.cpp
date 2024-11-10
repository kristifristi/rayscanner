
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#include "geometry.hpp"

void newLine(){
	std::cout << std::endl;
}

Vec3D::Vec3D(float a, float b, float c):x(a),y(b),z(c){}

void showFloat(std::string label,float scalar){
	std::cout << label << ": " << scalar << "\n";
}

void Vec3D::show(std::string label)const {
	std::cout << label << ": (" << x << ',' << y << ',' << z << ")\n";
}

Vec3D Vec3D::neg()const {
	return Vec3D(-x,-y,-z);
}

Vec3D Vec3D::add(Vec3D const &other)const {
	return Vec3D(x + other.x,y + other.y,z + other.z);
}

Vec3D Vec3D::sub(Vec3D const &other)const {
	return Vec3D(x - other.x,y - other.y,z - other.z);
}

Vec3D Vec3D::mul(float scalar)const {
	return Vec3D(x * scalar,y * scalar,z * scalar);
}

Vec3D Vec3D::div(float scalar)const {
	return Vec3D(x / scalar,y / scalar, z / scalar);
}

float Vec3D::len()const {
	return sqrt(x*x + y*y + z*z);
}

Vec3D Vec3D::unit()const {
	float denom = len();
	return Vec3D(x/denom,y/denom,z/denom);
}

float Vec3D::dot(Vec3D const &other)const {
	return x * other.x + y * other.y + z * other.z;
}

Vec3D Vec3D::cross(Vec3D const &other)const {
	return Vec3D(
		y*other.z - z*other.y,
		z*other.x - x*other.z,
		x*other.y - y*other.x);
}

bool Vec3D::isFinite()const {
	return std::isfinite(x) && std::isfinite(y) && std::isfinite(z);
}

Vec3D Vec3D::rotate(Vec3D const &axis,float angle)const{
	Vec3D axisUnit = axis.unit();
	//https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
	return this->mul(cos(angle)).add(axisUnit.cross((*this)).mul(sin(angle))).add(axisUnit.mul(this->dot(axisUnit) * (1.0 - cos(angle))));
}

//doesn't normalize axis
Vec3D Vec3D::rotFast(Vec3D const &axis,float angle)const{
	return this->mul(cos(angle)).add(axis.cross((*this)).mul(sin(angle))).add(axis.mul(this->dot(axis) * (1.0 - cos(angle))));
}

//takes a vector pointing into a surface, and computes the reflection that points outward
Vec3D Vec3D::reflect(Vec3D const& normal)const{
	//https://mathworld.wolfram.com/Reflection.html (second example)
	Vec3D nunit = normal.unit();
	return this->sub(nunit.mul(this->dot(nunit)*2.0));
}

Vec3D Vec3D::project(Vec3D const& onto)const{
	return onto.mul(this->dot(onto)/onto.dot(onto));
}

float Vec3D::projectionLen(Vec3D const& onto)const{
	return this->dot(onto)/onto.dot(onto);
}

void vec3UnitTest(){
	std::cout << "creation: ";
	Vec3D(0,0,0).show("");
	std::cout << "negative of (0.0,53.3,-6000): ";
	Vec3D(0,53.3,-6000).neg().show("");
	std::cout << "addition (-10,3.3,3.4) + (12.2,-16,6.6): ";
	Vec3D(-10,3.3,3.4).add(Vec3D(12.2,-16,6.6)).show("");
	std::cout << "subtraction: (-10,3.3,3.4)(12.2,-16,6.6)";
	Vec3D(-10,3.3,3.4).sub(Vec3D(12.2,-16,6.6)).show("");
	std::cout << "(3,3,3) * -0.1";
	Vec3D(3,3,3).mul(-0.1).show("");
	Vec3D(-0.3,-0.3,-0.3).div(30).show("(-0.3,-0.3,-0.3) / 30)");
	showFloat("len of 5,4,3 ",Vec3D(5,4,3).len());
	Vec3D unitv = Vec3D(10,10,30).unit();
	unitv.show("unit of 10,10,30");
	showFloat("len of unit",unitv.len());
	showFloat("dot of (1,2,3), (-3,-4,-5)",Vec3D(1,2,3).dot(Vec3D(-3,-4,-5)));
	Vec3D(1,1,1).cross(Vec3D(1,1,2)).show("cross of 1,1,1 & 1,1,2");
	//rotatetest
	Vec3D v(1,0,0);
	Vec3D axis(0,1,0);
	v.show("original");
	axis.show("axis");
	for(int i = 0; i < 12;++i){
		Vec3D nv = v.rotate(axis,(float)i * M_PI / 6.0);
		std::cout << nv.len();
		nv.show("|");
		
	}
	//reflection
	Vec3D pointer(1,1,0);
	Vec3D normal(1,0,0);
	pointer.show("reflects");
	normal.show("normal");
	pointer.reflect(normal).show("result");
	newLine();
	//projection
	pointer = Vec3D(1,2,3);
	normal = Vec3D(-3,4,2);
	pointer.show("original");
	normal.show("onto");
	pointer.project(normal).show("result");
}

void Sphere::show(std::string label)const {
	std::cout << label << ": c:(" << center.x << ',' << center.y << ',' << center.z << "),r:" << radius << "\n";
}

Sphere::Sphere(float x,float y,float z,float r):center(x,y,z),radius(r){}

Sphere::Sphere(Vec3D const &pos,float r):center(pos),radius(r){}

float Sphere::distFromRay(Ray const &ray)const {
	return ray.support.sub(center).cross(ray.direction).len();
}

bool Sphere::hit(Ray const &ray)const {
	return distFromRay(ray) < radius;
}

Ray::Ray(Vec3D const &pos,Vec3D const &dir):support(pos),direction(dir.unit()){}

Ray::Ray(float x,float y,float z,float a,float b,float c):support(x,y,z),direction(Vec3D(a,b,c).unit()){}

Ray::Ray():support(NAN,NAN,NAN),direction(NAN,NAN,NAN){}


void Ray::show(std::string label)const {
	std::cout << label << ": pos:(" << support.x << ',' << support.y << ',' << support.z << "),dir:(" << direction.x << ',' << direction.y << "," << direction.z << ")\n";
}

Vec3D Sphere::hitPoint(Ray const &ray)const {
	//ray origin to circle center
	Vec3D oToc = ray.support.sub(center);
	float dirDot = ray.direction.dot(oToc);
	float determinant = pow(dirDot,2.0) - (pow(oToc.len(),2.0) - pow(radius,2.0));
	if(determinant < 0)
		return Vec3D(NAN,NAN,NAN);
	else {
		//I'll only take the smallest distance, because that would be what you'd see
		float dist = -dirDot - sqrt(determinant);
		if(dist > 0)// dont render spheres behind you or ones you are inside of
			return ray.direction.mul(dist).add(ray.support);
		else return Vec3D(NAN,NAN,NAN);
	}
}


void SphereRayTest(){
	//test1
	Sphere testA = Sphere(2,2,1,1.1);
	Ray testB = Ray(Vec3D(1,1,1),Vec3D(1,2,-0.1));
	testA.show("sphere");
	testB.show("ray");
	testA.hitPoint(testB).show("expected value: 1.15,1.3,0.98");
	newLine();
	//test2
	testA = Sphere(2,2,1,0.5);
	testB = Ray(Vec3D(1,-1,1),Vec3D(1,-2,-0.1));
	testA.show("sphere");
	testB.show("ray");
	testA.hitPoint(testB).show("expected value:nan");
	newLine();
	//test3
	testA = Sphere(1,-1,5,1);
	testB = Ray(Vec3D(1,-1,5),Vec3D(1,-2,-0.1));
	testA.show("sphere");
	testB.show("ray");
	testA.hitPoint(testB).show("expected value:nan");
	newLine();
	//test4
	testA = Sphere(1.3,-1,4,1);
	testB = Ray(Vec3D(1,-1,5),Vec3D(1,-2,-2.1));
	testA.show("sphere");
	testB.show("ray");
	testA.hitPoint(testB).show("expected value:1.02,-1.04,4.96");
	newLine();
}


Plane::Plane(Vec3D const &pos,Vec3D const &normal): support(pos), normal(normal.unit()){}

Vec3D Plane::hitPoint(Ray const &ray)const{
	//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
	float alignment = ray.direction.dot(normal);
	if(alignment == 0) 
		return Vec3D(NAN,NAN,NAN);

	float distance = normal.dot(support.sub(ray.support)) / alignment;
	if(distance < 0)
		return Vec3D(NAN,NAN,NAN);
	
	return ray.support.add(ray.direction.mul(distance));
}

