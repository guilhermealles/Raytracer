//
//  plane.h
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/17/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#ifndef __Raytracer__plane__
#define __Raytracer__plane__

#include <stdio.h>

#endif /* defined(__Raytracer__plane__) */

#include "object.h"

class Plane : public Object
{
public:
    Plane(Point q_point, Vector normal) : q_point(q_point), normal(normal){ }
    
    virtual Hit intersect(const Ray &ray);
    
    const Point q_point;
    const Vector normal;
};
