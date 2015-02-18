//
//  cylinder.cpp
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/17/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#include "cylinder.h"
#include <iostream>
#include <math.h>

Hit Cylinder::intersect(const Ray &ray)
{
    double a = pow(ray.O.x, 2) + pow(ray.O.y, 2);
    double b = 2 * ray.O.x * ray.D.x + 2 * ray.O.y * ray.D.y;
    double c = pow(ray.O.x, 2) + pow(ray.O.y, 2) - 1;
    
    double t1 = ((-1)*b + sqrt(pow(b, 2) - 4 * a * c))/2*a;
    double t2 = ((-1)*b - sqrt(pow(b, 2) - 4 * a * c))/2*a;
    
    return Hit::NO_HIT();
    
}