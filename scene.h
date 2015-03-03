//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    Camera camera;
    bool antiAliasingEnabled = false;
    bool shadowsEnabled = false;
    double min_t;
    double max_t;
    int maxRecursionDepth;
public:
    Color trace(const Ray &ray, int recursion = 0);
    Color zBufferTrace(const Ray &ray);
    Color normalBufferTrace(const Ray &ray);
    void render(Image &img, string mode);
    void initializeMinMaxT(int h, int w);
    void findMinMaxT(const Ray &ray);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setCamera(Camera c);
    double max (double x, double y);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
    void setAntiAliasing(bool state);
    void setShadows(bool state);
    void setMaxRecursionDepth(int recursionDepth);
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
