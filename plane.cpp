//
//  plane.cpp
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/17/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Plane::intersect(const Ray &ray)
{
    if (normal.dot(ray.D) == 0) // The ray is parallel to the plane. No intersection.
        return Hit::NO_HIT();
    
    double t = normal.dot(q_point - ray.O)/(normal.dot(ray.D));
    
    if (t < 0) // Plane is behind the eye. Therefore, no intersection.
    {
        return Hit::NO_HIT();
    }
    else
    {
        Point intersection_point = ray.O + t * ray.D;
        double distance = sqrt(pow(intersection_point.x-ray.O.x, 2) + pow(intersection_point.y - ray.O.y, 2) + pow(intersection_point.z-ray.O.z, 2));

        return Hit(distance, normal);
    }
    
}