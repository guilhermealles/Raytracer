//
//  camera.cpp
//  Raytracer
//
//  Created by Guilherme Alles on 03/03/15.
//  Copyright (c) 2015 Guilherme Rezende Alles. All rights reserved.
//

#include "camera.h"

void Camera::setEye(Triple e)
{
    eye = e;
}

void Camera::setViewDirection(Triple vD)
{
    viewDirection = vD;
}

void Camera::setUpVector(Triple upV)
{
    up = upV;
}

Triple Camera::getEye()
{
    return eye;
}

Triple Camera::getViewDirection()
{
    return viewDirection;
}

Triple Camera::getUpVector()
{
    return up;
}