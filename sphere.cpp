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
    
    double dx = ray.D.x-ray.O.x;
    double dy = ray.D.y-ray.O.y;
    double dz = ray.D.z-ray.O.z;
    
    double a = pow(dx,2) + pow(dy,2) + pow(dz,2);
    double b = 2*dx*(ray.O.x-position.x) + 2*dy*(ray.O.y-position.y) + 2*dz*(ray.O.z-position.z);
    double c = pow(position.x,2) + pow(position.y,2) + pow(position.z,2) + pow(ray.O.x,2) + pow(ray.O.y,2) + pow(ray.O.z,2) + (-2*(position.x*ray.O.x + position.y*ray.O.y + position.z*ray.O.z)) - pow(r,2);
    double delta = pow(b,2) - 4*a*c;
     
    
    //double a = ray.D.x*ray.D.x + ray.D.y*ray.D.y + ray.D.z*ray.D.z;
    //double b = 2 * (ray.D.x * (ray.O.x - position.x) + ray.D.y * (ray.O.y - position.y) + ray.D.z * (ray.O.z - position.z));
    //double c = (ray.O.x - position.x)*(ray.O.x - position.x) + (ray.O.y - position.y)*(ray.O.y - position.y) + (ray.O.z - position.z)*(ray.O.z - position.z) - r*r;
    //double delta = pow(b,2) - 4*a*c;
    
    
    // Intersection point
    double x;
    double y;
    double z;
    
    if (delta < 0) // No intersection
    {
        return Hit::NO_HIT();
    }
    else // Ray is tangent to sphere or there is intersection
    {
        // Calculates the distance between the intersection point and the eye origin
        double t = ((-1)*b - sqrt(delta))/2*a;
        
        x = ray.O.x + t * (ray.O.x - ray.D.x);
        y = ray.O.y + t * (ray.O.y - ray.D.y);
        z = ray.O.z + t * (ray.O.z - ray.D.z);
        
        Vector normal((x-position.x)/r, (y-position.y)/r, (z-position.z)/r);
        Vector intersection_point(x,y,z);
        
        Vector normalV((intersection_point-position).normalized());
        double distance = sqrt(pow(intersection_point.x - ray.O.x, 2) + pow(intersection_point.y - ray.O.y, 2) + pow(intersection_point.z-ray.O.z, 2));
        
        return Hit(distance, normalV);
    }
}
