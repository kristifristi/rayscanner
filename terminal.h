
/*
renderer
Kristiaan Cramer
1069459
hogeschool rotterdam
2024
*/
//module for interacting with the terminal

#include "int2_t.h"
#include <stdbool.h>

void clrScreen();
void resetCursor();
int2 getScreenSize(); //REMEMBER TO HALVE THE WIDTH TO GET THE PROPER AMOUNT OF PIXELS
void cursorVisibility(bool see);
void setCanonical(bool canon); //true->write on enter, false write per char
