//
//  plane.cpp
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/17/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#include "Tetrahedron.h"
#include "triangle.h"
#include "triangle.cpp"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Tetrahedron::intersect(const Ray &ray)
{
    Triangle *triangle1 = new Triangle(p0, p1, p2);
    Triangle *triangle2 = new Triangle(p0, p1, p3);
    Triangle *triangle3 = new Triangle(p0, p2, p3);
    Triangle *triangle4 = new Triangle(p1, p2, p3);
    
    Hit a = triangle1->intersect(ray);
    if (a.t >= 0)
        return a;
    Hit b = triangle2->intersect(ray);
    if (b.t >= 0)
        return b;
    Hit c = triangle3->intersect(ray);
    if (c.t >= 0)
        return c;
    Hit d = triangle4->intersect(ray);
    if (d.t >= 0)
        return d;
    
    return Hit:: NO_HIT();
}
