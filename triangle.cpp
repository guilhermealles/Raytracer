//
//  triangle.cpp
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/18/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#include "triangle.h"
#include <iostream>
#include <math.h>

Hit Triangle::intersect(const Ray &ray)
{
    double a = v0.x - v1.x;
    double b = v0.y - v1.y;
    double c = v0.z - v1.z;
    double d = v0.x - v2.x;
    double e = v0.y - v2.y;
    double f = v0.z - v2.z;
    double g = ray.D.x;
    double h = ray.D.y;
    double i = ray.D.z;
    double j = v0.x - ray.O.x;
    double k = v0.y - ray.O.y;
    double l = v0.z - ray.O.z;
    
    double M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);
    
    double beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g))/M;
    double gama = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c))/M;
    double t = - (f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c))/M;
    
    if (t < 0)
    {
        return Hit:: NO_HIT();
    }
    
    if (gama < 0 || gama > 1)
    {
        return Hit:: NO_HIT();
    }
    
    if (beta < 0 || beta > 1 - gama)
    {
        return Hit:: NO_HIT();
    }
    
    Vector ba = v0 - v1;
    Vector bc = v2 - v1;
    
    Vector normal = ba.cross(bc);
    normal.normalize();
    return Hit(t, normal);
    
}