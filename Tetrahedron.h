//
//  tetrahedron.h
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/18/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#ifndef __Raytracer__tetrahedron__
#define __Raytracer__tetrahedron__

#include <stdio.h>

#include "object.h"

class Tetrahedron : public Object
{
public:
    Tetrahedron(Point v0, Point v1, Point v2, Point v3) : p0(v0), p1(v1), p2(v2), p3(v3){ }
    
    virtual Hit intersect(const Ray &ray);
    
    const Point p0;
    const Point p1;
    const Point p2;
    const Point p3;
};

#endif /* defined(__Raytracer__tetrahedron__) */