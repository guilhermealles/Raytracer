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
    int superSamplingRate = 1;
    bool shadowsEnabled = false;
    bool cameraIsSet = false;
    double min_t;
    double max_t;
    int maxRecursionDepth;
    // Gooch parameters
    double gooch_b;
    double gooch_y;
    double gooch_alpha;
    double gooch_beta;
    
public:
    Color trace(const Ray &ray, int recursion = 0);
    Color goochTrace (const Ray &ray);
    Color zBufferTrace(const Ray &ray);
    Color normalBufferTrace(const Ray &ray);
    void render(Image &img, string mode);
    void initializeMinMaxT(int h, int w);
    void findMinMaxT(const Ray &ray);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setCamera(Camera c);
    void setCameraFlag(bool state);
    double max (double x, double y);
    unsigned int getNumObjects() { return (unsigned int)objects.size(); }
    unsigned int getNumLights() { return (unsigned int)lights.size(); }
    void setSuperSampling(int rate);
    void setShadows(bool state);
    void setMaxRecursionDepth(int recursionDepth);
    void setGoochParameters(double* b, double* y, double* alpha, double* beta);
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
