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
    
    double t1 = (-1)*b + ;
    double t2;
    
    
}