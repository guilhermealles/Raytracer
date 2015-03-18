//
//  plane.cpp
//  Raytracer
//
//  Created by Isadora Pedrini Possebon on 2/17/15.
//  Copyright (c) 2015 Isadora Pedrini Possebon. All rights reserved.
//

#include "Tetrahedron.h"
#include "triangle.h"
#include <iostream>
#include <math.h>

Hit Tetrahedron::intersect(const Ray &ray)
{
    Triangle triangles[4] = {
        Triangle(p0,p1,p2),
        Triangle(p0,p1,p3),
        Triangle(p0,p2,p3),
        Triangle(p1,p2,p3)
    };
    
    double closest_hit = std::numeric_limits<double>::infinity();
    Hit min_hit = Hit::NO_HIT();
    
    for (int i=0; i<4; i++)
    {
        Hit hit = triangles[i].intersect(ray);
        if (hit.t < closest_hit)
        {
            closest_hit = hit.t;
            min_hit = hit;
        }
    }
    
    return min_hit;
}
