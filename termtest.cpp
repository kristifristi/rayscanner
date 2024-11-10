#include <iostream>
#include "terminal.h"
#include <string>
#include <cstdio>

int main(void){
	clrScreen();
	resetCursor();
	int2 size = getScreenSize();
	std::cout << size.x << " by " << size.y << std::endl;
	cursorVisibility(false);
	setCanonical(false);
	std::cout << "enjoy typing weird!\n";
	std::string str = std::string();
	char i = getchar();
	std::cout << i << "? nice!\n";
	setCanonical(true);
	cursorVisibility(true);
	std::cout << "back to normal: ";
	std::cin >> str;
}
