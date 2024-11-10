#include "geometry.hpp"

using namespace std;

int main(){
	for (float far = 0;far <0.2;far += 0.05){
	cout << "testing rays start" << endl;
    Sphere sphere[] = {Sphere (-0.4, 0.23, -1, 0.4), Sphere (0.4, 0.4, -1.2, 0.3), Sphere (0.7, -0.15, -1.5, 0.2)};

    Ray rays[3];
    rays[0] = Ray(-far, 0.23, -1, far, 0, 0);
    rays[1] = Ray(0.4, -far, -1.2, 0, far, 0);
    rays[2] = Ray(0.7,-0.15, -far, 0, 0, far);

    cout << "testing collision start" << endl;
    for(int i = 0; i < 3; i++)
    {
	    float dist = -1;

        for(int j = 0; j < 3; j++)
        {
            if(sphere[j].hit(rays[i]))
            {//hit
               if(sphere[j].distFromRay(rays[i]) > dist){dist = sphere[j].distFromRay(rays[i]);}
                cout << "ray number: " << i << ", collided with sphere: " << j;
            }
            else
            {//miss
                cout << "ray number: " << i << ", missed sphere: " << j;
            }
           cout << "   ray number: " << i << ", length: " << dist << endl;


        }//end of loop j

    }//end of loop i
    newLine();
    vec3UnitTest();
}}
