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
    Point lookPoint;
    Vector up;
    double viewWidth=400, viewHeight=400;
public:
    Camera() { };
    Camera(Point eye, Point lookPoint, Vector up) : eye(eye), lookPoint(lookPoint), up(up) { };
    void setEye(Triple e);
    void setLookPoint(Point lP);
    void setUpVector(Triple upV);
    void setViewSize(int w, int h);
    double getViewWidth();
    double getViewHeight();
    Triple getEye();
    Triple getLookPoint();
    Triple getUpVector();
    double pixelSize();
    Ray getRay(double x, double y);
};

#endif
