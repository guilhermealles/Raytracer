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

void Object::mapToTexture (Image texture, Hit hit, const Ray &ray, int* texture_coords)
{
    // Converts sphere to polar coordinates.
    // Returns (u,v), which corresponds to the pixel of the texture image that will be the color of (x,y,z).
    
    Vector intersection_point(hit.t * ray.D);
    
    double theta = acos (intersection_point.y);
    double phi = atan2 (intersection_point.z, intersection_point.x);
    
    if (phi < 0.0)
    {
        phi = phi + 2*PI;
    }
    
    double u = phi / (2*PI);
    double v = (PI - theta)/PI;
    
    int column = (int) (texture.width() - 1) * u;
    int row = (int) (texture.height() - 1) * v;
    texture_coords[0] = row;
    texture_coords[1] = column;
}
