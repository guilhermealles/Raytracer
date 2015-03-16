//
//  triangle.h
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/18/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#ifndef __Raytracer__triangle__
#define __Raytracer__triangle__

#include <stdio.h>

#include "object.h"

class Triangle : public Object
{
public:
    Triangle(Point v0, Point v1, Point v2) : v0(v0), v1(v1), v2(v2){ }
    
    virtual Hit intersect(const Ray &ray);
    
    const Point v0;
    const Point v1;
    const Point v2;
};

#endif /* defined(__Raytracer__triangle__) */
