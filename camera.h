//
//  camera.h
//  Raytracer
//
//  Created by Guilherme Alles on 03/03/15.
//  Copyright (c) 2015 Guilherme Rezende Alles. All rights reserved.
//

#ifndef Raytracer_camera_h
#define Raytracer_camera_h

#include "triple.h"

class Camera
{
private:
    Point eye;
    Point center;
    Vector up;
public:
    void setEye(Triple e);
    void setCenter(Triple c);
    void setUpVector(Triple upV);
    Triple getEye();
    Triple getCenter();
    Triple getUpVector();
};

#endif
