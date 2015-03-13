//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"
#include "stdio.h"
#include "raytracer.h"
#define recursion_max 1


Color Scene::trace(const Ray &ray, int recursionLevel)
{
    if (recursionLevel < 0)
        return Color(0.0f,0.0f,0.0f);
    
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    
    for (unsigned int i = 0; i < objects.size(); ++i)
    {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t)
        {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj)
        return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/
    
    Color ambient_component(0.0f,0.0f,0.0f);
    Color diffuse_component(0.0f,0.0f,0.0f);
    Color specular_component(0.0f,0.0f,0.0f);
    Color reflection_component(0.0f,0.0f,0.0f);
    
    // The ambient component is added regardless of shadows
    ambient_component += material->ka * material->color;
    
    for (unsigned int i=0; i < lights.size(); i++)
    {
        Vector unitLight((lights.at(i)->position - hit).normalized());
        Vector reflection_vector((2*(unitLight.dot(N)))*N-unitLight);
        
        if (shadowsEnabled)
        {
            Ray pointToLight(hit, unitLight); // This is a vector from the hit point to the light source
            bool shadow = false;
            
            for (unsigned int i = 0; i < objects.size(); ++i) // Loops through all the objects in the scene,
            {
                Hit hit(objects[i]->intersect(pointToLight));
                
                if (hit.t >= 0)
                {
                    shadow = true;
                }
            }
            
            if (shadow == false) // if the object is not on shadow, then all the components are considered
            {
                double cosine_factor = N.normalized().dot(unitLight);
                if (cosine_factor < 0)
                    cosine_factor = 0;
                double specular_factor = material->ks * pow(max(0,reflection_vector.dot(V)), material->n);
                
                specular_component += specular_factor * lights.at(i)->color;
                diffuse_component += material->kd * cosine_factor * material->color * lights.at(i)->color;
            }
        }
        else
        {
            double cosine_factor = N.normalized().dot(unitLight);
            if (cosine_factor < 0)
                cosine_factor = 0;
            double specular_factor = material->ks * pow(max(0,reflection_vector.dot(V)), material->n);
            
            specular_component += specular_factor * lights.at(i)->color;
            diffuse_component += material->kd * cosine_factor * material->color * lights.at(i)->color;
        }
        
        // Add reflection component
        reflection_component = trace(Ray(hit, reflection_vector), --recursionLevel) * material->ks;
    }
    return Color(ambient_component + specular_component + diffuse_component + reflection_component);
}

Color Scene::zBufferTrace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }
    
    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);
    
    double z = 1-(min_hit.t-min_t) / (max_t-min_t);
    z = (z * 0.9) + 0.1;
    return Color(z,z,z);
}

Color Scene::normalBufferTrace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }
    
    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);
    Vector N = min_hit.N;                          //the normal at hit point
    
    // transformar de -1,1 a 0,1
    N = N / 2 + 0.5;
    
    
    Color color(N);
    return color;
}

void Scene::render(Image &img, string mode)
{
    int w = img.width();
    int h = img.height();
    if (mode == "zbuffer")
    {
        initializeMinMaxT(w,h);
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                Point pixel(x+0.5, h-1-y+0.5, 0);
                Ray ray(eye, (pixel-eye).normalized());
                Color col = zBufferTrace(ray);
                col.clamp();
                img(x,y) = col;
            }
        }
    }
    else if (mode == "normal")
    {
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                Point pixel(x+0.5, h-1-y+0.5, 0);
                Ray ray(eye, (pixel-eye).normalized());
                Color col = normalBufferTrace(ray);
                col.clamp();
                img(x,y) = col;
            }
        }
    }
    else // Phong shading
    {
        if (mode != "phong")
            cerr << "Warning: render mode is invalid! Default: phong." << endl;
        for (int y = 0; y < h; y++)
        {
            for (int x = 0; x < w; x++)
            {
                Color pixelColor(0.0f,0.0f,0.0f);
                for(int subY = 0; subY < superSamplingRate; subY++)
                {
                    for(int subX = 0; subX < superSamplingRate; subX++)
                    {
                        if (cameraIsSet)
                        {
                            Ray ray(camera.getRay(x+((double)subX/superSamplingRate), h-1-y+((double)subY/superSamplingRate)));
                            Color subColor = trace(ray, maxRecursionDepth);
                            pixelColor += subColor;
                        }
                        else
                        {
                            Point pixel(x+((double)subX/superSamplingRate), h-1-y+((double)subY/superSamplingRate));
                            Ray ray(eye, (pixel-eye).normalized());
                            Color subColor = trace(ray, maxRecursionDepth);
                            pixelColor += subColor;
                        }
                    }
                }
                pixelColor = pixelColor/(double)pow(superSamplingRate, 2); //grid of 4x4
                pixelColor.clamp();
                img(x,y) = pixelColor;
            }
        }
    }
}

void Scene::initializeMinMaxT(int h, int w)
{
    min_t = std::numeric_limits<double>::infinity();
    max_t = 0;
    
    for (int y=0; y<h; y++)
    {
        for (int x=0; x<w; x++)
        {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            findMinMaxT(ray);
        }
    }
}

void Scene::findMinMaxT(const Ray &ray)
{
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_t)
        {
            min_t = hit.t;
        }
        if (hit.t>max_t)
        {
            max_t = hit.t;
        }
    }
}

double Scene::max(double x, double y)
{
    if (x > y)
        return x;
    else
        return y;
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setCamera(Camera c)
{
    camera.setEye(c.getEye());
    camera.setLookPoint(c.getLookPoint());
    camera.setUpVector(c.getUpVector());
}

void Scene::setCameraFlag (bool state)
{
    cameraIsSet = state;
}

void Scene::setSuperSampling(int rate)
{
    if (rate > 0) {
        superSamplingRate = rate;
    }
    else {
        superSamplingRate = 1;
    }
}

void Scene::setShadows(bool state)
{
    shadowsEnabled = state;
}

void Scene::setMaxRecursionDepth(int recursionDepth)
{
    maxRecursionDepth = recursionDepth;
}