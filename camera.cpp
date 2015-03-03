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

void Camera::setLookPoint(Point lP)
{
    lookPoint = lP;
}

void Camera::setUpVector(Triple upV)
{
    up = upV;
}

void Camera::setViewSize(int w, int h)
{
    viewWidth = w;
    viewHeight = h;
}

double Camera::getViewWidth()
{
    return viewWidth;
}

double Camera::getViewHeight()
{
    return viewHeight;
}

Triple Camera::getEye()
{
    return eye;
}

Triple Camera::getLookPoint()
{
    return lookPoint;
}

Triple Camera::getUpVector()
{
    return up;
}

double Camera::pixelSize()
{
    return up.length();
}

Ray Camera:: getRay(double x, double y)
{
    Vector E = eye;
    Vector G = lookPoint - eye;
    Vector U = up;
    double w = viewWidth * pixelSize();
    double h = viewHeight * pixelSize();
    
    Vector A = G.cross(U);
    Vector B = A.cross(G);
    Vector M = lookPoint;
    Vector H = A * 0.5 * w / A.length();
    Vector V = B * 0.5 * h / B.length();
    double sx = x / viewWidth;
    double sy = y / viewHeight;
    
    Point P = M + (2 * sx - 1) * H + (2 * sy - 1) * V;
    
    return Ray(E, (P - E).normalized());
}