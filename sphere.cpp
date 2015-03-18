//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>
#define PI 3.14159265359

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    Vector L(position-ray.O);
    double tc = L.dot(ray.D);
    
    if (tc < 0)
    {
        return Hit::NO_HIT();
    }
    else
    {
        double d = sqrt(pow(L.length(),2) - pow(tc,2));
        if (d > r)
        {
            return Hit::NO_HIT();
        }
        else
        {
            double t1c = sqrt(pow(r,2) - pow(d,2));
            double t1 = tc-t1c;
            Point intersection_point(ray.O.x+(ray.D.x * t1), ray.O.y+(ray.D.y * t1), ray.O.z+(ray.D.z * t1));
            
            Vector normalV((intersection_point-position).normalized());
            double distance = sqrt(pow(intersection_point.x-ray.O.x, 2) + pow(intersection_point.y - ray.O.y, 2) + pow(intersection_point.z-ray.O.z, 2));
            
            return Hit(distance, normalV);
        }
        
    }
}

Color Sphere::getTextureColor(Hit hit, const Ray &ray) {
    Color return_color;
    if (material->texture != NULL) {
        double texture_coords[2] = {0,0};
        mapToTexture(hit, ray, texture_coords);
        return_color = material->texture->colorAt(texture_coords[0], texture_coords[1]);
    }
    else {
        return_color = material->color;
    }
    return return_color;
}

void Sphere::mapToTexture (Hit hit, const Ray &ray, double* texture_coords)
{
    // Converts sphere to spherical coordinates.
    // Returns (u,v), which corresponds to the pixel of the texture image that will be the color of (x,y,z).

    Vector intersection_point(ray.at(hit.t));
    
     double theta = acos ((intersection_point.z - position.z)/r);
     double phi = atan2 (intersection_point.y - position.y, intersection_point.x - position.x);
     
     if (phi < 0.0)
     {
     phi = phi + 2*PI;
     }
     
     double u = phi / (2*PI);
     double v = (PI - theta)/PI;
     
     // This might solve the segmentation fault problems
     if (u > 1.0 || u < 0.0) {
     u -= ((int)(u/1)); // consider only the decimal parts of u
     }
     if (v > 1.0 || v < 0.0) {
     v -= ((int)(v/1)); // consider only the decimal parts of v
     }
     
     texture_coords[0] = u;
     texture_coords[1] = v;
}

void Sphere::setRotationParameters(Vector vector, double angle)
{
    rotation_vector = vector;
    rotation_angle = angle;
}

