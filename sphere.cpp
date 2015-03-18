//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "sphere.h"
#include <iostream>
#include <math.h>
#define PI 3.14159265359

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    Vector L(position-ray.O);
    double tc = L.dot(ray.D);
    
    if (tc < 0)
    {
        return Hit::NO_HIT();
    }
    else
    {
        double d = sqrt(pow(L.length(),2) - pow(tc,2));
        if (d > r)
        {
            return Hit::NO_HIT();
        }
        else
        {
            double t1c = sqrt(pow(r,2) - pow(d,2));
            double t1 = tc-t1c;
            Point intersection_point(ray.O.x+(ray.D.x * t1), ray.O.y+(ray.D.y * t1), ray.O.z+(ray.D.z * t1));
            
            Vector normalV((intersection_point-position).normalized());
            double distance = sqrt(pow(intersection_point.x-ray.O.x, 2) + pow(intersection_point.y - ray.O.y, 2) + pow(intersection_point.z-ray.O.z, 2));
            
            return Hit(distance, normalV);
        }
        
    }
}

Color Sphere::getTextureColor(Hit hit, const Ray &ray) {
    Color return_color;
    if (material->texture != NULL) {
        double texture_coords[2] = {0,0};
        mapToTexture(hit, ray, texture_coords);
        return_color = material->texture->colorAt(texture_coords[0], texture_coords[1]);
    }
    else {
        return_color = material->color;
    }
    return return_color;
}

void Sphere::mapToTexture (Hit hit, const Ray &ray, double* texture_coords)
{
    // Converts sphere to spherical coordinates.
    // Returns (u,v), which corresponds to the pixel of the texture image that will be the color of (x,y,z).
    
    Vector p(ray.at(hit.t));
    
    double x = p.x, y = p.y, z = p.z;
    double u, v, w, a, b, c;
    double cosT, sinT;
    double au, cw, cu, bv, cv, bw, aw, bu, av;
    double aVVWW, bUUWW, cUUVV;
    double vv, uu, ww;
    double phi = rotation_angle * (PI/180.0);
    double D;
    
    u = rotation_vector.x;
    v = rotation_vector.y;
    w = rotation_vector.z;
    a = position.x;
    b = position.y;
    c = position.z;
    vv = v * v;
    uu = u * u;
    ww = w * w;
    cosT = cos(phi);
    sinT = sin(phi);
    
    au = a * u;
    cw = c * w;
    cu = c * u;
    bv = b * v;
    cv = c * v;
    bw = b * w;
    aw = a * w;
    bu = b * u;
    av = a * v;
    aVVWW = a * (vv + ww);
    bUUWW = b * (uu + ww);
    cUUVV = c * (uu + vv);
    
    D = u * x + v * y + w * z;
    
    p.x = (aVVWW - u * (bv + cw - D)) * (1 - cosT)
    + x * cosT + sinT * (-cv + bw - w * y + v * z);
    
    p.y = (bUUWW - v * (au + cw - D)) * (1 - cosT)
    + y * cosT + sinT * (cu - aw + w * x - u * z);
    
    p.z = (cUUVV - w * (au + bv - D)) * (1 - cosT)
    + z * cosT + sinT * (-bu + av - v * x + u * y);
    
    Vector d = (position - p).normalized();
    
    texture_coords[0] = 0.5 + atan2(d.z, d.x) / (2 * PI);
    texture_coords[1] = 0.5 - asin(d.y) / PI;
    
    /*
    
    double theta = acos ((intersection_point.z - position.z)/r);
    double phi = atan2 (intersection_point.y - position.y, intersection_point.x - position.x);

    if (phi < 0.0)
    {
        phi = phi + 2*PI;
    }
    
    double u = phi / (2*PI);
    double v = (PI - theta)/PI;
    
    // This might solve the segmentation fault problems
    if (u > 1.0 || u < 0.0) {
        u -= ((int)(u/1)); // consider only the decimal parts of u
    }
    if (v > 1.0 || v < 0.0) {
        v -= ((int)(v/1)); // consider only the decimal parts of v
    }
    
    texture_coords[0] = u;
    texture_coords[1] = v;
    */
}

void Sphere::setRotationParameters(Vector vector, double angle)
{
    rotation_vector = vector;
    rotation_angle = angle;
}

void Sphere:: rotate()
{
    // radius[0]*radius[1].normalized()
    /*
    rotation_vector.normalize();
    r * rotation_vector;
    
    // map the vector to (0, 1,0)
    Vector y(0, 1, 0);
    
    // Calculates the angle between the rotation vector and the Y axis
    double vectors_angle_cos = rotation_vector.dot(y);
    double vectors_angle = acos(vectors_angle_cos);
    
    // Finds an orthogonal rotation vector
    Vector orthogonal_rotation_vector(rotation_vector.cross(y));
    
    // Rotates around the orthogonal_rotation_vector by the vectors_angle
    
    double rotationMatrix_y[3][3] = {{cos(vectors_angle), 0.0, -1 * sin(vectors_angle)},
                                {0.0, 0.0, 1.0},
                                {sin(vectors_angle), 0.0, cos(vectors_angle)}};
    double vectorMatrix[3][1] = {{orthogonal_rotation_vector.x}, {orthogonal_rotation_vector.y}, {orthogonal_rotation_vector.z}};
    double product_y[3][1] = {{0.0}, {0.0}, {0.0}};
    
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < 1; inner++)
            {
                product_y[row][col] += rotationMatrix_y[row][inner] * vectorMatrix[inner][col];
            }
        }
    }
    
    // Now we rotate it along X axis.
    Vector x(1, 0, 0);
    
    // Calculates the angle between the rotation vector and the X axis
    Vector rotation_vector_x(product_y[0][1], product_y[1][1], product_y[2][1]);
    double vectors_angle_cos_x = rotation_vector_x.dot(x);
    double vectors_angle_x = acos(vectors_angle_cos_x);
    
    // Finds an orthogonal rotation vector
    Vector orthogonal_rotation_vector_x(rotation_vector_x.cross(x));
    
    double rotationMatrix_x[3][3] = {{1.0, 0.0, 0.0},
                                    {0.0, cos(vectors_angle_x), sin(vectors_angle_x)},
                                    {0.0, -1* sin(vectors_angle_x), cos(vectors_angle_x)}};
    double product_x[3][1] = {{0.0}, {0.0}, {0.0}};
    
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < 1; inner++)
            {
                product_x[row][col] += rotationMatrix_x[row][inner] * product_y[inner][col];
            }
        }
    }

    // Now we rotated as asked in the yaml file.
    //Rotacionar por phi.
     */
    
}


