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

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/
    
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
            double t2 = tc+t1c;
            Point intersection_point(ray.O.x+(ray.D.x * t1), ray.O.y+(ray.D.y * t1), ray.O.z+(ray.D.z * t1));
            
            Vector normalV((intersection_point-position).normalized());
            double distance = sqrt(pow(intersection_point.x-ray.O.x, 2) + pow(intersection_point.y - ray.O.y, 2) + pow(intersection_point.z-ray.O.z, 2));
            
            return Hit(distance, normalV);
        }
        
    }
}
