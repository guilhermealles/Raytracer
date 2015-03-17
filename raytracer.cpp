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
#include "Tetrahedron.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include "glm.h"
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

void Raytracer:: parseGoochParameters(const YAML::Node& node, double*  b_pointer, double* y_pointer, double* alpha_pointer, double* beta_pointer)
{
    double b, y, alpha, beta;
    
    node["b"] >> b;
    node["y"] >> y;
    node["alpha"] >> alpha;
    node["beta"]>> beta;
    
    *b_pointer = b;
    *y_pointer = y;
    *alpha_pointer = alpha;
    *beta_pointer = beta;
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
    
    try
    {
        string texture_filename;
        node["texture"] >> texture_filename;
        m->texture = new Image(texture_filename.c_str());
        
        if (m->texture->width()==0 || m->texture->height()==0)
        {
            m->texture = NULL;
        }
    }
    catch (exception)
    {
        m->texture = NULL;
    }
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
    
    if (objectType == "tetrahedron")
    {
        Point vertice1, vertice2, vertice3, vertice4;
        node["point1"] >> vertice1;
        node["point2"] >> vertice2;
        node["point3"] >> vertice3;
        node["point4"] >> vertice4;
        Tetrahedron *tetrahedron = new Tetrahedron(vertice1, vertice2, vertice3, vertice4);
        returnObject = tetrahedron;
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

void Raytracer::parseTriangleMesh(const YAML::Node& node, Scene *scene) {
    string model_filename = parseString(node["MeshFile"]);
    if (model_filename.empty()) {
        return;
    }
    // The next 3 lines convert "std::string" type to "char*"
    char *char_filename = new char[model_filename.size() + 1];
    std::copy(model_filename.begin(), model_filename.end(), char_filename);
    char_filename[model_filename.size()] = '\0';
    
    GLMmodel *model = glmReadOBJ(char_filename);
    
    float scaleFactor;
    node["MeshScale"] >> scaleFactor;
    glmScale(model, scaleFactor);
    
    Material *m = parseMaterial(node["MeshMaterial"]);
    
    GLMgroup *group = model->groups;
    // Loops through all the groups in this model
    while (group) {
        // Loops through all the triangles in this group
        for (unsigned int i=0; i<group->numtriangles; i++) {
            GLMtriangle triangle = model->triangles[group->triangles[i]];
            Vector triangle_indices(triangle.vindices[0], triangle.vindices[1], triangle.vindices[2]);
            Vector triangle_vertices[3] = {
                Vector(model->vertices[(int)triangle_indices.data[0]*3], model->vertices[(int)triangle_indices.data[0]*3+1], model->vertices[(int)triangle_indices.data[0]*3+2]),
                Vector(model->vertices[(int)triangle_indices.data[1]*3], model->vertices[(int)triangle_indices.data[1]*3+1], model->vertices[(int)triangle_indices.data[1]*3+2]),
                Vector(model->vertices[(int)triangle_indices.data[2]*3], model->vertices[(int)triangle_indices.data[2]*3+1], model->vertices[(int)triangle_indices.data[2]*3+2]),
            };
            
            // Add the position of the model to the vertices of the triangles
            Point model_position;
            model_position = parseTriple(node["MeshPosition"]);
            
            triangle_vertices[0] += model_position;
            triangle_vertices[1] += model_position;
            triangle_vertices[2] += model_position;
            
            Triangle *t = new Triangle(triangle_vertices[0], triangle_vertices[1], triangle_vertices[2]);
            
            t->material = m;
            
            scene->addObject(t);
        }
        group = group->next;
    }
    delete[] char_filename;
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
                
                if (render_mode == "gooch")
                {
                    if (doc.FindValue("GoochParameters"))
                    {
                        double b, y, alpha, beta;
                        const auto& gooch = doc["GoochParameters"];
                        gooch["alpha"] >> alpha;
                        gooch["beta"] >> beta;
                        gooch["b"] >> b;
                        gooch["y"] >> y;
                        
                        scene->setGoochParameters(&b, &y, &alpha, &beta);
                    }
                    else
                    {
                        cerr << "Error: expected a sequence of Gooch parameters." << endl;
                        return false;
                    }
                }
                
            } catch (exception) {
                cerr << "Warning: RenderMode not found! Default: phong" << endl;
                render_mode = "phong";
            }

            try {
                const YAML::Node& antiAliasingNode = doc["SuperSamplingRate"];
                string superSamplingRateString = parseString(antiAliasingNode);
                int superSamplingRate = (int) std::strtol(superSamplingRateString.c_str(), NULL, 10);
                if (superSamplingRate > 0) {
                    scene->setSuperSampling(superSamplingRate);
                }
                else {
                    cerr << "Warning: invalid super sampling value! Default: 1" << endl;
                    scene->setSuperSampling(1);
                }
            } catch (exception) {
                cerr << "Warning: super sampling configs not found! Disabling anti aliasing..." << endl;
                scene->setSuperSampling(1);
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
                cerr << "Warning: Reflection recursion depth invalid! Default: 0" << endl;
                scene->setMaxRecursionDepth(0);
            }
            
            try {
                Triple eye(parseTriple(doc["CameraEye"]));
                Triple lookPoint(parseTriple(doc["CameraLookPoint"]));
                Triple upVector(parseTriple(doc["CameraUpVector"]));
                scene->setCamera(*new Camera(eye, lookPoint, upVector.normalized()));
                scene->setCameraFlag(true);
            } catch (exception) {
                cerr << "Warning: Camera parameters not set! Will use eye parameter." << endl;
                scene->setCameraFlag(false);
            }
            
            try {
                parseTriangleMesh(doc, scene);
            } catch (exception) {
                cout << "Triangle Mesh was not found or is invalid!" << endl;
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
                if (obj) {
                    scene->addObject(obj);
                } else {
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