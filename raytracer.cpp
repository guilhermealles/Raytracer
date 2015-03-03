//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "plane.h"
#include "triangle.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

int parseInt(const YAML::Node& node)
{
    int i;
    node>>i;
    return i;
}

string parseString(const YAML::Node& node)
{
    std::string s;
    node>>s;
    return s;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    node["color"] >> m->color;	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere")
    {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
    }
    
    if (objectType == "plane")
    {
        Point q_point;
        node["point"] >> q_point;
        Vector normal;
        node["normal"] >> normal;
        Plane *plane = new Plane(q_point, normal);
        returnObject = plane;
    }
    
    if (objectType == "triangle")
    {
        Point point1, point2, point3;
        node["point1"] >> point1;
        node["point2"] >> point2;
        node["point3"] >> point3;
        Triangle *triangle = new Triangle(point1, point2, point3);
        returnObject = triangle;
    }

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

Camera* Raytracer::parseCamera(const YAML::Node& node)
{
    return new Camera (Point(200,200,1000), Vector(200,200,100), Vector(0,1,0));
    Point eye;
    node["eye"] >> eye;
    Vector viewDir;
    node["viewDirection"] >> viewDir;
    Vector up;
    node["up"] >> up;
    //return new Camera(eye, viewDir, up);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    
    if (!fin)
    {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try
    {
        YAML::Parser parser(fin);
        if (parser)
        {
            YAML::Node doc;
            parser.GetNextDocument(doc);
            
            // Read scene configuration options
            scene->setEye(parseTriple(doc["Eye"]));
            
            try {
                const YAML::Node& renderMode = doc["RenderMode"];
                render_mode = parseString(renderMode);
            } catch (exception) {
                render_mode = "";
                // TODO
                cerr << "Warning: RenderMode not found or is invalid!" << endl;
            }

            try {
                const YAML::Node& antiAliasingNode = doc["AntiAliasing"];
                string antiAliasingEnabled = parseString(antiAliasingNode);
                if (antiAliasingEnabled == "true") {
                    scene->setAntiAliasing(true);   
                } else {
                    scene->setAntiAliasing(false);
                }
            } catch (exception) {
                cerr << "Warning: AntiAliasing configs not found! Disabling AA..." << endl;
                scene->setAntiAliasing(false);
            }
            
            try {
                const YAML::Node& shadowsNode = doc["Shadows"];
                string shadowsEnabled = parseString(shadowsNode);
                if (shadowsEnabled == "true") {
                    scene->setShadows(true);
                } else {
                    scene->setShadows(false);
                }
            } catch (exception) {
                cerr << "Warning: Shadows configs not found! Default: disabled" << endl;
                scene->setShadows(false);
            }
            
            try {
                const YAML::Node& maxRecDepthNode = doc["MaxRecursionDepth"];
                int maxRecursionDepth = parseInt(maxRecDepthNode);
                scene->setMaxRecursionDepth(maxRecursionDepth);
            } catch (exception) {
                cerr << "Warning: Reflection recursion depth invalid! Default: 2" << endl;
                scene->setMaxRecursionDepth(2);
            }
            
            try {
                const YAML::Node& camera = doc["Camera"];
                scene->setCamera(*parseCamera(camera));
                
            } catch (exception) {
                cerr << "Warning: Camera parameters not set!" << endl;
            }

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE)
            {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it)
            {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj)
                {
                    scene->addObject(obj);
                }
                else
                {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE)
            {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for (YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it)
            {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser)
        {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(400,400);
    cout << "Tracing..." << endl;;
    scene->render(img, render_mode);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}