#include "ppmReader.hpp"

#define MAX_LEN 255

ValueMap* missingTexture(){
	ValueMap* err = new ValueMap(2,2);
	err->matrix[0][0] = 1.0;
	err->matrix[1][1] = 1.0;
	return err;	
}


ValueMap* readPPM(std::string path){
	std::ifstream image(path.c_str());
	
	if(image.get() != 'P'){
		return missingTexture();
	}

	//get dimensions
	image.seekg(3);
	char buffer[MAX_LEN];
	image.getline(buffer,MAX_LEN);
	std::string reader;
	reader.replace(0,MAX_LEN,buffer);
	size_t next;
	int width;
	width = std::stoi(reader,&next,10);
	reader.erase(0,next);
	int height;
	height = std::stoi(reader,&next,10);

	image.getline(buffer,MAX_LEN); //throw away the color depth
	ValueMap* map;	
	map = new ValueMap(width,height);
	for(int i = 0;i < width*height;++i){
		float red = image.get();
		image.seekg(2,std::ios::cur); //yep, I'm not actually grayscaling, im just sampling red chanel
		map->matrix[i/width][i%width] = red / 255.0;
	}
	return map;
}

