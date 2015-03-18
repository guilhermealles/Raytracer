//
//  Framework for a raytracer
//  File: sphere.h
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

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

class Sphere : public Object
{
public:
    Sphere(Point position,double r) : position(position), r(r) { }

    virtual Hit intersect(const Ray &ray);
    virtual Color getTextureColor(Hit hit, const Ray &ray);
    
    void mapToTexture(Hit hit, const Ray &ray, double* texture_coords);
    void setRotationParameters (Vector rotation_vector, double angle);
    void rotate();

    const Point position;
    const double r;
    Vector rotation_vector;
    double rotation_angle = 0.0; // default value. It's only valid if not zero
};

#endif /* end of include guard: SPHERE_H_115209AE */
