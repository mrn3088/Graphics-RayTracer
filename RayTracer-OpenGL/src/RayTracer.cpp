#include "RayTracer.h"
#include <ctime>
#include <thread>
#include <vector>
#include <chrono>
void RayTracer::Raytrace(Camera cam, RTScene *scene, Image &image){ // PAGE 9
    auto start = std::chrono::steady_clock::now();
    std::vector<std::thread> threads;
	int w = image.width; int h = image.height;
    auto loc = [&](int& i, int& j) {
        return i + j * w;
    };
	for (int j = 0; j < h; j++){
        threads.push_back(std::thread([&](int j) {
            for (int i = 0; i < w; i++){
                auto ray = RayThruPixel(cam, i, j, w, h );
                auto hit = Intersect(ray, scene);
                auto co = FindColor(hit, 1, scene);
                image.pixels[loc(i, j)] = co;
            }
        }, j));
        if (threads.size() == 12) {
            for (auto& thread : threads) {
                thread.join();
            }
            threads.clear();
        }
        
	}
    std::cout << "threads pushed" << std::endl;
    for (auto& thread : threads) {
        thread.join();
    }
    printf("\rcomplete\n");
    fflush(stdout);
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;

}

Ray RayTracer::RayThruPixel(Camera cam, int i, int j, int width, int height) {
    float alpha = 2 * (i + 0.5) / width - 1, beta = 1 - 2 * (j + 0.5) / height;
    glm::vec3 w = glm::normalize(cam.eye - cam.target);
    glm::vec3 u = glm::normalize(glm::cross(cam.up, w));
    glm::vec3 v = glm::cross(w, u);
    glm::vec3 d = glm::normalize(alpha * cam.aspect * glm::tan(cam.fovy / 2) * u 
    + beta * glm::tan(cam.fovy/2) * v - w);
    return Ray(cam.eye, d);
}

Intersection RayTracer::Intersect(Ray ray, Triangle* triangle) {
    Intersection result;
    result.triangle = triangle;
    glm::vec4 b = glm::vec4(ray.p0, 1);
    glm::mat4 A;
    A[0] = glm::vec4(triangle -> P[0], 1);
    A[1] = glm::vec4(triangle -> P[1], 1);
    A[2] = glm::vec4(triangle -> P[2], 1);
    A[3] = glm::vec4(-ray.dir, 0);
    glm::vec4 lambda_t = glm::inverse(A) * b;
    if(lambda_t.x >= 0 && lambda_t.y >= 0 && lambda_t.z >= 0 && lambda_t.w >= 0) {
        result.P = lambda_t.x * triangle -> P[0] + lambda_t.y * triangle -> P[1] + lambda_t.z * triangle -> P[2];
        result.N = glm::normalize(lambda_t.x * triangle -> N[0] + lambda_t.y * triangle -> N[1] + lambda_t.z * triangle -> N[2]);
        result.V = -ray.dir;
        result.dist = lambda_t.w;
    } else { 
        result.dist = INFINITY;
    }
    return result;
}

Intersection RayTracer::Intersect(Ray ray, RTScene* sceneptr) {
    float mindist = INFINITY;
    Intersection hit;
    hit.dist = mindist;
    std::vector<Triangle> list = sceneptr -> triangle_soup;
    std::for_each(sceneptr -> triangle_soup.begin(), sceneptr -> triangle_soup.end(), 
    [&](Triangle& object){
        Intersection hit_temp = Intersect(ray, &object);
        if(hit_temp.dist < mindist) {
            mindist = hit_temp.dist;
            hit = hit_temp;
        }
    });
    return hit;
}

glm::vec3 RayTracer::FindColor(Intersection hit, int recursion_depth, RTScene* scene) {
    if (hit.dist == INFINITY) return glm::vec3(1.0f,1.0f,1.0f);
    std::vector<glm::vec4> lightPositions, lightColors;
    for_each(scene -> light.begin(), scene -> light.end(), [&](std::pair<const std::string, std::shared_ptr<Light>>& l){
        lightPositions.push_back((l.second) -> position);
        lightColors.push_back((l.second) -> color);
    });
    auto color = hit.triangle -> material -> emision;
    size_t nlights = scene -> light.size();
    for(size_t i = 0; i < nlights; i++) {
        // ambient component
        glm::vec3 light = glm::vec3(hit.triangle -> material -> ambient);
        // diffuse component
        glm::vec4 lightPos = lightPositions[i];
        glm::vec3 dir = glm::normalize(glm::vec3(lightPos) - hit.P);
        glm::vec3 diffuse = glm::vec3(hit.triangle -> material -> diffuse) 
            * glm::max(glm::dot(hit.N, dir), 0.0f );
        light += diffuse;
        glm::vec3 dirView = glm::normalize(-hit.N);
        glm::vec3 halfwayDir = glm::normalize(dir + glm::reflect(-dirView, hit.N));
        glm::vec3 specularComp = glm::vec3(hit.triangle -> material -> specular)*glm::pow(glm::max(glm::dot(hit.N, halfwayDir), 0.0f), hit.triangle->material->shininess);
        if (!recursion_depth) {
            light += specularComp;
        } else {
            Ray mirrorRay(glm::reflect(hit.V, hit.N), hit.P + 0.1f * hit.N);
            Intersection hitPoint = Intersect(mirrorRay, scene);
            light += FindColor(hitPoint, recursion_depth - 1, scene) * glm::vec3(hit.triangle -> material -> specular);
        }
        Ray secondRay(hit.P + 0.1f * hit.N, dir);
        float distance = glm::sqrt((lightPos.x - hit.P.x) * (lightPos.x - hit.P.x) + 
            (lightPos.y - hit.P.y) * (lightPos.y - hit.P.y) + (lightPos.z - hit.P.z) * (lightPos.z - hit.P.z));
        Intersection intersectionPoint = Intersect(secondRay, scene);
        int visible = intersectionPoint.dist >= distance;
        light.x = light.x * lightColors[i].x * visible;
        light.y = light.y * lightColors[i].y * visible;
        light.z = light.z * lightColors[i].z * visible;
        color = color + glm::vec4(light,0);
    }
    return color;

}
