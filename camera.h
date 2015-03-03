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
#include "ray.h"

class Camera
{
private:
    Point eye;
    Vector viewDirection;
    Vector up;
    double viewWidth=400, viewHeight=400;
public:
    Camera() { };
    Camera(Point eye, Vector viewDirection, Vector up) : eye(eye), viewDirection(viewDirection), up(up) { };
    void setEye(Triple e);
    void setViewDirection(Triple vD);
    void setUpVector(Triple upV);
    void setViewSize(int w, int h);
    double getViewWidth();
    double getViewHeight();
    Triple getEye();
    Triple getViewDirection();
    Triple getUpVector();
    double pixelSize();
    Ray getRay(int x, int y);
};

#endif
