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
    
    // Yet Another attempt
    Vector L = position - ray.O;
    
    // Calculate tca (distance from the origin to the point on the ray halfway between the 2 intersection points)
    double tca = L.dot(ray.D);
    
    // If it's behind the ray direction
    if (tca < 0)
        return Hit::NO_HIT();
    
    double d =  sqrt(L.dot(L) - (tca * tca));
    
    if (d > r) // If d > r then the ray doesn't meet the sphere
        return Hit::NO_HIT();
    
    double thc = sqrt((r * r) - (d*d)); // Calculates thc
    
    // Points of intersections
    double t0 = tca - thc; // First point of intersection
    //t1 = tca + thc; // Second point of intersection
    
    double t = t0;
    
    // Coordinates of the point of intersection
    Vector P = (ray.O + t*ray.D);
    
    
    /****************************************************
     * RT1.2: NORMAL CALCULATION
     *
     * Given: t, C, r
     * Sought: N
     *
     * Insert calculation of the sphere's normal at the intersection point.
     ****************************************************/
    
    // Normal on intersection point
    Vector N = (P - position).normalized();
    
    return Hit(t,N);
    /*
    // New Attempt
    Vector L(position-ray.O);
    double tc = L.dot(ray.D);
    
    if (tc < 0)
    {
        return Hit::NO_HIT();
    }
    else
    {
        double d = sqrt(pow(tc,2) - pow(L.length(),2));
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
     */
    
    /* -----Old Attempt
    double dx = ray.D.x-ray.O.x;
    double dy = ray.D.y-ray.O.y;
    double dz = ray.D.z-ray.O.z;
    

    //double a = pow(dx,2) + pow(dy,2) + pow(dz,2);
    //double b = 2*dx*(ray.O.x-position.x) + 2*dy*(ray.O.y-position.y) + 2*dz*(ray.O.z-position.z);
    //double c = pow(position.x,2) + pow(position.y,2) + pow(position.z,2) + pow(ray.O.x,2) + pow(ray.O.y,2) + pow(ray.O.z,2) + (-2*(position.x*ray.O.x + position.y*ray.O.y + position.z*ray.O.z)) - pow(r,2);
    //double delta = pow(b,2) - 4*a*c;
     
    
    double a = ray.D.x*ray.D.x + ray.D.y*ray.D.y + ray.D.z*ray.D.z;
    double b = 2 * (ray.D.x * (ray.O.x - position.x) + ray.D.y * (ray.O.y - position.y) + ray.D.z * (ray.O.z - position.z));
    double c = (ray.O.x - position.x)*(ray.O.x - position.x) + (ray.O.y - position.y)*(ray.O.y - position.y) + (ray.O.z - position.z)*(ray.O.z - position.z) - r*r;
    double delta = pow(b,2) - 4*a*c;
    
    
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
    */
}
