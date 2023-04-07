#include "Camera.h"
#include "RTScene.h"
#include "Image.h"
#include "Ray.h"
#include "Intersection.h"

namespace RayTracer{
    void Raytrace(Camera cam, RTScene* scene, Image& image);
    Ray RayThruPixel(Camera cam, int i, int j, int width, int height);
    Intersection Intersect(Ray ray, Triangle* triangle);
    Intersection Intersect(Ray ray, RTScene* sceneptr);
    glm::vec3 FindColor(Intersection hit, int recursion_depth, RTScene* scene); 
};