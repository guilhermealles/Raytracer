//
//  Framework for a raytracer
//  File: main.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Introduction to Computer Graphics - Raytracer" << endl << endl;
    
    string const SCENE_FILE = "../../../Raytracer/scene02.yaml";
    string const OUTPUT_FILE = "../../../Raytracer/out.png";

    Raytracer raytracer;

    if (!raytracer.readScene(SCENE_FILE))
    {
        cerr << "Error: reading scene from " << SCENE_FILE << " failed - no output generated."<< endl;
        return 1;
    }
    
    string outputFileName = OUTPUT_FILE;
    
    raytracer.renderToFile(outputFileName);

    return 0;
}
