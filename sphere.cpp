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
    
    Vector intersection_point(ray.at(hit.t));
    
    double cosT, sinT;
    double xx, zz, zx, yy, zy, yz, xz, yx, xy;
    double xYYZZ, yXXZZ, zXXYY;
    double rot_vector_xx, rot_vector_yy, rot_vector_zz;
    double phi = rotation_angle * (PI/180.0);
    double D;
    
    rot_vector_xx = pow(rotation_vector.x, 2);
    rot_vector_yy = pow(rotation_vector.y, 2);
    rot_vector_zz = pow(rotation_vector.z, 2);
    cosT = cos(phi);
    sinT = sin(phi);
    
    xx = position.x * rotation_vector.x;
    zz = position.z * rotation_vector.z;
    zx = position.z * rotation_vector.x;
    yy = position.y * rotation_vector.y;
    zy = position.z * rotation_vector.y;
    yz = position.y * rotation_vector.z;
    xz = position.x * rotation_vector.z;
    yx = position.y * rotation_vector.x;
    xy = position.x * rotation_vector.y;
    
    xYYZZ = position.x * (rot_vector_yy + rot_vector_zz);
    yXXZZ = position.y * (rot_vector_xx + rot_vector_zz);
    zXXYY = position.z * (rot_vector_xx + rot_vector_yy);
    
    D = rotation_vector.x * intersection_point.x + rotation_vector.y * intersection_point.y + rotation_vector.z * intersection_point.z;
    
    intersection_point.x = (xYYZZ - rotation_vector.x * (yy + zz - D)) * (1 - cosT)
    + intersection_point.x * cosT + sinT * (-zy + yz - rotation_vector.z * intersection_point.y + rotation_vector.y * intersection_point.z);
    
    intersection_point.y = (yXXZZ - rotation_vector.y * (xx + zz - D)) * (1 - cosT)
    + intersection_point.y * cosT + sinT * (zx - xz + rotation_vector.z * intersection_point.x - rotation_vector.x * intersection_point.z);
    
    intersection_point.z = (zXXYY - rotation_vector.z * (xx + yy - D)) * (1 - cosT)
    + intersection_point.z * cosT + sinT * (-yx + xy - rotation_vector.y * intersection_point.x + rotation_vector.x * intersection_point.y);
    
    Vector d = (position - intersection_point).normalized();
    
    double u = 0.5 + atan2(d.z, d.x) / (2 * PI);
    double v = 0.5 - asin(d.y) / PI;
    
    texture_coords[0] = u;
    texture_coords[1] = v;
    
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


