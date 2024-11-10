
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
#include "terminal.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

void clrScreen(){
	printf("\033[2J");
}

void resetCursor(){
	printf("\033[H");
}

int2 getScreenSize(){
	struct winsize w;
	int2 size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	size.x = w.ws_col;
	size.y = w.ws_row;
	return size;
}

void cursorVisibility(bool see){
	printf("\033[?25%c", see ? 'h' : 'l');
}

void setCanonical(bool canon){
	struct termios settings;
	tcgetattr(1, &settings);
	if(canon)
		settings.c_lflag |= ICANON;
	else
		settings.c_lflag &= ~ICANON;
	tcsetattr(1, TCSANOW, &settings);
}
 
