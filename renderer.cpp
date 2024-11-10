
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#include "renderer.hpp"
#include <vector>

int minAxis(int2 screen){
std::vector<int*> listy(0);
	return screen.x < screen.y ? screen.x : screen.y;
}

ValueMap::ValueMap(int width,int height): matrix(height){
	dims.width = width;
	dims.height = height;
	dims.scale = minAxis({width,height})/2;
	for(int i = 0;i < height;++i){
		matrix[i] = std::vector<float>(width);
		for(int j = 0;j < width;j++){
			matrix[i][j]=0.0;
		}
	}
}

int2 convert_pos(vec2 pos,screenDims screen){
	int2 ret;
	int scale = screen.scale;
	ret.x = pos.x * scale + screen.width / 2;
	ret.y = (pos.y * scale * -1 + screen.height / 2);
	return ret;
}

vec2 revert_pos(int2 pos, screenDims screen){
	vec2 ret;
	ret.x = (double)(pos.x - screen.width / 2) / screen.scale;
	ret.y = (double)(pos.y - screen.height / 2) / screen.scale * -1;
	return ret;
}

char valueConvert(float val){
	const int levels = 7;
	char chars[] = {' ','`','+','|','T','N','@'};
	int index = 0;
	if(val >= 1){
		index = levels - 1;
	}else if(val <= 0){
	}else{
		index = round(val*(levels-1));
	}
	return chars[index];
}

void printAscii(ValueMap* map){
	clrScreen();
	resetCursor();
	for(int i = 0;i < map->dims.height;++i){
		for(int j = 0;j < map->dims.width;++j){
			char ch = valueConvert(map->matrix[i][j]);
			std::cout << ch << ch;
		}
		std::cout << "\n";
	}
}

//converts float value to a value useful for ansi
int floatToBrightness(float val){
	int ret = val * 23 + 232;
	return ret > 255 ? 255 : ret;
}

void printMap(ValueMap* map){
	resetCursor();
	for(int i = 0;i < map->dims.height;++i){
		for(int j = 0;j < map->dims.width;++j){
			//256 color ansi escape
			std::cout << "\033[48;5;" << floatToBrightness(map->matrix[i][j]) << "m  "; 
		}
		std::cout << "\n";
	}

	return;
}



//raycasting section!!
Camera::Camera(Scene& scen,ValueMap** mp): 
	screenDistance(3.0),
	fov(3.0),
	scene(scen),
	map(mp),
	pitchVector(0,1,0),
	yawVector(0,0,1),
	rollVector(1,0,0),
	support(0,0,2)
{}

void Camera::changePitch(float angle){
	rollVector = rollVector.rotate(pitchVector,angle);
	yawVector = yawVector.rotate(pitchVector,angle);
}

void Camera::changeYaw(float angle){
	rollVector = rollVector.rotate(yawVector,angle);
	pitchVector = pitchVector.rotate(yawVector,angle);
}

void Camera::changeRoll(float angle){
	pitchVector = pitchVector.rotate(rollVector,angle);
	yawVector = yawVector.rotate(rollVector,angle);
}

#define MAX_RECURSE 5
colorBuffer raycastScene(Ray const& ray,Scene const& scene,float screenDistance,float levels){
	colorBuffer buffer = {0.0,INFINITY};
	for(int obi = 0; obi < scene.size(); ++obi){
		colorBuffer newColor = scene[obi]->getColorBufferLM(ray,levels);
		buffer = newColor.dist > screenDistance && newColor.dist < buffer.dist ? newColor : buffer;
	}
	return buffer;
}



void Camera::render(){
	//quicker reference
	screenDims dims = (*map)->dims;
	//for every row,for every colomn in a row, for every object check the color/collision
	for(int i = 0;i < dims.height;++i){
		for(int j = 0; j < dims.width;++j){
			vec2 pixel = revert_pos({j,i},dims);
			Ray colorFinder(support,
				rollVector.mul(fov).add(yawVector.mul(pixel.y).add(pitchVector.mul(pixel.x))));
			
			(*map)->matrix[i][j] = raycastScene(colorFinder,scene,screenDistance,MAX_RECURSE).value;
		}
	}
}

void Camera::renderThread(int i){
	screenDims dims = (*map)->dims;
	for(int j = 0; j < dims.width;++j){
		vec2 pixel = revert_pos({j,i},dims);
		Ray colorFinder(support,
			rollVector.mul(fov).add(yawVector.mul(pixel.y).add(pitchVector.mul(pixel.x))));
		
		(*map)->matrix[i][j] = raycastScene(colorFinder,scene,screenDistance,MAX_RECURSE).value;
	}
}

void Camera::renderMultiThread(){
	screenDims dims = (*map)->dims;
	std::vector<std::thread> threads;
	for(int i = 0;i < dims.height;++i){
		threads.push_back(std::thread(&Camera::renderThread,this,i));
	}
	for(std::thread& th : threads){
		th.join();
	}
}
