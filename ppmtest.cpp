#include "ppmReader.hpp"
#include <fstream>

void fileDump(std::string path){
	std::fstream image;
	image.open(path,std::ios::binary | std::ios::in);	
	while(image){
		std::cout << (char)image.get();
	}
}

int main(){
	ValueMap* picture = readPPM("hi.ppm");
	printMap(picture);

	std::cout << std::endl;

	picture = readPPM("moyai.ppm");
	printMap(picture);
}
