#include "renderer.hpp"
#include "terminal.h"
#include <iostream>
#include "objectRegistry.hpp"
#include <cstdio> //iostream sucks and i hate it

using namespace std;

void testingRaysc(){

   cout << "testing rays start" << endl;
    Sphere sphere[] = {Sphere (0,0,20,5), Sphere (10, 0, 14 ,5 ), Sphere (20, 0, 14, 5)};

    Ray rays[3];
    rays[0] = Ray(0, 0, 0, 0, 0, 1);
    rays[1] = Ray(10, 1, 0, 0, 0, 1);
    rays[2] = Ray(20, 2, 0, 0, 0, 1);

    cout << "testing collision start" << endl;
    for(int i = 0; i < 3; i++)
    {
cout << endl;

        for(int j = 0; j < 3; j++)
        {
		sphere[j].center.show("ball center");
		rays[i].support.show("ray sup");
		rays[i].direction.show("ray dir");
	    cout << sphere[j].distFromRay(rays[i]) << std::endl << std::endl;
            if(sphere[j].hit(rays[i]))

            {//hit
                cout << "ray number: " << i << ", collided with sphere: " << j;
                Vec3D hit = sphere[j].hitPoint(rays[i]);
		hit.show("   Hitpoint:  ");
                if(hit.isFinite()){
		rays[i].direction.reflect(hit.sub(sphere[j].center)).show("   refray:  ");;
                }

            else
            {//miss
              //  cout << "ray number: " << i << ", missed sphere: " << j;
            }
cout << endl;


        }//end of loop j

    }//end of loop i
}
}

int main(void){
	testingRaysc();
}
