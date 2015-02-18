//
//  cylinder.h
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/17/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#ifndef __Raytracer__cylinder__
#define __Raytracer__cylinder__

#include <stdio.h>
#include "object.h"

class Cylinder : public Object
{
public:
    Cylinder(double h,double r) : h(h), r(r) { }
    
    virtual Hit intersect(const Ray &ray);
    
    const double h;
    const double r;
};


#endif /* defined(__Raytracer__cylinder__) */
